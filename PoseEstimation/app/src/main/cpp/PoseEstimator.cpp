/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "PoseEstimator.h"

#include <android/bitmap.h>
#include <android/hardware_buffer.h>

#include <chrono>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "PoseEstimationConfig.h"
#include "Utils.h"
#include "ml/NnapiExecutor.h"
#include "renderer/GlComputeRenderer.h"
#include "renderer/VulkanComputeRenderer.h"

namespace pose_estimation {
namespace {

float durationMsBetween(const std::chrono::high_resolution_clock::time_point& start,
                        const std::chrono::high_resolution_clock::time_point& end) {
    return (end - start).count() / 1000'000.0f;
}

}  // namespace

PoseEstimator::PoseEstimator(PoseEstimationConfig config, AAssetManager* assetManager,
                             const float* textureTransform)
    : mConfig(config) {
    // Initialize the GPU renderer based on the configuration
    switch (config.renderer) {
        case Renderer::GLES:
            mRenderer = std::make_unique<GlComputeRenderer>(config, textureTransform);
            break;
        case Renderer::VULKAN:
            mRenderer =
                    std::make_unique<VulkanComputeRenderer>(config, assetManager, textureTransform);
            break;
        default:
            CHECK(false);
    }

    // Initialize the ML executor based on the configuration
    switch (config.mlExecutor) {
        case MlExecutor::NATIVE_NNAPI:
            mMlExecutor = std::make_unique<NnapiExecutor>(config, assetManager);
            break;
        default:
            CHECK(false);
    }

    // Allocate the AHardwareBuffer for the intermediate result between GPU and ML workloads
    const uint32_t inputMemorySize = mMlExecutor->getRequiredInputMemorySize();
    mIntermediateMemory = std::make_unique<ManagedBlobAhwb>(
            inputMemorySize,
            AHARDWAREBUFFER_USAGE_GPU_DATA_BUFFER | AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN);
    mRenderer->setOutputFromHardwareBuffer(mIntermediateMemory->handle());
    mMlExecutor->setInputFromHardwareBuffer(mIntermediateMemory->handle());
}

PoseEstimationResult PoseEstimator::run(AHardwareBuffer* cameraInput) {
    auto start = std::chrono::high_resolution_clock::now();

    // Run GPU workload
    // We prefer synchronizing the GPU and ML workloads with Android sync fence if supported
    bool preferSyncFence = mMlExecutor->supportsAndroidSyncFence();
    UniqueFd syncFenceFd = mRenderer->run(cameraInput, preferSyncFence);
    auto rendererFinished = std::chrono::high_resolution_clock::now();

    // Run ML workload
    mMlExecutor->run(std::move(syncFenceFd));
    auto mlExecutorFinished = std::chrono::high_resolution_clock::now();

    // Run postprocessing
    auto keypoints = computeKeypoints();

    return {
            .keypoints = std::move(keypoints),
            .renderLatencyMs = durationMsBetween(start, rendererFinished),
            .mlLatencyMs = durationMsBetween(rendererFinished, mlExecutorFinished),
    };
}

std::vector<Keypoint> PoseEstimator::computeKeypoints() {
    const float* outputHeatmap = mMlExecutor->getOutputHeatmapAddress();
    const float* outputOffsets = mMlExecutor->getOutputOffsetsAddress();

    std::vector<Keypoint> keypoints(kNumberOfKeypoints);

    for (uint32_t k = 0; k < kNumberOfKeypoints; k++) {
        // Find the index and value of the maximum point in the heatmap
        float maxValue = outputHeatmap[k];
        uint32_t maxX = 0, maxY = 0;
        for (uint32_t h = 0; h < kHeatmapSize; h++) {
            for (uint32_t w = 0; w < kHeatmapSize; w++) {
                float value = outputHeatmap[(h * kHeatmapSize + w) * kNumberOfKeypoints + k];
                if (value > maxValue) {
                    maxValue = value;
                    maxX = w;
                    maxY = h;
                }
            }
        }

        // Get the corresponding offset value
        uint32_t yOffsetIndex = (maxY * kHeatmapSize + maxX) * kNumberOfKeypoints * 2 + k;
        uint32_t xOffsetIndex = yOffsetIndex + kNumberOfKeypoints;
        float yOffset = outputOffsets[yOffsetIndex], xOffset = outputOffsets[xOffsetIndex];

        // Compute the normalized location of the keypoint
        keypoints[k].x =
                maxX / static_cast<float>(kHeatmapSize - 1) + xOffset / kRendererOutputWidth;
        keypoints[k].y =
                maxY / static_cast<float>(kHeatmapSize - 1) + yOffset / kRendererOutputHeight;

        // Compute the keypoint score
        keypoints[k].score = 1.0f / (1.0f + std::exp(-maxValue));
    }

    return keypoints;
}

}  // namespace pose_estimation
