#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"
#include "image.hpp"
#include "window.hpp"
#include "app_window.hpp"
#include "base.hpp"
#include "components.hpp"
#include "ui/notifications/notifications.hpp"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#include <memory>
#include <functional>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

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

namespace Cherry
{
	class Window;

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

		// Set static components
		static Application &Get();
		static VkDevice &GetVkDevice();
		static VkPhysicalDevice &GetVkPhysicalDevice();
		static uint32_t &GetQueueFamily();
		static VkQueue &GetVkQueue();
		static VkAllocationCallbacks &GetVkAllocationCallbacks();
		static VkDebugReportCallbackEXT &GetVkDebugReportCallbackEXT();
		static VkPipelineCache &GetVkPipelineCache();
		static VkDescriptorPool &GetVkDescriptorPool();
		static int &GetMinImageCount();
		static std::shared_ptr<Cherry::Window> &GetCurrentRenderedWindow();
		static std::shared_ptr<Cherry::WindowDragDropState> &GetCurrentDragDropState();
		static bool &GetDockIsDragging();
		static std::string &GetLastWindowPressed();
		static bool &GetValidDropZoneFounded();
		static VkInstance GetInstance();
		static VkPhysicalDevice GetPhysicalDevice();
		static VkDevice GetDevice();
		static VkCommandBuffer GetCommandBufferOfWin(const std::string &win_name, bool begin);
    	static VkCommandBuffer GetCommandBuffer(bool begin, const std::shared_ptr<Window>& win);
		static ImFont *GetFont(const std::string &name);
		static std::unordered_map<std::string, ImFont *> &GetFontList();
		std::vector<std::pair<std::string, std::pair<std::string, float>>> &GetCustomFonts();

		// Set static components
		static void SetCurrentDragDropState(const std::shared_ptr<Cherry::WindowDragDropState> &state);
		static void SetValidDropZoneFounded(const bool &founded);
		static void SetCurrentDragDropStateDragOwner(const std::string &new_name);
		static void SetCurrentDragDropStateDockIsDragging(const bool &is_dragging);
		static void SetCurrentDragDropStateAppWindow(const std::string &new_name);
		static void SetCurrentDragDropStateAppWindowHost(const std::string &new_name);
		static void SetCurrentDragDropStateWindow(const std::string &new_name);
		static void SetCurrentDragDropStateDraggingPlace(const DockEmplacement &place);
		static void SetMasterSwapChainRebuild(const bool &new_state);
		static void SetLastWindowPressed(const std::string &name);
		static void IncrementWindowNumber();

		// Utils
		static void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		static void SubmitResourceFree(std::function<void()> &&func, const std::string &winname);
		static std::string CookPath(const std::string input_path);
		static void PushRedockEvent(const std::shared_ptr<Cherry::WindowDragDropState> &state);
		static std::vector<uint8_t> LoadPngHexa(const std::string &path);

		// Set callbacks
		void SetCloseCallback(const std::function<void()> &closeCallback) { m_CloseCallback = closeCallback; }
		void SetMenubarCallback(const std::function<void()> &menubarCallback) { m_MenubarCallback = menubarCallback; }
		void SetFramebarCallback(const std::function<void()> &framebarCallback) { m_FramebarCallback = framebarCallback; }
		void SetCloseCallback(const std::function<bool()> &closeCallback) { m_CloseCallback = closeCallback; }
		void SetMainRenderCallback(const std::function<void()> &mainRenderCallback) { m_MainRenderCallback = mainRenderCallback; }

		// Window factory
		std::string SpawnWindow();
		std::string SpawnWindow(ApplicationSpecification spec);
		void SpawnWindow(const std::string &name);
		void SpawnWindow(const std::string &name, ApplicationSpecification spec);
		void UnspawnWindow(const std::string &name);

		// Main loop utilities
		void ApplyDockingFromSave();
		void ApplyDockingFromDefault();
		void RebuildDockingLinks();
		void PresentAllWindows();
		void CleanupEmptyWindows();
		void CurrentDockRequestOnNewWindow();

		// Window "Low" Rendering
		void HandleResizing(Window *window);
		void HandleDockingModeRendering(Window *window);
		void HandleTabsModeRendering(Window *window);
		void HandleSimpleWindowRendering(Window *window);
		void HandleSimpleRendering(Window *window);
		void HandleLayerStackUpdate(Window *window);
		void PrepareViewport(Window *window);

