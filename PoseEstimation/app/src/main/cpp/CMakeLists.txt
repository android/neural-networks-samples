#
# Copyright (C) 2021 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.10.2)

add_library(nnapiposeestimationdemo_jni
    SHARED
    PoseEstimationDemo_jni.cpp
    NdkFunctions.cpp
    PoseEstimator.cpp
    ml/NnapiExecutor.cpp
    ml/NnapiModel.cpp
    ml/NnapiUtils.cpp
    renderer/GlComputeRenderer.cpp
    renderer/VulkanComputeRenderer.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEGL_EGLEXT_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_GLEXT_PROTOTYPES")

target_link_libraries(nnapiposeestimationdemo_jni
    android
    EGL
    GLESv3
    jnigraphics
    log
    nativewindow
    neuralnetworks
    sync
    vulkan
)
