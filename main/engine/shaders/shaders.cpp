//
//  shaders.cpp
//  Sources for Cherry shaders API
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "shaders.hpp"

#ifdef CHERRY_ENABLE_COMPUTE_SHADERS
namespace Cherry {
  namespace Shaders {

    void CleanupVulkanCompute(ComputeContext* ctx) {
      if (!ctx)
        return;

      if (!ctx->initialized)
        return;

      VkDevice device = Cherry::Application::GetDevice();
      if (!device)
        return;

      vkDeviceWaitIdle(device);

      if (ctx->imguiTextureID) {
        VkDescriptorSet descSet = (VkDescriptorSet)ctx->imguiTextureID;
        VkDescriptorPool cherryPool = Cherry::Application::GetVkDescriptorPool();
        vkFreeDescriptorSets(device, cherryPool, 1, &descSet);
        ctx->imguiTextureID = nullptr;
      }

      if (ctx->fence)
        vkDestroyFence(device, ctx->fence, nullptr);
      if (ctx->cmdPool)
        vkDestroyCommandPool(device, ctx->cmdPool, nullptr);
      if (ctx->descPool)
        vkDestroyDescriptorPool(device, ctx->descPool, nullptr);
      if (ctx->pipeline)
        vkDestroyPipeline(device, ctx->pipeline, nullptr);
      if (ctx->pipelineLayout)
        vkDestroyPipelineLayout(device, ctx->pipelineLayout, nullptr);
      if (ctx->descSetLayout)
        vkDestroyDescriptorSetLayout(device, ctx->descSetLayout, nullptr);
      if (ctx->sampler)
        vkDestroySampler(device, ctx->sampler, nullptr);
      if (ctx->imageView)
        vkDestroyImageView(device, ctx->imageView, nullptr);
      if (ctx->image)
        vkDestroyImage(device, ctx->image, nullptr);
      if (ctx->imageMemory)
        vkFreeMemory(device, ctx->imageMemory, nullptr);

      ctx->initialized = false;
      CH_INFO_TAG("[Compute shaders]", "Cleanup complete");
    }

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(Cherry::Application::GetPhysicalDevice(), &memProperties);

      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
          return i;
        }
      }
      CH_ERROR_TAG("[Compute shaders]", "[Error] Failed to find suitable memory type");
      return 0;
    }

    std::vector<uint32_t> ReadShaderFile(const std::string& filepath) {
      std::ifstream file;
      std::string foundPath;

      file.open(filepath, std::ios::ate | std::ios::binary);
      if (file.is_open()) {
        foundPath = filepath;
      }

      if (!file.is_open()) {
        std::string l = "Current dir: " + filepath;
        CH_ERROR_TAG("[Compute shaders]", l.c_str());
        return {};
      }

      size_t fileSize = static_cast<size_t>(file.tellg());
      if (fileSize == 0 || fileSize % 4 != 0) {
        std::string l = "Invalid shader file size: " + std::to_string(fileSize);
        CH_ERROR_TAG("[Compute shaders]", l.c_str());
        return {};
      }

      std::vector<uint32_t> buffer(fileSize / 4);
      file.seekg(0);
      file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
      file.close();

      std::string l = "Loaded shader: " + foundPath + " (" + std::to_string(fileSize) + " bytes)";
      CH_INFO_TAG("[Compute shaders]", l.c_str());
      return buffer;
    }

  }  // namespace Shaders
}  // namespace Cherry

#endif  // CHERRY_ENABLE_COMPUTE_SHADERS