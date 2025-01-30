#include "app.hpp"
#include "cef.hpp"

#ifdef CHERRY_CEF
namespace Cherry
{
    void UpdateBrowserMouse(ImVec2 windowPos, ImVec2 cursorPos)
    {
        g_windowPos = windowPos;
        g_cursorPos = cursorPos;
    }

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(Cherry::Application::GetVkPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type.");
    }

    void RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
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

    void UpdateCefTexture(const void *buffer, int width, int height)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = width * height * 4;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vkCreateBuffer(Cherry::Application::GetDevice(), &bufferCreateInfo, nullptr, &stagingBuffer);

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(Cherry::Application::GetDevice(), stagingBuffer, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        vkAllocateMemory(Cherry::Application::GetDevice(), &allocInfo, nullptr, &stagingBufferMemory);
        vkBindBufferMemory(Cherry::Application::GetDevice(), stagingBuffer, stagingBufferMemory, 0);

        void *data;
        VkResult result = vkMapMemory(Cherry::Application::GetDevice(), stagingBufferMemory, 0, VK_WHOLE_SIZE, 0, &data);
        if (result != VK_SUCCESS)
        {
            std::cerr << "Failed to map Vulkan memory: " << result << std::endl;
            return;
        }
        memcpy(data, buffer, width * height * 4);
        vkUnmapMemory(Cherry::Application::GetDevice(), stagingBufferMemory);

        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width = width;
        imageCreateInfo.extent.height = height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM; // BGRA Format
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.flags = 0;

        VkImage image;
        if (vkCreateImage(Cherry::Application::GetDevice(), &imageCreateInfo, nullptr, &image) != VK_SUCCESS)
        {
            std::cerr << "Failed to create Vulkan image." << std::endl;
            return;
        }

        VkMemoryRequirements imageMemoryRequirements;
        vkGetImageMemoryRequirements(Cherry::Application::GetDevice(), image, &imageMemoryRequirements);

        VkMemoryAllocateInfo imageAllocInfo = {};
        imageAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        imageAllocInfo.allocationSize = imageMemoryRequirements.size;
        imageAllocInfo.memoryTypeIndex = FindMemoryType(imageMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkDeviceMemory imageMemory;
        vkAllocateMemory(Cherry::Application::GetDevice(), &imageAllocInfo, nullptr, &imageMemory);
        vkBindImageMemory(Cherry::Application::GetDevice(), image, imageMemory, 0);

        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkImageMemoryBarrier imageBarrier = {};
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.image = image;
        imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageBarrier.subresourceRange.baseMipLevel = 0;
        imageBarrier.subresourceRange.levelCount = 1;
        imageBarrier.subresourceRange.baseArrayLayer = 0;
        imageBarrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             0, 0, nullptr, 0, nullptr, 1, &imageBarrier);

        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};

        vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                             0, 0, nullptr, 0, nullptr, 1, &imageBarrier);

        EndSingleTimeCommands(commandBuffer);

        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = image;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        vkCreateImageView(Cherry::Application::GetDevice(), &imageViewCreateInfo, nullptr, &imageView);

        if (imageView == VK_NULL_HANDLE)
        {
            std::cerr << "Failed to create image view." << std::endl;
            return;
        }

        cefTextureId = (ImTextureID)imageView;

        std::cout << " QFS" << std::endl;

        vkDestroyBuffer(Cherry::Application::GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(Cherry::Application::GetDevice(), stagingBufferMemory, nullptr);
    }

    void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
    {
        std::cout << "Buffer received from CEF: " << buffer << ", Width: " << width << ", Height: " << height << std::endl;

        UpdateCefTexture(buffer, width, height);
    }

    void RenderHandler::resize(int w, int h)
    {
        width = w;
        height = h;
    }

    void RenderHandler::render()
    {
        // SDL_RenderCopy(renderer, texture, NULL, NULL);
    }

    void ChangeBrowserURL(char *URL)
    {
        browser->GetMainFrame()->LoadURL(CefString(URL));
    }

    void ShowBrowserWindow(bool *p_open, ImTextureID tex_id)
    {
        //// Menu
        // if (ImGui::BeginMenuBar())
        //{
        //     if (ImGui::BeginMenu("Menu"))
        //     {
        //         //ShowExampleMenuFile();
        //         ImGui::EndMenu();
        //     }
        //     if (ImGui::BeginMenu("Examples"))
        //     {
        //         ImGui::EndMenu();
        //     }
        //     if (ImGui::BeginMenu("Help"))
        //     {
        //         ImGui::EndMenu();
        //     }
        //     ImGui::EndMenuBar();
        // }
        // ImGui::Spacing();

        static char AddressURL[500] = "https://www.google.com";

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
            ImTextureID my_tex_id = tex_id;
            float my_tex_w = 800.0f;
            float my_tex_h = 600.0f;

            ImVec2 curpos = ImGui::GetCursorPos();
            ImVec2 winpos = ImGui::GetWindowPos();

            ImGui::ImageButton(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
            if (ImGui::IsItemHovered())
            {
                UpdateBrowserMouse(winpos, curpos);
                // process mouse data
                // cancel all mouse event
            }
        }
        else
        {
            ImGui::Text("Null");
        }
    }

    void InitCEF(int width, int height)
    {
        renderHandler = new RenderHandler(width, height);

        CefWindowInfo window_info;
        CefBrowserSettings browserSettings;

        // browserSettings.windowless_frame_rate = 60; // 30 is default
        window_info.SetAsWindowless(NULL); // false means no transparency (site background colour)
        browserClient = new BrowserClient(renderHandler);
        browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "https://google.com", browserSettings, nullptr, nullptr);

        CefDoMessageLoopWork();
        CefDoMessageLoopWork();
        CefDoMessageLoopWork();
    }

    void OnCEFFrame()
    {
        CefDoMessageLoopWork();
    }

    int ImGui_ImplSDL2_CefInit(int argc, char **argv)
    {
        CefMainArgs main_args(argc, argv);

        CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
        command_line->InitFromArgv(argc, argv);

        int result = CefExecuteProcess(main_args, nullptr, nullptr);
        if (result >= 0)
        {
            return result;
        }

        CefSettings settings;
        settings.no_sandbox = true;
        settings.command_line_args_disabled = false;
        settings.windowless_rendering_enabled = true;


        CefString(&settings.root_cache_path) = SDL_GetBasePath();
        CefString(&settings.cache_path) = std::string(SDL_GetBasePath()) + "cache/";
        std::ostringstream ss;
        ss << SDL_GetBasePath() << "locales/";

        CefString(&settings.locales_dir_path) = ss.str();
        CefString(&settings.resources_dir_path) = SDL_GetBasePath();
        settings.log_severity = LOGSEVERITY_VERBOSE;
        CefString(&settings.log_file) = "cef_debug.log";

#if !defined(CEF_USE_SANDBOX)
        settings.no_sandbox = true;
#endif

        settings.windowless_rendering_enabled = true;

        if (!CefInitialize(main_args, settings, nullptr, nullptr))
        {
            std::cerr << "Failed to initialize CEF." << std::endl;
            return -1;
        }

        return 0;
    }

}
#endif // CHERRY_CEF