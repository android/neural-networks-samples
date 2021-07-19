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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_NNAPI_EXECUTOR_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_NNAPI_EXECUTOR_H

#include <android/asset_manager_jni.h>
#include <android/hardware_buffer.h>

#include "../NdkFunctions.h"
#include "../PoseEstimationConfig.h"
#include "MlExecutorBase.h"
#include "NnapiUtils.h"

namespace pose_estimation {

class NnapiExecutor : public MlExecutorBase {
   public:
    NnapiExecutor(PoseEstimationConfig config, AAssetManager* assetManager);
    ~NnapiExecutor() override;

    uint32_t getRequiredInputMemorySize() const override { return mExecutionInputMemorySize; }
    void setInputFromHardwareBuffer(AHardwareBuffer* ahwb) override;

    const float* getOutputHeatmapAddress() const override { return mOutputHeatmap; }
    const float* getOutputOffsetsAddress() const override { return mOutputOffsets; }

    // NNAPI supports sync fence (ANeuralNetworksExecution_startComputeWithDependencies) since NNAPI
    // feature level 4. However, many drivers at NNAPI feature level 4 cannot run fenced computation
    // properly. So, it is recommended to prefer fenced execution starting from NNAPI feature
    // level 5.
    bool supportsAndroidSyncFence() const override {
        return NdkFunctions::nnapiFeatureLevel() >= ANEURALNETWORKS_FEATURE_LEVEL_5;
    }

    void run(UniqueFd syncFenceFd) override;

   private:
    void layoutExecutionMemory();
    void createAndSetupExecution();

    // Model
    ANeuralNetworksModel* mModel = nullptr;
    ANeuralNetworksMemory* mModelData = nullptr;

    // Compilation
    ANeuralNetworksCompilation* mCompilation = nullptr;

    // Execution
    ANeuralNetworksExecution* mExecution = nullptr;
    ANeuralNetworksBurst* mBurst = nullptr;

    // Execution input
    uint32_t mExecutionInputMemorySize = 0;
    ANeuralNetworksMemory* mExecutionInput = nullptr;

    // Execution output
    struct OutputLayout {
        uint32_t offset;
        uint32_t paddedLength;
    };
    std::vector<OutputLayout> mExecutionOutputLayouts;
    uint32_t mExecutionOutputMemorySize = 0;
    std::unique_ptr<ManagedMemory> mExecutionOutputMemory;
    ANeuralNetworksMemory* mExecutionOutput = nullptr;
    float* mOutputData = nullptr;
    const float* mOutputHeatmap = nullptr;
    const float* mOutputOffsets = nullptr;
};

void populatePoseEstimationModel(ANeuralNetworksModel* model, ANeuralNetworksMemory* memory);

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_NNAPI_EXECUTOR_H
