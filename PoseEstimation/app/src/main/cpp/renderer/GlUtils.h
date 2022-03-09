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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_GL_UTILS_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_GL_UTILS_H

// clang-format off
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl31.h>
#include <GLES2/gl2ext.h>
// clang-format on

#include <map>
#include <sstream>
#include <string>

#include "../Utils.h"

namespace pose_estimation {

inline void checkGLError(const char* name) {
    int error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOGE("%s: glError: %d", name, error);
    }
}

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_GL_UTILS_H
