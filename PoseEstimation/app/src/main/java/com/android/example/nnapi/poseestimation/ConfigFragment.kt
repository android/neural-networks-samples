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

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Process
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Spinner
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.fragment.app.commit
import androidx.fragment.app.replace
import com.android.example.nnapi.poseestimation.databinding.FragmentConfigBinding
import kotlin.time.ExperimentalTime

/**
 * The Fragment for the configuration screen.
 *
 * It is the starting point of the application for user to choose which camera,
 * GPU renderer, and ML executor to use for the pose estimation task. Once it is
 * properly configured, the user can press the "START" button to launch a pose
 * estimation session implemented in PoseEstimationFragment.
 *
 * This fragment is also responsible for checking and requesting the camera
 * permission, and validating if at least GLES 3.1 or Vulkan 1.1 is supported.
 */
@ExperimentalTime
class ConfigFragment : Fragment() {

    private val configModel: ConfigViewModel by activityViewModels()

    private var _binding: FragmentConfigBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentConfigBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        // Configure spinners to select camera facing, renderer, ML executor, execution mode,
        // and renderer output type
        configureEnumSpinner<CameraFacing>(binding.cameraFacingSpinner) {
            configModel.config.cameraFacing = it
        }
        configureEnumSpinner<Renderer>(binding.rendererSpinner) {
            configModel.config.renderer = it
        }
        configureEnumSpinner<MlExecutor>(binding.mlExecutorSpinner) {
            configModel.config.mlExecutor = it
        }

        // Button to start the pose estimation fragment
        binding.startButton.setOnClickListener { startCameraPreview() }
        binding.startButton.isEnabled = false

        // Check the camera permission and request if has not been granted yet
        val cameraPermission = requireActivity().checkPermission(
            Manifest.permission.CAMERA,
            Process.myPid(),
            Process.myUid()
        )
        if (cameraPermission != PackageManager.PERMISSION_GRANTED) {
            registerForActivityResult(
                ActivityResultContracts.RequestPermission()
            ) { isGranted: Boolean ->
                if (isGranted) {
                    binding.startButton.isEnabled = true
                } else {
                    Toast.makeText(
                        requireContext(),
                        "Camera permission is required for this demo",
                        Toast.LENGTH_SHORT
                    ).show()
                }
            }.launch(Manifest.permission.CAMERA)
        } else {
            binding.startButton.isEnabled = true
        }
    }

    // Helper method to setup a spinner with options from a Enum class
    private inline fun <reified T : Enum<T>> configureEnumSpinner(
        spinner: Spinner,
        crossinline onItemSelected: (T) -> Unit
    ) {
        spinner.apply {
            adapter = ArrayAdapter(
                requireContext(),
                android.R.layout.simple_spinner_item,
                enumValues<T>()
            ).apply {
                setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            }
            onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                override fun onItemSelected(
                    parent: AdapterView<*>?,
                    view: View?,
                    position: Int,
                    id: Long
                ) {
                    onItemSelected(enumValues<T>()[position])
                }

                override fun onNothingSelected(parent: AdapterView<*>?) {}
            }
        }
    }

    private fun startCameraPreview() {
        Log.i(TAG, "startCameraPreview with config: ${configModel.config}")

        // Check if the device supports the current configuration
        if (!checkConfiguration()) return

        // Launch PoseEstimationFragment
        parentFragmentManager.commit {
            replace<PoseEstimationFragment>(R.id.fragment_container)
            setReorderingAllowed(true)
            addToBackStack("cameraPreview")
        }
    }

    // Returns false if the current configuration is not available on the device
    private fun checkConfiguration(): Boolean {
        val pm = requireActivity().packageManager
        when (configModel.config.renderer) {
            Renderer.GLES -> {
                // GLES 3.1 is required
                val glesVersion =
                    pm.systemAvailableFeatures.find { it.name == null }?.reqGlEsVersion ?: 0
                if (glesVersion < 0x30001) {
                    Toast.makeText(
                        requireContext(),
                        "GLES 3.1 is required for the demo",
                        Toast.LENGTH_SHORT
                    ).show()
                    return false
                }
            }
            Renderer.VULKAN -> {
                // Vulkan 1.1 is required
                val hasVulkan11 =
                    pm.hasSystemFeature(PackageManager.FEATURE_VULKAN_HARDWARE_VERSION, 0x401000)
                if (!hasVulkan11) {
                    Toast.makeText(
                        requireContext(),
                        "Vulkan 1.1 is required for the demo",
                        Toast.LENGTH_SHORT
                    ).show()
                    return false
                }
            }
        }
        return true
    }

    companion object {
        private val TAG = ConfigFragment::class.java.simpleName
    }
}
