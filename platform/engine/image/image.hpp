#pragma once

#include <string>

#include "../../../lib/imgui/backends/imgui_impl_sdl.h"
#include "../../../lib/imgui/backends/imgui_impl_vulkan.h"
#include "../../../lib/imgui/imgui.h"
#include "../../../lib/imgui/imgui_internal.h"
#include "../../../lib/json/single_include/nlohmann/json.hpp"
#include "vulkan/vulkan.h"

namespace Cherry {
enum class ImageFormat {
  None = 0,

  RGBA,
  RGBA32F
};

class Image {
public:
  Image(std::string_view path, const std::string &winname);
  Image(uint32_t width, uint32_t height, ImageFormat format,
        const std::string &winname, const void *data = nullptr);
  ~Image();

  // void SetData(const void* data);
  void SetData(const void *data, const std::string &winname);
  void SetData(const void *ImGuiDataType);

  VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }

  ImTextureID GetImGuiTextureID(
      VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) const {
    return ImGui_ImplVulkan_AddTexture(m_Sampler, m_ImageView, layout);
  }

  void Resize(uint32_t width, uint32_t height);

  uint32_t GetWidth() const { return m_Width; }
  uint32_t GetHeight() const { return m_Height; }

  static void *Decode(const void *data, uint64_t length, uint32_t &outWidth,
                      uint32_t &outHeight);

private:
  void AllocateMemory(uint64_t size);
  void Release();

private:
  uint32_t m_Width = 0, m_Height = 0;
  VkSemaphore imageReadySemaphore;
  VkImage m_Image = nullptr;
  VkImageView m_ImageView = nullptr;
  VkDeviceMemory m_Memory = nullptr;
  VkSampler m_Sampler = nullptr;

  ImageFormat m_Format = ImageFormat::None;

  VkBuffer m_StagingBuffer = nullptr;
  VkDeviceMemory m_StagingBufferMemory = nullptr;

  size_t m_AlignedSize = 0;

  VkDescriptorSet m_DescriptorSet = nullptr;
  std::string m_Winname;

  std::string m_Filepath;
};

} // namespace Cherry
