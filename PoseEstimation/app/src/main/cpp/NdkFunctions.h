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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_NDK_FUNCTIONS_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_NDK_FUNCTIONS_H

#include <android/NeuralNetworks.h>

namespace pose_estimation {

// NNAPI feature level codes from the latest NeuralNetworksTypes.h
typedef enum {
    ANEURALNETWORKS_FEATURE_LEVEL_1 = 27,
    ANEURALNETWORKS_FEATURE_LEVEL_2 = 28,
    ANEURALNETWORKS_FEATURE_LEVEL_3 = 29,
    ANEURALNETWORKS_FEATURE_LEVEL_4 = 30,
    ANEURALNETWORKS_FEATURE_LEVEL_5 = 31,
} FeatureLevelCode;

#define NDK_FUNCTION(minApiLevel, ReturnType, name, ...) \
    typedef ReturnType (*PFN_##name)(__VA_ARGS__);
#include "NdkFunctions.inl"

// Manages NDK functions dynamically loaded from *.so. Only one instance of this class will exist.
// Use NdkFunctions::get() to retrieve it.
// This class only contains NDK functions that are introduced after API level 29 (NNAPI feature
// level 3) (exclusive) and used in this demo app. This is because this demo app is targeting
// minSdkVersion of 29, and NDK functions that are already present in API level 29 can be directly
// linked.
class NdkFunctions {
   public:
    static const NdkFunctions& get() {
        static NdkFunctions ndkFunctions;
        return ndkFunctions;
    }

    static int apiLevel() { return get().mApiLevel; }
    static int nnapiFeatureLevel() { return get().mNnapiFeatureLevel; }

#define NDK_FUNCTION(minApiLevel, ReturnType, name, ...) PFN_##name name = nullptr;
#include "NdkFunctions.inl"

   private:
    NdkFunctions();
    ~NdkFunctions();

    void* mLibNeuralNetworks = nullptr;
    void* mLibNativeWindow = nullptr;
    int mApiLevel = 0;
    int mNnapiFeatureLevel = 0;
};

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_NDK_FUNCTIONS_H
