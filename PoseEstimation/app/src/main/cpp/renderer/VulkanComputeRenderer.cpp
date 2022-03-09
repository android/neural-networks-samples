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

#include "VulkanComputeRenderer.h"

#include <android/hardware_buffer.h>

#include <cmath>
#include <vector>

#include "../NdkFunctions.h"
#include "VulkanUtils.h"

namespace pose_estimation {
namespace {

bool isExtensionSupported(const std::vector<VkExtensionProperties>& supportedExtensions,
                          const char* requestedExtension) {
    return std::any_of(supportedExtensions.begin(), supportedExtensions.end(),
                       [requestedExtension](const auto& extension) {
                           return strcmp(extension.extensionName, requestedExtension) == 0;
                       });
}

bool areExtensionsSupported(const std::vector<VkExtensionProperties>& supportedExtensions,
                            const std::vector<const char*>& requestedExtensions) {
    return std::all_of(requestedExtensions.begin(), requestedExtensions.end(),
                       [&supportedExtensions](const char* extension) {
                           return isExtensionSupported(supportedExtensions, extension);
                       });
}

bool areExtensionsSupported(VkPhysicalDevice device,
                            const std::vector<const char*>& requestedExtensions) {
    // Enumerate device extensions
    uint32_t numberOfExtensions = 0;
    CALL_VK(vkEnumerateDeviceExtensionProperties, device, nullptr, &numberOfExtensions, nullptr);
    std::vector<VkExtensionProperties> supportedExtensions(numberOfExtensions);
    CALL_VK(vkEnumerateDeviceExtensionProperties, device, nullptr, &numberOfExtensions,
            supportedExtensions.data());

    return areExtensionsSupported(supportedExtensions, requestedExtensions);
}

bool isExportingSyncFenceFdSupported(VkPhysicalDevice device) {
    // These extensions are required to export a sync fence FD
    std::vector<const char*> deviceExtensionsToExportSyncFenceFd = {
            VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME,
            VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    };
    if (!areExtensionsSupported(device, deviceExtensionsToExportSyncFenceFd)) {
        return false;
    }

    // Check external fence properties
    const VkPhysicalDeviceExternalFenceInfo externalFenceInfo = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FENCE_INFO,
            .pNext = nullptr,
            .handleType = VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT,
    };
    VkExternalFenceProperties externalFenceProperties;
    vkGetPhysicalDeviceExternalFenceProperties(device, &externalFenceInfo,
                                               &externalFenceProperties);
    if (!(externalFenceProperties.externalFenceFeatures &
          VK_EXTERNAL_FENCE_FEATURE_EXPORTABLE_BIT)) {
        return false;
    }

    return true;
}

// Choose the work group size of the compute shader.
// In this sample app, we are using a square execution dimension.
uint32_t chooseWorkGroupSize(const VkPhysicalDeviceLimits& limits) {
    // Use 64 as the baseline.
    uint32_t size = 64;

    // Make sure the size does not exceed the limit along the X and Y axis.
    size = std::min<uint32_t>(size, limits.maxComputeWorkGroupSize[0]);
    size = std::min<uint32_t>(size, limits.maxComputeWorkGroupSize[1]);

    // Make sure the total number of invocations does not exceed the limit.
    size = std::min<uint32_t>(
            size, static_cast<uint32_t>(std::sqrt(limits.maxComputeWorkGroupInvocations)));

    // We prefer the workgroup size to be a multiple of 4.
    size &= ~(3u);

    LOGI("maxComputeWorkGroupInvocations: %d, maxComputeWorkGroupSize: (%d, %d)",
         limits.maxComputeWorkGroupInvocations, limits.maxComputeWorkGroupSize[0],
         limits.maxComputeWorkGroupSize[1]);
    LOGI("Choose workgroup size: (%d, %d)", size, size);
    return size;
}

std::vector<char> readShaderCodeFromAsset(AAssetManager* assetManager, const char* shaderFilePath) {
    AAsset* shaderFile = AAssetManager_open(assetManager, shaderFilePath, AASSET_MODE_BUFFER);
    CHECK(shaderFile != nullptr);
    const size_t shaderSize = AAsset_getLength(shaderFile);
    std::vector<char> shader(shaderSize);
    int status = AAsset_read(shaderFile, shader.data(), shaderSize);
    AAsset_close(shaderFile);
    CHECK(status >= 0);
    return shader;
}

void addImageTransitionBarrier(VkCommandBuffer commandBuffer, VkImage image,
                               VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                               VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                               VkImageLayout oldLayout, VkImageLayout newLayout,
                               uint32_t srcQueue = VK_QUEUE_FAMILY_IGNORED,
                               uint32_t dstQueue = VK_QUEUE_FAMILY_IGNORED) {
    const VkImageSubresourceRange subResourcerange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
    };
    const VkImageMemoryBarrier imageBarrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = srcAccessMask,
            .dstAccessMask = dstAccessMask,
            .oldLayout = oldLayout,
            .newLayout = newLayout,
            .srcQueueFamilyIndex = srcQueue,
            .dstQueueFamilyIndex = dstQueue,
            .image = image,
            .subresourceRange = subResourcerange,
    };
    vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1,
                         &imageBarrier);
}

