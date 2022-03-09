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

#include "GlComputeRenderer.h"

#include <android/hardware_buffer.h>

#include <cmath>
#include <optional>
#include <utility>

#include "../NdkFunctions.h"
#include "../Utils.h"
#include "GlUtils.h"

namespace pose_estimation {
namespace {

// The GLSL shader is separated into header and body to insert custom "#define"s.
const char* kComputeShaderHeader = R"glsl(#version 310 es
#extension GL_OES_EGL_image_external_essl3 : require
)glsl";

const char* kComputeShaderBody = R"glsl(
layout (local_size_x = WORK_GROUP_SIZE_X, local_size_y = WORK_GROUP_SIZE_Y) in;

layout (std430, binding=0) buffer Output {
    float data[];
} outputBuffer;

uniform samplerExternalOES cameraTexture;
uniform mat4 textureTransform;

void main() {
    if (gl_GlobalInvocationID.x >= 257u || gl_GlobalInvocationID.y >= 257u) return;

    // Compute the texture coordinate
    float fx = float(gl_GlobalInvocationID.x) / 256.0;
    float fy = float(gl_GlobalInvocationID.y) / 256.0;
    vec2 texCoord = (textureTransform * vec4(fx, fy, 0.0, 1.0)).xy;

    // Sample the color at the texture coordinate, resulting in a RGBA vector with range [0.0, 1.0]
    vec4 color = texture(cameraTexture, texCoord);

    // Normalize the color to [-1.0, 1.0]
    color = color * 2.0 - 1.0;

    // Write rgb colors to the output buffer
    int index = (int(gl_GlobalInvocationID.y) * 257 + int(gl_GlobalInvocationID.x)) * 3;
    outputBuffer.data[index] = color.r;
    outputBuffer.data[index + 1] = color.g;
    outputBuffer.data[index + 2] = color.b;
}
)glsl";

// Choose the work group size of the compute shader.
// In this sample app, we are using a square execution dimension.
uint32_t chooseWorkGroupSize() {
    // Use 64 as the baseline.
    uint32_t size = 64;

    // Make sure the size does not exceed the limit along the X and Y axis.
    GLint maxWorkGroupSizeX, maxWorkGroupSizeY;
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxWorkGroupSizeX);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxWorkGroupSizeY);
    size = std::min<uint32_t>(size, maxWorkGroupSizeX);
    size = std::min<uint32_t>(size, maxWorkGroupSizeY);

    // Make sure the total number of invocations does not exceed the limit.
    GLint maxWorkGroupInvocations;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxWorkGroupInvocations);
    size = std::min<uint32_t>(size, static_cast<uint32_t>(std::sqrt(maxWorkGroupInvocations)));

    // We prefer the workgroup size to be a multiple of 4.
    size &= ~(3u);

    LOGI("maxComputeWorkGroupInvocations: %d, maxComputeWorkGroupSize: (%d, %d)",
         maxWorkGroupInvocations, maxWorkGroupSizeX, maxWorkGroupSizeY);
    LOGI("Choose workgroup size: (%d, %d)", size, size);
    return size;
}

inline GLuint compileShader(GLenum type, const char* header, const char* body,
                            const std::map<std::string, std::string>& defines) {
    // Concat the header and body, insert defines
    std::stringstream ss;
    ss << header;
    for (const auto& [name, value] : defines) {
        ss << "#define " << name << " " << value << "\n";
    }
    ss << body;
    const auto codeString = ss.str();
    const char* code = codeString.c_str();

    // Compile shader
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    // Check compilation status
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        LOGE("Failed to compile shader: %s", infoLog);
    }
    return success ? shader : 0u;
}

}  // namespace

