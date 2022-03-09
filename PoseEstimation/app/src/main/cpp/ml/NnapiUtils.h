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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_NNAPI_UTILS_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_NNAPI_UTILS_H

#include <android/NeuralNetworks.h>

#include <vector>

#include "../Utils.h"

#define CALL_NN(method, ...)                                                                   \
    do {                                                                                       \
        auto _result = method(__VA_ARGS__);                                                    \
        if (_result != ANEURALNETWORKS_NO_ERROR) {                                             \
            LOG_FATAL("%s failed with %s at %s:%u", #method, nnResultToStr(_result), __FILE__, \
                      __LINE__);                                                               \
        }                                                                                      \
    } while (0)

namespace pose_estimation {

#define NN_RESULT_TO_STR_SWITCH_CASE(code) \
    case ANEURALNETWORKS_##code:           \
        return #code

inline const char* nnResultToStr(int result) {
    switch (result) {
        NN_RESULT_TO_STR_SWITCH_CASE(NO_ERROR);
        NN_RESULT_TO_STR_SWITCH_CASE(OUT_OF_MEMORY);
        NN_RESULT_TO_STR_SWITCH_CASE(INCOMPLETE);
        NN_RESULT_TO_STR_SWITCH_CASE(UNEXPECTED_NULL);
        NN_RESULT_TO_STR_SWITCH_CASE(BAD_DATA);
        NN_RESULT_TO_STR_SWITCH_CASE(OP_FAILED);
        NN_RESULT_TO_STR_SWITCH_CASE(BAD_STATE);
        NN_RESULT_TO_STR_SWITCH_CASE(UNMAPPABLE);
        NN_RESULT_TO_STR_SWITCH_CASE(OUTPUT_INSUFFICIENT_SIZE);
        NN_RESULT_TO_STR_SWITCH_CASE(UNAVAILABLE_DEVICE);
        NN_RESULT_TO_STR_SWITCH_CASE(MISSED_DEADLINE_TRANSIENT);
        NN_RESULT_TO_STR_SWITCH_CASE(MISSED_DEADLINE_PERSISTENT);
        NN_RESULT_TO_STR_SWITCH_CASE(RESOURCE_EXHAUSTED_TRANSIENT);
        NN_RESULT_TO_STR_SWITCH_CASE(RESOURCE_EXHAUSTED_PERSISTENT);
        NN_RESULT_TO_STR_SWITCH_CASE(DEAD_OBJECT);
        default:
            return "(unknown NNAPI result)";
    }
}

#undef NN_RESULT_TO_STR_SWITCH_CASE

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_ML_NNAPI_UTILS_H
