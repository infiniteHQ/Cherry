#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"
#include "image.hpp"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#include <memory>
#include <functional>
#include <filesystem>

#include "ui/ui.hpp"
#include "imgui/ImGuiTheme.h"
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

namespace UIKit
{
	enum class Window_Type
	{
		ENTIRE_WINDOW,
		SIMPLE_WINDOW,
	};

	struct WindowClickEvent
	{
		int button;
		int action;
		int mods;
	};

	struct WindowMoveEvent
	{
		double xpos;
		double ypos;
	};

	enum class DockEmplacement
	{
		DockBlank,
		DockUp,
		DockDown,
		DockLeft,
		DockRight,
		DockFull
	};

	enum class DefaultAppWindowBehaviors
	{
		DefaultDocking,
		DefaultSize
	};

	enum class AppWindowTypes
	{
		InstanciableWindow,
		StaticWindow
	};

	struct EnumClassHash
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	struct WindowDragDropState
	{
		std::string LastDraggingWindow = "unknown";
		std::string LastDraggingAppWindowHost = "unknown";
		std::string LastDraggingAppWindow = "unknown";
		bool DockIsDragging = false;
		UIKit::DockEmplacement LastDraggingPlace = UIKit::DockEmplacement::DockBlank;
		bool CreateNewWindow = false;
		std::string DragOwner = "none";
		int mouseX;
		int mouseY;
	};

	struct RedockRequest
	{
		std::string m_ParentWindow;
		std::string m_ParentAppWindowHost;
		std::string m_ParentAppWindow;
		DockEmplacement m_DockPlace;
		bool m_IsHandled;
	};

	class AppWindow
	{
	public:
		AppWindow() : m_ID("undefined"), m_Name("undefined")
		{
		}

		AppWindow(const std::string &id, const std::string &name) : m_ID(id), m_Name(name)
		{
		}

		AppWindow(const std::string &id, const std::string &name, const std::string &icon) : m_ID(id), m_Name(name), m_Icon(icon)
		{
			SetIcon(m_Icon);
		}
		// AppWindow(const std::string& win_name, LOGO);

		void push_event();
		void CtxRender(std::vector<std::shared_ptr<RedockRequest>> *reqs, const std::string &winname);
		std::shared_ptr<RedockRequest> CreateEvent(const std::string &parentWindow, DockEmplacement emplacement, const std::string &appWindow = "none")
		{
			std::shared_ptr<RedockRequest> req = std::make_shared<RedockRequest>();
			req->m_DockPlace = emplacement;
			req->m_ParentAppWindow = appWindow;
			req->m_ParentAppWindowHost = this->m_Name;
			req->m_ParentWindow = parentWindow;
			return req;
		}

		std::shared_ptr<UIKit::Image> GetImage(const std::string &path);
		ImTextureID *GetTexture(const std::string &path);

		void SetSimpleStorage(const std::string &key, const std::string &value)
		{
			m_Storage[key] = value;
		}

		float EstimateMenubarRightWidth()
		{
			ImVec2 initialCursorPos = ImGui::GetCursorPos();
			ImGui::PushClipRect(ImVec2(0, 0), ImVec2(0, 0), false);
			ImGuiID id = ImGui::GetID("TempID");

			ImGui::PushID(id);
			ImGui::SetCursorPosX(0.0f);
			if (m_MenubarRight)
			{
				m_MenubarRight();
			}
			float width = ImGui::GetCursorPosX();
			ImGui::PopID();

			ImGui::PopClipRect();
			ImGui::SetCursorPos(initialCursorPos);

			return width;
		}

		void SetWindowStorage(const std::string &key, const std::string &value)
		{
			m_WindowStorage[key] = value;
		}
		std::string GetSimpleStorage(const std::string &key)
		{
			if (m_Storage.find(key) != m_Storage.end())
			{
				return m_Storage[key];
			}
			else
			{
				return "undefined";
			}
		}
		std::string GetWindowStorage(const std::string &key)
		{
			if (m_WindowStorage.find(key) != m_WindowStorage.end())
			{
				return m_WindowStorage[key];
			}
			else
			{
				return "undefined";
			}
		}
		void SetDefaultBehavior(DefaultAppWindowBehaviors behavior, const std::string &value)
		{
			m_DefaultBehaviors[behavior] = value;
		}

		std::string GetDefaultBehavior(DefaultAppWindowBehaviors behavior)
		{
			if (m_DefaultBehaviors.find(behavior) != m_DefaultBehaviors.end())
			{
				return m_DefaultBehaviors[behavior];
			}
			else
			{
				return "undefined";
			}
		}

		void SetRenderCallback(const std::function<void()> &render)
		{
			m_Render = render;
		}

