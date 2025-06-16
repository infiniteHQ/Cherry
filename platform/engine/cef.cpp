#include "cef.hpp"
#include "app/app.hpp"
#include <thread>

#ifdef CHERRY_CEF
namespace Cherry {
std::vector<std::shared_ptr<CherryBrowser>> registered_browsers;

void CherryBrowser::UpdateBrowserMouse(ImVec2 windowPos, ImVec2 cursorPos) {
  g_windowPos = windowPos;
  g_cursorPos = cursorPos;
}

void BrowserClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
  rect = CefRect(0, 0, width, height);
}

VkCommandBuffer BeginSingleTimeCommands() {
  VkDevice device = Application::GetDevice();
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

  ImGui_ImplVulkanH_Window *wd =
      &Application::GetCurrentRenderedWindow()->m_WinData;
  VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

  allocInfo.commandPool = command_pool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  return commandBuffer;
}

void EndSingleTimeCommands(VkCommandBuffer commandBuffer) {
  VkDevice device = Application::GetDevice();
  VkQueue graphicsQueue = Application::GetVkQueue();

  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  ImGui_ImplVulkanH_Window *wd =
      &Application::GetCurrentRenderedWindow()->m_WinData;
  VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

  VkFence fence;
  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  vkCreateFence(device, &fenceInfo, nullptr, &fence);

  vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence);
  vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);

  vkDestroyFence(device, fence, nullptr);
  vkFreeCommandBuffers(device, command_pool, 1, &commandBuffer);
}

ImTextureID CherryBrowser::GetCefTexture() { return cefTextureId; }

uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(Application::GetPhysicalDevice(),
                                      &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }
  throw std::runtime_error("Failed to find suitable memory type!");
}

void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer &buffer,
                  VkDeviceMemory &bufferMemory) {
  VkDevice device = Application::GetDevice();

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      FindMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate buffer memory!");
  }

  vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void TransitionImageLayout(VkImage image, VkFormat format,
                           VkImageLayout oldLayout, VkImageLayout newLayout) {
  VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    throw std::invalid_argument("Unsupported layout transition!");
  }

  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0,
                       nullptr, 0, nullptr, 1, &barrier);

  EndSingleTimeCommands(commandBuffer);
}
void FillImageWithRed(VkImage image) {
  VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

  VkClearColorValue clearColor = {1.0f, 0.0f, 0.0f, 1.0f};
  VkImageSubresourceRange range{};
  range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  range.baseMipLevel = 0;
  range.levelCount = 1;
  range.baseArrayLayer = 0;
  range.layerCount = 1;

  vkCmdClearColorImage(commandBuffer, image,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1,
                       &range);

  EndSingleTimeCommands(commandBuffer);
}

void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                       uint32_t height) {
  VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  assert(image != VK_NULL_HANDLE && "Image handle is null!");
  assert(buffer != VK_NULL_HANDLE && "Buffer handle is null!");

  vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  EndSingleTimeCommands(commandBuffer);
}

void BrowserClient::OnPaint(CefRefPtr<CefBrowser> browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList &dirtyRects,
                            const void *buffer, int width, int height) {
  if (type == PET_VIEW) {
    for (auto &browser : registered_browsers) {
      if (browser) {
        if (browser->IsReady()) {
          std::cout << "w : " << browser->m_Width << " /  " << width
                    << std::endl;
          std::cout << "h : " << browser->m_Height << " /  " << height
                    << std::endl;
          browser->UpdateCefTexture(buffer, browser->m_Width,
                                    browser->m_Height);
        } else {
          std::cout << "[WakeUp] Browser not ready, trying to wake it up."
                    << std::endl;
          browser->WakeUp();
        }
      }
    }
  }
}

void CherryBrowser::WakeUp() {
  auto now = std::chrono::steady_clock::now();
  if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastWakeTime)
          .count() < 500)
    return;

  lastWakeTime = now;

  if (m_Browser) {
    auto host = m_Browser->GetHost();
    host->WasResized();
    host->Invalidate(PET_VIEW);
    // host->SendExternalBeginFrame();
  }
}

bool CherryBrowser::IsReady() const {
  if (!m_Browser) {
    std::cout << "Browser isnt valid" << std::endl;
  }

  if (m_Browser->IsLoading()) {
    std::cout << "Browser is loading" << std::endl;
  }

  if (cefTextureId == nullptr) {
    std::cout << "cefTextureId is null:" << std::endl;
  }

  return m_Browser && !m_Browser->IsLoading();
}

