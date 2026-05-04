//
//  shaders.hpp
//  Headers for Cherry shaders API
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

// NOTE: This compute shader system is WIP. For advanced purposes only.
// Expect crashes/bugs.

#pragma once
#include <options.hpp>

#ifdef CHERRY_ENABLE_COMPUTE_SHADERS
#include <main/core/color/color.hpp>
#include <main/core/logs/log.hpp>
#include <main/engine/app/app.hpp>

#ifndef SHADERS_HPP
#define SHADERS_HPP

namespace Cherry {
  namespace Shaders {
#define VK_CHECK(x)                                                                             \
  do {                                                                                          \
    VkResult err = (x);                                                                         \
    if (err != VK_SUCCESS) {                                                                    \
      std::string e = std::string("[Vulkan Error] Code: ") + std::to_string(err) + " in " + #x; \
      CH_ERROR_TAG("[Compute shaders]", e.c_str());                                             \
    }                                                                                           \
  } while (0)

    struct ComputeContext {
      VkImage image = VK_NULL_HANDLE;
      VkDeviceMemory imageMemory = VK_NULL_HANDLE;
      VkImageView imageView = VK_NULL_HANDLE;
      VkSampler sampler = VK_NULL_HANDLE;
      VkDescriptorSetLayout descSetLayout = VK_NULL_HANDLE;
      VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
      VkPipeline pipeline = VK_NULL_HANDLE;
      VkDescriptorPool descPool = VK_NULL_HANDLE;
      VkDescriptorSet descSet = VK_NULL_HANDLE;
      VkCommandPool cmdPool = VK_NULL_HANDLE;
      VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
      VkFence fence = VK_NULL_HANDLE;
      ImTextureID imguiTextureID = nullptr;
      uint32_t width = 512;
      uint32_t height = 512;
      bool initialized = false;
      std::string shaderPath;
    };

    // api
    void CleanupVulkanCompute(ComputeContext* ctx);

    // utils
    std::vector<uint32_t> ReadShaderFile(const std::string& filepath);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props);