		static void FramePresent(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win);
		static void FrameRender(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win, ImDrawData *draw_data);

		// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
		// Your real engine/app may not use them.
		static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height, Cherry::Window *win);
		static void CleanupVulkanWindow(Cherry::Window *win);
		static void CleanupSpecificVulkanWindow(Cherry::Window *win);
		static void CleanupVulkan();
		static void CleanupVulkan(Cherry::Window *win);
		static void SetupVulkan(const char **extensions, uint32_t extensions_count);

		Window *GetWindowByHandle(SDL_Window *window_handle);
		std::string GetRootPath() { return m_RootPath; }

		bool IsMaximized(const std::shared_ptr<Window> &win) const;

		void BoostrappWindow();
		void Run();

		void PushLayer(const std::shared_ptr<Layer> &layer);
		void NewWinInstance(const std::string &name);

		void Close();
		bool IsMaximized() const;
		float GetTime();
		bool IsTitleBarHovered() const { return m_TitleBarHovered; }


		ImDrawData *RenderWindow(Window *window);
		void InitializeWindowStates();
		void SaveData();
		void SetWindowSaveDataFile(const std::string &path, const bool &relative = false);
		void SynchronizeWindows();
		std::vector<std::shared_ptr<AppWindow>> GetLastSaveInstanciableAppWindows();
		std::string GetComponentData(const std::string &id, const std::string &topic);
		std::string PutWindow(std::shared_ptr<AppWindow> win);
		void AddFont(const std::string &name, const std::string &ttf_file_path, const float &size = 20.0f);
		void SetFavIconPath(const std::string &icon_path);
		void DeleteAppWindow(const std::shared_ptr<AppWindow>& win);

		// Locales & Traductions
		void AddLocale(const std::string &locale_name, const std::string &data_path);
		void SetLocale(const std::string &locale_name);
		void SetDefaultLocale(const std::string &locale_name);
		std::string GetLocale(const std::string &locale_type);

		// Generic utils
		template <typename T, typename... Args>
		std::shared_ptr<T> CreateComponent(Args... args)
		{
			std::shared_ptr<T> component = std::make_shared<T>(args...);
			this->m_ApplicationComponents.push_back(component);
			return component;
		}

		template <typename T>
		void PushLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
			m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
		}

		// Resources
		// TODO: move out of application class since this can't be tied
		//            to application lifetime
		std::function<void()> m_MenubarCallback;
		std::function<void()> m_FramebarCallback;
		std::function<void()> m_MainRenderCallback;
		std::function<void()> m_CloseCallback;
		ApplicationSpecification m_DefaultSpecification;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;

		std::string m_RootPath;

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
		std::vector<std::pair<std::string, std::pair<std::string, float>>> m_CustomFonts;
		std::string m_FavIconPath;

		std::vector<std::shared_ptr<Component>> m_ApplicationComponents;

		std::unordered_map<std::string, nlohmann::json> m_Locales;
		std::string m_SelectedLocale;
		std::string m_DefaultLocale;

	private:
		void Init();
		void Shutdown();

	private:
		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

		bool m_TitleBarHovered = false;
	};

	//
	// Main utilities
	//

	// Window
	std::shared_ptr<Cherry::Window> &GetCurrentRenderedWindow();

	// AppWindow
 	void AddAppWindow(const std::shared_ptr<AppWindow>& win);
 	void DeleteAppWindow(const std::shared_ptr<AppWindow>& win);
 	std::shared_ptr<AppWindow> GetAppWindowByName(const std::string& win_name);
    std::shared_ptr<Window> GetWindowByName(const std::string &win_name);

	// Images
	ImTextureID GetTexture(const std::string &path);
	ImVec2 GetTextureSize(const std::string &path);

	// Runtime
	std::string GetPath(const std::string &path);
	std::string GetLocale(const std::string &topic); 

	// Data (theses functions can return JSON to string format or legacy string.)
	std::string GetComponentData(const std::string& id, const std::string topic);
	std::string GetWindowData(const std::string& id, const std::string topic);

#define UIKIT_DATA(id, topic) Application::Get().GetComponentData(id, topic)

	// Implemented by CLIENT
	static Application *CreateApplication(int argc, char **argv);
}