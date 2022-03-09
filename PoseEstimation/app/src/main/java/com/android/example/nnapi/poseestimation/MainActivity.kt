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

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.add
import androidx.fragment.app.commit
import kotlin.time.ExperimentalTime

/**
 * Main entry point of the application.
 *
 * This Kotlin main class triggers the ConfigFragment for user to set up the
 * pose estimation task. The user can start the PoseEstimationFragment that
 * implements the camera preview and pose estimation pipeline from the
 * ConfigFragment. The user can navigate back to the ConfigFragment from the
 * PoseEstimationFragment by pressing the back button.
 *
 * The pose estimation task involves pipelining camera, GPU, and NNAPI
 * workloads. Behind the scenes, the GPU and NNAPI feature is powered by C/C++,
 * and the GPU -> NNAPI synchronization utilizes AHardwareBuffer and sync fence
 * to keep it as efficient as possible. This application requires the device to
 * support GLES 3.1+ or Vulkan 1.1+ for GPU compute shader, and NNAPI 1.2+ (API
 * level 29+) for the neural networks inference.
 */
@ExperimentalTime
class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        supportFragmentManager.commit {
            add<ConfigFragment>(R.id.fragment_container)
            setReorderingAllowed(true)
        }
    }
}
