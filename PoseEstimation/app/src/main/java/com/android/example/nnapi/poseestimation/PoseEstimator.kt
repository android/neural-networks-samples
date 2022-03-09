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

import android.content.Context
import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.ImageFormat
import android.graphics.Paint
import android.graphics.PorterDuff
import android.hardware.HardwareBuffer
import android.media.ImageReader
import android.opengl.Matrix
import android.os.Handler
import android.os.HandlerThread
import android.os.Process
import android.view.Surface
import androidx.annotation.Keep
import kotlin.time.DurationUnit
import kotlin.time.ExperimentalTime
import kotlin.time.measureTimedValue

/**
 * Manages a pose estimation pipeline.
 *
 * This class manages a pose estimation pipeline that takes a camera frame from
 * ImageReader, preprocesses it with GPU, and analyzes the image with a pose
 * estimation neural networks model. Behind the scenes, the GPU preprocessing is
 * powered by GLES/Vulkan compute shader, and the neural networks inference is
 * powered by Android Neural Networks API (NNAPI). Both GLES/Vulkan and NNAPI
 * codes are implemented in C/C++, and this class invokes native JNI functions
 * to leverage that logic. The camera frame is passed to GLES/Vulkan via
 * HardwareBuffer to avoid memory copying.
 *
 * This class will launch a background thread and initialize the pipeline
 * immediately on creation. The pose estimation results are communicated with
 * the caller through a callback object. The caller should call close() to clean
 * up all of the resources when the pipeline is no longer needed. This class
 * should not be used in any way after close() is called.
 */
