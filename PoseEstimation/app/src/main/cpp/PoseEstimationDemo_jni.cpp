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

#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/hardware_buffer_jni.h>
#include <jni.h>

#include <algorithm>

#include "PoseEstimationConfig.h"
#include "PoseEstimator.h"
#include "Utils.h"

using namespace pose_estimation;

extern "C" JNIEXPORT jlong JNICALL
Java_com_android_example_nnapi_poseestimation_PoseEstimator_createNativePoseEstimator(
        JNIEnv* env, jobject /* this */, jobject jAssetManager, jfloatArray textureTransform,
        jint renderer, jint mlExecutor, jint maxNumberOfCameraImages) {
    PoseEstimationConfig config = {
            .renderer = static_cast<Renderer>(renderer),
            .mlExecutor = static_cast<MlExecutor>(mlExecutor),
            .maxNumberOfCameraImages = static_cast<uint32_t>(maxNumberOfCameraImages),
    };
    AAssetManager* assetManager = AAssetManager_fromJava(env, jAssetManager);
    const float* transform = env->GetFloatArrayElements(textureTransform, nullptr);
    auto estimator = std::make_unique<PoseEstimator>(config, assetManager, transform);
    CHECK(estimator != nullptr);
    return (jlong)estimator.release();
}

extern "C" JNIEXPORT void JNICALL
Java_com_android_example_nnapi_poseestimation_PoseEstimator_destroyNativePoseEstimator(
        JNIEnv* env, jobject /* this */, jlong handle) {
    auto* estimator = (PoseEstimator*)handle;
    delete estimator;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_android_example_nnapi_poseestimation_PoseEstimator_estimatePose(JNIEnv* env,
                                                                         jobject /* this */,
                                                                         jlong handle,
                                                                         jobject buffer) {
    auto* estimator = (PoseEstimator*)handle;
    auto* ahwb = AHardwareBuffer_fromHardwareBuffer(env, buffer);
    auto result = estimator->run(ahwb);

    // Get the java Keypoint & NativeResult class and their constructors
    jclass keypointClass =
            env->FindClass("com/android/example/nnapi/poseestimation/PoseEstimator$Keypoint");
    CHECK(keypointClass != nullptr);
    jmethodID keypointClassCtor = env->GetMethodID(keypointClass, "<init>", "(FFF)V");
    CHECK(keypointClassCtor != nullptr);
    jclass resultClass =
            env->FindClass("com/android/example/nnapi/poseestimation/PoseEstimator$NativeResult");
    CHECK(resultClass != nullptr);
    jmethodID resultClassCtor = env->GetMethodID(
            resultClass, "<init>",
            "([Lcom/android/example/nnapi/poseestimation/PoseEstimator$Keypoint;FF)V");
    CHECK(resultClassCtor != nullptr);

    // Convert C++ result struct to java native result class
    jobjectArray jkeypoints = env->NewObjectArray(result.keypoints.size(), keypointClass, nullptr);
    for (uint32_t i = 0; i < result.keypoints.size(); i++) {
        const auto& keypoint = result.keypoints[i];
        jobject jkeypoint = env->NewObject(keypointClass, keypointClassCtor, keypoint.x, keypoint.y,
                                           keypoint.score);
        env->SetObjectArrayElement(jkeypoints, i, jkeypoint);
    }
    jobject jresult = env->NewObject(resultClass, resultClassCtor, jkeypoints,
                                     result.renderLatencyMs, result.mlLatencyMs);
    return jresult;
}
