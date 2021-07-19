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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_UTILS_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_UTILS_H

#include <android/NeuralNetworks.h>
#include <android/hardware_buffer.h>
#include <android/log.h>
#include <android/sharedmem.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <vector>

#define LOG_TAG "DEMO"
#define LOG(severity, ...) __android_log_print(severity, LOG_TAG, __VA_ARGS__)
#define LOGI(...) LOG(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOGE(...) LOG(ANDROID_LOG_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) __android_log_assert("", LOG_TAG, __VA_ARGS__)

// Log an error and abort if condition fails
#define CHECK(condition)                                                             \
    do {                                                                             \
        if (!(condition)) {                                                          \
            LOG_FATAL("Check failed at %s:%u - %s", __FILE__, __LINE__, #condition); \
        }                                                                            \
    } while (0)

#define DISABLE_COPY_AND_ASSIGN(Type) \
    Type(const Type&) = delete;       \
    Type& operator=(const Type&) = delete;

namespace pose_estimation {

constexpr uint32_t kRendererOutputHeight = 257;
constexpr uint32_t kRendererOutputWidth = 257;
constexpr uint32_t kRendererOutputChannels = 3;
constexpr uint32_t kRendererOutputSizeBytes =
        kRendererOutputHeight * kRendererOutputWidth * kRendererOutputChannels * sizeof(float);

constexpr uint32_t kHeatmapSize = 9;
constexpr uint32_t kNumberOfKeypoints = 17;
constexpr uint32_t kNumberOfDisplacements = 32;
constexpr uint32_t kOutputHeatmapSize = kHeatmapSize * kHeatmapSize * kNumberOfKeypoints;
constexpr uint32_t kOutputHeatmapSizeBytes = kOutputHeatmapSize * sizeof(float);
constexpr uint32_t kOutputOffsetsSize = kHeatmapSize * kHeatmapSize * kNumberOfKeypoints * 2;
constexpr uint32_t kOutputOffsetsSizeBytes = kOutputOffsetsSize * sizeof(float);
constexpr uint32_t kOutputDisplacementsSize = kHeatmapSize * kHeatmapSize * kNumberOfDisplacements;
constexpr uint32_t kOutputDisplacementsSizeBytes = kOutputDisplacementsSize * sizeof(float);

// RAII wrapper of a file descriptor
class UniqueFd {
    DISABLE_COPY_AND_ASSIGN(UniqueFd);

   public:
    UniqueFd() {}
    explicit UniqueFd(int fd) { reset(fd); }
    ~UniqueFd() { reset(); }
    UniqueFd(UniqueFd&& other) { reset(other.release()); }
    UniqueFd& operator=(UniqueFd&& other) {
        reset(other.release());
        return *this;
    }

    int get() const { return mFd; }
    bool ok() const { return mFd >= 0; }
    void reset(int fd = -1) {
        if (ok()) close(mFd);
        mFd = fd;
    }
    int release() {
        int result = mFd;
        mFd = -1;
        return result;
    }

   private:
    int mFd = -1;
};

// Base class for RAII wrappers of different Android memory types
class ManagedMemory {
    DISABLE_COPY_AND_ASSIGN(ManagedMemory);

   public:
    ManagedMemory() = default;
    virtual ~ManagedMemory() = default;

    virtual void* data() = 0;
    virtual ANeuralNetworksMemory* createANeuralNetworksMemory() const = 0;
};

// RAII wrapper of an ASharedMemory
class ManagedAshmem : public ManagedMemory {
   public:
    ManagedAshmem(const char* name, size_t size) : mSize(size) {
        int fd = ASharedMemory_create(name, size);
        CHECK(fd >= 0);
        mFd = UniqueFd(fd);
        mData = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, /*offset=*/0);
        CHECK(mData != nullptr);
    }
    ~ManagedAshmem() override { munmap(mData, mSize); }

    void* data() override { return mData; }
    ANeuralNetworksMemory* createANeuralNetworksMemory() const override;

   private:
    size_t mSize = 0;
    UniqueFd mFd;
    void* mData = nullptr;
};

// RAII wrapper of an AHardwareBuffer with format AHARDWAREBUFFER_FORMAT_BLOB
class ManagedBlobAhwb : public ManagedMemory {
   public:
    ManagedBlobAhwb(uint32_t size, uint64_t usage) : mCpuUsage(usage & kCpuUsageMask) {
        AHardwareBuffer_Desc desc = {
                .width = size,
                .height = 1u,
                .layers = 1u,
                .format = AHARDWAREBUFFER_FORMAT_BLOB,
                .usage = usage,
        };
        CHECK(AHardwareBuffer_allocate(&desc, &mHandle) == 0);
        CHECK(mHandle != nullptr);
    }
    ~ManagedBlobAhwb() override {
        if (isLocked()) {
            AHardwareBuffer_unlock(mHandle, /*fence=*/nullptr);
        }
        AHardwareBuffer_release(mHandle);
    }

    void* data() override {
        if (isLocked()) {
            return mData;
        }
        CHECK(AHardwareBuffer_lock(mHandle, mCpuUsage, /*fence=*/-1, /*rect=*/nullptr, &mData) ==
              0);
        return mData;
    }
    ANeuralNetworksMemory* createANeuralNetworksMemory() const override;

    bool isLocked() const { return mData != nullptr; }
    AHardwareBuffer* handle() const { return mHandle; }

   private:
    static constexpr uint64_t kCpuUsageMask =
            AHARDWAREBUFFER_USAGE_CPU_READ_MASK | AHARDWAREBUFFER_USAGE_CPU_WRITE_MASK;
    AHardwareBuffer* mHandle;
    uint64_t mCpuUsage;
    void* mData = nullptr;
};

// Round up "size" to the nearest multiple of "multiple". "multiple" must be a power of 2.
inline uint32_t roundUp(uint32_t size, uint32_t multiple) {
    CHECK(multiple != 0);
    CHECK((multiple & (multiple - 1)) == 0);
    return (size + (multiple - 1)) & ~(multiple - 1);
}

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_UTILS_H
