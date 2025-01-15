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
        vkGetPhysicalDeviceMemoryProperties(Application::GetPhysicalDevice(), &memProperties); // g_physicalDevice est votre appareil physique Vulkan.

        // Parcourir tous les types de mémoire disponibles
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            // Vérifier si le type de mémoire est compatible avec les propriétés demandées
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }

    // Fonction pour créer une image Vulkan
    void CreateVulkanImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                           VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
    {
        VkImageCreateInfo imageCreateInfo{};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width = width;
        imageCreateInfo.extent.height = height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.format = format;
        imageCreateInfo.tiling = tiling;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage = usage;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.flags = 0;

        if (vkCreateImage(device, &imageCreateInfo, nullptr, &image) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Vulkan image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate memory for Vulkan image!");
        }

        vkBindImageMemory(device, image, imageMemory, 0);
    }

    // Fonction pour créer un ImageView Vulkan
    void CreateVulkanImageView(VkDevice device, VkImage image, VkFormat format, VkImageView &imageView)
    {
        VkImageViewCreateInfo viewCreateInfo{};
        viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCreateInfo.image = image;
        viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewCreateInfo.format = format;
        viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewCreateInfo.subresourceRange.baseMipLevel = 0;
        viewCreateInfo.subresourceRange.levelCount = 1;
        viewCreateInfo.subresourceRange.baseArrayLayer = 0;
        viewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Vulkan image view!");
        }
    }

    void VulkanCreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create buffer");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate buffer memory");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    void VulkanTransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
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

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier);
    }

    void VulkanCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
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

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region);
    }

    void VulkanSubmitCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);
    }

    ImTextureID ImGui_ImplSDL2_GetCefTexture()
    {
        return (ImTextureID)(intptr_t)vulkanImage;
    }

    void RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
	{
		rect = CefRect(0, 0, width, height);
	}

	void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
	{
		// Assurez-vous que ces objets sont initialisés au préalable :
		VkDevice device = Application::GetDevice();									   // Exemple global
		VkCommandBuffer commandBuffer = Application::GetCommandBufferOfWin("0", true); // CommandBuffer global pour l'exemple
		VkQueue graphicsQueue = Application::GetVkQueue();

		// Créez l'image Vulkan pour afficher la texture
		VkDeviceMemory vulkanImageMemory;
		VkImageView vulkanImageView;
		CreateVulkanImage(device, width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
						  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
						  vulkanImage, vulkanImageMemory);

		CreateVulkanImageView(device, vulkanImage, VK_FORMAT_R8G8B8A8_UNORM, vulkanImageView);

		// Taille de la mémoire du buffer en octets
		VkDeviceSize imageSize = width * height * 4; // RGBA (4 bytes par pixel)

		// Création du buffer source temporaire
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		VulkanCreateBuffer(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Copie des données de buffer dans le staging buffer
		void *data;
		vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, buffer, static_cast<size_t>(imageSize));
		vkUnmapMemory(device, stagingBufferMemory);

		// Transition de l'image Vulkan pour qu'elle soit prête à recevoir les données (layout transition)
		VulkanTransitionImageLayout(commandBuffer, vulkanImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copier le staging buffer dans l'image Vulkan
		VulkanCopyBufferToImage(commandBuffer, stagingBuffer, vulkanImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

		// Transition de l'image Vulkan pour qu'elle soit prête à être échantillonnée par le shader
		VulkanTransitionImageLayout(commandBuffer, vulkanImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Soumettre les commandes et attendre leur exécution
		VulkanSubmitCommandBuffer(commandBuffer, graphicsQueue);

		// Nettoyage du buffer temporaire
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
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
		// We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(680, 680), ImGuiCond_FirstUseEver);

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_MenuBar;

		if (!ImGui::Begin("ImGui Browser", p_open, window_flags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

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
		// End of ShowDemoWindow()
		ImGui::End();
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
	}

	void OnCEFFrame()
	{
		CefDoMessageLoopWork();
	}
}
#endif // CHERRY_CEF