void CherryBrowser::UpdateCefTexture(const void *buffer, int width,
                                     int height) {
  VkDevice device = Application::GetDevice();
  vkDeviceWaitIdle(device);

  if (textureImage == VK_NULL_HANDLE || width != m_Width ||
      height != m_Height) {
    if (textureImageView != VK_NULL_HANDLE) {
      vkDestroyImageView(device, textureImageView, nullptr);
      textureImageView = VK_NULL_HANDLE;
    }

    if (textureImage != VK_NULL_HANDLE) {
      vkDestroyImage(device, textureImage, nullptr);
      textureImage = VK_NULL_HANDLE;
    }

    if (textureMemory != VK_NULL_HANDLE) {
      vkFreeMemory(device, textureMemory, nullptr);
      textureMemory = VK_NULL_HANDLE;
    }

    m_Width = width;
    m_Height = height;

    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
    imageCreateInfo.extent = {static_cast<uint32_t>(width),
                              static_cast<uint32_t>(height), 1};
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage =
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    if (vkCreateImage(device, &imageCreateInfo, nullptr, &textureImage) !=
        VK_SUCCESS)
      throw std::runtime_error("Failed to create image!");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, textureImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &textureMemory) !=
        VK_SUCCESS)
      throw std::runtime_error("Failed to allocate image memory!");

    vkBindImageMemory(device, textureImage, textureMemory, 0);

    TransitionImageLayout(textureImage, VK_FORMAT_B8G8R8A8_UNORM,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  }

  VkDeviceSize imageSize = static_cast<VkDeviceSize>(m_Width) * m_Height * 4;
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, buffer, static_cast<size_t>(imageSize));
  vkUnmapMemory(device, stagingBufferMemory);

  CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(m_Width),
                    static_cast<uint32_t>(m_Height));

  TransitionImageLayout(textureImage, VK_FORMAT_B8G8R8A8_UNORM,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  vkDestroyBuffer(device, stagingBuffer, nullptr);
  vkFreeMemory(device, stagingBufferMemory, nullptr);

  if (textureImageView == VK_NULL_HANDLE) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &viewInfo, nullptr, &textureImageView) !=
        VK_SUCCESS)
      throw std::runtime_error("Failed to create image view!");
  }

  cefTextureId =
      ImGui_ImplVulkan_AddTexture(textureSampler, textureImageView,
                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void BrowserClient::resize(int w, int h) {
  width = w;
  height = h;
}

void BrowserClient::render() {
  // SDL_RenderCopy(renderer, texture, NULL, NULL);
}

// CefRefPtr<RenderHandler> renderHandler;

void CherryBrowser::ChangeBrowserURL(char *URL) {
  m_Browser->GetMainFrame()->LoadURL(CefString(URL));
}

int CherryBrowser::GetCefModifiersFromImGui(const ImGuiIO &io) {
  int modifiers = 0;
  if (io.KeyShift)
    modifiers |= EVENTFLAG_SHIFT_DOWN;
  if (io.KeyCtrl)
    modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (io.KeyAlt)
    modifiers |= EVENTFLAG_ALT_DOWN;
  if (io.MouseDown[0])
    modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (io.MouseDown[1])
    modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  if (io.MouseDown[2])
    modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
  return modifiers;
}

void CherryBrowser::ForwardImGuiInputsToCEF(CefRefPtr<CefBrowser> browser,
                                            const ImGuiIO &io, int browserWidth,
                                            int browserHeight) {
  if (!browser)
    return;

  static bool mouseDownPrev[5] = {false};

  ImVec2 mousePos = io.MousePos;

  CefMouseEvent mouse_event;
  mouse_event.x = static_cast<int>(mousePos.x);
  mouse_event.y = static_cast<int>(mousePos.y);
  mouse_event.modifiers = GetCefModifiersFromImGui(io);
  browser->GetHost()->SendMouseMoveEvent(mouse_event, false);

  for (int i = 0; i < 3; ++i) {
    if (io.MouseDown[i] != mouseDownPrev[i]) {
      CefBrowserHost::MouseButtonType btn = MBT_LEFT;
      if (i == 1)
        btn = MBT_MIDDLE;
      else if (i == 2)
        btn = MBT_RIGHT;

      browser->GetHost()->SendMouseClickEvent(mouse_event, btn,
                                              !io.MouseDown[i], 1);
      mouseDownPrev[i] = io.MouseDown[i];
    }
  }

  if (io.MouseWheel != 0.0f || io.MouseWheelH != 0.0f) {
    browser->GetHost()->SendMouseWheelEvent(
        mouse_event, static_cast<int>(io.MouseWheelH * 100),
        static_cast<int>(io.MouseWheel * 100));
  }

  for (int n = 0; n < io.InputQueueCharacters.Size; n++) {
    CefKeyEvent key_event;
    key_event.character = io.InputQueueCharacters[n];
    key_event.windows_key_code = io.InputQueueCharacters[n];
    key_event.native_key_code = 0;
    key_event.type = KEYEVENT_CHAR;
    key_event.modifiers = GetCefModifiersFromImGui(io);
    browser->GetHost()->SendKeyEvent(key_event);
  }

  for (int key = 0; key < IM_ARRAYSIZE(io.KeysDown); ++key) {
    if (io.KeysDown[key]) {
      CefKeyEvent key_event;
      key_event.windows_key_code = key;
      key_event.native_key_code = 0;
      key_event.type = KEYEVENT_RAWKEYDOWN;
      key_event.modifiers = GetCefModifiersFromImGui(io);
      browser->GetHost()->SendKeyEvent(key_event);
    }
  }
}

void CherryBrowser::ShowBrowserWindow(bool *p_open, ImTextureID tex_id) {
  static char AddressURL[500] = "about:blank";

  ImGui::Text("Address:");
  ImGui::SameLine();
  ImGui::InputText("", AddressURL, IM_ARRAYSIZE(AddressURL));
  ImGui::SameLine();
  if (ImGui::Button("Go")) {
    ChangeBrowserURL(AddressURL);
  }

  static float null_tex_timer = 0.0f;

  if (tex_id != nullptr) {
    null_tex_timer = 0.0f;

    ImTextureID my_tex_id = tex_id;
    ImVec2 curpos = ImGui::GetCursorPos();
    ImVec2 winpos = ImGui::GetWindowPos();

    if (m_Browser && m_BrowserClient) {
      m_Browser->GetHost()->Invalidate(PET_VIEW);
    }

    if (tex_id != nullptr && tex_id != (ImTextureID)VK_NULL_HANDLE) {
      ImGui::ImageButton(my_tex_id, ImVec2(m_Width, m_Height), ImVec2(0, 0),
                         ImVec2(1, 1), ImColor(255, 255, 255, 255),
                         ImColor(255, 255, 255, 128));
      if (ImGui::IsItemHovered()) {
        if (ImGui::IsItemHovered()) {
          ForwardImGuiInputsToCEF(m_Browser, ImGui::GetIO(), m_Width, m_Height);
        }
      }
    } else {
      ImGui::Text("Texture is null");
    }
  } else {
    null_tex_timer += ImGui::GetIO().DeltaTime;

    ImVec2 size(m_Width, m_Height);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImU32 col = IM_COL32(50, 50, 50, 255);
    ImGui::GetWindowDrawList()->AddRectFilled(
        pos, ImVec2(pos.x + size.x, pos.y + size.y), col);

    const char *message = "Loading webview...";
    if (null_tex_timer > 4.0f)
      message = "Webview take time to load... But still loading";
    else if (null_tex_timer > 2.0f)
      message = "The webview will comming...";

    ImVec2 text_size = ImGui::CalcTextSize(message);
    float total_content_height = text_size.y;
    float button_width = 150.0f;
    float button_height = 0.0f;

    if (null_tex_timer > 4.0f) {
      button_height = ImGui::GetFrameHeight();
      total_content_height += 10.0f + button_height;
    }

    ImVec2 content_start(pos.x + (size.x - text_size.x) * 0.5f,
                         pos.y + (size.y - total_content_height) * 0.5f);

    ImGui::SetCursorScreenPos(content_start);
    ImGui::TextUnformatted(message);

    if (null_tex_timer > 4.0f) {
      ImVec2 button_pos(pos.x + (size.x - button_width) * 0.5f,
                        content_start.y + text_size.y + 10.0f);
      ImGui::SetCursorScreenPos(button_pos);
      if (ImGui::Button("Open in browser", ImVec2(button_width, 0))) {
        // TODO
      }
    }

    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + size.y));
    ImGui::Dummy(size);
  }
}