void addBufferTransitionBarrier(VkCommandBuffer commandBuffer, VkBuffer buffer,
                                VkPipelineStageFlags srcStageMask,
                                VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask,
                                VkAccessFlags dstAccessMask,
                                uint32_t srcQueue = VK_QUEUE_FAMILY_IGNORED,
                                uint32_t dstQueue = VK_QUEUE_FAMILY_IGNORED) {
    const VkBufferMemoryBarrier bufferBarrier = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = srcAccessMask,
            .dstAccessMask = dstAccessMask,
            .srcQueueFamilyIndex = srcQueue,
            .dstQueueFamilyIndex = dstQueue,
            .buffer = buffer,
            .offset = 0,
            .size = VK_WHOLE_SIZE,
    };
    vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 1,
                         &bufferBarrier, 0, nullptr);
}

}  // namespace

VulkanContext::VulkanContext() {
    // Create instance
    const VkApplicationInfo applicationDesc = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "nnapi_pose_estimation_demo",
            .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
            .apiVersion = VK_API_VERSION_1_1,
    };
    const VkInstanceCreateInfo instanceDesc = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &applicationDesc,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr,
    };
    CALL_VK(vkCreateInstance, &instanceDesc, nullptr, &mInstance);

    // Enumerate physical devices
    uint32_t numberOfDevices = 0;
    CALL_VK(vkEnumeratePhysicalDevices, mInstance, &numberOfDevices, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(numberOfDevices);
    CALL_VK(vkEnumeratePhysicalDevices, mInstance, &numberOfDevices, physicalDevices.data());

    // Pick the first device with a compute queue
    for (const auto& physicalDevice : physicalDevices) {
        uint32_t numberOfQueueFamilies = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numberOfQueueFamilies, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(numberOfQueueFamilies);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numberOfQueueFamilies,
                                                 queueFamilies.data());

        uint32_t pickedQueueFamilyIndex = 0;
        bool hasComputeQueue = false;
        for (uint32_t i = 0; i < queueFamilies.size(); i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                pickedQueueFamilyIndex = i;
                hasComputeQueue = true;
                break;
            }
        }
        if (!hasComputeQueue) continue;
        mPhysicalDevice = physicalDevice;
        mQueueFamilyIndex = pickedQueueFamilyIndex;
        break;
    }
    CHECK(mPhysicalDevice != VK_NULL_HANDLE);

    // Check physical device feature
    VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR ycbcrFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES_KHR,
            .pNext = nullptr,
    };
    VkPhysicalDeviceFeatures2KHR physicalDeviceFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
            .pNext = &ycbcrFeatures,
    };
    vkGetPhysicalDeviceFeatures2(mPhysicalDevice, &physicalDeviceFeatures);
    CHECK(ycbcrFeatures.samplerYcbcrConversion == VK_TRUE);

    // Get physical device properties
    vkGetPhysicalDeviceProperties(mPhysicalDevice, &mPhysicalDeviceProperties);
    vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mPhysicalDeviceMemoryProperties);
    LOGI("Using physical device '%s'", mPhysicalDeviceProperties.deviceName);

    // Choose work group size
    mWorkGroupSize = chooseWorkGroupSize(mPhysicalDeviceProperties.limits);

    // Required device extensions
    // These extensions are required to import/export an AHardwareBuffer to/from Vulkan
    std::vector<const char*> deviceExtensions = {
            VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME,
            VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
            VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME,
            VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
            VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
            VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
    };

    bool supportsExportingSyncFenceFd = isExportingSyncFenceFdSupported(mPhysicalDevice);
    if (supportsExportingSyncFenceFd) {
        // These extensions are required to export a sync fence FD
        std::vector<const char*> deviceExtensionsToExportSyncFenceFd = {
                VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME,
                VK_KHR_MAINTENANCE1_EXTENSION_NAME,
        };
        deviceExtensions.insert(deviceExtensions.end(), deviceExtensionsToExportSyncFenceFd.begin(),
                                deviceExtensionsToExportSyncFenceFd.end());
    }

    // Create logical device
    const float queuePriority = 1.0f;
    const VkDeviceQueueCreateInfo queueDesc = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = mQueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
    };
    const VkDeviceCreateInfo deviceDesc = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueDesc,
            .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
            .ppEnabledExtensionNames = deviceExtensions.data(),
            .pEnabledFeatures = nullptr,
    };
    CALL_VK(vkCreateDevice, mPhysicalDevice, &deviceDesc, nullptr, &mDevice);
    vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 0, &mQueue);

    // Get extension function pointers
    if (supportsExportingSyncFenceFd) {
        mPfnVkGetFenceFd = (PFN_vkGetFenceFdKHR)vkGetDeviceProcAddr(mDevice, "vkGetFenceFdKHR");
    }
    if (mPfnVkGetFenceFd != nullptr) {
        LOGI("Android sync fence is supported");
    } else {
        LOGI("Android sync fence is not supported");
    }
}

