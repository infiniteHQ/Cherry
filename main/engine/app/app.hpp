//
//  app.hpp
//  Headers for main application features of Cherry
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include <lib/sdl2/include/SDL_scancode.h>

#include <filesystem>
#include <functional>
#include <iostream>
#include <main/core/color/color.hpp>
#include <main/core/logs/log.hpp>
#include <main/core/types/type_safety.hpp>
#include <main/engine/app/app.hpp>
#include <main/engine/app_window/app_window.hpp>
#include <main/engine/base.hpp>
#include <main/engine/components/components.hpp>
#include <main/engine/drawing/drawing.hpp>
#include <main/engine/image/image.hpp>
#include <main/engine/options.hpp>
#include <main/engine/themes/themes.hpp>
#include <main/engine/ui/notifications/notifications.hpp>
#include <main/engine/window/window.hpp>
#include <memory>
#include <mutex>
#include <options.hpp>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "app_helpers.hpp"

#ifdef CHERRY_ENABLE_AUDIO
#include "../audio/audio.hpp"
#endif  // CHERRY_ENABLE_AUDIO

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <limits.h>
#include <mach-o/dyld.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

#include "../imgui/imgui_theme.h"
#include "../imgui/wrappers/wrappers.hpp"
#include "../ui/ui.hpp"
#include "vulkan/vulkan.h"

static int current_fps;

#define CHERRY_VERSION "1.6"

#ifndef CHERRY_APP_HPP
#define CHERRY_APP_HPP

// usefull macros
#define CherryMain                 *Cherry::CreateApplication
#define CherryAppConfig            Cherry::ApplicationSpecification
#define CherryRunAsync(argc, argv) std::thread([&]() { Cherry::Main(argc, argv); }).join();
#define CherryRun(argc, argv)      Cherry::Main(argc, argv);
#define CherryComponentsPool       std::vector<std::shared_ptr<Cherry::Component>>
#define CherryNextProp(key, value) Cherry::SetNextComponentProperty(key, value)
#define CherryPath(path)           Cherry::GetPath(path)

#define CherryApp              Cherry::Application::Get()                                    // Application&
#define CherryWindow           Cherry::Application::Get().GetSafeCurrentRenderedWindow()     // Window&
#define CherryAppWindow        Cherry::Application::Get().GetSafeCurrentRenderedAppWindow()  // AppWindow&
#define CherryLastComponent    Cherry::Application::Get().GetSafeLastComponent()             // Component&
#define CherryNextComponent    Cherry::Application::Get().GetSafeNextComponent()             // Component&
#define CherryCurrentComponent Cherry::Application::Get().GetSafeCurrentComponent()          // Component&

#define CherryInline     CherryID(Cherry::Identifier::GetUniqueIndex(), Cherry::RenderMode::Inline)
#define CherryCreateOnly CherryID(Cherry::Identifier::GetUniqueIndex(), Cherry::RenderMode::CreateOnly)

namespace Cherry {
  class Component;
}  // namespace Cherry

void check_vk_result(VkResult err);

static void AppPushTabStyle() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 6.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 3.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 6.0f));
}

static void AppPopTabStyle() {
  ImGui::PopStyleVar(3);
}

namespace Cherry {
  class Window;

  class CHERRY_API Application {
   public:
    Application(const ApplicationSpecification &applicationSpecification = ApplicationSpecification());
    ~Application();

    static std::atomic<bool> &RunningState();
#ifdef CHERRY_DEBUG
    static std::atomic<bool> &DebugToolState();
    void EnableDebugTools();
    void DisableDebugTools();
    void ToggleDebugTools();

    bool m_DevtoolsSwitchUsed = false;
#endif  // CHERRY_DEBUG

    static void RequestShutdown();

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
    static std::shared_ptr<Cherry::AppWindow> &GetCurrentRenderedAppWindow();
    static std::shared_ptr<Cherry::WindowDragDropState> &GetCurrentDragDropState();
    static std::vector<std::shared_ptr<Cherry::AppWindow>> &GetTempAppWindows();
    static bool &GetDockIsDragging();
    static std::string &GetLastWindowPressed();
    static bool &GetValidDropZoneFounded();
    static VkInstance GetInstance();
    static VkPhysicalDevice GetPhysicalDevice();
    static VkDevice GetDevice();
    static VkCommandBuffer GetCommandBuffer(bool begin, const std::shared_ptr<Window> &win);
    static ImFont *GetFont(const std::string &name);
    static std::unordered_map<std::string, ImFont *> &GetFontList();
    std::vector<std::pair<std::string, std::pair<std::string, float>>> &GetCustomFonts();
    Window &GetSafeCurrentRenderedWindow();
    AppWindow &GetSafeCurrentRenderedAppWindow();

