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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_GL_COMPUTE_RENDERER_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_GL_COMPUTE_RENDERER_H

#include <android/hardware_buffer.h>

#include <map>

#include "../PoseEstimationConfig.h"
#include "GlUtils.h"
#include "RendererBase.h"

namespace pose_estimation {

class GlComputeRenderer : public RendererBase {
   public:
    GlComputeRenderer(PoseEstimationConfig config, const float* textureTransform);
    ~GlComputeRenderer() override;

    void setOutputFromHardwareBuffer(AHardwareBuffer* ahwb) override;

    UniqueFd run(AHardwareBuffer* cameraInput, bool preferSyncFence) override;

   private:
    EGLImageKHR getCameraEglImage(AHardwareBuffer* cameraInput);
    bool supportsAndroidSyncFence() const { return mPfnEglDupNativeFenceFDANDROID != nullptr; }
    int eglDupNativeFenceFD(EGLDisplay display, EGLSyncKHR sync) const {
        CHECK(mPfnEglDupNativeFenceFDANDROID != nullptr);
        return mPfnEglDupNativeFenceFDANDROID(display, sync);
    }

    // EGL
    EGLDisplay mEglDisplay = EGL_NO_DISPLAY;
    EGLContext mEglContext = EGL_NO_CONTEXT;
    EGLSurface mEglSurface = EGL_NO_DISPLAY;
    // Function pointer to the eglDupNativeFenceFDANDROID extension function.
    // It is nullptr if the extension is not supported on the device.
    PFNEGLDUPNATIVEFENCEFDANDROIDPROC mPfnEglDupNativeFenceFDANDROID = nullptr;

    // GL
    uint32_t mWorkGroupSize = 64;

    // Shader program
    GLint mProgram = 0;

    // Camera input
    GLuint mCameraTexture = 0;
    // A map of {unique_ahwb_id -> imported_egl_image}.
    // We will cache the imported EGL image if the unique AHardwareBuffer ID is available, so that
    // we can avoid the overhead of importing AHardwareBuffer to GLES again when the same
    // AHardwareBuffer is reused for another camera frame.
    std::map<uint64_t, EGLImageKHR> mCachedCameraEglImages;
    // If the unique AHardwareBuffer ID is not available, we have to import the AHardwareBuffer in
    // every GlComputeRenderer::run. The imported EGL image will be held here because we want its
    // lifetime to outlive GlComputeRenderer::run for fenced execution. It will be cleaned up at the
    // beginning of the next GlComputeRenderer::run.
    EGLImageKHR mCameraEglImageWithoutId = EGL_NO_IMAGE_KHR;

    // Output
    GLuint mOutputBuffer = 0;
};

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_GL_COMPUTE_RENDERER_H