VulkanContext::~VulkanContext() {
    vkDestroyDevice(mDevice, nullptr);
    vkDestroyInstance(mInstance, nullptr);
}

uint32_t VulkanContext::findMemoryType(uint32_t memoryTypeBitsRequirement,
                                       VkFlags requirementsMask) const {
    for (uint32_t memoryIndex = 0; memoryIndex < VK_MAX_MEMORY_TYPES; ++memoryIndex) {
        const uint32_t memoryTypeBits = (1 << memoryIndex);
        const bool isRequiredMemoryType = memoryTypeBitsRequirement & memoryTypeBits;
        const bool satisfiesFlags =
                (mPhysicalDeviceMemoryProperties.memoryTypes[memoryIndex].propertyFlags &
                 requirementsMask) == requirementsMask;
        if (isRequiredMemoryType && satisfiesFlags) return memoryIndex;
    }

    // failed to find memory type.
    CHECK(false);
    return 0;
}

VulkanAHardwareBufferImage::VulkanAHardwareBufferImage(VulkanContext* context,
                                                       AHardwareBuffer* buffer)
    : mContext(context) {
    AHardwareBuffer_Desc bufferDesc;
    AHardwareBuffer_describe(buffer, &bufferDesc);
    CHECK(bufferDesc.layers == 1);

    // Get Vulkan AHardwareBuffer properties
    VkAndroidHardwareBufferFormatPropertiesANDROID formatInfo = {
            .sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID,
            .pNext = nullptr,
    };
    VkAndroidHardwareBufferPropertiesANDROID properties = {
            .sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID,
            .pNext = &formatInfo,
    };
    CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID, mContext->device(), buffer, &properties);

    // If formatInfo.format == VK_FORMAT_UNDEFINED, then there is no equivalent Vulkan format for
    // the AHardwareBuffer. In such a case, Vulkan allows implementation to use an external format.
    const bool useExternalFormat = formatInfo.format == VK_FORMAT_UNDEFINED;

    // Create an image to bind to our AHardwareBuffer
    const VkExternalFormatANDROID externalFormat = {
            .sType = VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_ANDROID,
            .pNext = nullptr,
            .externalFormat = formatInfo.externalFormat,
    };
    const VkExternalMemoryImageCreateInfo externalCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO,
            .pNext = useExternalFormat ? &externalFormat : nullptr,
            .handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_ANDROID_HARDWARE_BUFFER_BIT_ANDROID,
    };
    const VkImageCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = &externalCreateInfo,
            .flags = 0u,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = formatInfo.format,
            .extent =
                    {
                            bufferDesc.width,
                            bufferDesc.height,
                            1u,
                    },
            .mipLevels = 1u,
            .arrayLayers = 1u,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_SAMPLED_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    CALL_VK(vkCreateImage, mContext->device(), &createInfo, nullptr, &mImage);

    // Allocate the device memory for the image
    const VkImportAndroidHardwareBufferInfoANDROID androidHardwareBufferInfo = {
            .sType = VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID,
            .pNext = nullptr,
            .buffer = buffer,
    };
    const VkMemoryDedicatedAllocateInfo memoryDedicatedAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO,
            .pNext = &androidHardwareBufferInfo,
            .image = mImage,
            .buffer = VK_NULL_HANDLE,
    };
    const VkMemoryAllocateInfo memoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = &memoryDedicatedAllocateInfo,
            .allocationSize = properties.allocationSize,
            .memoryTypeIndex =
                    mContext->findMemoryType(properties.memoryTypeBits, 0u /* requirementsMask */),
    };
    CALL_VK(vkAllocateMemory, mContext->device(), &memoryAllocateInfo, nullptr, &mMemory);

    // Bind the allocated memory to the image
    CALL_VK(vkBindImageMemory, mContext->device(), mImage, mMemory, 0);

    // Create YUV conversion, this is needed to sample a texture image with external format
    if (useExternalFormat) {
        const VkSamplerYcbcrConversionCreateInfo conversionCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_CREATE_INFO,
                .pNext = &externalFormat,
                .format = formatInfo.format,
                .ycbcrModel = formatInfo.suggestedYcbcrModel,
                .ycbcrRange = formatInfo.suggestedYcbcrRange,
                .components = formatInfo.samplerYcbcrConversionComponents,
                .xChromaOffset = formatInfo.suggestedXChromaOffset,
                .yChromaOffset = formatInfo.suggestedYChromaOffset,
                .chromaFilter = VK_FILTER_NEAREST,
                .forceExplicitReconstruction = VK_FALSE,
        };
        CALL_VK(vkCreateSamplerYcbcrConversion, mContext->device(), &conversionCreateInfo, nullptr,
                &mConversion);
    }

    // Create a sampler
    const VkSamplerYcbcrConversionInfo samplerConversionInfo = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO,
            .pNext = &externalFormat,
            .conversion = mConversion,
    };
    const VkSamplerCreateInfo samplerCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = (mConversion == VK_NULL_HANDLE) ? nullptr : &samplerConversionInfo,
            .magFilter = VK_FILTER_NEAREST,
            .minFilter = VK_FILTER_NEAREST,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .mipLodBias = 0.0f,
            .anisotropyEnable = VK_FALSE,
            .maxAnisotropy = 1,
            .compareEnable = VK_FALSE,
            .compareOp = VK_COMPARE_OP_NEVER,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
            .unnormalizedCoordinates = VK_FALSE,
    };
    CALL_VK(vkCreateSampler, mContext->device(), &samplerCreateInfo, nullptr, &mSampler);

    // Create an image view
    const VkImageViewCreateInfo viewCreateInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = (mConversion == VK_NULL_HANDLE) ? nullptr : &samplerConversionInfo,
            .flags = 0,
            .image = mImage,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = formatInfo.format,
            .components =
                    {
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                    },
            .subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
    };
    CALL_VK(vkCreateImageView, mContext->device(), &viewCreateInfo, nullptr, &mView);
}