    template<typename T>
    void InitVulkanCompute(const T& userParams, ComputeContext* ctx) {
      if (!ctx)
        return;

      if (ctx->initialized)
        return;

      VkDevice device = Cherry::Application::GetDevice();
      if (!device) {
        CH_ERROR_TAG("[Compute shaders]", "Vulkan device not available");
        return;
      }

      CH_INFO_TAG("[Compute shaders]", "Initializing Vulkan compute pipeline...");

      VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
      imageInfo.imageType = VK_IMAGE_TYPE_2D;
      imageInfo.extent = { ctx->width, ctx->height, 1 };
      imageInfo.mipLevels = 1;
      imageInfo.arrayLayers = 1;
      imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
      imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageInfo.usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
      imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &ctx->image));

      VkMemoryRequirements memReqs;
      vkGetImageMemoryRequirements(device, ctx->image, &memReqs);

      VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
      allocInfo.allocationSize = memReqs.size;
      allocInfo.memoryTypeIndex = FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &ctx->imageMemory));
      VK_CHECK(vkBindImageMemory(device, ctx->image, ctx->imageMemory, 0));

      VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
      viewInfo.image = ctx->image;
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
      viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      viewInfo.subresourceRange.baseMipLevel = 0;
      viewInfo.subresourceRange.levelCount = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount = 1;

      VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &ctx->imageView));

      VkSamplerCreateInfo samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
      samplerInfo.magFilter = VK_FILTER_LINEAR;
      samplerInfo.minFilter = VK_FILTER_LINEAR;
      samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

      VK_CHECK(vkCreateSampler(device, &samplerInfo, nullptr, &ctx->sampler));

      auto shaderCode = ReadShaderFile(ctx->shaderPath);
      if (shaderCode.empty()) {
        CH_ERROR_TAG("[Compute shaders]", "Cannot load compute shader");
        return;
      }

      VkShaderModuleCreateInfo shaderInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
      shaderInfo.codeSize = shaderCode.size() * sizeof(uint32_t);
      shaderInfo.pCode = shaderCode.data();

      VkShaderModule shaderModule;
      VK_CHECK(vkCreateShaderModule(device, &shaderInfo, nullptr, &shaderModule));

      VkDescriptorSetLayoutBinding binding = {};
      binding.binding = 0;
      binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
      binding.descriptorCount = 1;
      binding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

      VkDescriptorSetLayoutCreateInfo descLayoutInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
      descLayoutInfo.bindingCount = 1;
      descLayoutInfo.pBindings = &binding;

      VK_CHECK(vkCreateDescriptorSetLayout(device, &descLayoutInfo, nullptr, &ctx->descSetLayout));

      VkPushConstantRange pushRange = {};
      pushRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
      pushRange.offset = 0;
      pushRange.size = static_cast<uint32_t>(sizeof(T));

      VkPipelineLayoutCreateInfo pipeLayoutInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
      pipeLayoutInfo.setLayoutCount = 1;
      pipeLayoutInfo.pSetLayouts = &ctx->descSetLayout;
      pipeLayoutInfo.pushConstantRangeCount = 1;
      pipeLayoutInfo.pPushConstantRanges = &pushRange;

      VK_CHECK(vkCreatePipelineLayout(device, &pipeLayoutInfo, nullptr, &ctx->pipelineLayout));

      VkComputePipelineCreateInfo pipelineInfo = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
      pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
      pipelineInfo.stage.module = shaderModule;
      pipelineInfo.stage.pName = "main";
      pipelineInfo.layout = ctx->pipelineLayout;

      VK_CHECK(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ctx->pipeline));

      vkDestroyShaderModule(device, shaderModule, nullptr);

      VkCommandPoolCreateInfo poolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
      poolInfo.queueFamilyIndex = Cherry::Application::GetQueueFamily();
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      VK_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &ctx->cmdPool));

      VkCommandBufferAllocateInfo cmdAllocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
      cmdAllocInfo.commandPool = ctx->cmdPool;
      cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      cmdAllocInfo.commandBufferCount = 1;

      VK_CHECK(vkAllocateCommandBuffers(device, &cmdAllocInfo, &ctx->cmdBuffer));

      VkDescriptorPoolSize poolSize = { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 };
      VkDescriptorPoolCreateInfo descPoolInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
      descPoolInfo.maxSets = 1;
      descPoolInfo.poolSizeCount = 1;
      descPoolInfo.pPoolSizes = &poolSize;

      VK_CHECK(vkCreateDescriptorPool(device, &descPoolInfo, nullptr, &ctx->descPool));

      VkDescriptorSetAllocateInfo descAllocInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
      descAllocInfo.descriptorPool = ctx->descPool;
      descAllocInfo.descriptorSetCount = 1;
      descAllocInfo.pSetLayouts = &ctx->descSetLayout;

      VK_CHECK(vkAllocateDescriptorSets(device, &descAllocInfo, &ctx->descSet));

      VkDescriptorImageInfo imgDescInfo = {};
      imgDescInfo.imageView = ctx->imageView;
      imgDescInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

      VkWriteDescriptorSet writeDesc = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
      writeDesc.dstSet = ctx->descSet;
      writeDesc.dstBinding = 0;
      writeDesc.descriptorCount = 1;
      writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
      writeDesc.pImageInfo = &imgDescInfo;

      vkUpdateDescriptorSets(device, 1, &writeDesc, 0, nullptr);

      VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      VK_CHECK(vkCreateFence(device, &fenceInfo, nullptr, &ctx->fence));

      ctx->imguiTextureID = ImGui_ImplVulkan_AddTexture(ctx->sampler, ctx->imageView, VK_IMAGE_LAYOUT_GENERAL);

      VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      vkBeginCommandBuffer(ctx->cmdBuffer, &beginInfo);

      VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
      barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = ctx->image;
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      barrier.subresourceRange.baseMipLevel = 0;
      barrier.subresourceRange.levelCount = 1;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount = 1;
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

      vkCmdPipelineBarrier(
          ctx->cmdBuffer,
          VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
          VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
          0,
          0,
          nullptr,
          0,
          nullptr,
          1,
          &barrier);

      vkEndCommandBuffer(ctx->cmdBuffer);

      VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &ctx->cmdBuffer;

      vkResetFences(device, 1, &ctx->fence);
      vkQueueSubmit(Cherry::Application::GetVkQueue(), 1, &submitInfo, ctx->fence);
      vkWaitForFences(device, 1, &ctx->fence, VK_TRUE, UINT64_MAX);

      ctx->initialized = true;

      {
        std::string l = "Compute pipeline ready! (" + std::to_string(ctx->width) + "x" + std::to_string(ctx->height) + ")";
        CH_INFO_TAG("[Compute shaders]", l.c_str());
      }
    }

    template<typename T>
    void ExecuteShader(const T& userParams, ComputeContext* ctx) {
      static_assert(sizeof(T) <= 128, "Struct too large for Vulkan push constants");
      if (!ctx || !ctx->initialized)
        return;

      VkDevice device = Cherry::Application::GetDevice();
      vkWaitForFences(device, 1, &ctx->fence, VK_TRUE, UINT64_MAX);
      vkResetFences(device, 1, &ctx->fence);
      vkResetCommandBuffer(ctx->cmdBuffer, 0);

      VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
      vkBeginCommandBuffer(ctx->cmdBuffer, &beginInfo);

      vkCmdBindPipeline(ctx->cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, ctx->pipeline);
      vkCmdBindDescriptorSets(
          ctx->cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, ctx->pipelineLayout, 0, 1, &ctx->descSet, 0, nullptr);

      vkCmdPushConstants(
          ctx->cmdBuffer,
          ctx->pipelineLayout,
          VK_SHADER_STAGE_COMPUTE_BIT,
          0,
          static_cast<uint32_t>(sizeof(T)),
          &userParams);

      uint32_t groupsX = (ctx->width + 15) / 16;
      uint32_t groupsY = (ctx->height + 15) / 16;
      vkCmdDispatch(ctx->cmdBuffer, groupsX, groupsY, 1);

      VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
      barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      barrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = ctx->image;
      barrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

      vkCmdPipelineBarrier(
          ctx->cmdBuffer,
          VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
          VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
          0,
          0,
          nullptr,
          0,
          nullptr,
          1,
          &barrier);

      vkEndCommandBuffer(ctx->cmdBuffer);

      VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &ctx->cmdBuffer;
      vkQueueSubmit(Cherry::Application::GetVkQueue(), 1, &submitInfo, ctx->fence);
    }

  }  // namespace Shaders
}  // namespace Cherry

#endif  // SHADERS_HPP
#endif  // CHERRY_ENABLE_COMPUTE_SHADERS