/*void RunCEFLoop()
{
    while (runningCEF)
    {
        CefDoMessageLoopWork();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}*/

void CherryBrowser::RunCEFLoop() {
  while (m_BrowserClient->runningCEF) {
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}

void CherryBrowser::OnCEFFrame() { CefDoMessageLoopWork(); }

void CherryBrowser::ShutdownCEF() {
  m_BrowserClient->runningCEF = false;
  if (cefThread.joinable()) {
    cefThread.join();
  }
  CefShutdown();
}

void CherryBrowser::ProcessSDLEvent(const SDL_Event &event) {
  if (!m_Browser || !m_BrowserClient->runningCEF) {
    std::cout << "Browser is null" << std::endl;
    return;
  }

  CefRefPtr<CefBrowserHost> browserHost = m_Browser->GetHost();

  switch (event.type) {
  case SDL_KEYDOWN:
  case SDL_KEYUP: {
    CefKeyEvent keyEvent;
    keyEvent.windows_key_code = event.key.keysym.sym;
    keyEvent.native_key_code = event.key.keysym.sym;
    keyEvent.modifiers = 0; // TODO: Modifiers
    keyEvent.type =
        (event.type == SDL_KEYDOWN) ? KEYEVENT_RAWKEYDOWN : KEYEVENT_KEYUP;

    browserHost->SendKeyEvent(keyEvent);
    break;
  }

  case SDL_TEXTINPUT: {
    CefKeyEvent textEvent;
    textEvent.type = KEYEVENT_CHAR;
    textEvent.windows_key_code = event.text.text[0];
    browserHost->SendKeyEvent(textEvent);
    break;
  }

  case SDL_MOUSEMOTION: {
    CefMouseEvent mouseEvent;
    mouseEvent.x = event.motion.x;
    mouseEvent.y = event.motion.y;
    mouseEvent.modifiers = 0; // TODO: Modifiers

    browserHost->SendMouseMoveEvent(mouseEvent, false);
    break;
  }

  case SDL_MOUSEBUTTONDOWN:
  case SDL_MOUSEBUTTONUP: {
    CefMouseEvent mouseEvent;
    mouseEvent.x = event.button.x;
    mouseEvent.y = event.button.y;

    CefBrowserHost::MouseButtonType btnType;
    switch (event.button.button) {
    case SDL_BUTTON_LEFT:
      btnType = MBT_LEFT;
      break;
    case SDL_BUTTON_RIGHT:
      btnType = MBT_RIGHT;
      break;
    case SDL_BUTTON_MIDDLE:
      btnType = MBT_MIDDLE;
      break;
    default:
      return;
    }

    std::cout << "Send click " << std::endl;

    browserHost->SendMouseClickEvent(mouseEvent, btnType,
                                     (event.type == SDL_MOUSEBUTTONUP), 1);
    break;
  }

  case SDL_MOUSEWHEEL: {
    CefMouseEvent mouseEvent;
    mouseEvent.x = event.wheel.x;
    mouseEvent.y = event.wheel.y;
    mouseEvent.modifiers = 0;

    browserHost->SendMouseWheelEvent(mouseEvent, event.wheel.x * 10,
                                     event.wheel.y * 10);
    break;
  }
  }
}

void CherryBrowser::InitCEF() {
  CefWindowInfo window_info;
  // window_info.shared_texture_enabled = false;
  // window_info.external_begin_frame_enabled = true;
  window_info.windowless_rendering_enabled = true;

  CefBrowserSettings settings;
  settings.windowless_frame_rate = 60;

  CefRefPtr<CefRequestContext> requestContext =
      CefRequestContext::GetGlobalContext();
  m_BrowserClient = new BrowserClient(m_Width, m_Height);

  m_Browser = CefBrowserHost::CreateBrowserSync(
      window_info, m_BrowserClient, m_Url, settings, nullptr, nullptr);

  if (m_Browser) {

    auto host = m_Browser->GetHost();
    host->WasResized();
  }
}

int ImGui_ImplSDL2_CefInit(int argc, char **argv) {
  CefMainArgs args;

  CefSettings settings;
  settings.no_sandbox = true;

  {
    std::ostringstream ss;
    ss << SDL_GetBasePath() << "locales/";
    CefString(&settings.locales_dir_path) = ss.str();
  }

  CefString(&settings.resources_dir_path) = SDL_GetBasePath();

  bool bResult = CefInitialize(args, settings, nullptr, nullptr);
  if (!bResult) {
    return -1;
  }

  return 0;
}

int HandleCEF() {
  CefMainArgs args;

  int exitCode = CefExecuteProcess(args, nullptr, nullptr);
  if (exitCode >= 0) {
    return 0;
  }

  return 1;
}

void BrowserView(const std::string &id, int width, int height,
                 const std::string &url) {
  for (auto browser : registered_browsers) {
    if (browser->m_Id == id) {
      if (browser->m_Width != width || browser->m_Height != height) {
        browser->m_NeedResize = true;
      }

      if (browser->m_Browser) {
        browser->m_Browser->GetHost()->SendExternalBeginFrame();
      }

      if (browser->m_NeedResize) {
        browser->m_Width = width;
        browser->m_Height = height;
        browser->m_BrowserClient->resize(width, height);
        browser->m_Browser->GetHost()->WasResized();
      }

      browser->ShowBrowserWindow(&browser->m_ShowWindow,
                                 browser->GetCefTexture());
      return;
    }
  }
  auto new_browser = std::make_shared<CherryBrowser>(id, width, height, url);
  registered_browsers.push_back(new_browser);
}

} // namespace Cherry
#endif // CHERRY_CEF