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

import android.annotation.SuppressLint
import android.hardware.camera2.CameraCaptureSession
import android.hardware.camera2.CameraDevice
import android.hardware.camera2.CameraManager
import android.hardware.camera2.params.OutputConfiguration
import android.hardware.camera2.params.SessionConfiguration
import android.util.Log
import android.view.Surface
import kotlinx.coroutines.asCoroutineDispatcher
import java.util.concurrent.Executors

/**
 * Manages a camera capture session with a repeating request.
 *
 * This class will launch a background thread and open up a camera capture
 * session immediately on creation. The caller should call close() to clean up
 * all of the resources when the camera capture session is no longer needed.
 * This class should not be used in any way after close() is called.
 *
 * This class assumes that the camera permission is already granted.
 * The camera permission is handled in the ConfigFragment.
 */
@SuppressLint("MissingPermission")
class CameraStream(manager: CameraManager, cameraId: String, surfaces: List<Surface>) {

    // The executor where all camera operations run
    private val dispatcher =
        Executors.newSingleThreadExecutor(NamedThreadFactory("CameraBackground"))
            .asCoroutineDispatcher()

    private lateinit var cameraDevice: CameraDevice
    private lateinit var captureSession: CameraCaptureSession

    private val cameraCaptureCallback = object : CameraCaptureSession.CaptureCallback() {}

    private val cameraCaptureSessionCallback = object : CameraCaptureSession.StateCallback() {
        override fun onConfigured(cameraCaptureSession: CameraCaptureSession) {
            captureSession = cameraCaptureSession
            val request = cameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_RECORD)
                .apply { surfaces.forEach { addTarget(it) } }.build()
            cameraCaptureSession.setSingleRepeatingRequest(
                request,
                dispatcher.executor,
                cameraCaptureCallback
            )
        }

        override fun onConfigureFailed(cameraCaptureSession: CameraCaptureSession) {
            Log.e(TAG, "CameraCaptureSession.StateCallback.onConfigureFailed")
        }
    }

    private val cameraDeviceCallback = object : CameraDevice.StateCallback() {
        override fun onOpened(camera: CameraDevice) {
            cameraDevice = camera
            cameraDevice.createCaptureSession(
                SessionConfiguration(
                    SessionConfiguration.SESSION_REGULAR,
                    surfaces.map { OutputConfiguration(it) },
                    dispatcher.executor,
                    cameraCaptureSessionCallback
                )
            )
        }

        override fun onDisconnected(camera: CameraDevice) {
            Log.e(TAG, "CameraDevice.StateCallback.onDisconnected")
        }

        override fun onError(camera: CameraDevice, p1: Int) {
            Log.e(TAG, "CameraDevice.StateCallback.onError")
        }
    }

    init {
        manager.openCamera(cameraId, dispatcher.executor, cameraDeviceCallback)
    }

    fun close() {
        cameraDevice.close()
        dispatcher.close()
    }

    companion object {
        private val TAG = CameraStream::class.java.simpleName
    }
}
