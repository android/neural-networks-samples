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

// Functions are defined as
//   NDK_FUNCTION_<lib_name>(minApiLevel, ResultType, name, ...args)

#ifndef NDK_FUNCTION
#define NDK_FUNCTION(...)
#endif

// Functions in libneuralnetworks.so
#ifndef NDK_FUNCTION_LIB_NEURAL_NETWORKS
#define NDK_FUNCTION_LIB_NEURAL_NETWORKS NDK_FUNCTION
#endif

// NNAPI functions introduced in NNAPI feature level 4 (Android 11)
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_4, int,
                                 ANeuralNetworksExecution_startComputeWithDependencies,
                                 ANeuralNetworksExecution* execution,
                                 const ANeuralNetworksEvent* const* dependencies,
                                 uint32_t num_dependencies, uint64_t duration,
                                 ANeuralNetworksEvent** event);
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_4, int,
                                 ANeuralNetworksEvent_createFromSyncFenceFd, int sync_fence_fd,
                                 ANeuralNetworksEvent** event);

// NNAPI functions introduced in NNAPI feature level 5 (Android 12)
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_5, int64_t,
                                 ANeuralNetworks_getRuntimeFeatureLevel);
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_5, int,
                                 ANeuralNetworksExecution_setReusable,
                                 ANeuralNetworksExecution* execution, bool reusable);
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_5, int,
                                 ANeuralNetworksExecution_enableInputAndOutputPadding,
                                 ANeuralNetworksExecution* execution, bool enable);
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_5, int,
                                 ANeuralNetworksCompilation_getPreferredMemoryAlignmentForInput,
                                 const ANeuralNetworksCompilation* compilation, uint32_t index,
                                 uint32_t* alignment);
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_5, int,
                                 ANeuralNetworksCompilation_getPreferredMemoryAlignmentForOutput,
                                 const ANeuralNetworksCompilation* compilation, uint32_t index,
                                 uint32_t* alignment);
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_5, int,
                                 ANeuralNetworksCompilation_getPreferredMemoryPaddingForInput,
                                 const ANeuralNetworksCompilation* compilation, uint32_t index,
                                 uint32_t* padding);
NDK_FUNCTION_LIB_NEURAL_NETWORKS(ANEURALNETWORKS_FEATURE_LEVEL_5, int,
                                 ANeuralNetworksCompilation_getPreferredMemoryPaddingForOutput,
                                 const ANeuralNetworksCompilation* compilation, uint32_t index,
                                 uint32_t* padding);

#undef NDK_FUNCTION_LIB_NEURAL_NETWORKS

// Functions in libnativewindow.so
#ifndef NDK_FUNCTION_LIB_NATIVE_WINDOW
#define NDK_FUNCTION_LIB_NATIVE_WINDOW NDK_FUNCTION
#endif

NDK_FUNCTION_LIB_NATIVE_WINDOW(31, int, AHardwareBuffer_getId, const AHardwareBuffer* buffer,
                               uint64_t* outId);

#undef NDK_FUNCTION_LIB_NATIVE_WINDOW

#undef NDK_FUNCTION