VulkanAHardwareBufferImage::~VulkanAHardwareBufferImage() {
    vkDestroyImageView(mContext->device(), mView, nullptr);
    vkDestroySampler(mContext->device(), mSampler, nullptr);
    vkDestroySamplerYcbcrConversion(mContext->device(), mConversion, nullptr);
    vkFreeMemory(mContext->device(), mMemory, nullptr);
    vkDestroyImage(mContext->device(), mImage, nullptr);
}

VulkanComputePipeline::VulkanComputePipeline(VulkanContext* context,
                                             VulkanComputeRenderer* renderer,
                                             AHardwareBuffer* cameraInput)
    : mContext(context), mRenderer(renderer), mCameraTexture(context, cameraInput) {
    // Create descriptor set layout
    VkSampler sampler = mCameraTexture.sampler();
    const std::vector<VkDescriptorSetLayoutBinding> descriptorsetLayoutBinding = {
            {
                    .binding = 0,  // input image
                    .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .descriptorCount = 1,
                    .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
                    .pImmutableSamplers = mCameraTexture.isSamplerImmutable() ? &sampler : nullptr,
            },
            {
                    .binding = 1,  // output buffer
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .descriptorCount = 1,
                    .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
            },

    };
    const VkDescriptorSetLayoutCreateInfo descriptorsetLayoutDesc = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(descriptorsetLayoutBinding.size()),
            .pBindings = descriptorsetLayoutBinding.data(),
    };
    CALL_VK(vkCreateDescriptorSetLayout, mContext->device(), &descriptorsetLayoutDesc, nullptr,
            &mDescriptorSetLayout);

    // Allocate descriptor set
    const VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = mRenderer->mDescriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts = &mDescriptorSetLayout,
    };
    CALL_VK(vkAllocateDescriptorSets, mContext->device(), &descriptorSetAllocateInfo,
            &mDescriptorSet);

    // Create pipeline layout
    const VkPushConstantRange pushConstantRange = {
            .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
            .offset = 0,
            .size = static_cast<uint32_t>(mRenderer->mTextureTransform.size() * sizeof(float)),
    };
    const VkPipelineLayoutCreateInfo layoutDesc = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &mDescriptorSetLayout,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &pushConstantRange,
    };
    CALL_VK(vkCreatePipelineLayout, mContext->device(), &layoutDesc, nullptr, &mPipelineLayout);

    // Create compute pipeline
    const uint32_t workgroupSize = mContext->workgroupSize();
    const uint32_t specializationData[] = {workgroupSize, workgroupSize};
    const std::vector<VkSpecializationMapEntry> specializationMap = {
            // clang-format off
            {0, 0 * sizeof(uint32_t), sizeof(uint32_t)},
            {1, 1 * sizeof(uint32_t), sizeof(uint32_t)},
            // clang-format on
    };
    const VkSpecializationInfo specializationInfo = {
            .mapEntryCount = static_cast<uint32_t>(specializationMap.size()),
            .pMapEntries = specializationMap.data(),
            .dataSize = sizeof(specializationData),
            .pData = specializationData,
    };
    const VkComputePipelineCreateInfo pipelineDesc = {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage =
                    {
                            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage = VK_SHADER_STAGE_COMPUTE_BIT,
                            .module = mRenderer->mShaderModule,
                            .pName = "main",
                            .pSpecializationInfo = &specializationInfo,
                    },
            .layout = mPipelineLayout,
    };
    CALL_VK(vkCreateComputePipelines, mContext->device(), mRenderer->mPipelineCache, 1,
            &pipelineDesc, nullptr, &mPipeline);

    // Create the descriptor sets.
    const VkDescriptorImageInfo cameraTextureDesc = {
            .sampler = mCameraTexture.sampler(),
            .imageView = mCameraTexture.view(),
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };
    const VkDescriptorBufferInfo outputBufferDesc = {
            .buffer = mRenderer->mOutputBuffer,
            .offset = 0,
            .range = VK_WHOLE_SIZE,
    };
    const std::vector<VkWriteDescriptorSet> writeDst = {
            {
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext = nullptr,
                    .dstSet = mDescriptorSet,
                    .dstBinding = 0,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .pImageInfo = &cameraTextureDesc,
                    .pBufferInfo = nullptr,
                    .pTexelBufferView = nullptr,
            },
            {
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext = nullptr,
                    .dstSet = mDescriptorSet,
                    .dstBinding = 1,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .pImageInfo = nullptr,
                    .pBufferInfo = &outputBufferDesc,
                    .pTexelBufferView = nullptr,
            },
    };
    vkUpdateDescriptorSets(mContext->device(), writeDst.size(), writeDst.data(), 0, nullptr);

    // Create a command buffer
    const VkCommandBufferAllocateInfo cmdBufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = mRenderer->mCommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
    };
    CALL_VK(vkAllocateCommandBuffers, mContext->device(), &cmdBufferCreateInfo, &mCommandBuffer);

    // Record command buffer
    const VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr,
    };
    CALL_VK(vkBeginCommandBuffer, mCommandBuffer, &commandBufferBeginInfo);

    // Image and buffer barriers to get the input camera texture and output buffer ready for the
    // compute shader kernel
    addImageTransitionBarrier(mCommandBuffer, mCameraTexture.image(),
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, VK_ACCESS_SHADER_READ_BIT,
                              VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                              VK_QUEUE_FAMILY_FOREIGN_EXT, mContext->queueFamilyIndex());
    addBufferTransitionBarrier(mCommandBuffer, mRenderer->mOutputBuffer,
                               VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                               VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, VK_ACCESS_SHADER_WRITE_BIT,
                               VK_QUEUE_FAMILY_FOREIGN_EXT, mContext->queueFamilyIndex());

    // Setup resources and dispatch compute
    vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, mPipeline);
    vkCmdBindDescriptorSets(mCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, mPipelineLayout, 0, 1,
                            &mDescriptorSet, 0, nullptr);
    vkCmdPushConstants(mCommandBuffer, mPipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0,
                       mRenderer->mTextureTransform.size() * sizeof(float),
                       mRenderer->mTextureTransform.data());
    const uint32_t groupCountX = (kRendererOutputWidth + workgroupSize - 1) / workgroupSize;
    const uint32_t groupCountY = (kRendererOutputHeight + workgroupSize - 1) / workgroupSize;
    vkCmdDispatch(mCommandBuffer, groupCountX, groupCountY, 1);

    // Buffer barrier to get the buffer ready for host reading
    addBufferTransitionBarrier(mCommandBuffer, mRenderer->mOutputBuffer,
                               VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_HOST_BIT,
                               VK_ACCESS_SHADER_WRITE_BIT, 0, mContext->queueFamilyIndex(),
                               VK_QUEUE_FAMILY_FOREIGN_EXT);

    // Finish recording the command buffer
    CALL_VK(vkEndCommandBuffer, mCommandBuffer);
}