    // Set static components
    static void SetCurrentRenderedAppWindow(std::shared_ptr<Cherry::AppWindow> appWindow);
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
    static std::string CookPath(const std::string &input_path);
    static void PushRedockEvent(const std::shared_ptr<Cherry::WindowDragDropState> &state);
    static std::vector<uint8_t> LoadPngHexa(const std::string &path);
    static void SetExecutablePath();
    static std::string &GetExecutablePath();
    static SDL_Window *GetWindowHandle(const std::string &winname);
    static void AddProcessCallback(ProcessCallback process, const std::function<void()> callback);
    static void ExecuteProcessCallbacks(ProcessCallback process);

    static const std::unordered_map<CherryKey, SDL_Scancode> &GetKeyMap();
    static std::optional<CherryKey> StringToCherryKey(std::string str);

    // Set callbacks
    void SetCloseCallback(const std::function<void()> &closeCallback);
    void SetMenubarCallback(const std::function<void()> &menubarCallback);
    void SetFramebarCallback(const std::function<void()> &framebarCallback);
    void SetCloseCallback(const std::function<bool()> &closeCallback);
    void SetMainRenderCallback(const std::function<void()> &mainRenderCallback);
    void SetCurrentRedockEvent(const std::shared_ptr<Cherry::WindowDragDropState> &state);

    std::string CertifyWindowName(const std::string &name);

    // Window factory
    std::string SpawnWindow();
    std::string SpawnWindow(ApplicationSpecification spec);
    void SpawnWindow(const std::string &name);
    void SpawnWindow(const std::string &name, ApplicationSpecification spec);
    void UnspawnWindow(const std::string &name);

    // Main loop utilities
    void ApplyDockingFromSave();
    void ApplyDockingFromDefault();
    void ProcessDeferredRequests();
    void RebuildDockingLinks();
    void PresentAllWindows();
    void CleanupWindowIfEmpty(const std::shared_ptr<Window> &win);
    void CurrentDockRequestOnNewWindow();

    // Window "Low" Rendering
    void HandleResizing(Window *window);
    void HandleDockingModeRendering(Window *window);
    void HandleTabsModeRendering(Window *window);
    void HandleSimpleWindowRendering(Window *window);
    void HandleSimpleRendering(Window *window);
    void HandleLayerStackUpdate(Window *window);
    void PrepareViewport(Window *window);

    // Audio service if CHERRY_ENABLE_AUDIO
    void StartAudioService();
    void StopAudioService();
    void PlaySound(const std::string &wav_file_path);
#ifdef CHERRY_ENABLE_AUDIO
    void CreateChannel(const std::string &name);
    std::shared_ptr<AudioChannel> GetChannel(const std::string &name);
    std::map<std::string, std::shared_ptr<AudioChannel>> &GetChannels();
#endif

    static void FramePresent(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win);
    static void FrameRender(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win, ImDrawData *draw_data);

    // All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers
    // used by the demo. Your real engine/app may not use them.
    static void
    SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height, Cherry::Window *win);
    static void CleanupVulkanWindow(Cherry::Window *win);
    static void CleanupSpecificVulkanWindow(Cherry::Window *win);
    static void CleanupVulkan();
    static void CleanupVulkan(Cherry::Window *win);
    static void SetupVulkan(const char **extensions, uint32_t extensions_count);

    bool IsKeyPressed(CherryKey key);
    bool IsMouseClicked(int btn, bool repeat = false);
    bool IsMouseDoubleClicked(int btn);
    bool IsMousePressed(int btn);

    Window *GetWindowByHandle(SDL_Window *window_handle);
    std::string GetRootPath();

    bool IsMaximized(const std::shared_ptr<Window> &win) const;

    void BoostrappWindow();

    void SingleThreadRuntime();       // Run in single thread mode. (Better for third
                                      // party thread safety)
    void MultiThreadWindowRuntime();  // EXPERIMENTAL : Run in mutliple thread
                                      // mode. (Worse for third party thread safety
                                      // but more performant.)

