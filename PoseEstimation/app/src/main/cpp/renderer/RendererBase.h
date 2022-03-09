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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_RENDERER_BASE_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_RENDERER_BASE_H

#include <android/asset_manager_jni.h>

#include <utility>

#include "../PoseEstimationConfig.h"
#include "../Utils.h"

namespace pose_estimation {

class RendererBase {
    DISABLE_COPY_AND_ASSIGN(RendererBase);

   public:
    RendererBase(PoseEstimationConfig config) : mConfig(config) {}
    virtual ~RendererBase() = default;

    // Must be invoked prior to RendererBase::run
    // Imports the AHardwareBuffer to the GPU framework and sets up related resources
    virtual void setOutputFromHardwareBuffer(AHardwareBuffer* ahwb) = 0;

    // Returns a valid sync fence FD if exporting a sync fence FD is supported and
    // preferSyncFence is true; otherwise, returns an invalid FD
    virtual UniqueFd run(AHardwareBuffer* cameraInput, bool preferSyncFence) = 0;

   protected:
    PoseEstimationConfig mConfig;
};

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_RENDERER_BASE_H