GlComputeRenderer::GlComputeRenderer(PoseEstimationConfig config, const float* textureTransform)
    : RendererBase(config) {
    LOGI("GlComputeRenderer::GlComputeRenderer");

    // Initialize EGL
    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    CHECK(mEglDisplay != EGL_NO_DISPLAY);
    CHECK(eglInitialize(mEglDisplay, nullptr, nullptr));
    EGLint configAttributeList[] = {
            EGL_RED_SIZE,
            8,
            EGL_GREEN_SIZE,
            8,
            EGL_BLUE_SIZE,
            8,
            EGL_ALPHA_SIZE,
            8,
            EGL_RENDERABLE_TYPE,
            EGL_OPENGL_ES3_BIT_KHR,
            EGL_NONE,
    };
    EGLConfig eglConfig;
    EGLint numConfig = 0;
    CHECK(eglChooseConfig(mEglDisplay, configAttributeList, &eglConfig, 1, &numConfig));
    CHECK(numConfig > 0);
    EGLint contextAttributeList[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    mEglContext = eglCreateContext(mEglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttributeList);
    CHECK(mEglContext != EGL_NO_CONTEXT);
    EGLint surfaceAttributeList[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
    mEglSurface = eglCreatePbufferSurface(mEglDisplay, eglConfig, surfaceAttributeList);
    CHECK(mEglSurface != EGL_NO_SURFACE);
    CHECK(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext));
    mPfnEglDupNativeFenceFDANDROID =
            (PFNEGLDUPNATIVEFENCEFDANDROIDPROC)eglGetProcAddress("eglDupNativeFenceFDANDROID");
    if (mPfnEglDupNativeFenceFDANDROID != nullptr) {
        LOGI("Android sync fence is supported");
    } else {
        LOGI("Android sync fence is not supported");
    }

    // Initialize GL
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    LOGI("GL version: %s", version);
    mWorkGroupSize = chooseWorkGroupSize();
    checkGLError("Initialize GL");

    // Compile shader program
    unsigned int computeShader =
            compileShader(GL_COMPUTE_SHADER, kComputeShaderHeader, kComputeShaderBody,
                          {
                                  {"WORK_GROUP_SIZE_X", std::to_string(mWorkGroupSize)},
                                  {"WORK_GROUP_SIZE_Y", std::to_string(mWorkGroupSize)},
                          });
    mProgram = glCreateProgram();
    glAttachShader(mProgram, computeShader);
    glLinkProgram(mProgram);
    glDeleteShader(computeShader);
    int success;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(mProgram, sizeof(infoLog), nullptr, infoLog);
        LOGE("Failed to link compute program: %s", infoLog);
        CHECK(false);
    }
    glUseProgram(mProgram);
    checkGLError("Compile program");

    // Create input camera texture
    glGenTextures(1, &mCameraTexture);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mCameraTexture);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
    checkGLError("Create input camera texture");

    // Create output buffer
    glGenBuffers(1, &mOutputBuffer);

    // Set uniform values
    GLint cameraTextureLocation = glGetUniformLocation(mProgram, "cameraTexture");
    glUniform1i(cameraTextureLocation, 0);
    GLint textureTransformLocation = glGetUniformLocation(mProgram, "textureTransform");
    glUniformMatrix4fv(textureTransformLocation, 1, GL_FALSE, textureTransform);
    checkGLError("Set uniform values");

    // Prepare for compute
    glUseProgram(mProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mCameraTexture);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mOutputBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, /*index=*/0, mOutputBuffer);
    glUniform1i(cameraTextureLocation, 0);
    checkGLError("Prepare for compute");
}

void GlComputeRenderer::setOutputFromHardwareBuffer(AHardwareBuffer* ahwb) {
    LOGI("GlComputeRenderer::setOutputFromHardwareBuffer");
    AHardwareBuffer_Desc desc;
    AHardwareBuffer_describe(ahwb, &desc);
    EGLClientBuffer clientBuffer = eglGetNativeClientBufferANDROID(ahwb);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mOutputBuffer);
    glBufferStorageExternalEXT(GL_SHADER_STORAGE_BUFFER, 0, desc.width, clientBuffer,
                               GL_DYNAMIC_STORAGE_BIT_EXT);
    checkGLError("setOutputFromHardwareBuffer");
}