    void NewWinInstance(const std::string &name);

    void Close();
    bool IsMaximized() const;
    float GetTime();
    bool IsTitleBarHovered() const;

    ImDrawData *RenderWindow(Window *window);
    void InitializeWindowStates();
    void SaveData();
    void SetWindowSaveDataFile(const std::string &path, const bool &relative = false);
    void SynchronizeWindows();
    std::vector<std::shared_ptr<AppWindow>> GetLastSaveInstanciableAppWindows();
    std::string PutWindow(const std::shared_ptr<AppWindow> &win);
    void AddFont(const std::string &name, const std::string &ttf_file_path, const float &size = 20.0f);
    void SetFavIconPath(const std::string &icon_path);
    void DeleteAppWindow(const std::shared_ptr<AppWindow> &win);
    void EnableNoAppWindowSafety();
    void DisableNoAppWindowSafety();

    // Locales & Traductions
    void AddLocale(const std::string &locale_name, const std::string &data_path);
    void OverrideLocale(const std::string &locale_name, const std::string &data_path);
    void SetLocale(const std::string &locale_name);
    void SetDefaultLocale(const std::string &locale_name);
    std::string GetLocale(const std::string &locale_type);

    // Net & Cache
    std::string GetHttpCacheFolderName();
    void SetHttpCacheFolderName(const std::string &name);

    // Identified component
    static Component &GetAnonymousComponent(const Identifier &identifier);
    static Component &GetComponent(const Identifier &identifier);
    static std::shared_ptr<Component> GetComponentPtr(const Identifier &identifier);

    std::string GetComponentData(const Identifier &id, const std::string &topic);

    std::string GetName();

    Cherry::Component &GetSafeLastComponent();
    void SetLastComponent(Component *component);
    void ResetLastComponent();

    Cherry::Component &GetSafeNextComponent();
    void SetNextComponent(Component *component);
    void ResetNextComponent();

    Cherry::Component &GetSafeCurrentComponent();
    void SetCurrentComponent(Component *component);
    void ResetCurrentComponent();

    // Add component array, create
    // components will create in it
    void PushComponentPool(ComponentsPool *component_pool);
    // REstore previous component array, if 0 it will be
    // created in the application component array
    void PopComponentPool();
    ComponentsPool *GetComponentPool();
    std::vector<ComponentsPool *> &GetComponentPoolStack();
    void ClearComponentPool();
    void ClearComponentPoolStack();

    void RenderComponent(Component &component);
    void RenderComponent(const std::shared_ptr<Component> &component);

    // App window data
    void SetDescriptionForAppWindow(const std::string &windowId, const std::string &description);
    void SetLogoPathForAppWindow(const std::string &windowId, const std::string &path);
    const std::string &GetDescriptionForAppWindow(const std::string &windowId);
    const std::string &GetLogoPathForAppWindow(const std::string &windowId);
    void ClearThemes();

    static Identifier GetAnonymousID();

    template<typename... Args>
    static Identifier GenerateUniqueID(const Args &...args) {
      std::size_t seed = 0;
      (HashAny(seed, args), ...);
      return Identifier(std::to_string(seed));
    }

    static Identifier GenerateUniqueID(const std::vector<std::any> &args) {
      std::size_t seed = 0;
      for (const auto &arg : args) {
        if (arg.type() == typeid(int)) {
          HashAnyImpl(seed, std::any_cast<int>(arg), std::true_type{});
        } else if (arg.type() == typeid(float)) {
          HashAnyImpl(seed, std::any_cast<float>(arg), std::true_type{});
        } else if (arg.type() == typeid(double)) {
          HashAnyImpl(seed, std::any_cast<double>(arg), std::true_type{});
        } else if (arg.type() == typeid(std::string)) {
          HashAnyImpl(seed, std::any_cast<std::string>(arg), std::true_type{});
        } else if (arg.type() == typeid(const char *)) {
          HashAnyImpl(seed, std::string(std::any_cast<const char *>(arg)), std::true_type{});
        } else {
          // Fallback pointer hashing
          HashAnyImpl(seed, reinterpret_cast<std::size_t>(&arg), std::false_type{});
        }
      }
      return Cherry::Identifier(std::to_string(seed));
    }

