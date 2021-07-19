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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_ML_EXECUTOR_BASE_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_ML_EXECUTOR_BASE_H

#include <android/hardware_buffer.h>

#include <utility>

#include "../PoseEstimationConfig.h"
#include "../Utils.h"

namespace pose_estimation {

class MlExecutorBase {
    DISABLE_COPY_AND_ASSIGN(MlExecutorBase);

   public:
    MlExecutorBase(PoseEstimationConfig config) : mConfig(std::move(config)) {}
    virtual ~MlExecutorBase() = default;

    // Get the minimum required size of the memory in bytes for the input image
    virtual uint32_t getRequiredInputMemorySize() const = 0;

    // Must be invoked prior to MlExecutorBase::run
    // The size of the memory must be greater than or equal to MlExecutorBase::getInputMemorySize
    virtual void setInputFromHardwareBuffer(AHardwareBuffer* ahwb) = 0;

    // Get the address of output tensors
    virtual const float* getOutputHeatmapAddress() const = 0;
    virtual const float* getOutputOffsetsAddress() const = 0;

    // Whether the executor is able to wait on an Android sync fence FD or not
    virtual bool supportsAndroidSyncFence() const = 0;

    // If supportsAndroidSyncFence() returns true, syncFenceFd may specify a valid sync fence FD
    // that the execution should wait for; otherwise, syncFenceFd must be invalid
    virtual void run(UniqueFd syncFenceFd) = 0;

   protected:
    PoseEstimationConfig mConfig;
};

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_ML_EXECUTOR_BASE_H