GlComputeRenderer::~GlComputeRenderer() {
    LOGI("GlComputeRenderer::~GlComputeRenderer");
    for (const auto& [_, eglImage] : mCachedCameraEglImages) {
        CHECK(eglDestroyImageKHR(mEglDisplay, eglImage));
    }
    if (mCameraEglImageWithoutId != EGL_NO_IMAGE_KHR) {
        CHECK(eglDestroyImageKHR(mEglDisplay, mCameraEglImageWithoutId));
    }
    glDeleteTextures(1, &mCameraTexture);
    glDeleteBuffers(1, &mOutputBuffer);
    glDeleteProgram(mProgram);
}

EGLImageKHR GlComputeRenderer::getCameraEglImage(AHardwareBuffer* cameraInput) {
    // Cleanup any EGL image without a cache ID
    if (mCameraEglImageWithoutId != EGL_NO_IMAGE_KHR) {
        CHECK(eglDestroyImageKHR(mEglDisplay, mCameraEglImageWithoutId));
    }

    // Get the system wide unique ID for an AHardwareBuffer if supported
    std::optional<uint64_t> maybeAhwbId;
    if (NdkFunctions::apiLevel() >= 31) {
        uint64_t ahwbId;
        CHECK(NdkFunctions::get().AHardwareBuffer_getId(cameraInput, &ahwbId) == 0);
        maybeAhwbId = ahwbId;
    }

    // If cache hit, used the cache directly
    auto cachedEglImage =
            maybeAhwbId ? mCachedCameraEglImages.find(*maybeAhwbId) : mCachedCameraEglImages.end();
    if (cachedEglImage != mCachedCameraEglImages.end()) {
        return cachedEglImage->second;
    }

    // Import the camera input AHardwareBuffer to GLES
    EGLClientBuffer eglBuffer = eglGetNativeClientBufferANDROID(cameraInput);
    CHECK(eglBuffer != nullptr);
    EGLint attrs[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    EGLImageKHR eglImage = eglCreateImageKHR(mEglDisplay, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID,
                                             eglBuffer, attrs);
    CHECK(eglImage != EGL_NO_IMAGE_KHR);

    if (maybeAhwbId) {
        // Cache the EGL image if we have the unique ID
        mCachedCameraEglImages.emplace(*maybeAhwbId, eglImage);
    } else {
        // Otherwise, we still need to record the EGL image because we have to destroy it in the
        // next run
        mCameraEglImageWithoutId = eglImage;
    }
    return eglImage;
}

UniqueFd GlComputeRenderer::run(AHardwareBuffer* cameraInput, bool preferSyncFence) {
    // Create or get the imported EGL image for camera input texture
    EGLImageKHR eglImage = getCameraEglImage(cameraInput);

    // Update the camera input texture
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, eglImage);

    // Dispatch compute
    const uint32_t groupCountX = (kRendererOutputWidth + mWorkGroupSize - 1) / mWorkGroupSize;
    const uint32_t groupCountY = (kRendererOutputHeight + mWorkGroupSize - 1) / mWorkGroupSize;
    glDispatchCompute(groupCountX, groupCountY, 1);

    // For ExecutionMode::FENCED, export a Android sync fence FD and immediately return;
    // otherwise, wait until everything is finished
    UniqueFd syncFenceFd;
    if (preferSyncFence && supportsAndroidSyncFence()) {
        EGLSyncKHR sync = eglCreateSyncKHR(mEglDisplay, EGL_SYNC_NATIVE_FENCE_ANDROID, nullptr);
        CHECK(sync != EGL_NO_SYNC_KHR);
        glFlush();
        syncFenceFd = UniqueFd(eglDupNativeFenceFD(mEglDisplay, sync));
        CHECK(eglDestroySyncKHR(mEglDisplay, sync));
    } else {
        glFinish();
    }

    checkGLError("GlComputeRenderer::run");
    return syncFenceFd;
}

}  // namespace pose_estimation