    template<typename T>
    static void HashAnyImpl(std::size_t &seed, const T &value, std::true_type) {
      std::hash<T> hasher;
      seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<typename T>
    static void HashAnyImpl(std::size_t &seed, const T &value, std::false_type) {
      seed ^= reinterpret_cast<std::size_t>(&value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<typename T>
    static void HashAny(std::size_t &seed, const T &value) {
      HashAnyImpl(seed, value, std::is_invocable<std::hash<T>, T>{});
    }

    static Identifier GetAnonymousID(const std::string &label);

    template<typename T>
    static std::shared_ptr<Component> CreateAnonymousComponent(const T &component) {
      T component_copy = component;

      if (component_copy.GetIdentifier().string().empty() || component_copy.GetIdentifier().string() == "anonymous") {
        Identifier anonymous_id = component_copy.GetIdentifier();
        anonymous_id.set(Identifier::GetUniqueIndex());
        component_copy.SetIdentifier(anonymous_id);
      }

      std::shared_ptr<Component> new_component = std::make_shared<T>(component_copy);

      ComponentsPool *pool = Application::Get().GetComponentPool();
      pool->AnonymousComponents.push_back(new_component);

      return new_component;
    }

    template<typename T, typename... Args>
    Component &PushComponent(const Identifier &identifier, Args &&...args) {
      switch (GetRenderMode()) {
        case RenderMode::Inline: {
          auto new_button = std::make_shared<T>(identifier, std::forward<Args>(args)...);
          new_button->RenderWrapper();
          return *new_button;
        }
        case RenderMode::CreateOnly: {
          auto &existing_button = Application::GetComponent(identifier);
          if (existing_button.GetIdentifier().string() != "undefined") {
            CherryApp.ResetNextComponent();
            return existing_button;
          } else {
            auto new_button = Application::CreateComponent<T>(identifier, std::forward<Args>(args)...);
            CherryApp.ResetNextComponent();
            return *new_button;
          }
        }
        default: {
          auto &existing_button = Application::GetComponent(identifier);
          if (existing_button.GetIdentifier().string() != "undefined") {
            existing_button.RenderWrapper();
            return existing_button;
          } else {
            auto new_button = Application::CreateComponent<T>(identifier, std::forward<Args>(args)...);
            new_button->RenderWrapper();
            return *new_button;
          }
        }
      }
    }

    template<typename T, typename... Args>
    static std::shared_ptr<Component> CreateComponent(const Identifier &id, Args &&...args) {
      if (id.component_array_ptr() != nullptr) {
        auto *array = &id.component_array_ptr()->IdentifiedComponents;

        for (const auto &existing_component : *array) {
          if (existing_component->GetIdentifier() == id) {
            return existing_component;
          }
        }

        auto component_ptr = std::make_shared<T>(id, std::forward<Args>(args)...);
        array->push_back(component_ptr);
        return component_ptr;
      }

      ComponentsPool *pool = Application::Get().GetComponentPool();

      for (const auto &existing_component : pool->IdentifiedComponents) {
        if (existing_component->GetIdentifier() == id) {
          return existing_component;
        }
      }

      auto component_ptr = std::make_shared<T>(id, std::forward<Args>(args)...);
      pool->IdentifiedComponents.push_back(component_ptr);
      return component_ptr;
    }

    void PushComponentGroup(const std::string &groupname);
    void PopComponentGroup(int pop_number = 0);
    std::string GetComponentGroup() const;

    void PushCurrentComponent(const std::shared_ptr<Component> &component);
    void PopCurrentComponent(int pop_number = 0);
    std::shared_ptr<Component> GetCurrentComponent() const;

    // Theme engine
    void AddTheme(const Theme &theme);
    void RemoveTheme(const std::string &theme_name);
    void PushTheme(const std::string &theme_name);
    void PopTheme(int number = 1);
    std::string GetThemeProperty(const std::string &theme_name, const std::string &key);
    std::string GetActiveThemeProperty(const std::string &key);

    void PurgeNoRenderedComponents(ComponentsPool *pool = nullptr);
    void RefreshComponentsRenderFlags(ComponentsPool *pool = nullptr);
    void DestroyComponent(const Identifier &id, ComponentsPool *pool = nullptr);
    void RefreshComponent(const Identifier &id, ComponentsPool *pool = nullptr);

    void PushComponentRenderMode(Cherry::RenderMode render_mode);
    void PopRenderMode(int number = 1);
    RenderMode GetRenderMode() const;
    bool IsValidRenderMode(Cherry::RenderMode mode) const;

    // Different than the default theme !
    // When the user specify this, the runtime will search level 1 theme at
    // GetThemeProperty(theme, key) "manually", and not from the active theme.
    void SetTheme(const std::string &theme_name);
    void RemoveTheme();
    void SetPropertyOnGroup(const std::string &group_name, const std::string &key, const std::string &value);

    void SetDataOnGroup(const std::string &group_name, const std::string &key, const std::string &value);

#ifdef CHERRY_ENABLE_AUDIO
    ma_engine m_AudioEngine;
    std::map<std::string, std::shared_ptr<AudioChannel>> m_Channels;
#endif

    std::vector<std::pair<std::string, std::string>> m_PermanentProperties;  // Properties will be added in every component
                                                                             // (until pop)
    void PushPermanentProperty(const std::string &property, const std::string &value);
    void PopPermanentProperty(int number_of_pops = 0);

    std::unordered_map<std::string, std::string> m_OnTimeProperties;  // Properties will be added in the next component
    void AddOneTimeProperty(const std::string &property, const std::string &value);
    const std::unordered_map<std::string, std::string> &GetOneTimeProperties();
    void RemoveOneTimeProperty(const std::string &key);
    void ClearOneTimeProperties();

    std::vector<std::shared_ptr<Component>> m_ParentComponentsStack;
    void PushParentComponent(const std::shared_ptr<Component> &component);
    void PopParentComponent();
    std::shared_ptr<Component> GetParent(int parent_number = 0);

    // std::vector<std::shared_ptr<Component>> m_ApplicationComponents;
    // std::vector<std::shared_ptr<Component>> m_ApplicationAnonymousComponents;

    std::vector<std::shared_ptr<Window>> &GetWindows() {
      return m_Windows;
    }
    std::vector<std::shared_ptr<AppWindow>> &GetAppWindows() {
      return m_AppWindows;
    }
    std::vector<std::shared_ptr<AppWindow>> &GetSavedAppWindows() {
      return m_SavedAppWindows;
    }
    const std::vector<std::shared_ptr<Window>> &GetWindows() const {
      return m_Windows;
    }
    const std::vector<std::shared_ptr<AppWindow>> &GetAppWindows() const {
      return m_AppWindows;
    }
    const std::vector<std::shared_ptr<AppWindow>> &GetSavedAppWindows() const {
      return m_SavedAppWindows;
    }

    bool &GetIsDataSaved() {
      return m_IsDataSaved;
    }
    bool &GetIsDataInitialized() {
      return m_IsDataInitialized;
    }

    const bool &GetIsDataSaved() const {
      return m_IsDataSaved;
    }
    const bool &GetIsDataInitialized() const {
      return m_IsDataInitialized;
    }

    void SetIsDataSaved(bool value) {
      m_IsDataSaved = value;
    }
    void SetIsDataInitialized(bool value) {
      m_IsDataInitialized = value;
    }

    std::function<void()> &GetMenubarCallback() {
      return m_MenubarCallback;
    }
    std::function<void()> &GetFramebarCallback() {
      return m_FramebarCallback;
    }
    std::function<void()> &GetMainRenderCallback() {
      return m_MainRenderCallback;
    }
    std::function<void()> &GetCloseCallback() {
      return m_CloseCallback;
    }

    // const overloads
    const std::function<void()> &GetMenubarCallback() const {
      return m_MenubarCallback;
    }
    const std::function<void()> &GetFramebarCallback() const {
      return m_FramebarCallback;
    }
    const std::function<void()> &GetMainRenderCallback() const {
      return m_MainRenderCallback;
    }
    const std::function<void()> &GetCloseCallback() const {
      return m_CloseCallback;
    }
    ApplicationSpecification &GetDefaultSpecification() {
      return m_DefaultSpecification;
    }
    const ApplicationSpecification &GetDefaultSpecification() const {
      return m_DefaultSpecification;
    }

   private:
    void Init();
    void Shutdown();

   private:
    float m_TimeStep = 0.0f;
    float m_FrameTime = 0.0f;
    float m_LastFrameTime = 0.0f;
    std::vector<std::shared_ptr<Window>> m_Windows;
    std::vector<std::shared_ptr<AppWindow>> m_AppWindows;
    std::vector<std::shared_ptr<AppWindow>> m_SavedAppWindows;

    std::vector<std::string> m_PushedComponentGroups;                  // List of groups
    std::vector<std::shared_ptr<Component>> m_PushedCurrentComponent;  // List of groups

    bool m_TitleBarHovered = false;
    ComponentsPool m_ApplicationComponentPool;
    std::vector<ComponentsPool *> m_ComponentPoolStack;

    std::unordered_map<std::string, nlohmann::json> m_Locales;
    std::string m_SelectedLocale;
    std::string m_DefaultLocale;

    std::vector<RenderMode> m_RenderModeStack;

    std::unordered_map<std::string, Theme> m_Themes;
    std::vector<Theme> m_ActiveThemes;
    std::string m_SelectedTheme = "undefined";

    std::string m_HttpCacheFolderName = "http_cache";
    std::unordered_map<std::string, WindowMetaData> m_AppWindowRegistry;
    std::mutex m_WindowCreationMutex;

    std::string m_WindowSaveDataPath;
    bool m_SaveWindowData = false;
    bool m_IsDataSaved = true;
    bool m_IsDataInitialized = false;
    nlohmann::json m_PreviousSaveData;
    std::string m_RootPath;

    std::function<void()> m_MenubarCallback;
    std::function<void()> m_FramebarCallback;
    std::function<void()> m_MainRenderCallback;
    std::function<void()> m_CloseCallback;
    ApplicationSpecification m_DefaultSpecification;

    std::shared_ptr<Component> m_FocusedDebugComponent;
    bool m_ClosePending = false;

    bool m_NoAppWindowSafetyEnabled = false;
    bool m_Running = false;
    bool m_Dragging = false;

    SDL_Window *m_MouseHoveredWindow = nullptr;

    std::vector<std::shared_ptr<RedockRequest>> m_RedockRequests;
    std::vector<std::pair<std::string, std::pair<std::string, float>>> m_CustomFonts;
    std::string m_FavIconPath;
  };

  //
  // Main utilities
  // TODO : Move to api section

  // Window
  CHERRY_API std::shared_ptr<Cherry::Window> &GetCurrentRenderedWindow();

  // AppWindow
  CHERRY_API void AddAppWindow(const std::shared_ptr<AppWindow> &win);
  CHERRY_API void DeleteAppWindow(const std::shared_ptr<AppWindow> &win);
  CHERRY_API std::shared_ptr<AppWindow> GetAppWindowByName(const std::string &win_name);
  CHERRY_API std::shared_ptr<AppWindow> GetCurrentRenderedAppWindow();
  CHERRY_API std::shared_ptr<Window> GetWindowByName(const std::string &win_name);

  // Images
  CHERRY_API ImTextureID GetTexture(const std::string &path);
  CHERRY_API ImVec2 GetTextureSize(const std::string &path);

  // Notification
  CHERRY_API void AddNotification(const ImGuiToast &toast);

  // Font
  CHERRY_API void PushFont(const std::string &font_name);
  CHERRY_API void PopFont();

  // Runtime
  CHERRY_API std::string GetPath(const std::string &path);
  CHERRY_API std::string GetLocale(const std::string &topic);
#ifdef CHERRY_ENABLE_NET
  CHERRY_API std::string GetHttpPath(const std::string &url);
#endif  // CHERRY_ENABLE_NET

  // Data (theses functions can return JSON to string format or legacy string.)
  CHERRY_API std::string GetData(const Identifier &id, const std::string topic);
  CHERRY_API std::string GetWindowData(const std::string &id, const std::string topic);

  CHERRY_API void PushPermanentProperty(const std::string &property, const std::string &value);
  CHERRY_API void PopPermanentProperty(int number_of_pops = 0);
  CHERRY_API void SetNextComponentProperty(const std::string &property, const std::string &value);

  CHERRY_API void PushParentComponent(const std::shared_ptr<Component> &component);
  CHERRY_API void PopParentComponent();
  CHERRY_API std::shared_ptr<Component> GetParent(int parent_number = 0);

  CHERRY_API bool IsReady();
  CHERRY_API float GetDeltaTime();
  CHERRY_API float GetTime();

  // Implemented by CLIENT
  Application *CreateApplication(int argc, char **argv);
}  // namespace Cherry

#endif