@ExperimentalTime
class PoseEstimator(
    context: Context,
    poseEstimationConfig: PoseEstimationConfig,
    cameraPreviewConfig: CameraPreviewConfig,
    private val callback: Callback,
    private val callbackHandler: Handler
) {
    // The 17 body parts corresponds to the 17 keypoints in the NativeResult
    enum class BodyPart {
        NOSE,
        LEFT_EYE,
        RIGHT_EYE,
        LEFT_EAR,
        RIGHT_EAR,
        LEFT_SHOULDER,
        RIGHT_SHOULDER,
        LEFT_ELBOW,
        RIGHT_ELBOW,
        LEFT_WRIST,
        RIGHT_WRIST,
        LEFT_HIP,
        RIGHT_HIP,
        LEFT_KNEE,
        RIGHT_KNEE,
        LEFT_ANKLE,
        RIGHT_ANKLE
    }

    // Describes the pose estimation result of a single keypoint,
    // corresponds to Keypoint in cpp/PoseEstimator.h
    @Keep
    private data class Keypoint(val x: Float, val y: Float, val score: Float)

    // The pose estimation result from the native pipeline,
    // corresponds to PoseEstimationResult in cpp/PoseEstimator.h
    @Keep
    private data class NativeResult(
        val keypoints: Array<Keypoint>,
        val renderLatencyMs: Float,
        val mlLatencyMs: Float,
    )

    // The final pose estimation result reported to the callback
    data class Result(
        // The overlay bitmap with annotations for keypoints and joints
        val overlay: Bitmap,

        // The averaged score over all keypoints
        // This represents the level of confidence that there is a person in the frame
        val score: Float,

        // The latency values in milliseconds
        val totalLatencyMs: Float,
        val renderLatencyMs: Float,
        val mlLatencyMs: Float,
    )

    // A callback object for receiving updates related to the pose estimation pipeline
    interface Callback {
        // Called when the pose estimator has been successfully initialized
        fun onInitialized(estimator: PoseEstimator)

        // Called when the pose estimator has finished processing one camera frame
        fun onResult(result: Result)
    }

    private external fun createNativePoseEstimator(
        assetManager: AssetManager,
        textureTransform: FloatArray,
        renderer: Int,
        mlExecutor: Int,
        maxNumberOfCameraImages: Int,
    ): Long

    private external fun destroyNativePoseEstimator(handle: Long)
    private external fun estimatePose(handle: Long, buffer: HardwareBuffer): NativeResult

    // The handler thread on which the whole pose estimation pipeline will run
    private val handlerThread =
        HandlerThread("PoseEstimator", Process.THREAD_PRIORITY_MORE_FAVORABLE).apply { start() }
    private val handler = Handler(handlerThread.looper)

    // Native pose estimator object
    private var nativePoseEstimator: Long = 0

    // Camera input
    private val cameraImageReader = ImageReader.newInstance(
        cameraPreviewConfig.cameraSize.width,
        cameraPreviewConfig.cameraSize.height,
        ImageFormat.PRIVATE,
        NUMBER_OF_SWAP_IMAGES,
        HardwareBuffer.USAGE_GPU_SAMPLED_IMAGE
    )
    val cameraSurface: Surface get() = cameraImageReader.surface

    // Overlay result
    private val paint = Paint().apply {
        color = Color.BLUE
        textSize = 50.0f
        strokeWidth = 8.0f
    }
    private val overlayBitmaps = Array<Bitmap>(NUMBER_OF_SWAP_IMAGES) {
        Bitmap.createBitmap(
            cameraPreviewConfig.displaySize.width,
            cameraPreviewConfig.displaySize.height,
            Bitmap.Config.ARGB_8888
        )
    }
    private var currentImageIndex = 0

    init {
        handler.post {
            nativePoseEstimator = createNativePoseEstimator(
                context.assets,
                getTextureTransform(cameraPreviewConfig),
                poseEstimationConfig.renderer.value,
                poseEstimationConfig.mlExecutor.value,
                cameraImageReader.maxImages + 1,
            )
            cameraImageReader.setOnImageAvailableListener({ run(it) }, handler)
            callbackHandler.post { callback.onInitialized(this) }
        }
    }

    // Process one camera frame from the ImageReader
    private fun run(reader: ImageReader) {
        val image = reader.acquireLatestImage() ?: return

        // We expect the image to be backed by HardwareBuffer because the ImageReader
        // is constructed with PRIVATE format and GPU usage
        val buffer = image.hardwareBuffer
            ?: throw RuntimeException("expect images from ImageReader backed by HardwareBuffer")

        // Run native pose estimation pipeline
        val (nativeResult, duration) = measureTimedValue {
            estimatePose(nativePoseEstimator, buffer)
        }

        // Draw the overlay bitmap
        val overlay = overlayBitmaps[currentImageIndex]
        drawOverlay(overlay, nativeResult.keypoints)
        currentImageIndex = (currentImageIndex + 1) % NUMBER_OF_SWAP_IMAGES

        // Compose the final pose estimation result
        val result = Result(
            overlay,
            score = nativeResult.keypoints.map { it.score }.average().toFloat(),
            totalLatencyMs = duration.toDouble(DurationUnit.MILLISECONDS).toFloat(),
            renderLatencyMs = nativeResult.renderLatencyMs,
            mlLatencyMs = nativeResult.mlLatencyMs,
        )
        callbackHandler.post { callback.onResult(result) }

        buffer.close()
        image.close()
    }

    fun close() {
        handler.post {
            destroyNativePoseEstimator(nativePoseEstimator)
            cameraImageReader.close()
        }
        handlerThread.quitSafely()
        handlerThread.join()
    }

    // Get the 4x4 transformation matrix applying to the texture coordinate to rotate, crop,
    // and optionally flip the camera frame to fit the display. It is used in the compute shader.
    private fun getTextureTransform(cameraPreviewConfig: CameraPreviewConfig): FloatArray {
        // Compute the crop ratio along X and Y of the camera frame to fit the display size
        val rotatedDisplaySize =
            getRotatedSize(cameraPreviewConfig.displaySize, cameraPreviewConfig.rotation)
        val scaledDisplaySize = scaleToFit(rotatedDisplaySize, cameraPreviewConfig.cameraSize)
        val xCropRatio = scaledDisplaySize.width.toFloat() / cameraPreviewConfig.cameraSize.width
        val yCropRatio = scaledDisplaySize.height.toFloat() / cameraPreviewConfig.cameraSize.height

        // Build up the 4x4 transformation matrix
        return FloatArray(4 * 4).also {
            Matrix.setIdentityM(it, 0)
            Matrix.translateM(it, 0, 0.5f, 0.5f, 0f)
            Matrix.scaleM(it, 0, xCropRatio, yCropRatio, 1.0f)
            Matrix.rotateM(it, 0, -cameraPreviewConfig.rotation.toFloat(), 0f, 0f, 1f)
            if (cameraPreviewConfig.shouldFlipHorizontally) {
                Matrix.scaleM(it, 0, -1.0f, 1.0f, 1.0f)
            }
            Matrix.translateM(it, 0, -0.5f, -0.5f, 0f)
        }
    }

    private fun drawOverlay(overlay: Bitmap, keypoints: Array<Keypoint>) {
        val canvas = Canvas(overlay)
        canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)

        // Draw keypoints
        keypoints.forEach { keypoint ->
            if (keypoint.score >= KEYPOINT_SCORE_THRESHOLD) {
                canvas.drawCircle(
                    keypoint.x * canvas.width,
                    keypoint.y * canvas.height,
                    8.0f,
                    paint
                )
            }
        }

        // Draw body joints
        check(keypoints.size == BodyPart.values().size)
        BODY_JOINTS.forEach { joint ->
            val firstKeypoint = keypoints[joint.first.ordinal]
            val secondKeypoint = keypoints[joint.second.ordinal]
            if (firstKeypoint.score >= KEYPOINT_SCORE_THRESHOLD &&
                secondKeypoint.score >= KEYPOINT_SCORE_THRESHOLD
            ) {
                canvas.drawLine(
                    firstKeypoint.x * canvas.width,
                    firstKeypoint.y * canvas.height,
                    secondKeypoint.x * canvas.width,
                    secondKeypoint.y * canvas.height,
                    paint
                )
            }
        }
    }

    companion object {
        private val TAG = PoseEstimator::class.java.simpleName

        // The maximum number of images that can be acquired from the ImageReader
        private const val NUMBER_OF_SWAP_IMAGES = 3

        // Keypoint with a score less than this value will not be drawn
        private const val KEYPOINT_SCORE_THRESHOLD = 0.5f

        // List of body joints that should be connected
        private val BODY_JOINTS = listOf(
            Pair(BodyPart.LEFT_WRIST, BodyPart.LEFT_ELBOW),
            Pair(BodyPart.LEFT_ELBOW, BodyPart.LEFT_SHOULDER),
            Pair(BodyPart.LEFT_SHOULDER, BodyPart.RIGHT_SHOULDER),
            Pair(BodyPart.RIGHT_SHOULDER, BodyPart.RIGHT_ELBOW),
            Pair(BodyPart.RIGHT_ELBOW, BodyPart.RIGHT_WRIST),
            Pair(BodyPart.LEFT_SHOULDER, BodyPart.LEFT_HIP),
            Pair(BodyPart.LEFT_HIP, BodyPart.RIGHT_HIP),
            Pair(BodyPart.RIGHT_HIP, BodyPart.RIGHT_SHOULDER),
            Pair(BodyPart.LEFT_HIP, BodyPart.LEFT_KNEE),
            Pair(BodyPart.LEFT_KNEE, BodyPart.LEFT_ANKLE),
            Pair(BodyPart.RIGHT_HIP, BodyPart.RIGHT_KNEE),
            Pair(BodyPart.RIGHT_KNEE, BodyPart.RIGHT_ANKLE)
        )

        init {
            System.loadLibrary("nnapiposeestimationdemo_jni")
        }
    }
}
