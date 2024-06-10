#include "ApplicationGUI.h"
#include "../../../src/core/Log.h"

//
// Adapted from Dear ImGui Vulkan example
//

/*
TODO :

Move, Resize, Click (click callback, and link to imgui ctx ?)
Window factory to spawn window when undock
Cool widgets & ui elements !

*/

#include <stdio.h>	// printf, fprintf
#include <thread>	// thread
#include <stdlib.h> // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

#include <iostream>

// Emedded font
#include "ImGui/Roboto-Regular.embed"
#include "ImGui/Roboto-Bold.embed"
#include "ImGui/Roboto-Italic.embed"
#include "ImGui/Hack-Regular.embed"
#include "ImGui/Inconsolatas.embed"

extern bool g_ApplicationRunning;

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// #define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

/*static VkAllocationCallbacks *win->g_Allocator = NULL;
static VkInstance win->g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice win->g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice win->g_Device = VK_NULL_HANDLE;
static uint32_t win->g_QueueFamily = (uint32_t)-1;
static VkQueue g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT win->g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache win->g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool win->g_DescriptorPool = VK_NULL_HANDLE;*/

// Per-frame-in-flight
// static std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
// static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;

// Unlike g_MainWindowData.FrameIndex, this is not the the swapchain image index
// and is always guaranteed to increase (eg. 0, 1, 2, 0, 1, 2)
// static uint32_t s_CurrentFrameIndex = 0;

static std::unordered_map<std::string, ImFont *> s_Fonts;

static UIKit::Application *s_Instance = nullptr;

