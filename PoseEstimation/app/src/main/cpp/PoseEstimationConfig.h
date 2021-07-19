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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_POSE_ESTIMATION_CONFIG_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_POSE_ESTIMATION_CONFIG_H

namespace pose_estimation {

enum class Renderer { VULKAN = 0, GLES = 1 };
enum class MlExecutor { NATIVE_NNAPI = 0 };

struct PoseEstimationConfig {
    Renderer renderer = Renderer::VULKAN;
    MlExecutor mlExecutor = MlExecutor::NATIVE_NNAPI;

    // The maximum number of unique camera AHardwareBuffers that will be sent to the renderer,
    // can be obtained via ImageReader.maxImages
    uint32_t maxNumberOfCameraImages = 0;
};

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_POSE_ESTIMATION_CONFIG_H
