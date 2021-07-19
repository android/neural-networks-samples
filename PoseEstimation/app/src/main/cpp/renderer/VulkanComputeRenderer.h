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

#ifndef NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_VULKAN_COMPUTE_RENDERER_H
#define NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_VULKAN_COMPUTE_RENDERER_H

#include <android/hardware_buffer.h>

#include <map>

#include "RendererBase.h"
#include "VulkanUtils.h"

namespace pose_estimation {

class VulkanComputeRenderer;

// Manages the Vulkan environment
class VulkanContext {
    DISABLE_COPY_AND_ASSIGN(VulkanContext);

   public:
    VulkanContext();
    ~VulkanContext();

    VkDevice device() const { return mDevice; }
    uint32_t queueFamilyIndex() const { return mQueueFamilyIndex; }
    VkQueue queue() const { return mQueue; }

    uint32_t workgroupSize() const { return mWorkGroupSize; }

    // Find a suitable memory type that matches the memoryTypeBits and the required properties.
    uint32_t findMemoryType(uint32_t memoryTypeBits, VkFlags properties) const;

    VkResult vkGetFenceFd(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) const {
        CHECK(mPfnVkGetFenceFd != nullptr);
        return mPfnVkGetFenceFd(device, pGetFdInfo, pFd);
    }

    bool supportsAndroidSyncFence() const { return mPfnVkGetFenceFd != nullptr; }

   private:
    // Instance
    VkInstance mInstance = VK_NULL_HANDLE;

    // Physical device and queue family
    VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties mPhysicalDeviceProperties{};
    VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties{};
    uint32_t mQueueFamilyIndex = 0;
    uint32_t mWorkGroupSize = 32;

    // Logical device and queue
    VkDevice mDevice = VK_NULL_HANDLE;
    VkQueue mQueue = VK_NULL_HANDLE;

    // Extension functions
    PFN_vkGetFenceFdKHR mPfnVkGetFenceFd = nullptr;
};

// Manages a Vulkan sampled image imported from an AHardwareBuffer
class VulkanAHardwareBufferImage {
    DISABLE_COPY_AND_ASSIGN(VulkanAHardwareBufferImage);

   public:
    VulkanAHardwareBufferImage(VulkanContext* context, AHardwareBuffer* buffer);
    ~VulkanAHardwareBufferImage();

    VkImage image() const { return mImage; }
    VkSampler sampler() const { return mSampler; }
    VkImageView view() const { return mView; }
    bool isSamplerImmutable() const { return mConversion != VK_NULL_HANDLE; }

   private:
    VulkanContext* mContext;
    VkImage mImage = VK_NULL_HANDLE;
    VkDeviceMemory mMemory = VK_NULL_HANDLE;
    VkSampler mSampler = VK_NULL_HANDLE;
    VkImageView mView = VK_NULL_HANDLE;
    VkSamplerYcbcrConversion mConversion = VK_NULL_HANDLE;
};

// Manages a Vulkan compute pipeline with an AHardwareBuffer as the input texture
// A VulkanComputePipeline is exclusive for an camera input AHardwareBuffer
class VulkanComputePipeline {
    DISABLE_COPY_AND_ASSIGN(VulkanComputePipeline);

   public:
    VulkanComputePipeline(VulkanContext* context, VulkanComputeRenderer* renderer,
                          AHardwareBuffer* cameraInput);
    ~VulkanComputePipeline();

    VkCommandBuffer commandBuffer() const { return mCommandBuffer; }

   private:
    // Context
    VulkanContext* mContext = nullptr;
    VulkanComputeRenderer* mRenderer = nullptr;

    // Camera input
    VulkanAHardwareBufferImage mCameraTexture;

    // Compute pipeline
    VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet mDescriptorSet = VK_NULL_HANDLE;
    VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
    VkPipeline mPipeline = VK_NULL_HANDLE;

    // Command buffer
    VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;
};

class VulkanComputeRenderer : public RendererBase {
   public:
    VulkanComputeRenderer(PoseEstimationConfig config, AAssetManager* assetManager,
                          const float* textureTransform);
    ~VulkanComputeRenderer() override;

    void setOutputFromHardwareBuffer(AHardwareBuffer* ahwb) override;

    UniqueFd run(AHardwareBuffer* cameraInput, bool preferSyncFence) override;

   private:
    friend VulkanComputePipeline;

    std::shared_ptr<VulkanComputePipeline> getComputePipeline(AHardwareBuffer* cameraInput);

    // Context
    VulkanContext mContext;

    // Pools
    VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
    VkCommandPool mCommandPool = VK_NULL_HANDLE;

    // Pipeline
    VkShaderModule mShaderModule = VK_NULL_HANDLE;
    VkPipelineCache mPipelineCache = VK_NULL_HANDLE;
    std::vector<float> mTextureTransform;
    // A map of {unique_ahwb_id -> imported_egl_image}.
    // A VulkanComputePipeline is exclusive for an camera input AHardwareBuffer. We will cache the
    // compute pipeline if the unique AHardwareBuffer ID is available, so that we can avoid the
    // overhead of creating the Vulkan compute pipeline again when the same AHardwareBuffer is
    // reused for another camera frame.
    std::map<uint64_t, std::shared_ptr<VulkanComputePipeline>> mCachedComputePipelines;
    // If the unique AHardwareBuffer ID is not available, we have to create the compute pipeline in
    // every VulkanComputeRenderer::run. The created compute pipeline will be held here because we
    // want its lifetime to outlive VulkanComputeRenderer::run for fenced execution. It will be
    // cleaned up at the beginning of the next VulkanComputeRenderer::run.
    std::shared_ptr<VulkanComputePipeline> mComputePipelineWithoutId;

    // Output buffer
    VkBuffer mOutputBuffer = VK_NULL_HANDLE;
    VkDeviceMemory mOutputBufferMemory = VK_NULL_HANDLE;

    // Fence
    VkFence mFence = VK_NULL_HANDLE;
};

}  // namespace pose_estimation

#endif  // NNAPI_POSE_ESTIMATION_DEMO_APP_SRC_MAIN_CPP_RENDERER_VULKAN_COMPUTE_RENDERER_H
