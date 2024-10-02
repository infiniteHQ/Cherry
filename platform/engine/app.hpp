#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"
#include "image.hpp"
#include "window.hpp"
#include "app_window.hpp"
#include "base.hpp"
#include "components.hpp"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#include <memory>
#include <functional>
#include <filesystem>
#include <SDL2/SDL_image.h>

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

namespace UIKit
{
	class Window;
	class Component;

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

		static std::string CookPath(const std::string input_path)
		{
			std::string output_path;
			std::string root_path;

#ifdef _WIN32
			char result[MAX_PATH];
			if (GetModuleFileNameA(NULL, result, MAX_PATH) != 0)
			{
				root_path = result;
			}
			else
			{
				std::cerr << "Failed while get the root path" << std::endl;
			}
#else
			char result[PATH_MAX];
			ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
			if (count != -1)
			{
				result[count] = '\0';
				root_path = result;
			}
			else
			{
				std::cerr << "Failed while get the root path" << std::endl;
			}
#endif

			root_path = std::filesystem::path(root_path).parent_path();

			if (!input_path.empty() && input_path.front() == '/')
			{
				output_path = input_path;
			}
			else
			{
				output_path = root_path + "/" + input_path;
			}

			return output_path;
		}

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
		static std::shared_ptr<UIKit::Window> &GetCurrentRenderedWindow();
		static UIKit::WindowDragDropState &GetCurrentDragDropState();
		static bool &GetDockIsDragging();
		static std::string &GetLastWindowPressed();
		static bool &GetValidDropZoneFounded();
		static VkInstance GetInstance();
		static VkPhysicalDevice GetPhysicalDevice();
		static VkDevice GetDevice();
		static VkCommandBuffer GetCommandBuffer(bool begin);
		// static VkCommandBuffer GetCommandBuffer(bool begin);
		static VkCommandBuffer GetCommandBuffer(bool begin, ImGui_ImplVulkanH_Window *wd, const std::string &win);
		static VkCommandBuffer GetCommandBufferOfWin(const std::string &win_name, bool begin);
		static VkCommandBuffer GetCommandBuffer(const std::string &win_name, bool begin);
		static ImFont *GetFont(const std::string &name);
		static std::unordered_map<std::string, ImFont *> &GetFontList();
		std::vector<std::pair<std::string, std::pair<std::string, float>>> &GetCustomFonts();

		// Set static components
		static void SetCurrentDragDropState(UIKit::WindowDragDropState *state);
		static void SetValidDropZoneFounded(const bool &founded);
		static void SetCurrentDragDropStateDragOwner(const std::string &new_name);
		static void SetCurrentDragDropStateDockIsDragging(const bool &is_dragging);
		static void SetCurrentDragDropStateAppWindow(const std::string &new_name);
		static void SetCurrentDragDropStateAppWindowHost(const std::string &new_name);
		static void SetCurrentDragDropStateWindow(const std::string &new_name);
		static void SetCurrentDragDropStateDraggingPlace(const DockEmplacement &place);
		static void SetMasterSwapChainRebuild(const bool &new_state);
		static void IncrementWindowNumber();

		// Utils
		static void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		static void SubmitResourceFree(std::function<void()> &&func, const std::string &winname);

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

		static void FramePresent(ImGui_ImplVulkanH_Window *wd, UIKit::Window *win);
		static void FrameRender(ImGui_ImplVulkanH_Window *wd, UIKit::Window *win, ImDrawData *draw_data);

		// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
		// Your real engine/app may not use them.
		static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height, UIKit::Window *win);
		static void CleanupVulkanWindow(UIKit::Window *win);
		static void CleanupSpecificVulkanWindow(UIKit::Window *win);
		static void CleanupVulkan();
		static void CleanupVulkan(UIKit::Window *win);
		static void SetupVulkan(const char **extensions, uint32_t extensions_count);

		Window *GetWindowByHandle(SDL_Window *window_handle);
		std::string GetRootPath() { return m_RootPath; }

		bool IsMaximized(const std::shared_ptr<Window> &win) const;

		// Main Docking function
		static void PushRedockEvent(UIKit::WindowDragDropState *state);

		static std::vector<uint8_t> LoadPngHexa(const std::string &path);

		void Run();
		void LoadImages();

		template <typename T, typename... Args>
		std::shared_ptr<T> CreateComponent(Args... args)
		{
			std::shared_ptr<T> component = std::make_shared<T>(args...);
			this->m_ApplicationComponents.push_back(component);
			return component;
		}

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
		std::shared_ptr<Image> GetApplicationIcon(const std::string &window) const;
		float GetTime();
		bool IsTitleBarHovered() const { return m_TitleBarHovered; }

		ApplicationSpecification m_Specification;
		void UI_DrawTitlebar(float &outTitlebarHeight, Window *window);
		void UI_DrawMenubar();

		ImDrawData *RenderWindow(Window *window);
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

		void AddTTFFont(const std::string &name, const std::string &ttf_file_path, const float &size = 20.0f)
		{
			m_CustomFonts.push_back({name, {ttf_file_path, size}});
		};

		void SetFavIconPath(const std::string &icon_path)
		{
			m_FavIconPath = icon_path;
		}

		// Resources
		// TODO: move out of application class since this can't be tied
		//            to application lifetime
		std::function<void()> m_MenubarCallback;
		std::function<void()> m_FramebarCallback;
		std::function<void()> m_MainRenderCallback;
		std::function<void()> m_CloseCallback;

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
		
void AddLocale(const std::string& locale_name, const std::string& data_path) {
    std::ifstream file(data_path);
    if (file.is_open()) {
        nlohmann::json json_data;
        file >> json_data;
        file.close();

        if (m_Locales.find(locale_name) != m_Locales.end()) {
            auto& existing_locale = m_Locales[locale_name]["locales"];
            for (const auto& new_item : json_data["locales"]) {
                existing_locale.push_back(new_item);
            }
        } else {
            m_Locales[locale_name] = json_data;
        }
    }
}

    void SetLocale(const std::string& locale_name) {
        if (m_Locales.find(locale_name) != m_Locales.end()) {
            m_SelectedLocale = locale_name;
        }
    }

    std::string GetLocale(const std::string& locale_type) {
        if (m_SelectedLocale.empty() || m_Locales.find(m_SelectedLocale) == m_Locales.end()) {
            return "locale_undefined";
        }

        const nlohmann::json& current_locale = m_Locales[m_SelectedLocale];

        for (const auto& item : current_locale["locales"]) {
            if (item.contains(locale_type)) {
                return item[locale_type].get<std::string>();
            }
        }

        return "locale_undefined";
    }

	private:
		void Init();
		void Shutdown();

	private:
		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

		bool m_TitleBarHovered = false;
	};


#define BINPATH(rpath) UIKit::Application::CookPath(rpath);

#define UIKIT_DATA(id, topic) Application::Get().GetComponentData(id, topic)

	// Implemented by CLIENT
	static Application *CreateApplication(int argc, char **argv);
}