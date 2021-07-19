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

package com.android.example.nnapi.poseestimation

import android.hardware.camera2.CameraCharacteristics
import androidx.annotation.Keep
import androidx.lifecycle.ViewModel
import kotlin.time.ExperimentalTime

enum class CameraFacing(val value: Int) {
    BACK(CameraCharacteristics.LENS_FACING_BACK), FRONT(CameraCharacteristics.LENS_FACING_FRONT)
}

// Corresponds to Renderer in cpp/PoseEstimationConfig.h
@Keep
enum class Renderer(val value: Int) {
    VULKAN(0), GLES(1)
}

// Corresponds to MlExecutor in cpp/PoseEstimationConfig.h
@Keep
enum class MlExecutor(val value: Int) {
    NATIVE_NNAPI(0)
}

// The pose estimation pipeline configuration
data class PoseEstimationConfig(
    var cameraFacing: CameraFacing,

    // The following fields correspond to PoseEstimationConfig in cpp/PoseEstimationConfig.h
    var renderer: Renderer,
    var mlExecutor: MlExecutor,
)

@ExperimentalTime
class ConfigViewModel : ViewModel() {
    val config = PoseEstimationConfig(
        CameraFacing.BACK,
        Renderer.VULKAN,
        MlExecutor.NATIVE_NNAPI,
    )
}
