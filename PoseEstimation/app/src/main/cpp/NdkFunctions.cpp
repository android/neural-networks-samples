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

#include "NdkFunctions.h"

#include <android/api-level.h>
#include <dlfcn.h>

#include "Utils.h"

namespace pose_estimation {
namespace {

void* loadFunction(void* handle, const char* name) {
    void* pfn = dlsym(handle, name);
    CHECK(pfn != nullptr);
    return pfn;
}

int64_t getNnapiFeatureLevel(int64_t apiLevel, void* libNeuralNetworks) {
    // On Android devices with API level 30 and older, the Android API level value equals the NNAPI
    // feature level value.
    if (apiLevel <= 30) {
        return apiLevel;
    }

    // On Android devices with API level 31 and newer, the NNAPI feature level should be queried
    // with the NNAPI function.
    auto ANeuralNetworks_getRuntimeFeatureLevel =
            reinterpret_cast<PFN_ANeuralNetworks_getRuntimeFeatureLevel>(
                    loadFunction(libNeuralNetworks, "ANeuralNetworks_getRuntimeFeatureLevel"));
    return ANeuralNetworks_getRuntimeFeatureLevel();
}

}  // namespace

NdkFunctions::NdkFunctions() {
    mLibNeuralNetworks = dlopen("libneuralnetworks.so", RTLD_LAZY | RTLD_LOCAL);
    CHECK(mLibNeuralNetworks != nullptr);
    mLibNativeWindow = dlopen("libnativewindow.so", RTLD_LAZY | RTLD_LOCAL);
    CHECK(mLibNativeWindow != nullptr);
    mApiLevel = android_get_device_api_level();
    mNnapiFeatureLevel = getNnapiFeatureLevel(mApiLevel, mLibNeuralNetworks);

#define NDK_FUNCTION_LIB_NEURAL_NETWORKS(minFeatureLevel, ReturnType, name, ...)      \
    if (mNnapiFeatureLevel >= minFeatureLevel) {                                      \
        name = reinterpret_cast<PFN_##name>(loadFunction(mLibNeuralNetworks, #name)); \
    }
#include "NdkFunctions.inl"

#define NDK_FUNCTION_LIB_NATIVE_WINDOW(minApiLevel, ReturnType, name, ...)          \
    if (mApiLevel >= minApiLevel) {                                                 \
        name = reinterpret_cast<PFN_##name>(loadFunction(mLibNativeWindow, #name)); \
    }
#include "NdkFunctions.inl"
}

NdkFunctions::~NdkFunctions() {
    dlclose(mLibNeuralNetworks);
    dlclose(mLibNativeWindow);
}

}  // namespace pose_estimation
