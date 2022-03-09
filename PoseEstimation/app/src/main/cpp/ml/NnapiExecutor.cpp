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

#include "NnapiExecutor.h"

#include <android/asset_manager_jni.h>
#include <android/hardware_buffer.h>
#include <sys/mman.h>

#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "../NdkFunctions.h"
#include "MlExecutorBase.h"
#include "NnapiUtils.h"

namespace pose_estimation {
namespace {

// Create ANeuralNetworksMemory from an asset file.
//
// Note that, at NNAPI feature level 4 (API level 30) or earlier, the NNAPI drivers may not have the
// permission to access the asset file. To work around this issue, here we will:
// 1. Allocate a large-enough shared memory to hold the model data;
// 2. Copy the asset file to the shared memory;
// 3. Create the NNAPI memory with the file descriptor of the shared memory.
ANeuralNetworksMemory* createMemoryFromAsset(AAsset* asset) {
    off_t length = AAsset_getLength(asset);
    auto ashmem = std::make_unique<ManagedAshmem>("model_data", length);
    AAsset_read(asset, ashmem->data(), length);
    return ashmem->createANeuralNetworksMemory();
}

}  // namespace

NnapiExecutor::NnapiExecutor(PoseEstimationConfig config, AAssetManager* assetManager)
    : MlExecutorBase(config) {
    LOGI("NnapiExecutor::NnapiExecutor");

    // Model data memory
    AAsset* modelDataAsset = AAssetManager_open(assetManager, "model_data.bin", AASSET_MODE_BUFFER);
    CHECK(modelDataAsset != nullptr);
    mModelData = createMemoryFromAsset(modelDataAsset);
    AAsset_close(modelDataAsset);

    // Model
    CALL_NN(ANeuralNetworksModel_create, &mModel);
    populatePoseEstimationModel(mModel, mModelData);
    CALL_NN(ANeuralNetworksModel_relaxComputationFloat32toFloat16, mModel, /*allow=*/true);
    CALL_NN(ANeuralNetworksModel_finish, mModel);

    // Compilation
    CALL_NN(ANeuralNetworksCompilation_create, mModel, &mCompilation);
    CALL_NN(ANeuralNetworksCompilation_finish, mCompilation);

    // Plan the execution memory layout, and allocate the memory for execution outputs
    // The execution input memory will be set by NnapiExecutor::setInputFromHardwareBuffer
    layoutExecutionMemory();
    mExecutionOutputMemory =
            std::make_unique<ManagedAshmem>("execution_outputs", mExecutionOutputMemorySize);
    mExecutionOutput = mExecutionOutputMemory->createANeuralNetworksMemory();

    // Get the start address of the output tensors of heatmap (index=2) and offsets (index=3)
    mOutputData = reinterpret_cast<float*>(mExecutionOutputMemory->data());
    CHECK(mOutputData != nullptr);
    mOutputHeatmap = mOutputData + mExecutionOutputLayouts[2].offset / sizeof(float);
    mOutputOffsets = mOutputData + mExecutionOutputLayouts[3].offset / sizeof(float);

    // The NNAPI burst execution is designed to reduce the overhead and improve the performance of a
    // rapid sequence of executions. Although NNAPI burst execution is introduced in NNAPI feature
    // level 3, we recommend to use NNAPI burst execution starting from NNAPI feature level 5 to get
    // the best performance. At NNAPI feature level 4 or earlier, synchronous execution is
    // recommended
    if (NdkFunctions::nnapiFeatureLevel() >= ANEURALNETWORKS_FEATURE_LEVEL_5) {
        CALL_NN(ANeuralNetworksBurst_create, mCompilation, &mBurst);
    }
}

void NnapiExecutor::setInputFromHardwareBuffer(AHardwareBuffer* ahwb) {
    LOGI("NnapiExecutor::setInputFromHardwareBuffer");
    CHECK(mExecutionInput == nullptr);
    CALL_NN(ANeuralNetworksMemory_createFromAHardwareBuffer, ahwb, &mExecutionInput);
}

void NnapiExecutor::layoutExecutionMemory() {
    // We will allocate two memory pools for PoseNet: one for the input image, one to bundle all
    // output memories. NNAPI supports querying the preferred memory alignment and padding since
    // NNAPI feature level 5. To get the best performance, we will make sure the input and output
    // memories satisfy the given memory preference.

    // Layout execution input
    // We only have one input and the alignment will always be satisfied with offset = 0,
    // so we only query the padding here.
    uint32_t inputPadding = 1;
    if (NdkFunctions::nnapiFeatureLevel() >= ANEURALNETWORKS_FEATURE_LEVEL_5) {
        CALL_NN(NdkFunctions::get().ANeuralNetworksCompilation_getPreferredMemoryPaddingForInput,
                mCompilation, /*index=*/0, &inputPadding);
    }
    mExecutionInputMemorySize = roundUp(kRendererOutputSizeBytes, inputPadding);

    // Size in bytes for each output tensor
    const std::vector<uint32_t> kOutputSizes = {
            kOutputDisplacementsSizeBytes,
            kOutputDisplacementsSizeBytes,
            kOutputHeatmapSizeBytes,
            kOutputOffsetsSizeBytes,
    };

    // Layout execution output
    uint32_t outputOffset = 0;
    mExecutionOutputLayouts.resize(kOutputSizes.size());
    for (uint32_t i = 0; i < kOutputSizes.size(); i++) {
        // Query the preferred output alignment and padding
        uint32_t alignment = sizeof(float);
        uint32_t padding = 1;
        if (NdkFunctions::nnapiFeatureLevel() >= ANEURALNETWORKS_FEATURE_LEVEL_5) {
            CALL_NN(NdkFunctions::get()
                            .ANeuralNetworksCompilation_getPreferredMemoryAlignmentForOutput,
                    mCompilation, i, &alignment);
            CALL_NN(NdkFunctions::get()
                            .ANeuralNetworksCompilation_getPreferredMemoryPaddingForOutput,
                    mCompilation, i, &padding);
        }

        // Build the output memory with preferred alignment and padding
        outputOffset = roundUp(outputOffset, alignment);
        mExecutionOutputLayouts[i].offset = outputOffset;
        mExecutionOutputLayouts[i].paddedLength = roundUp(kOutputSizes[i], padding);
        outputOffset += mExecutionOutputLayouts[i].paddedLength;
    }
    mExecutionOutputMemorySize = outputOffset;
}

void NnapiExecutor::createAndSetupExecution() {
    ANeuralNetworksExecution_free(mExecution);
    CALL_NN(ANeuralNetworksExecution_create, mCompilation, &mExecution);

    // Enable reusable execution and memory padding if supported
    // We can only use reusable execution and padded memories when the features are enabled on the
    // execution object.
    if (NdkFunctions::nnapiFeatureLevel() >= ANEURALNETWORKS_FEATURE_LEVEL_5) {
        CALL_NN(NdkFunctions::get().ANeuralNetworksExecution_setReusable, mExecution,
                /*reusable=*/true);
        CALL_NN(NdkFunctions::get().ANeuralNetworksExecution_enableInputAndOutputPadding,
                mExecution, /*enable=*/true);
    }

    // Input memory
    CHECK(mExecutionInput != nullptr);
    CALL_NN(ANeuralNetworksExecution_setInputFromMemory, mExecution, /*index=*/0, /*type=*/nullptr,
            mExecutionInput, /*offset=*/0, mExecutionInputMemorySize);

    // Output memories
    for (uint32_t i = 0; i < mExecutionOutputLayouts.size(); i++) {
        const auto& layout = mExecutionOutputLayouts[i];
        CALL_NN(ANeuralNetworksExecution_setOutputFromMemory, mExecution, i, /*type=*/nullptr,
                mExecutionOutput, layout.offset, layout.paddedLength);
    }
}

NnapiExecutor::~NnapiExecutor() {
    LOGI("NnapiExecutor::~NnapiExecutor");

    ANeuralNetworksExecution_free(mExecution);
    ANeuralNetworksBurst_free(mBurst);
    ANeuralNetworksCompilation_free(mCompilation);
    ANeuralNetworksModel_free(mModel);
    ANeuralNetworksMemory_free(mModelData);
    ANeuralNetworksMemory_free(mExecutionInput);
    ANeuralNetworksMemory_free(mExecutionOutput);
}

void NnapiExecutor::run(UniqueFd syncFenceFd) {
    // All input and output memories bindings are fixed in this demo, so we could benefit from NNAPI
    // reusable execution that is supported since NNAPI feature level 5.
    // If NNAPI reusable execution is supported, we create and setup the execution only in the first
    // NnapiExecutor::run and reuse it in subsequent runs. Otherwise, we need to create and setup
    // the execution in every NnapiExecutor::run.
    if (NdkFunctions::nnapiFeatureLevel() < ANEURALNETWORKS_FEATURE_LEVEL_5 ||
        mExecution == nullptr) {
        createAndSetupExecution();
    }

    // Attempt fenced execution if a valid syncFenceFd is supplied.
    if (syncFenceFd.ok()) {
        CHECK(supportsAndroidSyncFence());

        // Setup dependencies
        ANeuralNetworksEvent* start;
        CALL_NN(NdkFunctions::get().ANeuralNetworksEvent_createFromSyncFenceFd, syncFenceFd.get(),
                &start);

        // Fenced compute
        ANeuralNetworksEvent* finished;
        CALL_NN(NdkFunctions::get().ANeuralNetworksExecution_startComputeWithDependencies,
                mExecution, &start, 1u, /*infinite timeout*/ 0, &finished);
        CALL_NN(ANeuralNetworksEvent_wait, finished);

        // Cleanup
        ANeuralNetworksEvent_free(start);
        ANeuralNetworksEvent_free(finished);
        return;
    }

    // We may reach this point if either:
    // - exporting sync fence is not supported by the GPU renderer
    // - the GPU rendering has already finished prior to exporting the fence
    // - the device is at NNAPI feature level 4 or earlier, where the fenced execution is not
    //   recommended
    // Attempt burst compute if available.
    if (mBurst != nullptr) {
        CALL_NN(ANeuralNetworksExecution_burstCompute, mExecution, mBurst);
        return;
    }

    // We may reach this point if the device is at NNAPI feature level 4 or earlier, where neither
    // the fenced execution nor the burst execution is preferable. In such a case, we compute
    // synchronously.
    CALL_NN(ANeuralNetworksExecution_compute, mExecution);
}

}  // namespace pose_estimation
