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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_VULKAN_UTILS_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_VULKAN_UTILS_H

// clang-format off
// vulkan_core.h must be included before vulkan_android.h
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_android.h>
// clang-format on

#include <map>
#include <sstream>
#include <string>

#include "../Utils.h"

// Invoke a Vulkan method, log an error and abort if the result is not VK_SUCCESS
#define CALL_VK(method, ...)                                                          \
    do {                                                                              \
        auto _result = method(__VA_ARGS__);                                           \
        if (_result != VK_SUCCESS) {                                                  \
            LOG_FATAL("%s failed with %s at %s:%u", #method,                          \
                      ::pose_estimation::vkResultToStr(_result), __FILE__, __LINE__); \
        }                                                                             \
    } while (0)

namespace pose_estimation {

#define VK_RESULT_TO_STR_SWITCH_CASE(code) \
    case VK_##code:                        \
        return #code

inline const char* vkResultToStr(VkResult result) {
    switch (result) {
        VK_RESULT_TO_STR_SWITCH_CASE(SUCCESS);
        VK_RESULT_TO_STR_SWITCH_CASE(NOT_READY);
        VK_RESULT_TO_STR_SWITCH_CASE(TIMEOUT);
        VK_RESULT_TO_STR_SWITCH_CASE(EVENT_SET);
        VK_RESULT_TO_STR_SWITCH_CASE(EVENT_RESET);
        VK_RESULT_TO_STR_SWITCH_CASE(INCOMPLETE);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_OUT_OF_HOST_MEMORY);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_OUT_OF_DEVICE_MEMORY);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_INITIALIZATION_FAILED);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_DEVICE_LOST);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_MEMORY_MAP_FAILED);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_LAYER_NOT_PRESENT);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_EXTENSION_NOT_PRESENT);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_FEATURE_NOT_PRESENT);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_INCOMPATIBLE_DRIVER);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_TOO_MANY_OBJECTS);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_FORMAT_NOT_SUPPORTED);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_FRAGMENTED_POOL);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_OUT_OF_POOL_MEMORY);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_INVALID_EXTERNAL_HANDLE);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_SURFACE_LOST_KHR);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        VK_RESULT_TO_STR_SWITCH_CASE(SUBOPTIMAL_KHR);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_OUT_OF_DATE_KHR);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_VALIDATION_FAILED_EXT);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_INVALID_SHADER_NV);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_FRAGMENTATION_EXT);
        VK_RESULT_TO_STR_SWITCH_CASE(ERROR_NOT_PERMITTED_EXT);
        default:
            return "(unknown VkResult)";
    }
}

#undef VK_RESULT_TO_STR_SWITCH_CASE

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_VULKAN_UTILS_H
