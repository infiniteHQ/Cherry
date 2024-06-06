#pragma once

#include "../../../src/Layer.h"
#include "Image.h"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#include <memory>
#include <functional>
#include <filesystem>

#include "UI/UI.h"
#include "ImGui/ImGuiTheme.h"
#include "vulkan/vulkan.h"

void check_vk_result(VkResult err);

static void AppPushTabStyle()
{
	ImGuiStyle &style = ImGui::GetStyle();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 6.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 6.0f));
}
static void AppPopTabStyle()
{
	ImGui::PopStyleVar(3);
}

struct GLFWwindow;

namespace UIKit
{

	class Window
	{
	public:
		Window(const std::string &name, int width, int height);

		~Window()
		{
			// Détruisez la fenêtre GLFW
			if (m_WindowHandle)
			{
				glfwDestroyWindow(m_WindowHandle);
			}
		}

		VkCommandBuffer GetCommandBuffer(bool begin);
		GLFWwindow *GetWindowHandle() const { return m_WindowHandle; }



	void OnWindowResize(int width, int height);
	void OnWindowMove(int xpos, int ypos);

		void BeginFrame()
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void EndFrame()
		{
			ImGui::Render();
		}

		void LoadImages();

		void OnUpdate();

		const std::string &GetName() const
		{
			return m_Name;
		}

		void CreateImage(std::shared_ptr<UIKit::Image> image, void *data);

		void UI_DrawTitlebar(float &outTitlebarHeight);
		void UI_DrawMenubar();

		void CleanupVulkanWindow();


		template <typename Func>
		void QueueEvent(Func &&func)
		{
			m_EventQueue.push(func);
		}


		std::shared_ptr<UIKit::Image> m_AppHeaderIcon;
		std::shared_ptr<UIKit::Image> m_IconClose;
		std::shared_ptr<UIKit::Image> m_IconMinimize;
		std::shared_ptr<UIKit::Image> m_IconMaximize;
		std::shared_ptr<UIKit::Image> m_IconRestore;
		int m_Width, m_Height;
		ImGui_ImplVulkanH_Window m_WinData;

		std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;
		uint32_t s_CurrentFrameIndex = 0;
		
 int g_MinImageCount = 2;
 bool g_SwapChainRebuild = false;
 VkAllocationCallbacks *g_Allocator = NULL;
 VkInstance g_Instance = VK_NULL_HANDLE;
 VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
 uint32_t g_QueueFamily = (uint32_t)-1;
 VkQueue g_Queue = VK_NULL_HANDLE;
 VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
 VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
 VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;

 VkDevice g_Device = VK_NULL_HANDLE;
 std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;

		std::mutex m_EventQueueMutex;
		std::queue<std::function<void()>> m_EventQueue;
 ImGuiContext* m_ImGuiContext;
	private:
		std::function<void()> m_MenubarCallback;
		std::string m_Name;
		GLFWwindow *m_WindowHandle;
	};

	struct ParentWindow
	{
		std::vector<std::shared_ptr<Layer>> m_LayerStack;
		std::string window_name;
	};

	struct ApplicationSpecification
	{
		std::string Name = "UIKit App";
		uint32_t Width = 1850;
		uint32_t Height = 1000;

		std::filesystem::path IconPath;

		bool WindowResizeable = true;

		// Uses custom UIKit titlebar instead
		// of Windows default
		bool CustomTitlebar = false;

		// Window will be created in the center
		// of primary monitor
		bool CenterWindow = false;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification &applicationSpecification = ApplicationSpecification());
		~Application();

		static Application &Get();

		void Run();
		void SetMenubarCallback(const std::function<void()> &menubarCallback) { m_MenubarCallback = menubarCallback; }

		void NewWinInstance(const std::string &name);
		template <typename T>
		void PushLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
			m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
		}

		void PushLayer(const std::shared_ptr<Layer> &layer)
		{
			m_LayerStack.emplace_back(layer);
			layer->OnAttach();
		}

		void Close();

		bool IsMaximized() const;
		std::shared_ptr<Image> GetApplicationIcon() const { return m_AppHeaderIcon; }

		float GetTime();
		GLFWwindow *GetWindowHandle(const std::string& winname) const;
		bool IsTitleBarHovered() const { return m_TitleBarHovered; }

		static VkInstance GetInstance(const std::string& win);
		static VkPhysicalDevice GetPhysicalDevice(const std::string& win);
		static VkDevice GetDevice(const std::string& win);

		//static VkCommandBuffer GetCommandBuffer(bool begin);
		static VkCommandBuffer GetCommandBuffer(bool begin, ImGui_ImplVulkanH_Window *wd, const std::string& win);
		static VkCommandBuffer GetCommandBufferOfWin(const std::string &win_name, bool begin);
		static void FlushCommandBuffer(VkCommandBuffer commandBuffer, const std::string& win);

		static void SubmitResourceFree(std::function<void()> &&func, const std::string& winname);

		static ImFont *GetFont(const std::string &name);

		ApplicationSpecification m_Specification;
		void UI_DrawTitlebar(float &outTitlebarHeight, Window *window);
		void UI_DrawMenubar();

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
		bool m_Running = false;



		

		// Resources
		// TODO(Yan): move out of application class since this can't be tied
		//            to application lifetime
		std::shared_ptr<UIKit::Image> m_AppHeaderIcon;
		std::shared_ptr<UIKit::Image> m_IconClose;
		std::shared_ptr<UIKit::Image> m_IconMinimize;
		std::shared_ptr<UIKit::Image> m_IconMaximize;
		std::shared_ptr<UIKit::Image> m_IconRestore;
		std::function<void()> m_MenubarCallback;
		std::vector<std::shared_ptr<Window>> m_Windows;

	private:
		void Init();
		void Shutdown();
		// For custom titlebars

	private:
		GLFWwindow *m_WindowHandle = nullptr;
		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

		bool m_TitleBarHovered = false;


		void RenderWindow(Window *window);
	};

	// Implemented by CLIENT
	static Application *CreateApplication(int argc, char **argv);
}