		void SetLeftMenubarCallback(const std::function<void()> &right_menubar)
		{
			m_MenubarLeft = right_menubar;
		}

		void SetRightMenubarCallback(const std::function<void()> &left_menubar)
		{
			m_MenubarRight = left_menubar;
		}

		void SetIcon(const std::string &name)
		{
			m_Icon = name;
		}


		void SetDockingMode(const bool& use_docking)
		{
			m_DockingMode = use_docking;
		}


		void SetParent(const std::shared_ptr<AppWindow>& parent)
		{
			m_ParentAppWindow = parent;
			m_HaveParentAppWindow = true;
		}

	public:
		int treated = 0;

		// Main informations
		std::string m_WinParent = "unknow";
		std::string m_DockParent = "unknow";
		std::string m_DockPlace = "unknow";
		std::string m_Name = "unknow";
		std::string m_ID = "unknow";
		// ImGuiWindow *m_ImGuiWindow;
		ImGuiID m_DockSpaceID;

		std::string m_Icon = "none";

		bool m_Closable = true;
		bool m_Opened = true;
		bool m_IsRendering = true;

		bool m_IsDragging;
		bool m_DockIsDraggingStarted;

		bool m_Pressed;

		bool m_DockingMode = false;

		bool m_HaveParentAppWindow = false;
		std::shared_ptr<AppWindow> m_ParentAppWindow;
		// Notifications
		// Number of unread notifs

		// Default behavior

		// Renders functions
		std::function<void()> m_Render;
		std::function<void()> m_MenubarRight;
		std::function<void()> m_MenubarLeft;

		std::function<void()> m_CloseEvent;

	private:
		std::vector<std::shared_ptr<AppWindow>> m_SubAppWindows;
		std::unordered_map<std::string, std::string> m_Storage;
		std::unordered_map<std::string, std::string> m_WindowStorage;
		std::unordered_map<DefaultAppWindowBehaviors, std::string, EnumClassHash> m_DefaultBehaviors;
	};

	class Window
	{
	public:
		Window(const std::string &name, int width, int height, bool cold_start);

		~Window();

		void ProcessMouseEvents();

		VkCommandBuffer GetCommandBuffer(bool begin);
		SDL_Window *GetWindowHandle() const { return m_WindowHandler; }

		// void OnWindowResize(GLFWwindow *windowHandle, int width, int height);
		// void OnWindowMove(int xpos, int ypos);

		void BeginFrame()
		{
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		}

		void EndFrame()
		{
			ImGui::Render();
		}

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

		void RequestResize(int width, int height);
		void RequestMove(int x, int y);

		bool hasImage(const std::string &image)
		{
			for (const auto &existingImage : this->m_ImageList)
			{
				if (existingImage.first == image)
				{
					return true;
				}
			}
			return false;
		}

		void ApplyPendingResize()
		{
			if (m_ResizePending)
			{
				m_WinData.Width = m_PendingWidth;
				m_WinData.Height = m_PendingHeight;
				SDL_Window *win = this->GetWindowHandle();
				ImGui_ImplVulkanH_Window *wd = &this->m_WinData;

				m_ResizePending = false;
			}
		}

		void Render();

		ImGuiContext *m_ImGuiContext;

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

		bool isMoving = false;
		ImVec2 clickOffset = {0.0f, 0.0f};
		// If true, listen mouse pos and click and return a dropp emplacement response.
		bool m_IsDraggingAppWindow = false;

		bool m_Resizing = false;

		std::mutex m_EventQueueMutex;
		std::queue<std::function<void()>> m_EventQueue;
		bool g_SwapChainRebuild = false;
		VkSwapchainKHR g_Swapchain;
		std::vector<VkImage> g_SwapchainImages;
		std::vector<VkImageView> g_SwapchainImageViews;
		VkFormat g_SwapchainImageFormat;
		std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;
		uint32_t s_CurrentFrameIndex = 0;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		ImGuiWindow *m_ImGuiWindow;

		// List of all images (absolute path / image ref)
		std::vector<std::pair<std::string, std::shared_ptr<UIKit::Image>>> m_ImageList;

		std::shared_ptr<UIKit::Image> add(const std::string &path);
		std::shared_ptr<UIKit::Image> add(const uint8_t data[], const std::string &name);
		void free();
		std::shared_ptr<UIKit::Image> get(const std::string &path);
		std::shared_ptr<UIKit::Image> get(const uint8_t data[], const std::string &name);
		ImTextureID get_texture(const std::string &path);

		VkSurfaceKHR m_Surface;
		std::shared_ptr<WindowClickEvent> m_PendingClick = nullptr;
		std::shared_ptr<WindowMoveEvent> m_PendingMove = nullptr;
		WindowDragDropState drag_dropstate;
		int WinID;