VulkanComputePipeline::~VulkanComputePipeline() {
    vkFreeCommandBuffers(mContext->device(), mRenderer->mCommandPool, 1, &mCommandBuffer);
    vkResetDescriptorPool(mContext->device(), mRenderer->mDescriptorPool, 0);
    vkDestroyPipeline(mContext->device(), mPipeline, nullptr);
    vkDestroyDescriptorSetLayout(mContext->device(), mDescriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(mContext->device(), mPipelineLayout, nullptr);
}

VulkanComputeRenderer::VulkanComputeRenderer(PoseEstimationConfig config,
                                             AAssetManager* assetManager,
                                             const float* textureTransform)
    : RendererBase(config) {
    // Create shader module
    const auto shaderCode = readShaderCodeFromAsset(assetManager, "shaders/shader.comp.spv");
    const VkShaderModuleCreateInfo shaderDesc = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .flags = 0,
            .codeSize = shaderCode.size(),
            .pCode = reinterpret_cast<const uint32_t*>(shaderCode.data()),
    };
    CALL_VK(vkCreateShaderModule, mContext.device(), &shaderDesc, nullptr, &mShaderModule);

    // Create command pool
    const VkCommandPoolCreateInfo cmdpoolDesc = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = mContext.queueFamilyIndex(),
    };
    CALL_VK(vkCreateCommandPool, mContext.device(), &cmdpoolDesc, nullptr, &mCommandPool);

    // Create descriptor pool
    const std::vector<VkDescriptorPoolSize> descriptorPoolSizes = {
            {
                    .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .descriptorCount = config.maxNumberOfCameraImages,
            },
            {
                    .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .descriptorCount = config.maxNumberOfCameraImages,
            },
    };
    const VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets = config.maxNumberOfCameraImages,
            .poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size()),
            .pPoolSizes = descriptorPoolSizes.data(),
    };
    CALL_VK(vkCreateDescriptorPool, mContext.device(), &descriptorPoolCreateInfo, nullptr,
            &mDescriptorPool);

    // Create pipeline cache
    const VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .initialDataSize = 0,
            .pInitialData = nullptr,
    };
    CALL_VK(vkCreatePipelineCache, mContext.device(), &pipelineCacheCreateInfo, nullptr,
            &mPipelineCache);

    // Create fence
    const VkExportFenceCreateInfo exportFenceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .handleTypes = VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT,
    };
    const VkFenceCreateInfo fenceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = &exportFenceCreateInfo,
            .flags = 0,
    };
    CALL_VK(vkCreateFence, mContext.device(), &fenceCreateInfo, nullptr, &mFence);

    // Save the texture transform matrix
    mTextureTransform = std::vector<float>(textureTransform, textureTransform + 16);
}

