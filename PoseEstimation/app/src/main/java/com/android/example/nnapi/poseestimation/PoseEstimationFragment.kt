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
import android.graphics.ImageFormat
import android.graphics.Point
import android.hardware.camera2.CameraCharacteristics
import android.hardware.camera2.CameraManager
import android.hardware.camera2.params.StreamConfigurationMap
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.util.Size
import android.view.*
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import com.android.example.nnapi.poseestimation.databinding.FragmentPoseEstimationBinding
import kotlin.time.ExperimentalTime

/**
 * The Fragment for the pose estimation screen.
 *
 * This fragment displays a camera preview, the overlay markers for the pose
 * estimation results, as well as the score and latency. This fragment uses
 * CameraStream to manage the camera preview session, and PoseEstimator to
 * manage the pose estimation pipeline.
 */
@ExperimentalTime
class PoseEstimationFragment : Fragment() {
    private val configModel: ConfigViewModel by activityViewModels()

    private var _binding: FragmentPoseEstimationBinding? = null
    private val binding get() = _binding!!

    private val cameraManager: CameraManager by lazy {
        requireContext().getSystemService(Context.CAMERA_SERVICE) as CameraManager
    }
    private val cameraId: String by lazy { getCameraId(configModel.config.cameraFacing.value) }
    private lateinit var cameraPreviewConfig: CameraPreviewConfig

