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

import android.util.Size
import java.util.concurrent.ThreadFactory

// A ThreadFactory that creates a named thread
class NamedThreadFactory(private val name: String) : ThreadFactory {
    override fun newThread(r: Runnable?): Thread {
        return Thread(r, name)
    }
}

// Records the configurations of displaying the camera frame
data class CameraPreviewConfig(
    // The size of the visible region on the screen of the camera preview
    val displaySize: Size,

    // The size of the camera output
    val cameraSize: Size,

    // The rotation in degrees needed for the camera output to match the display rotation
    val rotation: Int,

    // Whether we should flip the camera frame horizontally after correcting the rotation,
    // this is true if we are using a front facing camera
    val shouldFlipHorizontally: Boolean
)

// "rotation" is in degrees, and must be a multiple of 90
fun getRotatedSize(size: Size, rotation: Int): Size = when (rotation % 180) {
    0 -> size
    90 -> Size(size.height, size.width)
    else -> throw RuntimeException("unexpected rotation: $rotation")
}

// Scales the source size to just *fit* the target size while keeping the aspect ratio
// Returns the scaled size
fun scaleToFit(source: Size, target: Size): Size {
    val sourceAspectRatio = source.width.toFloat() / source.height
    val targetAspectRatio = target.width.toFloat() / target.height
    return if (sourceAspectRatio > targetAspectRatio) {
        // In this case, width should match
        Size(target.width, (target.width / sourceAspectRatio).toInt())
    } else {
        // In this case, height should match
        Size((target.height * sourceAspectRatio).toInt(), target.height)
    }
}

// Scales the source size to just *fill* the target size while keeping the aspect ratio
// Returns the scaled size
fun scaleToFill(source: Size, target: Size): Size {
    val sourceAspectRatio = source.width.toFloat() / source.height
    val targetAspectRatio = target.width.toFloat() / target.height
    return if (sourceAspectRatio > targetAspectRatio) {
        // In this case, height should match
        Size((target.height * sourceAspectRatio).toInt(), target.height)
    } else {
        // In this case, width should match
        Size(target.width, (target.width / sourceAspectRatio).toInt())
    }
}
