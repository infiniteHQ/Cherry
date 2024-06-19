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
	enum class Window_Type
	{
		ENTIRE_WINDOW,
		SIMPLE_WINDOW,
	};


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
	void ProcessMouseEvents();

		VkCommandBuffer GetCommandBuffer(bool begin);
		GLFWwindow *GetWindowHandle() const { return m_WindowHandle; }

		void OnWindowResize(GLFWwindow *windowHandle, int width, int height);
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
		void QueueEvent(Func &&func);

		bool m_ResizePending = false;
		bool m_MovePending = false;
		int m_PendingWidth = 0;
		int m_PendingHeight = 0;
		int m_PendingX = 0;
		int m_PendingY = 0;
		int m_PreviousWidth = 0;
		int m_PreviousHeight = 0;
		int m_PreviousX = 0;
		int m_PreviousY = 0;
		int m_PosX = 0;
		int m_PosY = 0;

		int WinID;

		void RequestResize(int width, int height);
		void RequestMove(int x, int y);

		void ApplyPendingResize()
		{
			if (m_ResizePending)
			{
				std::cout << "\33 RESIZE" << std::endl;
				m_WinData.Width = m_PendingWidth;
				m_WinData.Height = m_PendingHeight;
				GLFWwindow *win = this->GetWindowHandle();
				ImGui_ImplVulkanH_Window *wd = &this->m_WinData;

				glfwSetWindowSize(win, m_WinData.Width, m_WinData.Height);

				m_ResizePending = false;
			}
		}

		void HandleMouseMove(double xpos, double ypos);
		void HandleMouseButton(int button, int action, int mods);
		void HandleMouseScroll(double xoffset, double yoffset);

		void Render();

		std::shared_ptr<UIKit::Image> m_AppHeaderIcon;
		std::shared_ptr<UIKit::Image> m_IconClose;
		std::shared_ptr<UIKit::Image> m_IconMinimize;
		std::shared_ptr<UIKit::Image> m_IconMaximize;
		std::shared_ptr<UIKit::Image> m_IconRestore;
 		std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
    	ImDrawData DrawData;
		int m_Width, m_Height;
		ImGui_ImplVulkanH_Window m_WinData;

    	VkPipelineLayout pipelineLayout;
    	ImGuiViewport* m_Viewport = nullptr;
		bool m_IsDragging = false;
		ImVec2 m_InitialMousePos;
		ImVec2 m_InitialWindowPos;

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

		bool WindowOnlyClosable = false;

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

		void SpawnWindow();
		void UnspawnWindow();

		void Close();

		bool IsMaximized() const;
		std::shared_ptr<Image> GetApplicationIcon(const std::string &window) const;

		float GetTime();
		GLFWwindow *GetWindowHandle(const std::string &winname) const;
		bool IsTitleBarHovered() const { return m_TitleBarHovered; }

		Window* GetWindowByHandle(GLFWwindow* window_handle){
			for(auto win : this->m_Windows)
			{
				if(win->GetWindowHandle() == window_handle)
				{
					return win.get();
				}
			}
			return nullptr;
		}

		static VkInstance GetInstance(const std::string &win);
		static VkPhysicalDevice GetPhysicalDevice(const std::string &win);
		static VkDevice GetDevice(const std::string &win);

		// static VkCommandBuffer GetCommandBuffer(bool begin);
		static VkCommandBuffer GetCommandBuffer(bool begin, ImGui_ImplVulkanH_Window *wd, const std::string &win);
		static VkCommandBuffer GetCommandBufferOfWin(const std::string &win_name, bool begin);
		static VkCommandBuffer GetCommandBuffer(const std::string &win_name, bool begin);
		static void FlushCommandBuffer(VkCommandBuffer commandBuffer, const std::string &win);

		static void SubmitResourceFree(std::function<void()> &&func, const std::string &winname);
		static ImFont *GetFont(const std::string &name);

		ApplicationSpecification m_Specification;
		void UI_DrawTitlebar(float &outTitlebarHeight, Window *window);
		void UI_DrawMenubar();

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
		bool m_Running = false;

		ImDrawData* RenderWindow(Window *window);
		// Resources
		// TODO(Yan): move out of application class since this can't be tied
		//            to application lifetime
		std::function<void()> m_MenubarCallback;
		std::vector<std::shared_ptr<Window>> m_Windows;

	private:
		void Init();
		void Shutdown();
		// For custom titlebars

	private:
		//GLFWwindow *m_WindowHandle = nullptr;
		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

		bool m_TitleBarHovered = false;
	};

	// Implemented by CLIENT
	static Application *CreateApplication(int argc, char **argv);
}