void VulkanComputeRenderer::setOutputFromHardwareBuffer(AHardwareBuffer* ahwb) {
    LOGI("VulkanComputeRenderer::setOutputFromHardwareBuffer");

    // Check the AHardwareBuffer format and usage bits
    AHardwareBuffer_Desc desc;
    AHardwareBuffer_describe(ahwb, &desc);
    CHECK(desc.format == AHARDWAREBUFFER_FORMAT_BLOB);
    CHECK(desc.usage & AHARDWAREBUFFER_USAGE_GPU_DATA_BUFFER);

    // Get AHardwareBuffer properties
    VkAndroidHardwareBufferPropertiesANDROID properties = {
            .sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID,
            .pNext = nullptr,
    };
    CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID, mContext.device(), ahwb, &properties);

    // Create the output buffer with AHardwareBuffer memory
    const VkExternalMemoryBufferCreateInfo externalMemoryBufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_ANDROID_HARDWARE_BUFFER_BIT_ANDROID,
    };
    const VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = &externalMemoryBufferCreateInfo,
            .flags = 0u,
            .size = desc.width,
            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0u,
            .pQueueFamilyIndices = nullptr,
    };
    CALL_VK(vkCreateBuffer, mContext.device(), &bufferCreateInfo, nullptr, &mOutputBuffer);

    // Import the AHardwareBuffer memory
    const VkImportAndroidHardwareBufferInfoANDROID importMemoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID,
            .pNext = nullptr,
            .buffer = ahwb,
    };
    const VkMemoryAllocateInfo memoryAllocInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = &importMemoryAllocateInfo,
            .allocationSize = properties.allocationSize,
            .memoryTypeIndex = mContext.findMemoryType(properties.memoryTypeBits, 0),
    };
    CALL_VK(vkAllocateMemory, mContext.device(), &memoryAllocInfo, nullptr, &mOutputBufferMemory);

    // Bind the memory with the buffer
    CALL_VK(vkBindBufferMemory, mContext.device(), mOutputBuffer, mOutputBufferMemory, 0);
}

