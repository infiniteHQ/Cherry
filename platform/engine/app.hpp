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
#include <SDL2/SDL_image.h>

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
		bool DisableTitleBar = false;
		bool DisableResize = false;
		bool DisableWindowManagerTitleBar = false;
		bool EnableDocking = false;

		// Uses custom UIKit titlebar instead
		// of Windows default
		bool CustomTitlebar = false;

		// Window will be created in the center
		// of primary monitor
		bool CenterWindow = false;
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
		bool LastDraggingAppWindowHaveParent = false;
		bool DockIsDragging = false;
		UIKit::DockEmplacement LastDraggingPlace = UIKit::DockEmplacement::DockBlank;
		bool CreateNewWindow = false;
		bool FromSave = false;
		std::string DragOwner = "none";
		int mouseX;
		int mouseY;
	};

	struct ReattachRequest
	{
		ApplicationSpecification m_Specification;	
		std::string m_AppWindowName;
		bool m_IsFinished;
	};

	struct RedockRequest
	{
		std::string m_ParentWindow;
		std::string m_ParentAppWindowHost;
		std::string m_ParentAppWindow;
		DockEmplacement m_DockPlace;
		bool m_IsHandled;
		bool m_FromSave;
	};

	struct SimpleStorageItem
	{
		std::string m_Data;
		bool m_Persistant;

		SimpleStorageItem(const std::string &data, const bool &is_persistant) : m_Persistant(is_persistant),
																				m_Data(data) {};
	};

	struct WindowStorageItem
	{
		nlohmann::json m_JsonData;
		bool m_Persistant;

		WindowStorageItem(const nlohmann::json &data, const bool &is_persistant) : m_Persistant(is_persistant),
																				   m_JsonData(data) {};
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

		void push_event();
		void CtxRender(std::vector<std::shared_ptr<RedockRequest>> *reqs, const std::string &winname);
		std::shared_ptr<RedockRequest> CreateEvent(const std::string &parentWindow, DockEmplacement emplacement, const bool &fromSave = false, const std::string &appWindow = "none")
		{
			std::shared_ptr<RedockRequest> req = std::make_shared<RedockRequest>();
			req->m_DockPlace = emplacement;
			req->m_ParentAppWindow = appWindow;
			req->m_ParentAppWindowHost = this->m_Name;
			req->m_ParentWindow = parentWindow;
			req->m_FromSave = fromSave;
			return req;
		}

		void AttachOnNewWindow(ApplicationSpecification spec);
		void AttachOnWindow(const std::string winname);

		ReattachRequest m_AttachRequest;
		


		std::shared_ptr<UIKit::Image> GetImage(const std::string &path);
		ImTextureID *GetTexture(const std::string &path);

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

		void SetSimpleStorage(const std::string &key, const std::string &data, const bool &persistant)
		{
			std::shared_ptr<SimpleStorageItem> value = std::make_shared<SimpleStorageItem>(data, persistant);
			m_Storage[key] = value;
		}

		void SetWindowStorage(const std::string &key, const nlohmann::json &data, const bool &persistant)
		{
			std::shared_ptr<WindowStorageItem> value = std::make_shared<WindowStorageItem>(data, persistant);
			m_WindowStorage[key] = value;
		}

		std::shared_ptr<SimpleStorageItem> GetSimpleStorage(const std::string &key)
		{
			if (m_Storage.find(key) != m_Storage.end())
			{
				return m_Storage[key];
			}
			else
			{
				return nullptr;
			}
		}

		std::shared_ptr<WindowStorageItem> GetWindowStorage(const std::string &key)
		{
			if (m_WindowStorage.find(key) != m_WindowStorage.end())
			{
				return m_WindowStorage[key];
			}
			else
			{
				return nullptr;
			}
		}

		std::unordered_map<std::string, std::shared_ptr<WindowStorageItem>> DumpWindowStorage()
		{
			return m_WindowStorage;
		}
		std::unordered_map<std::string, std::shared_ptr<SimpleStorageItem>> DumpSimpleStorage()
		{
			return m_Storage;
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

		void SpawnInNewWindow()
		{
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

		void SetDockingMode(const bool &use_docking)
		{
			m_DockingMode = use_docking;
		}

		void SetFetchedSaveData(const std::string &key, const std::string &value)
		{
			m_LastSaveData[key] = value;
		}

		void SetInstanciable()
		{
			m_AppWindowType = AppWindowTypes::InstanciableWindow;
		}

		std::string GetFetchedSaveData(const std::string &key)
		{
			if (m_LastSaveData.find(key) != m_LastSaveData.end())
			{
				return m_LastSaveData[key];
			}
			else
			{
				return "undefined";
			}
		}

		void SetParent(const std::shared_ptr<AppWindow> &parent);

		bool CheckWinParent(const std::string &parentname);
		void AddUniqueWinParent(const std::string &parentnale);
		void AddWinParent(const std::string &parentnale);
		void DeleteWinParent(const std::string &parentnale);

		void SetClosable(const bool &is_closable)
		{
			this->m_Closable = is_closable;
		}

		void SetCloseCallback(std::function<void()> close_callback)
		{
			this->m_CloseCallback = close_callback;
		}

		void SetSaveMode(const bool &use_save_mode)
		{
			this->m_SaveMode = use_save_mode;
		}

		void SetSaved(const bool &new_state)
		{
			this->m_Saved = new_state;
		}

		void SetOpened(const bool &new_state)
		{
			this->m_Opened = new_state;
		}

		void SetDisableDragging(const bool &new_state)
		{
			this->m_DisableDragging = new_state;
		}

		void SetDisableContextMenu(const bool &new_state)
		{
			this->m_DisableContextMenu = new_state;
		}

		std::string m_WinParent;
	public:
		int treated = 0;

		// Main informations
		std::string m_DockParent = "unknow";
		std::string m_DockPlace = "unknow";
		std::string m_Name = "unknow";
		std::string m_ID = "unknow";
		// ImGuiWindow *m_ImGuiWindow;
		ImGuiID m_DockSpaceID;

		std::string m_Icon = "none";

		bool m_Closable = true;
		bool m_CloseSignal = true;

		bool m_SaveMode = false;
		bool m_Saved = false;
		bool m_UnsaveMarkerApplied = false;

		bool m_Opened = true;
		bool m_IsRendering = true;

		bool m_DisableDragging = false;
		bool m_DisableContextMenu = false;

		bool m_IsDragging;
		bool m_DockIsDraggingStarted;

		bool m_Pressed;

		bool m_DockingMode = false;

		std::function<void()> m_CloseCallback;
		std::function<void()> m_TabMenuCallback;

		//
		bool m_WindowRebuilded = false;
		bool m_WindowJustRebuilded = false;

		bool m_HaveParentAppWindow = false;
		std::shared_ptr<AppWindow> m_ParentAppWindow;

		AppWindowTypes m_AppWindowType = AppWindowTypes::StaticWindow;
		// Notifications
		// Number of unread notifs

		// Default behavior

		// Renders functions
		std::function<void()> m_Render;
		std::function<void()> m_MenubarRight;
		std::function<void()> m_MenubarLeft;

		std::function<void()> m_CloseEvent;
		std::vector<std::shared_ptr<AppWindow>> m_SubAppWindows;

	private:
		// Simple storage (only Key/Value), can be replicated in saves
		std::unordered_map<std::string, std::shared_ptr<SimpleStorageItem>> m_Storage;

		// Advanced storage (json & metrics), can be also replicated in saves
		std::unordered_map<std::string, std::shared_ptr<WindowStorageItem>> m_WindowStorage;

		// Fetched datas from last save
		std::unordered_map<std::string, std::string> m_LastSaveData;

		// Default behaviors (docking behaviors, default themes, internal docking emplacements.)
		std::unordered_map<DefaultAppWindowBehaviors, std::string, EnumClassHash> m_DefaultBehaviors;
	};

	class Component
	{
	public:
		Component(const std::string &id) : m_ID(id) {};
		virtual std::string GetData(const std::string &data_type) {};
		std::string m_ID;
	};

	class Window
	{
	public:
		Window(const std::string &name, int width, int height, ApplicationSpecification specs, bool cold_start);

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

		void SetFavIcon(const std::string &path)
		{
			SDL_Surface *iconSurface = IMG_Load(path.c_str());
			if (!iconSurface)
			{
				return;
			}
			SDL_SetWindowIcon(m_WindowHandler, iconSurface);
			SDL_FreeSurface(iconSurface);
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

		ApplicationSpecification m_Specifications;

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
		void SetMainRenderCallback(const std::function<void()> &mainRenderCallback) { m_MainRenderCallback = mainRenderCallback; }

		template <typename T, typename... Args>
		std::shared_ptr<T> CreateComponent(Args... args)
		{
			std::shared_ptr<T> component = std::make_shared<T>(args...);
			this->m_ApplicationComponents.push_back(component);
			return component;
		}

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

		// TODO : Can specify another "sub" specifications than the main window.
		std::string SpawnWindow();
		std::string SpawnWindow(ApplicationSpecification spec);
		void SpawnWindow(const std::string &name);
		void SpawnWindow(const std::string &name, ApplicationSpecification spec);
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
		void SetWindowSaveDataFile(const std::string &path, const bool &relative = false);
		void SynchronizeWindows();
		std::vector<std::shared_ptr<AppWindow>> GetLastSaveInstanciableAppWindows();

		std::string GetComponentData(const std::string &id, const std::string &topic)
		{
			for (auto &component : m_ApplicationComponents)
			{
				if (component->m_ID == id)
				{
					return component->GetData(topic);
				}
			}
			return "none";
		}

		std::vector<std::shared_ptr<Layer>> m_LayerStack;

		std::string PutWindow(std::shared_ptr<AppWindow> win)
		{
			m_AppWindows.push_back(win);
			return "id";
		}

		void SetFavIconPath(const std::string &path)
		{
			m_FavIconPath = path;
		}

		ImDrawData *RenderWindow(Window *window);

		// Resources
		// TODO(Yan): move out of application class since this can't be tied
		//            to application lifetime
		std::function<void()> m_MenubarCallback;
		std::function<void()> m_FramebarCallback;
		std::function<void()> m_MainRenderCallback;
		std::function<void()> m_CloseCallback; // need to destroy all windows manually

		bool m_ClosePending = false;

		std::mutex m_WindowCreationMutex;
		ImGuiContext *m_ImGuiMasterContext;

		std::string m_WindowSaveDataPath;
		bool m_SaveWindowData = false;
		bool m_IsDataSaved = true;
		bool m_IsDataInitialized = false;
		nlohmann::json m_PreviousSaveData;

		bool m_Running = false;
		bool m_Dragging = false;
		std::vector<std::shared_ptr<Window>> m_Windows;
		std::vector<std::shared_ptr<RedockRequest>> m_RedockRequests;
		std::vector<std::shared_ptr<AppWindow>> m_AppWindows;
		std::vector<std::shared_ptr<AppWindow>> m_SavedAppWindows;
		std::string m_FavIconPath;

		std::vector<std::shared_ptr<Component>> m_ApplicationComponents;

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

#define UIKIT_DATA(id, topic) Application::Get().GetComponentData(id, topic)

	// Implemented by CLIENT
	static Application *CreateApplication(int argc, char **argv);
}