#include "app.hpp"
#include "cef.hpp"
#include <thread>

#ifdef CHERRY_CEF
namespace Cherry
{
    void UpdateBrowserMouse(ImVec2 windowPos, ImVec2 cursorPos)
    {
        g_windowPos = windowPos;
        g_cursorPos = cursorPos;
    }

    void BrowserClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
    {
        rect = CefRect(0, 0, width, height);
    }

    VkCommandBuffer BeginSingleTimeCommands()
    {
        ImGui_ImplVulkanH_Window *wd = &Cherry::GetCurrentRenderedWindow()->m_WinData;

        VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = command_pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(Cherry::Application::GetVkDevice(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        beginInfo.pInheritanceInfo = nullptr;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void EndSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        ImGui_ImplVulkanH_Window *wd = &Cherry::GetCurrentRenderedWindow()->m_WinData;

        VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(Cherry::Application::GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(Cherry::Application::GetVkQueue());

        vkFreeCommandBuffers(Cherry::Application::GetVkDevice(), command_pool, 1, &commandBuffer);
    }

    ImTextureID ImGui_ImplSDL2_GetCefTexture()
    {
        return cefTextureId;
    }

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(Application::GetPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }
        throw std::runtime_error("Failed to find suitable memory type!");
    }

    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
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

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            throw std::invalid_argument("Unsupported layout transition!");
        }

        vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        EndSingleTimeCommands(commandBuffer);
    }
    void FillImageWithRed(VkImage image)
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkClearColorValue clearColor = {1.0f, 0.0f, 0.0f, 1.0f};
        VkImageSubresourceRange range{};
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = 0;
        range.levelCount = 1;
        range.baseArrayLayer = 0;
        range.layerCount = 1;

        vkCmdClearColorImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &range);

        EndSingleTimeCommands(commandBuffer);
    }

    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer &buffer, VkDeviceMemory &bufferMemory)
    {
        VkDevice device = Application::GetDevice();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
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

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        EndSingleTimeCommands(commandBuffer);
    }

    void UpdateCefTexture(const void *buffer, int width, int height)
    {
        VkDevice device = Application::GetDevice();
        VkPhysicalDevice physicalDevice = Application::GetPhysicalDevice();
        VkQueue graphicsQueue = Application::GetVkQueue();

        if (textureImage == VK_NULL_HANDLE)
        {
            VkImageCreateInfo imageCreateInfo{};
            imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            imageCreateInfo.extent.width = width;
            imageCreateInfo.extent.height = height;
            imageCreateInfo.extent.depth = 1;
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

            if (vkCreateImage(device, &imageCreateInfo, nullptr, &textureImage) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create image!");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(device, textureImage, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            if (vkAllocateMemory(device, &allocInfo, nullptr, &textureMemory) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to allocate image memory!");
            }

            vkBindImageMemory(device, textureImage, textureMemory, 0);

            TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM,
                                  VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VkDeviceSize imageSize = width * height * 4;

        CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, buffer, static_cast<size_t>(imageSize));
        vkUnmapMemory(device, stagingBufferMemory);

        CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

        TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

        if (textureImageView == VK_NULL_HANDLE)
        {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = textureImage;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &viewInfo, nullptr, &textureImageView) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create image view!");
            }
        }

        // 7️⃣ Mettre à jour ImGui avec la texture CEF
        if (cefTextureId == nullptr)
        {
            cefTextureId = ImGui_ImplVulkan_AddTexture(textureSampler, textureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
    }

    void BrowserClient::resize(int w, int h)
    {
        width = w;
        height = h;
    }

    void BrowserClient::render()
    {

        std::cout << "render" << std::endl;
        // SDL_RenderCopy(renderer, texture, NULL, NULL);
    }

    CefRefPtr<CefBrowser> browser;
    CefRefPtr<BrowserClient> browserClient;
    // CefRefPtr<RenderHandler> renderHandler;

    void ChangeBrowserURL(char *URL)
    {
        browser->GetMainFrame()->LoadURL(CefString(URL));
    }

    void ShowBrowserWindow(bool *p_open, ImTextureID tex_id)
    {
        static char AddressURL[500] = "about:blank";

        ImGui::Text("Address:");
        ImGui::SameLine();
        ImGui::InputText("", AddressURL, IM_ARRAYSIZE(AddressURL));
        ImGui::SameLine();
        if (ImGui::Button("Go"))
        {
            // switch the url
            ChangeBrowserURL(AddressURL);
        }

        if (tex_id != nullptr)
        {
            std::cout << "NOT NULL" << std::endl;
            std::cout << &tex_id << std::endl;
            ImTextureID my_tex_id = tex_id;
            float my_tex_w = 500.0f;
            float my_tex_h = 500.0f;

            ImVec2 curpos = ImGui::GetCursorPos();
            ImVec2 winpos = ImGui::GetWindowPos();

            std::cout << "render2" << std::endl;
            if (tex_id != nullptr && tex_id != (ImTextureID)VK_NULL_HANDLE)
            {
                ImGui::Image(tex_id, ImVec2(500, 500));
            }
            else
            {
                ImGui::Text("Texture is null");
            }
            std::cout << "render3" << std::endl;

            /*ImGui::ImageButton(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
            if (ImGui::IsItemHovered())
            {
                UpdateBrowserMouse(winpos, curpos);
                // process mouse data
                // cancel all mouse event
            }*/
            std::cout << "RENDERED" << std::endl;
        }
        else
        {
            ImGui::Text("Null");
        }
    }

    void InitCEF(int width, int height)
    {
        CefWindowInfo window_info;
        // window_info.shared_texture_enabled = true;
        // window_info.external_begin_frame_enabled = true;
        window_info.windowless_rendering_enabled = true;

        CefBrowserSettings settings;
        // settings.windowless_frame_rate = 60;

        CefRefPtr<CefRequestContext> requestContext = CefRequestContext::GetGlobalContext();
        CefRefPtr<BrowserClient> browserClient = new BrowserClient(width, height);

        std::string test_url = "data:text/html,<html><body><h1>Hello, World!</h1></body></html>";
        browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient, test_url, settings, nullptr, nullptr);

        if (browser)
        {
            std::cout << "🌐 Browser created successfully!" << std::endl;
        }
        else
        {
            std::cerr << "❌ Failed to create browser!" << std::endl;
        }
    }

    void OnCEFFrame()
    {
        CefDoMessageLoopWork();
    }

    int ImGui_ImplSDL2_CefInit(int argc, char **argv)
    {
        CefMainArgs main_args;

        CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
        command_line->InitFromArgv(argc, argv);

        int result = CefExecuteProcess(main_args, nullptr, nullptr);
        if (result >= 0)
        {
            return result;
        }

        CefSettings settings;
        // settings.no_sandbox = true;
        settings.multi_threaded_message_loop = false;
        settings.windowless_rendering_enabled = true;

        CefRunMessageLoop();

        CefString(&settings.locales_dir_path) = Cherry::GetPath("locales/").c_str();

        std::cout << "CefInitialize" << std::endl;

        if (!CefInitialize(main_args, settings, nullptr, nullptr))
        {
            std::cerr << "Failed to initialize CEF." << std::endl;
            return -1;
        }

        std::cout << " Initialized" << std::endl;
        return 0;
    }

}
#endif // CHERRY_CEF