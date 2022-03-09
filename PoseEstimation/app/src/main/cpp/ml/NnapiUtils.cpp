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

#include "NnapiUtils.h"

#include <android/NeuralNetworks.h>

namespace pose_estimation {

ANeuralNetworksMemory* ManagedAshmem::createANeuralNetworksMemory() const {
    ANeuralNetworksMemory* memory = nullptr;
    CALL_NN(ANeuralNetworksMemory_createFromFd, mSize, PROT_READ | PROT_WRITE, mFd.get(),
            /*offset=*/0, &memory);
    return memory;
}

ANeuralNetworksMemory* ManagedBlobAhwb::createANeuralNetworksMemory() const {
    ANeuralNetworksMemory* memory = nullptr;
    CALL_NN(ANeuralNetworksMemory_createFromAHardwareBuffer, mHandle, &memory);
    return memory;
}

}  // namespace pose_estimation