VulkanComputeRenderer::~VulkanComputeRenderer() {
    for (auto& [_, pipeline] : mCachedComputePipelines) {
        pipeline.reset();
    }
    mComputePipelineWithoutId.reset();
    vkFreeMemory(mContext.device(), mOutputBufferMemory, nullptr);
    vkDestroyBuffer(mContext.device(), mOutputBuffer, nullptr);
    vkDestroyFence(mContext.device(), mFence, nullptr);
    vkDestroyPipelineCache(mContext.device(), mPipelineCache, nullptr);
    vkDestroyShaderModule(mContext.device(), mShaderModule, nullptr);
    vkDestroyCommandPool(mContext.device(), mCommandPool, nullptr);
    vkDestroyDescriptorPool(mContext.device(), mDescriptorPool, nullptr);
}

std::shared_ptr<VulkanComputePipeline> VulkanComputeRenderer::getComputePipeline(
        AHardwareBuffer* cameraInput) {
    // Cleanup any compute pipeline without a cache ID
    mComputePipelineWithoutId.reset();

    // Get the system wide unique ID for an AHardwareBuffer if supported
    std::optional<uint64_t> maybeAhwbId;
    if (NdkFunctions::apiLevel() >= 31) {
        uint64_t ahwbId;
        CHECK(NdkFunctions::get().AHardwareBuffer_getId(cameraInput, &ahwbId) == 0);
        maybeAhwbId = ahwbId;
    }

    // If cache hit, used the cache directly
    auto cachedPipeline = maybeAhwbId ? mCachedComputePipelines.find(*maybeAhwbId)
                                      : mCachedComputePipelines.end();
    if (cachedPipeline != mCachedComputePipelines.end()) {
        return cachedPipeline->second;
    }

    // Create the compute pipeline with the camera input
    auto pipeline = std::make_shared<VulkanComputePipeline>(&mContext, this, cameraInput);

    if (maybeAhwbId) {
        // Cache the compute pipeline if we have the unique ID
        mCachedComputePipelines.emplace(*maybeAhwbId, pipeline);
    } else {
        // Otherwise, we still need to record the compute pipeline because we have to destroy it in
        // the next run
        mComputePipelineWithoutId = pipeline;
    }
    return pipeline;
}