    private var camera: CameraStream? = null
    private var poseEstimator: PoseEstimator? = null

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentPoseEstimationBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        binding.cameraPreview.holder.addCallback(surfaceHolderCallback)
    }

    override fun onPause() {
        stopPipeline()
        super.onPause()
    }

    private fun startPipeline() {
        if (camera != null || poseEstimator != null) {
            stopPipeline()
        }
        poseEstimator = PoseEstimator(
            requireContext(),
            configModel.config,
            cameraPreviewConfig,
            poseEstimatorCallback,
            callbackHandler = Handler(requireActivity().mainLooper)  // main thread handler
        )
    }

    private fun stopPipeline() {
        camera?.close()
        poseEstimator?.close()
        camera = null
        poseEstimator = null
    }

    // The pose estimation callback object.
    // Modifies the UI to display the pose estimation score and latency. The displayed latency
    // is averaged over the latest NUMBER_OF_LATENCIES_TO_AVERAGE iterations.
    private val poseEstimatorCallback = object : PoseEstimator.Callback {
        // History latency result
        private val totalLatencies = FloatArray(NUMBER_OF_LATENCIES_TO_AVERAGE)
        private val renderLatencies = FloatArray(NUMBER_OF_LATENCIES_TO_AVERAGE)
        private val mlLatencies = FloatArray(NUMBER_OF_LATENCIES_TO_AVERAGE)
        private var currentIndex = 0

        // Whether all entries of the latency arrays has been filled with data or not
        private var allLatencyEntriesFilled = false

        override fun onInitialized(estimator: PoseEstimator) {
            // Start camera stream
            camera = CameraStream(
                cameraManager, cameraId, listOf(
                    binding.cameraPreview.holder.surface, estimator.cameraSurface
                )
            )
        }

        override fun onResult(result: PoseEstimator.Result) {
            // Record latencies
            totalLatencies[currentIndex] = result.totalLatencyMs
            renderLatencies[currentIndex] = result.renderLatencyMs
            mlLatencies[currentIndex] = result.mlLatencyMs
            currentIndex = (currentIndex + 1) % NUMBER_OF_LATENCIES_TO_AVERAGE
            if (currentIndex == 0) {
                allLatencyEntriesFilled = true
            }

            // Update UI to show the pose estimation result.
            // The callback object will be executed on the main (UI) thread, so it's safe to
            // the modify UI components here.
            _binding?.apply {
                overlay.setImageBitmap(result.overlay)
                textScore.text = getString(R.string.preview_score, result.score)
                textTotalLatency.text =
                    getString(R.string.preview_total_latency, getAvgLatency(totalLatencies))
                textRenderLatency.text =
                    getString(R.string.preview_renderer_latency, getAvgLatency(renderLatencies))
                textMlLatency.text =
                    getString(R.string.preview_ml_latency, getAvgLatency(mlLatencies))
            }
        }

        private fun getAvgLatency(latencies: FloatArray): Float =
            if (allLatencyEntriesFilled) {
                latencies.average().toFloat()
            } else {
                latencies.sum() / currentIndex
            }
    }

    private val surfaceHolderCallback = object : SurfaceHolder.Callback {
        override fun surfaceCreated(holder: SurfaceHolder) {
            setupCameraAndPreviewSize()
            startPipeline()
        }

        override fun surfaceChanged(
            holder: SurfaceHolder,
            format: Int,
            width: Int,
            height: Int
        ) {
        }

        override fun surfaceDestroyed(holder: SurfaceHolder) {}
    }

    // Returns the first camera that satisfies the required facing in the camera list
    private fun getCameraId(requiredFacing: Int): String =
        cameraManager.cameraIdList.find {
            val characteristics = cameraManager.getCameraCharacteristics(it)
            requiredFacing == characteristics.get(CameraCharacteristics.LENS_FACING)
        } ?: throw RuntimeException("does not have a camera with the required facing!")

    private fun getCameraOrientation(cameraId: String): Int =
        cameraManager.getCameraCharacteristics(cameraId)
            .get(CameraCharacteristics.SENSOR_ORIENTATION)
            ?: throw RuntimeException("camera sensor orientation is null")

    // Scales the display size to fit the camera size, and returns the number of pixels
    // included in the scaled display region.
    private fun getIncludedPixels(cameraSize: Size, displaySize: Size): Int =
        scaleToFit(displaySize, cameraSize).let { it.width * it.height }

    // Choose the optimal camera output size based on the display size
    private fun chooseCameraSize(displaySize: Size, cameraToDisplayRotation: Int): Size {
        // Get an array of the available camera output sizes
        val map = cameraManager.getCameraCharacteristics(cameraId)
            .get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP) as StreamConfigurationMap
        val availableSizes = map.getOutputSizes(ImageFormat.PRIVATE)

        // Rotate the display size to match the camera sensor orientation
        val rotatedDisplaySize = getRotatedSize(displaySize, cameraToDisplayRotation)

        // Find all large enough and not large enough sizes
        val largeEnoughSizes = ArrayList<Size>()
        val notLargeEnoughSizes = ArrayList<Size>()
        for (size in availableSizes) {
            if (size.height >= rotatedDisplaySize.height &&
                size.width >= rotatedDisplaySize.width
            ) {
                largeEnoughSizes.add(size)
            } else {
                notLargeEnoughSizes.add(size)
            }
        }

        // If there exists a large enough size, we will choose the one with least pixels;
        // otherwise, we will choose the one with the most pixels included in the display region,
        // and if there is a tie, we will choose the one with the least pixels outside of
        // the display region.
        return if (largeEnoughSizes.size > 0) {
            largeEnoughSizes.minByOrNull { it.width * it.height }!!
        } else {
            notLargeEnoughSizes.maxWithOrNull { a, b ->
                val aIncludedPixels = getIncludedPixels(a, rotatedDisplaySize)
                val bIncludedPixels = getIncludedPixels(b, rotatedDisplaySize)
                if (aIncludedPixels != bIncludedPixels) {
                    aIncludedPixels - bIncludedPixels
                } else {
                    b.width * b.height - a.width * a.height
                }
            }!!
        }
    }

    private fun setupCameraAndPreviewSize() {
        // Query the display and camera rotation
        val displayRotation =
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                requireActivity().display?.rotation ?: 0
            } else {
                @Suppress("DEPRECATION")
                requireActivity().windowManager.defaultDisplay.rotation
            }
        val cameraOrientation = getCameraOrientation(cameraId)
        val cameraToDisplayRotation = (cameraOrientation + displayRotation) % 360

        // Query the display size, this is the size of the visible region on the screen of
        // the camera preview
        val displaySize =
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                val metrics = requireActivity().windowManager.currentWindowMetrics
                val insets = metrics.windowInsets.getInsetsIgnoringVisibility(
                    WindowInsets.Type.navigationBars() or WindowInsets.Type.displayCutout()
                )
                Size(
                    metrics.bounds.width() - insets.left - insets.right,
                    metrics.bounds.height() - insets.top - insets.bottom
                )
            } else {
                Point().let {
                    @Suppress("DEPRECATION")
                    requireActivity().windowManager.defaultDisplay.getSize(it)
                    Size(it.x, it.y)
                }
            }

        // Choose an optimal camera output size, ideally this size should be larger than
        // the display size with the least wasted pixels outside of the display region.
        // In the case where all available camera output sizes are smaller than the display region,
        // we choose the camera output size with the most number of pixels.
        val cameraSize = chooseCameraSize(displaySize, cameraToDisplayRotation)

        // We need to flip the camera frame horizontally after correcting the rotation
        // for front facing camera to match with the preview
        val shouldFlipHorizontally = configModel.config.cameraFacing == CameraFacing.FRONT

        // Record the results to the CameraAndPreviewConfig data class
        cameraPreviewConfig = CameraPreviewConfig(
            displaySize, cameraSize, cameraToDisplayRotation, shouldFlipHorizontally
        )
        Log.i(TAG, "cameraAndPreviewConfig = $cameraPreviewConfig")

        // Set the surface holder size, this is the size used by the camera to determine
        // the output size, so set it to be cameraSize
        binding.cameraPreview.holder.setFixedSize(cameraSize.width, cameraSize.height)

        // Set the size of the surface view for camera preview
        // This should at least cover displaySize, and should also maintain the same aspect ratio
        // as cameraSize to avoid stretching
        val rotatedCameraSize = getRotatedSize(cameraSize, cameraToDisplayRotation)
        val previewSize = scaleToFill(rotatedCameraSize, displaySize)
        binding.cameraPreview.layoutParams.width = previewSize.width
        binding.cameraPreview.layoutParams.height = previewSize.height

        // Set the size of the overlay image view
        binding.overlay.layoutParams.width = displaySize.width
        binding.overlay.layoutParams.height = displaySize.height
    }

    companion object {
        private val TAG = PoseEstimationFragment::class.java.simpleName

        // How many iterations to average for the displayed latency
        private const val NUMBER_OF_LATENCIES_TO_AVERAGE = 100
    }
}
