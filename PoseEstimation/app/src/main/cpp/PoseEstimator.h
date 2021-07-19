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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_POSE_ESTIMATOR_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_POSE_ESTIMATOR_H

#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/hardware_buffer.h>

#include <memory>
#include <vector>

#include "PoseEstimationConfig.h"
#include "Utils.h"
#include "ml/MlExecutorBase.h"
#include "renderer/RendererBase.h"

namespace pose_estimation {

struct Keypoint {
    float x;
    float y;
    float score;
};

struct PoseEstimationResult {
    std::vector<Keypoint> keypoints;
    // Time spent in render
    float renderLatencyMs;
    // Time spent in ML executor
    float mlLatencyMs;
};

class PoseEstimator {
   public:
    PoseEstimator(PoseEstimationConfig config, AAssetManager* assetManager,
                  const float* textureTransform);

    PoseEstimationResult run(AHardwareBuffer* cameraInput);

   private:
    // Postprocessing, compute the keypoints from the ML execution results
    std::vector<Keypoint> computeKeypoints();

    PoseEstimationConfig mConfig;
    std::unique_ptr<RendererBase> mRenderer;
    std::unique_ptr<MlExecutorBase> mMlExecutor;

    // The intermediate memory between GPU and ML workloads.
    // This is the output memory of the GPU renderer, as well as the input memory of the ML
    // executor. We prefer using AHardwareBuffer to avoid redundant memory copying.
    std::unique_ptr<ManagedBlobAhwb> mIntermediateMemory = nullptr;
};

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_POSE_ESTIMATOR_H