UniqueFd VulkanComputeRenderer::run(AHardwareBuffer* cameraInput, bool preferSyncFence) {
    // Create or get the compute pipeline
    auto pipeline = getComputePipeline(cameraInput);

    // Submit to queue
    VkCommandBuffer commandBuffer = pipeline->commandBuffer();
    const VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    CALL_VK(vkResetFences, mContext.device(), 1, &mFence);
    CALL_VK(vkQueueSubmit, mContext.queue(), 1, &submitInfo, mFence);

    // If sync fence is supported and preferred, export a Android sync fence FD and immediately
    // return; otherwise, wait until everything is finished
    UniqueFd syncFenceFd;
    if (preferSyncFence && mContext.supportsAndroidSyncFence()) {
        const VkFenceGetFdInfoKHR fenceGetFdInfo = {
                .sType = VK_STRUCTURE_TYPE_FENCE_GET_FD_INFO_KHR,
                .pNext = nullptr,
                .fence = mFence,
                .handleType = VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT,
        };
        int fd;
        CALL_VK(mContext.vkGetFenceFd, mContext.device(), &fenceGetFdInfo, &fd);
        syncFenceFd = UniqueFd(fd);
    } else {
        CALL_VK(vkWaitForFences, mContext.device(), 1, &mFence, VK_TRUE,
                /* infinite timeout */ ~(0ull));
    }

    return syncFenceFd;
}

}  // namespace pose_estimation
