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

    void BrowserClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
    {
        std::cout << "📏 GetViewRect called: " << width << "x" << height << std::endl;
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

        auto &device = Application::GetVkDevice();
        auto &physicalDevice = Application::GetVkPhysicalDevice();
        auto &queue = Application::GetVkQueue();
        auto commandBuffer = Application::GetCommandBuffer(true, Application::GetCurrentRenderedWindow());

        // Détruire les ressources précédentes
        vkDeviceWaitIdle(device); // Attendre que toutes les opérations soient terminées avant destruction
        if (cefImage != VK_NULL_HANDLE)
        {
            vkDestroyImageView(device, cefImageView, nullptr);
            vkDestroyImage(device, cefImage, nullptr);
            vkFreeMemory(device, cefImageMemory, nullptr);
            vkDestroySampler(device, cefSampler, nullptr);
        }

        // Création d'une image Vulkan
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        vkCreateImage(device, &imageInfo, nullptr, &cefImage);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, cefImage, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;

        uint32_t memoryTypeIndex;
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((memRequirements.memoryTypeBits & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
            {
                memoryTypeIndex = i;
                break;
            }
        }

        allocInfo.memoryTypeIndex = memoryTypeIndex;

        vkAllocateMemory(device, &allocInfo, nullptr, &cefImageMemory);
        vkBindImageMemory(device, cefImage, cefImageMemory, 0);

        VkCommandBuffer cmdBuffer = commandBuffer;

        // Barrière pour transition de VK_IMAGE_LAYOUT_UNDEFINED vers VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = cefImage;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                             0, nullptr, 0, nullptr, 1, &barrier);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                             0, nullptr, 0, nullptr, 1, &barrier);

        // Création de l'image view
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = cefImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        vkCreateImageView(device, &viewInfo, nullptr, &cefImageView);

        // Création du sampler
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        vkCreateSampler(device, &samplerInfo, nullptr, &cefSampler);

        // Associer la texture ImGui à l'image Vulkan
        ImGui_ImplVulkan_AddTexture(cefSampler, cefImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        cefTextureId = (ImTextureID)cefImageView;

        if (cefTextureId == nullptr || cefTextureId == (ImTextureID)VK_NULL_HANDLE)
        {
            std::cerr << "❌ Texture ID is null!" << std::endl;
        }
        else
        {
            std::cout << "✅ Texture assigned successfully!" << std::endl;
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

            if (tex_id != nullptr && tex_id != (ImTextureID)VK_NULL_HANDLE)
            {
                //ImGui::Image(tex_id, ImVec2(500, 500));
            }
            else
            {
                ImGui::Text("Texture is null");
            }

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
        settings.windowless_frame_rate = 60;

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
        // settings.multi_threaded_message_loop = false;
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