		std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
		ImDrawData DrawData;
		int m_Width, m_Height;

		ImGui_ImplVulkanH_Window m_WinData;

		VkPipelineLayout pipelineLayout;
		ImGuiViewport *m_Viewport = nullptr;
		bool m_IsDragging = false;
		ImVec2 m_InitialMousePos;
		ImVec2 m_InitialWindowPos;

	private:
		std::function<void()> m_MenubarCallback;
		std::string m_Name;
		SDL_Window *m_WindowHandler;
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
		uint32_t MinWidth = 100;
		uint32_t MinHeight = 100;

		std::filesystem::path IconPath;

		bool WindowResizeable = true;

		bool WindowOnlyClosable = false;

		bool WindowSaves = false;

		bool DisableTitle = false;
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

		void LoadImages();

		static Application &Get();
		void PollEvents()
		{
			for (auto &window : m_Windows)
			{
				// glfwPollEvents(); // Poll events in the main thread
			}
		}

		void Run();
		void SetCloseCallback(const std::function<void()> &closeCallback) { m_CloseCallback = closeCallback; }
		void SetMenubarCallback(const std::function<void()> &menubarCallback) { m_MenubarCallback = menubarCallback; }
		void SetFramebarCallback(const std::function<void()> &framebarCallback) { m_FramebarCallback = framebarCallback; }
		void SetCloseCallback(const std::function<bool()> &closeCallback) { m_CloseCallback = closeCallback; }

		std::shared_ptr<Window> GetCurrentRenderedWindow();
		// void SyncImages();

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

		std::string SpawnWindow();
		void SpawnWindow(const std::string &name);
		void UnspawnWindow(const std::string &name);

		void Close();

		bool IsMaximized() const;
		std::shared_ptr<Image> GetApplicationIcon(const std::string &window) const;

		float GetTime();
		bool IsTitleBarHovered() const { return m_TitleBarHovered; }

		Window *GetWindowByHandle(SDL_Window *window_handle)
		{
			for (auto win : this->m_Windows)
			{
				if (win->GetWindowHandle() == window_handle)
				{
					return win.get();
				}
			}
			return nullptr;
		}

		static VkInstance GetInstance();
		static VkPhysicalDevice GetPhysicalDevice();
		static VkDevice GetDevice();
		static VkCommandBuffer GetCommandBuffer(bool begin);

		// static VkCommandBuffer GetCommandBuffer(bool begin);
		static VkCommandBuffer GetCommandBuffer(bool begin, ImGui_ImplVulkanH_Window *wd, const std::string &win);
		static VkCommandBuffer GetCommandBufferOfWin(const std::string &win_name, bool begin);
		static VkCommandBuffer GetCommandBuffer(const std::string &win_name, bool begin);
		static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

		static void SubmitResourceFree(std::function<void()> &&func, const std::string &winname);
		static ImFont *GetFont(const std::string &name);

		ApplicationSpecification m_Specification;
		void UI_DrawTitlebar(float &outTitlebarHeight, Window *window);
		void UI_DrawMenubar();

		void RenderDockspace();
		void InitializeWindowStates();
		void SaveData();
		void SetWindowSaveDataFile(const std::string &path);
		void SynchronizeWindows();
		std::vector<std::shared_ptr<Layer>> m_LayerStack;

		std::string PutWindow(std::shared_ptr<AppWindow> win)
		{
			m_AppWindows.push_back(win);
			return "id";
		}

		ImDrawData *RenderWindow(Window *window);

		// Resources
		// TODO(Yan): move out of application class since this can't be tied
		//            to application lifetime
		std::function<void()> m_MenubarCallback;
		std::function<void()> m_FramebarCallback;
		std::function<void()> m_CloseCallback; // need to destroy all windows manually

		bool m_ClosePending = false;

		std::mutex m_WindowCreationMutex;
		ImGuiContext *m_ImGuiMasterContext;

		std::string m_WindowSaveDataPath;
		bool m_SaveWindowData = false;
		bool m_IsDataSaved = false;
		bool m_IsDataInitialized = false;
		nlohmann::json m_PreviousSaveData;

		bool m_Running = false;
		bool m_Dragging = false;
		std::vector<std::shared_ptr<Window>> m_Windows;
		std::vector<std::shared_ptr<RedockRequest>> m_RedockRequests;
		std::vector<std::shared_ptr<AppWindow>> m_AppWindows;

	private:
		void Init();
		void Shutdown();
		// For custom titlebars

	private:
		// GLFWwindow *m_WindowHandle = nullptr;
		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

		bool m_TitleBarHovered = false;
	};

	// Implemented by CLIENT
	static Application *CreateApplication(int argc, char **argv);
}