void check_vk_result(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData)
{
	(void)flags;
	(void)object;
	(void)location;
	(void)messageCode;
	(void)pUserData;
	(void)pLayerPrefix; // Unused arguments
						//	fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
	return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT

static void SetupVulkan(const char **extensions, uint32_t extensions_count, UIKit::Window *win)
{
	VkResult err;

	// Create Vulkan Instance
	{
		VkInstanceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.enabledExtensionCount = extensions_count;
		create_info.ppEnabledExtensionNames = extensions;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
		// Enabling validation layers
		const char *layers[] = {"VK_LAYER_KHRONOS_validation"};
		create_info.enabledLayerCount = 1;
		create_info.ppEnabledLayerNames = layers;

		// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
		const char **extensions_ext = (const char **)malloc(sizeof(const char *) * (extensions_count + 1));
		memcpy(extensions_ext, extensions, extensions_count * sizeof(const char *));
		extensions_ext[extensions_count] = "VK_EXT_debug_report";
		create_info.enabledExtensionCount = extensions_count + 1;
		create_info.ppEnabledExtensionNames = extensions_ext;

		// Create Vulkan Instance
		err = vkCreateInstance(&create_info, win->g_Allocator, &win->g_Instance);
		check_vk_result(err);
		free(extensions_ext);

		// Get the function pointer (required for any extensions)
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(win->g_Instance, "vkCreateDebugReportCallbackEXT");
		IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

		// Setup the debug report callback
		VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
		debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		debug_report_ci.pfnCallback = debug_report;
		debug_report_ci.pUserData = NULL;
		err = vkCreateDebugReportCallbackEXT(win->g_Instance, &debug_report_ci, win->g_Allocator, &win->g_DebugReport);
		check_vk_result(err);
#else
		// Create Vulkan Instance without any debug feature
		err = vkCreateInstance(&create_info, win->g_Allocator, &win->g_Instance);
		check_vk_result(err);
		IM_UNUSED(win->g_DebugReport);
#endif
	}

	// Select GPU
	{
		uint32_t gpu_count;
		err = vkEnumeratePhysicalDevices(win->g_Instance, &gpu_count, NULL);
		check_vk_result(err);
		IM_ASSERT(gpu_count > 0);

		VkPhysicalDevice *gpus = (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * gpu_count);
		err = vkEnumeratePhysicalDevices(win->g_Instance, &gpu_count, gpus);
		check_vk_result(err);

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		int use_gpu = 0;
		for (int i = 0; i < (int)gpu_count; i++)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(gpus[i], &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				use_gpu = i;
				break;
			}
		}

		win->g_PhysicalDevice = gpus[use_gpu];
		free(gpus);
	}

	// Select graphics queue family
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(win->g_PhysicalDevice, &count, NULL);
		VkQueueFamilyProperties *queues = (VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * count);
		vkGetPhysicalDeviceQueueFamilyProperties(win->g_PhysicalDevice, &count, queues);
		for (uint32_t i = 0; i < count; i++)
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				win->g_QueueFamily = i;
				break;
			}
		free(queues);
		IM_ASSERT(win->g_QueueFamily != (uint32_t)-1);
	}

	// Create Logical Device (with 1 queue)
	{
		int device_extension_count = 1;
		const char *device_extensions[] = {"VK_KHR_swapchain"};
		const float queue_priority[] = {1.0f};
		VkDeviceQueueCreateInfo queue_info[1] = {};
		queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info[0].queueFamilyIndex = win->g_QueueFamily;
		queue_info[0].queueCount = 1;
		queue_info[0].pQueuePriorities = queue_priority;
		VkDeviceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
		create_info.pQueueCreateInfos = queue_info;
		create_info.enabledExtensionCount = device_extension_count;
		create_info.ppEnabledExtensionNames = device_extensions;
		err = vkCreateDevice(win->g_PhysicalDevice, &create_info, win->g_Allocator, &win->g_Device);
		check_vk_result(err);
		vkGetDeviceQueue(win->g_Device, win->g_QueueFamily, 0, &win->g_Queue);
	}

	// Create Descriptor Pool
	{
		VkDescriptorPoolSize pool_sizes[] =
			{
				{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		err = vkCreateDescriptorPool(win->g_Device, &pool_info, win->g_Allocator, &win->g_DescriptorPool);
		check_vk_result(err);
	}
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height, UIKit::Window *win)
{
	wd->Surface = surface;

	// Check for WSI support
	VkBool32 res;
	vkGetPhysicalDeviceSurfaceSupportKHR(win->g_PhysicalDevice, win->g_QueueFamily, wd->Surface, &res);
	if (res != VK_TRUE)
	{
		fprintf(stderr, "Error no WSI support on physical device 0\n");
		exit(-1);
	}

	// Select Surface Format
	const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(win->g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

	// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
	VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
#else
	VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
	wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(win->g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
	// printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);
	// Create SwapChain, RenderPass, Framebuffer, etc.
	IM_ASSERT(win->g_MinImageCount >= 2);
	ImGui_ImplVulkanH_CreateOrResizeWindow(win->g_Instance, win->g_PhysicalDevice, win->g_Device, wd, win->g_QueueFamily, win->g_Allocator, width, height, win->g_MinImageCount);
}

static void CleanupVulkan(UIKit::Window *win)
{
	vkDestroyDescriptorPool(win->g_Device, win->g_DescriptorPool, win->g_Allocator);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
	// Remove the debug report callback
	auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(win->g_Instance, "vkDestroyDebugReportCallbackEXT");
	vkDestroyDebugReportCallbackEXT(win->g_Instance, win->g_DebugReport, win->g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

	vkDestroyDevice(win->g_Device, win->g_Allocator);
	vkDestroyInstance(win->g_Instance, win->g_Allocator);
}

static UIKit::Application *app;

static void CleanupVulkanWindow(UIKit::Window *win)
{
	ImGui_ImplVulkanH_DestroyWindow(win->g_Instance, win->g_Device, &win->m_WinData, win->g_Allocator);
}

static void CleanupSpecificVulkanWindow(UIKit::Window *win)
{
	ImGui_ImplVulkanH_DestroyWindow(win->g_Instance, win->g_Device, &win->m_WinData, win->g_Allocator);
}

static void FrameRender(ImGui_ImplVulkanH_Window *wd, UIKit::Window *win, ImDrawData *draw_data)
{
	VkResult err;

	VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	err = vkAcquireNextImageKHR(win->g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);

	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		win->g_SwapChainRebuild = true;
		return;
	}
	check_vk_result(err);

	win->s_CurrentFrameIndex = (win->s_CurrentFrameIndex + 1) % win->m_WinData.ImageCount;

	ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];
	{
		err = vkWaitForFences(win->g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);
		check_vk_result(err);

		err = vkResetFences(win->g_Device, 1, &fd->Fence);
		check_vk_result(err);
	}

	{
		for (auto &func : win->s_ResourceFreeQueue[win->m_WinData.FrameIndex])
			func();
		win->s_ResourceFreeQueue[win->m_WinData.FrameIndex].clear();
	}
	{
		auto &allocatedCommandBuffers = win->s_AllocatedCommandBuffers[wd->FrameIndex];
		if (allocatedCommandBuffers.size() > 0)
		{
			vkFreeCommandBuffers(win->g_Device, fd->CommandPool, (uint32_t)allocatedCommandBuffers.size(), allocatedCommandBuffers.data());
			allocatedCommandBuffers.clear();
		}

		err = vkResetCommandPool(win->g_Device, fd->CommandPool, 0);
		check_vk_result(err);
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		check_vk_result(err);
	}
	{
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = wd->RenderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = wd->Width;
		info.renderArea.extent.height = wd->Height;
		info.clearValueCount = 1;
		info.pClearValues = &wd->ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &image_acquired_semaphore;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &render_complete_semaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		check_vk_result(err);
		err = vkQueueSubmit(win->g_Queue, 1, &info, fd->Fence);
		check_vk_result(err);
	}
}

static void FramePresent(ImGui_ImplVulkanH_Window *wd, UIKit::Window *win)
{
	if (win->g_SwapChainRebuild)
		return;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &render_complete_semaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &wd->Swapchain;
	info.pImageIndices = &wd->FrameIndex;
	VkResult err = vkQueuePresentKHR(win->g_Queue, &info);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		win->g_SwapChainRebuild = true;
		return;
	}
	check_vk_result(err);
	wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
}

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace UIKit
{

#include "Embed/UIKit-Icon.embed"
#include "Embed/WindowImages.embed"

	Application::Application(const ApplicationSpecification &specification)
		: m_Specification(specification)
	{
		s_Instance = this;

		Init();
	}

	Application::~Application()
	{
		Shutdown();

		s_Instance = nullptr;
	}

	Application &Application::Get()
	{
		return *s_Instance;
	}

	void Application::Init()
	{
		app = &this->Get();
		this->m_Windows.push_back(std::make_shared<Window>("base", 1280, 720));
		// this->m_Windows.push_back(std::make_shared<Window>("baseadditionnal", 600, 650));
		// this->m_Windows.push_back(std::make_shared<Window>("baseadditiqsdonnal", 1200, 650));
	}

	void Application::Shutdown()
	{
		for (auto &layer : m_LayerStack)
			layer->OnDetach();

		m_LayerStack.clear();

		// Release resources
		// NOTE(Yan): to avoid doing this manually, we shouldn't
		//            store resources in this Application class
		// m_AppHeaderIcon.reset();
		// m_IconClose.reset();
		// m_IconMinimize.reset();
		// m_IconMaximize.reset();
		// m_IconRestore.reset();

		// Cleanup
		for (auto win : app->m_Windows)
		{
			VkResult err = vkDeviceWaitIdle(win->g_Device);
			check_vk_result(err);

			// Free resources in queue
			for (auto &queue : win->s_ResourceFreeQueue)
			{
				for (auto &func : queue)
					func();
			}
			win->s_ResourceFreeQueue.clear();
		}

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Cleanup
		for (auto win : app->m_Windows)
		{
			CleanupSpecificVulkanWindow(win.get());
			CleanupVulkan(win.get());
		}

		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();

		g_ApplicationRunning = false;

		Log::Shutdown();
	}
	void Window::UI_DrawTitlebar(float &outTitlebarHeight)
	{
		const float titlebarHeight = 58.0f;

		const bool isMaximized = (bool)glfwGetWindowAttrib(this->GetWindowHandle(), GLFW_MAXIMIZED);
		;
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = {ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									ImGui::GetCursorScreenPos().y + titlebarHeight};
		auto *bgDrawList = ImGui::GetBackgroundDrawList();
		auto *fgDrawList = ImGui::GetForegroundDrawList();
		bgDrawList->AddRectFilled(titlebarMin, titlebarMax, UI::Colors::Theme::titlebar);
		// DEBUG TITLEBAR BOUNDS
		// fgDrawList->AddRect(titlebarMin, titlebarMax, UI::Colors::Theme::invalidPrefab);

		// Logo
		{
			const int logoWidth = 48;  // m_LogoTex->GetWidth();
			const int logoHeight = 48; // m_LogoTex->GetHeight();
			const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = {ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y};
			const ImVec2 logoRectMax = {logoRectStart.x + logoWidth, logoRectStart.y + logoHeight};
			fgDrawList->AddImage(m_AppHeaderIcon->GetDescriptorSet(), logoRectStart, logoRectMax);
		}

		ImGui::SetItemAllowOverlap();
		ImGui::BeginHorizontal("Titlebar", {ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing()});

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;

		// Draw Menubar
		if (app->m_MenubarCallback)
		{
			ImGui::SuspendLayout();
			{
				ImGui::SetItemAllowOverlap();
				const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
				ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
				UI_DrawMenubar();
			}

			ImGui::ResumeLayout();
		}

		{
			// Centered Window title
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize(app->m_Specification.Name.c_str());
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", app->m_Specification.Name.c_str()); // Draw title
			ImGui::SetCursorPos(currentCursorPos);
		}

		// Window buttons
		const ImU32 buttonColN = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 0.9f);
		const ImU32 buttonColH = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.2f);
		const ImU32 buttonColP = UI::Colors::Theme::textDarker;
		const float buttonWidth = 14.0f;
		const float buttonHeight = 14.0f;

		if (app->m_Specification.CustomTitlebar)
		{
			// Minimize Button
			ImGui::Spring();
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = this->m_IconMinimize->GetWidth();
				const int iconHeight = this->m_IconMinimize->GetHeight();
				const float padY = (buttonHeight - (float)iconHeight) / 2.0f;

				{
					std::string label = "Minimize###" + this->GetName();
					if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
					{
						// TODO: move this stuff to a better place, like Window class
						if (this->GetWindowHandle())
						{
							this->QueueEvent([windowHandle = this->GetWindowHandle()]()
											 { glfwIconifyWindow(windowHandle); });
						}
					}
				}

				UI::DrawButtonImage(this->m_IconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
			}
			// Maximize Button
			ImGui::Spring(-1.0f, 17.0f);
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = this->m_IconMaximize->GetWidth();
				const int iconHeight = this->m_IconMaximize->GetHeight();

				const bool isMaximized = (bool)glfwGetWindowAttrib(this->GetWindowHandle(), GLFW_MAXIMIZED);

				{

					std::string label = "Maximize###" + this->GetName();
					if (ImGui::InvisibleButton(label.c_str(), ImVec2(buttonWidth, buttonHeight)))
					{
						this->QueueEvent([isMaximized, windowHandle = this->GetWindowHandle()]()
										 {
					if (isMaximized)
						glfwRestoreWindow(windowHandle);
					else
						glfwMaximizeWindow(windowHandle); });
					}
				}
				UI::DrawButtonImage(isMaximized ? this->m_IconRestore : this->m_IconMaximize, buttonColN, buttonColH, buttonColP);
			}
			{
				// Close Button
				ImGui::Spring(-1.0f, 15.0f);
				UI::ShiftCursorY(8.0f);
				{
					const int iconWidth = this->m_IconClose->GetWidth();
					const int iconHeight = this->m_IconClose->GetHeight();
					std::string label = "Close###" + this->GetName();
					if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
						Application::Get().Close();

					UI::DrawButtonImage(this->m_IconClose, UI::Colors::Theme::text, UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.4f), buttonColP);
				}
			}
			ImGui::Spring(-1.0f, 18.0f);
		}
		ImGui::EndHorizontal();

		outTitlebarHeight = titlebarHeight;
	}

	// REndre la title bar moovable comme le reste, fix l'erreur de double main win et rendre la fenetre dco:mplete
	void Application::UI_DrawTitlebar(float &outTitlebarHeight, Window *window)
	{
		const float titlebarHeight = 58.0f;
		const bool isMaximized = (bool)glfwGetWindowAttrib(window->GetWindowHandle(), GLFW_MAXIMIZED);
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = {ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									ImGui::GetCursorScreenPos().y + titlebarHeight};
		auto *bgDrawList = ImGui::GetBackgroundDrawList();
		auto *fgDrawList = ImGui::GetForegroundDrawList();
		bgDrawList->AddRectFilled(titlebarMin, titlebarMax, UI::Colors::Theme::titlebar);
		// DEBUG TITLEBAR BOUNDS
		// fgDrawList->AddRect(titlebarMin, titlebarMax, UI::Colors::Theme::invalidPrefab);

		// Logo
		{
			const int logoWidth = 48;  // m_LogoTex->GetWidth();
			const int logoHeight = 48; // m_LogoTex->GetHeight();
			const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = {ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y};
			const ImVec2 logoRectMax = {logoRectStart.x + logoWidth, logoRectStart.y + logoHeight};

			fgDrawList->AddImage(window->m_AppHeaderIcon->GetDescriptorSet(), logoRectStart, logoRectMax);
		}

		ImGui::BeginHorizontal("Titlebar", {ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing()});

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;
		// Title bar drag area
		// On Windows we hook into the GLFW win32 window internals
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset)); // Reset cursor pos
		// DEBUG DRAG BOUNDS
		// fgDrawList->AddRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + w - buttonsAreaWidth, ImGui::GetCursorScreenPos().y + titlebarHeight), UI::Colors::Theme::invalidPrefab);
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		m_TitleBarHovered = ImGui::IsItemHovered();

		// Inside your Application class

		// Initialize a static variable to keep track of click state
		static bool isFirstClick = true;
		static double lastClickTime = -1;

		// Inside your ImGui rendering code

		// Initialize a static variable to keep track of dragging state
		static bool isDraggingWindow = false;
		static ImVec2 windowDragStartPos;

		int windowPosX, windowPosY;
		glfwGetWindowPos(window->GetWindowHandle(), &windowPosX, &windowPosY);
		static ImVec2 windowPos{static_cast<float>(windowPosX), static_cast<float>(windowPosY)};

		// Title bar drag area
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset)); // Reset cursor pos
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		m_TitleBarHovered = ImGui::IsItemHovered();

		if (isMaximized)
		{
			float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
			if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
				m_TitleBarHovered = true; // Account for the top-most pixels which don't register
		}

		// Draw Menubar
		if (m_MenubarCallback)
		{
			ImGui::SuspendLayout();
			{
				ImGui::SetItemAllowOverlap();
				const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
				ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
				UI_DrawMenubar();

				if (ImGui::IsItemHovered())
					m_TitleBarHovered = false;
			}

			ImGui::ResumeLayout();
		}

		{
			// Centered Window title
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize(m_Specification.Name.c_str());
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", m_Specification.Name.c_str()); // Draw title
			ImGui::SetCursorPos(currentCursorPos);
		}

		// Window buttons
		const ImU32 buttonColN = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 0.9f);
		const ImU32 buttonColH = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.2f);
		const ImU32 buttonColP = UI::Colors::Theme::textDarker;
		const float buttonWidth = 14.0f;
		const float buttonHeight = 14.0f;

		if (m_Specification.CustomTitlebar)
		{
			// Minimize Button
			ImGui::Spring();
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = window->m_IconMinimize->GetWidth();
				const int iconHeight = window->m_IconMinimize->GetHeight();
				const float padY = (buttonHeight - (float)iconHeight) / 2.0f;
				if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
				{
					// TODO: move this stuff to a better place, like Window class
					if (window->GetWindowHandle())
					{
						window->QueueEvent([windowHandle = window->GetWindowHandle()]()
										   { glfwIconifyWindow(windowHandle); });
					}
				}

				UI::DrawButtonImage(window->m_IconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
			}

			// Maximize Button
			ImGui::Spring(-1.0f, 17.0f);
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = window->m_IconMaximize->GetWidth();
				const int iconHeight = window->m_IconMaximize->GetHeight();

				const bool isMaximized = (bool)glfwGetWindowAttrib(window->GetWindowHandle(), GLFW_MAXIMIZED);

				if (ImGui::InvisibleButton("Maximize", ImVec2(buttonWidth, buttonHeight)))
				{
					window->QueueEvent([isMaximized, windowHandle = window->GetWindowHandle()]()
									   {
					if (isMaximized)
						glfwRestoreWindow(windowHandle);
					else
						glfwMaximizeWindow(windowHandle); });
				}
				UI::DrawButtonImage(isMaximized ? window->m_IconRestore : window->m_IconMaximize, buttonColN, buttonColH, buttonColP);
			}

			// Close Button
			ImGui::Spring(-1.0f, 15.0f);
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = window->m_IconClose->GetWidth();
				const int iconHeight = window->m_IconClose->GetHeight();
				if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
					Application::Get().Close();

				UI::DrawButtonImage(window->m_IconClose, UI::Colors::Theme::text, UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.4f), buttonColP);
			}

			ImGui::Spring(-1.0f, 18.0f);
		}
		ImGui::EndHorizontal();

		outTitlebarHeight = titlebarHeight;
	}

	void Window::UI_DrawMenubar()
	{
		if (!app->m_MenubarCallback)
			return;

		if (app->m_Specification.CustomTitlebar)
		{
			const ImRect menuBarRect = {ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()}};

			ImGui::BeginGroup();
			if (UI::BeginMenubar(menuBarRect))
			{
				app->m_MenubarCallback();
			}

			UI::EndMenubar();
			ImGui::EndGroup();
		}
		else
		{
			if (ImGui::BeginMenuBar())
			{
				app->m_MenubarCallback();
				ImGui::EndMenuBar();
			}
		}
	}

	void Application::UI_DrawMenubar()
	{
		if (!m_MenubarCallback)
			return;

		if (m_Specification.CustomTitlebar)
		{
			const ImRect menuBarRect = {ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()}};

			ImGui::BeginGroup();
			if (UI::BeginMenubar(menuBarRect))
			{
				m_MenubarCallback();
			}

			UI::EndMenubar();
			ImGui::EndGroup();
		}
		else
		{
			if (ImGui::BeginMenuBar())
			{
				m_MenubarCallback();
				ImGui::EndMenuBar();
			}
		}
	}

	void CheckUndockedWindows()
	{
		ImGuiContext &g = *ImGui::GetCurrentContext();
		for (int n = 0; n < g.Windows.Size; n++)
		{
			ImGuiWindow *window = g.Windows[n];
			if (window->DockIsActive && window->DockNode == NULL) // Active window with no DockNode means it is undocked
			{
				std::cout << "Window '%s' is undocked\n"
						  << window->Name << std::endl;
			}
		}
	}

	void Window::Render()
	{
		ImGuiIO &io = ImGui::GetIO();
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		// Ensure the current ImGui context is set for this window
		ImGui::SetCurrentContext(this->m_ImGuiContext);

        glfwPollEvents();
        glfwMakeContextCurrent(this->GetWindowHandle());

		if (this->g_SwapChainRebuild)
		{
			int width, height;
			glfwGetFramebufferSize(this->GetWindowHandle(), &width, &height);
			if (width > 0 && height > 0)
			{
				ImGui_ImplVulkan_SetMinImageCount(this->g_MinImageCount);
				ImGui_ImplVulkanH_CreateOrResizeWindow(this->g_Instance, this->g_PhysicalDevice, this->g_Device, &this->m_WinData, this->g_QueueFamily, this->g_Allocator, this->m_WinData.Width, this->m_WinData.Height, this->g_MinImageCount);
				this->m_WinData.FrameIndex = 0;

				// Clear allocated command buffers from here since entire pool is destroyed
				this->s_AllocatedCommandBuffers.clear();
				this->s_AllocatedCommandBuffers.resize(this->m_WinData.ImageCount);

				this->g_SwapChainRebuild = false;
			}
		}

		if (this->m_ResizePending)
		{
			glfwSetWindowSize(this->GetWindowHandle(), this->m_PendingWidth, this->m_PendingHeight);
			this->m_ResizePending = false;
		}

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			std::scoped_lock<std::mutex> lock(this->m_EventQueueMutex);

			while (!this->m_EventQueue.empty())
			{
				auto &func = this->m_EventQueue.front();
				func();
				this->m_EventQueue.pop();
			}
		}

		app->RenderWindow(this);

		ImGui::Render();

		ImDrawData *main_draw_data = ImGui::GetDrawData();
		const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);

		ImGui_ImplVulkanH_Window *wd = &this->m_WinData;
		wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		wd->ClearValue.color.float32[3] = clear_color.w;

		if (!main_is_minimized)
			FrameRender(wd, this, main_draw_data);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		if (!main_is_minimized)
			FramePresent(wd, this);
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	void Application::Run()
	{
		for (auto &window : m_Windows)
		{
			window->LoadImages();
		}

		m_Running = true;

		while (m_Running)
{
    // Poll for events for each window
    for (auto &window : m_Windows)
    {
    }

    // Update layers
    for (auto &layer : m_LayerStack)
    {
        layer->OnUpdate(m_TimeStep);
    }

    // Render each window
    for (auto &window : m_Windows)
    {
        window->Render();
    }

    // Update time
    float time = GetTime();
    m_FrameTime = time - m_LastFrameTime;
    m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
    m_LastFrameTime = time;
}

	}

	Window::Window(const std::string &name, int width, int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{

		// Intialize logging
		Log::Init();

		// Setup GLFW window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			std::cerr << "Could not initalize GLFW!\n";
			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Pas de contexte OpenGL

		// Check if custom titlebar is enabled
		bool customTitlebar = app->m_Specification.CustomTitlebar;
		glfwWindowHint(GLFW_DECORATED, customTitlebar ? GLFW_FALSE : GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_WindowHandle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		if (!m_WindowHandle)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		// Center the window if needed
		if (app->m_Specification.CenterWindow)
		{
			GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
			int monitorX, monitorY;
			glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
			glfwSetWindowPos(m_WindowHandle,
							 monitorX + (videoMode->width - width) / 2,
							 monitorY + (videoMode->height - height) / 2);
		}

		glfwSetWindowUserPointer(m_WindowHandle, this);

		/*glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow *window, int width, int height)
{
	Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
	if (win)
	{
		win->m_Width = width;
		win->m_Height = height;

			ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window);
		if (viewport)
		{
			viewport->Size = ImVec2(static_cast<float>(width), static_cast<float>(height));
		}

		// Recréez les ressources Vulkan nécessaires ici
		win->OnWindowResize(window, width, height);
	}
});*/

		glfwSetWindowPosCallback(m_WindowHandle, [](GLFWwindow *window, int xpos, int ypos)
								 {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
        {
            ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window);
            if (viewport)
            {
                viewport->Pos = ImVec2(static_cast<float>(xpos), static_cast<float>(ypos));
            }
        } });

		// Set GLFW callbacks for mouse events
		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow *window, double xpos, double ypos)
								 {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            // Handle mouse cursor position event
            win->HandleMouseMove(xpos, ypos);
        } });

		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow *window, int button, int action, int mods)
								   {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            // Handle mouse button event
            win->HandleMouseButton(button, action, mods);
        } });

		glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow *window, double xoffset, double yoffset)
							  {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            // Handle scroll event
            win->HandleMouseScroll(xoffset, yoffset);
        } });

		// Setup Vulkan
		if (!glfwVulkanSupported())
		{
			glfwDestroyWindow(m_WindowHandle);
			glfwTerminate();
			throw std::runtime_error("GLFW: Vulkan not supported!");
		}

		// Set icon
		if (!app->m_Specification.IconPath.empty())
		{
			GLFWimage icon[1];
			int channels = 0;
			icon[0].pixels = stbi_load("/usr/include/vortex/icon.png", &icon[0].width, &icon[0].height, &channels, 4);
			glfwSetWindowIcon(m_WindowHandle, 1, icon);
			stbi_image_free(icon[0].pixels);
		}

		// Setup Vulkan instance, device, etc.
		uint32_t extensions_count = 0;
		const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
		SetupVulkan(extensions, extensions_count, this);

		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(g_Instance, m_WindowHandle, g_Allocator, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}

		// Create Framebuffers
		int w, h;
		glfwGetFramebufferSize(m_WindowHandle, &w, &h);
		ImGui_ImplVulkanH_Window *wd = &m_WinData;
		SetupVulkanWindow(wd, surface, w, h, this);
		s_AllocatedCommandBuffers.resize(wd->ImageCount);
		s_ResourceFreeQueue.resize(wd->ImageCount);

		// Create the ImGui context
		IMGUI_CHECKVERSION();
		m_ImGuiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_ImGuiContext);

		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows
		io.FontGlobalScale = 0.83f;

		// Theme colors
		UI::SetHazelTheme();

		// Style
		ImGuiStyle &style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.FramePadding = ImVec2(8.0f, 6.0f);
		style.ItemSpacing = ImVec2(6.0f, 6.0f);
		style.ChildRounding = 6.0f;
		style.PopupRounding = 6.0f;
		style.FrameRounding = 6.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(m_WindowHandle, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = g_Instance;
		init_info.PhysicalDevice = g_PhysicalDevice;
		init_info.Device = g_Device;
		init_info.QueueFamily = g_QueueFamily;
		init_info.Queue = g_Queue;
		init_info.PipelineCache = g_PipelineCache;
		init_info.DescriptorPool = g_DescriptorPool;
		init_info.Subpass = 0;
		init_info.MinImageCount = g_MinImageCount;
		init_info.ImageCount = wd->ImageCount;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = g_Allocator;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		ImFont *robotoFont = io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		ImFont *hackFont = io.Fonts->AddFontFromMemoryTTF((void *)g_HackRegular, sizeof(g_HackRegular), 20.0f, &fontConfig);

		ImFontConfig fontConfigExtra;
		fontConfigExtra.GlyphExtraSpacing.x = 1.3f;
		ImFont *inconsolatas = io.Fonts->AddFontFromMemoryTTF((void *)g_Inconsolatas, sizeof(g_Inconsolatas), 20.0f, &fontConfigExtra);

		s_Fonts["Default"] = hackFont;
		s_Fonts["Inconsolatas"] = inconsolatas;
		s_Fonts["Bold"] = io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoBold, sizeof(g_RobotoBold), 20.0f, &fontConfig);
		s_Fonts["Italic"] = io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoItalic, sizeof(g_RobotoItalic), 20.0f, &fontConfig);
		s_Fonts["HackRegular"] = io.Fonts->AddFontFromMemoryTTF((void *)g_HackRegular, sizeof(g_HackRegular), 20.0f, &fontConfig);
		io.FontDefault = hackFont;

		// Upload Fonts
		{
			VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
			VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

			VkResult err = vkResetCommandPool(g_Device, command_pool, 0);
			check_vk_result(err);
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(command_buffer, &begin_info);
			check_vk_result(err);

			ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

			VkSubmitInfo end_info = {};
			end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			end_info.commandBufferCount = 1;
			end_info.pCommandBuffers = &command_buffer;
			err = vkEndCommandBuffer(command_buffer);
			check_vk_result(err);
			err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
			check_vk_result(err);

			err = vkDeviceWaitIdle(g_Device);
			check_vk_result(err);
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}
	}

	void Window::HandleMouseMove(double xpos, double ypos)
	{
		// Implement your mouse move handling logic here
		std::cout << "POS " << xpos << ypos << " " << std::endl;
	}

	void Window::HandleMouseButton(int button, int action, int mods)
	{
		// Implement your mouse button handling logic here
		std::cout << "CLICK BUTTOn " << button << action << " " << std::endl;
	}

	void Window::HandleMouseScroll(double xoffset, double yoffset)
	{
		// Implement your mouse scroll handling logic here
	}

	void Window::LoadImages()
	{

		{
			uint32_t w, h;
			void *data = Image::Decode(g_UIKitIcon, sizeof(g_UIKitIcon), w, h);
			this->m_AppHeaderIcon = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}

		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowMinimizeIcon, sizeof(g_WindowMinimizeIcon), w, h);
			this->m_IconMinimize = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowMaximizeIcon, sizeof(g_WindowMaximizeIcon), w, h);
			this->m_IconMaximize = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowRestoreIcon, sizeof(g_WindowRestoreIcon), w, h);
			this->m_IconRestore = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowCloseIcon, sizeof(g_WindowCloseIcon), w, h);
			this->m_IconClose = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	std::shared_ptr<Image> Application::GetApplicationIcon(const std::string &window) const
	{

		for (auto win : app->m_Windows)
		{
			if (win->GetName() == window)
			{
				return win->m_AppHeaderIcon;
			}
		}
		return nullptr;
	}

	bool Application::IsMaximized() const
	{
		return (bool)glfwGetWindowAttrib(m_WindowHandle, GLFW_MAXIMIZED);
	}

	float Application::GetTime()
	{
		return (float)glfwGetTime();
	}

	VkInstance Application::GetInstance(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				return win->g_Instance;
			}
		}
		return nullptr;
	}

	VkPhysicalDevice Application::GetPhysicalDevice(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				return win->g_PhysicalDevice;
			}
		}
		return nullptr;
	}

	VkDevice Application::GetDevice(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				return win->g_Device;
			}
		}
		return nullptr;
	}

	VkCommandBuffer Window::GetCommandBuffer(bool begin)
	{
		ImGui_ImplVulkanH_Window *wd = &this->m_WinData;

		// Use any command queue
		VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

		VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
		cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufAllocateInfo.commandPool = command_pool;
		cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer &command_buffer = s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
		auto err = vkAllocateCommandBuffers(this->g_Device, &cmdBufAllocateInfo, &command_buffer);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(command_buffer, &begin_info);
		check_vk_result(err);

		return command_buffer;
	}

	VkCommandBuffer Application::GetCommandBufferOfWin(const std::string &win_name, bool begin)
	{
		ImGui_ImplVulkanH_Window *wd = nullptr;

		for (auto layer : app->m_LayerStack)
		{
			if (layer->ParentWindow == win_name)
			{
				for (auto win : app->m_Windows)
				{
					if (win->GetName() == layer->ParentWindow)
					{
						wd = &win->m_WinData;

						// Use any command queue
						VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

						VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
						cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
						cmdBufAllocateInfo.commandPool = command_pool;
						cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
						cmdBufAllocateInfo.commandBufferCount = 1;

						VkCommandBuffer &command_buffer = win->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
						auto err = vkAllocateCommandBuffers(win->g_Device, &cmdBufAllocateInfo, &command_buffer);

						VkCommandBufferBeginInfo begin_info = {};
						begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
						begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
						err = vkBeginCommandBuffer(command_buffer, &begin_info);
						check_vk_result(err);

						return command_buffer;
					}
				}
			}
		}

		return nullptr;
	}

	/*VkCommandBuffer Application::GetCommandBuffer(bool begin)
	{
		ImGui_ImplVulkanH_Window *wd = &g_MainWindowData;

		// Use any command queue
		VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

		VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
		cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufAllocateInfo.commandPool = command_pool;
		cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer &command_buffer = s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
		auto err = vkAllocateCommandBuffers(win->g_Device, &cmdBufAllocateInfo, &command_buffer);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(command_buffer, &begin_info);
		check_vk_result(err);

		return command_buffer;
	}*/

	VkCommandBuffer Application::GetCommandBuffer(bool begin, ImGui_ImplVulkanH_Window *wd, const std::string &winname)
	{

		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				// Use any command queue
				VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

				VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
				cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmdBufAllocateInfo.commandPool = command_pool;
				cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmdBufAllocateInfo.commandBufferCount = 1;

				VkCommandBuffer &command_buffer = win->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
				auto err = vkAllocateCommandBuffers(win->g_Device, &cmdBufAllocateInfo, &command_buffer);

				VkCommandBufferBeginInfo begin_info = {};
				begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				err = vkBeginCommandBuffer(command_buffer, &begin_info);
				check_vk_result(err);

				return command_buffer;
			}
		}
		return nullptr;
	}

	void Application::FlushCommandBuffer(VkCommandBuffer commandBuffer, const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

				VkSubmitInfo end_info = {};
				end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				end_info.commandBufferCount = 1;
				end_info.pCommandBuffers = &commandBuffer;
				auto err = vkEndCommandBuffer(commandBuffer);
				check_vk_result(err);

				// Create fence to ensure that the command buffer has finished executing
				VkFenceCreateInfo fenceCreateInfo = {};
				fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCreateInfo.flags = 0;
				VkFence fence;
				err = vkCreateFence(win->g_Device, &fenceCreateInfo, nullptr, &fence);
				check_vk_result(err);

				err = vkQueueSubmit(win->g_Queue, 1, &end_info, fence);
				check_vk_result(err);

				err = vkWaitForFences(win->g_Device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
				check_vk_result(err);

				vkDestroyFence(win->g_Device, fence, nullptr);
			}
		}
	}

	void Application::SubmitResourceFree(std::function<void()> &&func, const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				win->s_ResourceFreeQueue[win->s_CurrentFrameIndex].emplace_back(func);
			}
		}
	}

	ImFont *Application::GetFont(const std::string &name)
	{
		// if (!s_Fonts.contains(name))
		//		return nullptr;

		return s_Fonts.at(name);
	}

	GLFWwindow *GetWindowHandle(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (winname == win->GetName())
			{
				return win->GetWindowHandle();
			}
		}
	}

	void Window::OnWindowResize(GLFWwindow *windowHandle, int width, int height)
	{
		if (width == 0 || height == 0)
			return;

		vkDeviceWaitIdle(g_Device);

		this->CleanupVulkanWindow();

		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(g_Instance, m_WindowHandle, g_Allocator, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to recreate window surface!");
		}

		SetupVulkanWindow(&m_WinData, surface, width, height, this);

		// Mettez à jour les ressources ImGui si nécessaire
		ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
		ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &m_WinData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
	}

	void Application::RenderWindow(Window *window)
	{
		// Set the ImGui context for this window
		ImGui::SetCurrentContext(window->m_ImGuiContext);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport *viewport = ImGui::GetMainViewport();
		int width, height;
		glfwGetWindowSize(window->GetWindowHandle(), &width, &height);

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		if (!m_Specification.CustomTitlebar && m_MenubarCallback)
			window_flags |= ImGuiWindowFlags_MenuBar;

		const bool isMaximized = (bool)glfwGetWindowAttrib(window->GetWindowHandle(), GLFW_MAXIMIZED);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

		std::string label = "DockSpaceWindow." + window->GetName();
		ImGui::SetNextWindowDockID(0);
		ImGui::Begin(label.c_str(), nullptr, window_flags);

		ImGuiWindow *win = ImGui::GetCurrentWindow();

		ImGui::PopStyleColor(); // MenuBarBg
		ImGui::PopStyleVar(2);

		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
			if (!isMaximized)
				UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());
			ImGui::PopStyleColor(); // ImGuiCol_Border
		}

		if (m_Specification.CustomTitlebar)
		{
			float titleBarHeight;
			window->UI_DrawTitlebar(titleBarHeight);
			ImGui::SetCursorPosY(titleBarHeight);
		}

		ImGuiIO &io = ImGui::GetIO();
		ImGuiStyle &style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		AppPushTabStyle();
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		AppPopTabStyle();

		style.WindowMinSize.x = minWinSizeX;

		if (!m_Specification.CustomTitlebar)
			window->UI_DrawMenubar();

		for (auto &layer : m_LayerStack)
		{
			if (!layer->initialized)
			{
				layer->ParentWindow = window->GetName();
				layer->initialized = true;
			}

			static bool windowJustUndocked = false;

			layer->m_WindowControlCallbalck = [](ImGuiWindow *win)
			{
				if (win)
				{
					std::cout << "==========================================" << std::endl;
					std::cout << "[WINDOW] Name: " << win->Name << std::endl;
					std::cout << "[WINDOW] Dock Node : " << win->DockNode << std::endl;
					std::cout << "[WINDOW] Dock Tree Name : " << win->RootWindowDockTree->Name << std::endl;
					std::string str = win->RootWindowDockTree->Name;
					std::string str_finded = str.substr(str.find(".") + 1, str.size());
					std::cout << "[WINDOW] Deducted tree name : " << str_finded << std::endl;

					std::shared_ptr<Window> finded_win = nullptr;

					for (auto win : app->m_Windows)
					{
						if (str_finded == win->GetName())
						{
							finded_win = win;
						}
					}

					if (finded_win)
					{
						std::cout << "The window is attached to a parent window ! (" << finded_win->GetName() << ")" << std::endl;
					}
					else
					{
						std::cout << "The window is alone :(" << std::endl;
						// Assign the subwindow in a new window !
					}

					std::cout << "[WINDOW] Dock ID : " << win->DockId << std::endl;
					std::cout << "[WINDOW] Dock Host ID : " << win->DockNodeAsHost << std::endl;
					std::cout << "[WINDOW] Identified Parent window : " << win->DockNodeAsHost << std::endl;

					// Check if the window is being moved (header bar clicked and dragging)
					if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
					{
						std::cout << "[WINDOW] The window is being moved!" << std::endl;
					}
					else
					{
						std::cout << "[WINDOW] The window is not being moved." << std::endl;
					}

					std::cout << "==========================================" << std::endl;
				}
				else
				{
					std::cout << "==========================================" << std::endl;
					std::cout << "[WINDOW]: invalid" << std::endl;
					std::cout << "==========================================" << std::endl;
				}
			};

			if (layer->ParentWindow == window->GetName())
			{
				layer->OnUIRender();
			}
		}

		ImGui::End();

		// Check for mouse click in any ImGui window and handle dragging
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemHovered())
		{
			window->m_IsDragging = true;
			window->m_InitialMousePos = io.MousePos;
			window->m_InitialWindowPos = ImVec2(win->Pos.x, win->Pos.y);
		}

		if (window->m_IsDragging && !window->m_ResizePending)
		{
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				ImVec2 delta = ImVec2(io.MousePos.x - window->m_InitialMousePos.x, io.MousePos.y - window->m_InitialMousePos.y);
				if (delta.x != 0 || delta.y != 0)
				{ // Only update position if there is a change
					ImVec2 newWindowPos = ImVec2(window->m_InitialWindowPos.x + delta.x, window->m_InitialWindowPos.y + delta.y);
					glfwSetWindowPos(window->GetWindowHandle(), (int)newWindowPos.x, (int)newWindowPos.y);
				}
			}
			else
			{
				window->m_IsDragging = false;
			}
		}

		// Detect window resize
		if (win->Size.x != window->m_PreviousWidth || win->Size.y != window->m_PreviousHeight)
		{
			glfwSetWindowSize(window->GetWindowHandle(), win->Size.x, win->Size.y);
		}
	}

	void Window::CleanupVulkanWindow()
	{
		ImGui_ImplVulkanH_DestroyWindow(this->g_Instance, this->g_Device, &m_WinData, this->g_Allocator);
	}

}