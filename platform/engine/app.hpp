#pragma once

/*
TODO :

// COMPONENTS LOGICS
Cherry::GetLastRenderedComponent();

// THEME (with all generic properties (h1col, h2col, bg1, bg2, bg3, borders,
etc...)) Colors + Styles effects !! Cherry::SetThemeProperty();
Cherry::GetThemeProperty();
Cherry::PushTheme(); // Push a theme on a specified area.
Cherry::PopTheme();
Cherry::AddTheme();
Cherry::SetTheme();

 */

#include "../../options.hpp"
#include "../../src/core/color.hpp"
#include "../../src/layer.hpp"
#include "app_window.hpp"
#include "base.hpp"
#include "cef.hpp"
#include "components.hpp"
#include "image.hpp"
#include "themes.hpp"
#include "ui/notifications/notifications.hpp"
#include "window.hpp"

// Libs
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "../../lib/miniaudio/miniaudio.h"
#include "../../lib/sdl2/include/SDL_scancode.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

#include "imgui/ImGuiTheme.h"
#include "imgui/wrappers.hpp"
#include "ui/ui.hpp"
#include "vulkan/vulkan.h"

static int current_fps;

#define CHERRY_VERSION "1.3 WIP"

#ifndef CHERRY_APP_H
#define CHERRY_APP_H

// If not Disbale Macros
#define CherryCreateApp *Cherry::CreateApplication
#define CherryMain *Cherry::CreateApplication
#define CherryAppConfig Cherry::ApplicationSpecification
#define CherryRun(argc, argv)                                                  \
  std::thread([&]() { Cherry::Main(argc, argv); }).join();

#define CherryComponentsPool std::vector<std::shared_ptr<Cherry::Component>>

// Next component property
#define CherryNextProp(key, value) Cherry::SetNextComponentProperty(key, value)

// Code utils macros
#define CherryLambda(code) [=]() { code }

// Cherry utils macros
#define CherryPath(path) Cherry::GetPath(path)

/////////////////////////////////////////////////
// Cherry dynamic variables
/////////////////////////////////////////////////
// 3 Levels of rendering layers (App, Window and AppWindow)
#define CherryApp Cherry::Application::Get() // Application&
#define CherryWindow                                                           \
  Cherry::Application::Get().GetSafeCurrentRenderedWindow() // Window&
#define CherryAppWindow                                                        \
  Cherry::Application::Get().GetSafeCurrentRenderedAppWindow() // AppWindow&

// Components
#define CherryLastComponent                                                    \
  Cherry::Application::Get().GetSafeLastComponent() // Component&
#define CherryNextComponent                                                    \
  Cherry::Application::Get().GetSafeNextComponent() // Component&
#define CherryCurrentComponent                                                 \
  Cherry::Application::Get().GetSafeCurrentComponent() // Component&
/////////////////////////////////////////////////

// Cherry Identifier macros
#define CherryInline                                                           \
  CherryID(Cherry::Identifier::GetUniqueIndex(), Cherry::RenderMode::Inline)
#define CherryCreateOnly                                                       \
  CherryID(Cherry::Identifier::GetUniqueIndex(), Cherry::RenderMode::CreateOnly)

namespace Cherry {
class Component;
} // namespace Cherry

void check_vk_result(VkResult err);

static void AppPushTabStyle() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 6.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 3.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 6.0f));
}

static void AppPopTabStyle() { ImGui::PopStyleVar(3); }

namespace std {
template <> struct hash<Cherry::Identifier> {
  size_t operator()(const Cherry::Identifier &identifier) const {
    return std::hash<std::string>{}(identifier.string());
  }
};
} // namespace std

namespace Cherry {
class Window;

enum class ProcessCallback { ON_INITIALIZATION_FINISHED };

enum class CherryKey {
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  NUM_0,
  NUM_1,
  NUM_2,
  NUM_3,
  NUM_4,
  NUM_5,
  NUM_6,
  NUM_7,
  NUM_8,
  NUM_9,
  ESCAPE,
  SPACE,
  ENTER,
  BACKSPACE,
  TAB,
  SHIFT,
  CTRL,
  ALT,
  LEFT,
  RIGHT,
  UP,
  DOWN,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12
};
static const std::unordered_map<CherryKey, SDL_Scancode> &GetKeyMap() {
  static std::unordered_map<CherryKey, SDL_Scancode> keyMap = {
      {CherryKey::A, SDL_SCANCODE_A},
      {CherryKey::B, SDL_SCANCODE_B},
      {CherryKey::C, SDL_SCANCODE_C},
      {CherryKey::D, SDL_SCANCODE_D},
      {CherryKey::E, SDL_SCANCODE_E},
      {CherryKey::F, SDL_SCANCODE_F},
      {CherryKey::G, SDL_SCANCODE_G},
      {CherryKey::H, SDL_SCANCODE_H},
      {CherryKey::I, SDL_SCANCODE_I},
      {CherryKey::J, SDL_SCANCODE_J},
      {CherryKey::K, SDL_SCANCODE_K},
      {CherryKey::L, SDL_SCANCODE_L},
      {CherryKey::M, SDL_SCANCODE_M},
      {CherryKey::N, SDL_SCANCODE_N},
      {CherryKey::O, SDL_SCANCODE_O},
      {CherryKey::P, SDL_SCANCODE_P},
      {CherryKey::Q, SDL_SCANCODE_Q},
      {CherryKey::R, SDL_SCANCODE_R},
      {CherryKey::S, SDL_SCANCODE_S},
      {CherryKey::T, SDL_SCANCODE_T},
      {CherryKey::U, SDL_SCANCODE_U},
      {CherryKey::V, SDL_SCANCODE_V},
      {CherryKey::W, SDL_SCANCODE_W},
      {CherryKey::X, SDL_SCANCODE_X},
      {CherryKey::Y, SDL_SCANCODE_Y},
      {CherryKey::Z, SDL_SCANCODE_Z},
      {CherryKey::NUM_0, SDL_SCANCODE_0},
      {CherryKey::NUM_1, SDL_SCANCODE_1},
      {CherryKey::NUM_2, SDL_SCANCODE_2},
      {CherryKey::NUM_3, SDL_SCANCODE_3},
      {CherryKey::NUM_4, SDL_SCANCODE_4},
      {CherryKey::NUM_5, SDL_SCANCODE_5},
      {CherryKey::NUM_6, SDL_SCANCODE_6},
      {CherryKey::NUM_7, SDL_SCANCODE_7},
      {CherryKey::NUM_8, SDL_SCANCODE_8},
      {CherryKey::NUM_9, SDL_SCANCODE_9},
      {CherryKey::ESCAPE, SDL_SCANCODE_ESCAPE},
      {CherryKey::SPACE, SDL_SCANCODE_SPACE},
      {CherryKey::ENTER, SDL_SCANCODE_RETURN},
      {CherryKey::BACKSPACE, SDL_SCANCODE_BACKSPACE},
      {CherryKey::TAB, SDL_SCANCODE_TAB},
      {CherryKey::SHIFT, SDL_SCANCODE_LSHIFT},
      {CherryKey::CTRL, SDL_SCANCODE_LCTRL},
      {CherryKey::ALT, SDL_SCANCODE_LALT},
      {CherryKey::LEFT, SDL_SCANCODE_LEFT},
      {CherryKey::RIGHT, SDL_SCANCODE_RIGHT},
      {CherryKey::UP, SDL_SCANCODE_UP},
      {CherryKey::DOWN, SDL_SCANCODE_DOWN},
      {CherryKey::F1, SDL_SCANCODE_F1},
      {CherryKey::F2, SDL_SCANCODE_F2},
      {CherryKey::F3, SDL_SCANCODE_F3},
      {CherryKey::F4, SDL_SCANCODE_F4},
      {CherryKey::F5, SDL_SCANCODE_F5},
      {CherryKey::F6, SDL_SCANCODE_F6},
      {CherryKey::F7, SDL_SCANCODE_F7},
      {CherryKey::F8, SDL_SCANCODE_F8},
      {CherryKey::F9, SDL_SCANCODE_F9},
      {CherryKey::F10, SDL_SCANCODE_F10},
      {CherryKey::F11, SDL_SCANCODE_F11},
      {CherryKey::F12, SDL_SCANCODE_F12}};
  return keyMap;
}
struct ParentWindow {
  std::vector<std::shared_ptr<Layer>> m_LayerStack;
  std::string window_name;
};

class Application {
public:
  Application(const ApplicationSpecification &applicationSpecification =
                  ApplicationSpecification());
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
  static std::shared_ptr<Cherry::AppWindow> &GetCurrentRenderedAppWindow();

  Window &GetSafeCurrentRenderedWindow();
  AppWindow &GetSafeCurrentRenderedAppWindow();

  static std::shared_ptr<Cherry::WindowDragDropState> &
  GetCurrentDragDropState();
  static std::vector<std::shared_ptr<Cherry::AppWindow>> &GetTempAppWindows();
  static bool &GetDockIsDragging();
  static std::string &GetLastWindowPressed();
  static bool &GetValidDropZoneFounded();
  static VkInstance GetInstance();
  static VkPhysicalDevice GetPhysicalDevice();
  static VkDevice GetDevice();
  static VkCommandBuffer GetCommandBufferOfWin(const std::string &win_name,
                                               bool begin);
  static VkCommandBuffer GetCommandBuffer(bool begin,
                                          const std::shared_ptr<Window> &win);
  static ImFont *GetFont(const std::string &name);
  static std::unordered_map<std::string, ImFont *> &GetFontList();
  std::vector<std::pair<std::string, std::pair<std::string, float>>> &
  GetCustomFonts();

  // Set static components
  static void SetCurrentDragDropState(
      const std::shared_ptr<Cherry::WindowDragDropState> &state);
  static void SetValidDropZoneFounded(const bool &founded);
  static void SetCurrentDragDropStateDragOwner(const std::string &new_name);
  static void SetCurrentDragDropStateDockIsDragging(const bool &is_dragging);
  static void SetCurrentDragDropStateAppWindow(const std::string &new_name);
  static void SetCurrentDragDropStateAppWindowHost(const std::string &new_name);
  static void SetCurrentDragDropStateWindow(const std::string &new_name);
  static void
  SetCurrentDragDropStateDraggingPlace(const DockEmplacement &place);
  static void SetMasterSwapChainRebuild(const bool &new_state);
  static void SetLastWindowPressed(const std::string &name);
  static void IncrementWindowNumber();

  // Utils
  static void FlushCommandBuffer(VkCommandBuffer commandBuffer);
  static void SubmitResourceFree(std::function<void()> &&func,
                                 const std::string &winname);
  static std::string CookPath(std::string_view input_path);
  static void
  PushRedockEvent(const std::shared_ptr<Cherry::WindowDragDropState> &state);
  static std::vector<uint8_t> LoadPngHexa(const std::string &path);
  static void SetExecutablePath();
  static std::string &GetExecutablePath();
  static SDL_Window *GetWindowHandle(const std::string &winname);
  static void AddProcessCallback(ProcessCallback process,
                                 const std::function<void()> callback);
  static void ExecuteProcessCallbacks(ProcessCallback process);

  // Set callbacks
  void SetCloseCallback(const std::function<void()> &closeCallback) {
    m_CloseCallback = closeCallback;
  }
  void SetMenubarCallback(const std::function<void()> &menubarCallback) {
    m_MenubarCallback = menubarCallback;
  }
  void SetFramebarCallback(const std::function<void()> &framebarCallback) {
    m_FramebarCallback = framebarCallback;
  }
  void SetCloseCallback(const std::function<bool()> &closeCallback) {
    m_CloseCallback = closeCallback;
  }
  void SetMainRenderCallback(const std::function<void()> &mainRenderCallback) {
    m_MainRenderCallback = mainRenderCallback;
  }

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

  // Audio service if CHERRY_ENABLE_AUDIO
  void StartAudioService();
  void StopAudioService();
  void PlaySound(const std::string &wav_file_path);

  static void FramePresent(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win);
  static void FrameRender(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win,
                          ImDrawData *draw_data);

  // All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers
  // used by the demo. Your real engine/app may not use them.
  static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd,
                                VkSurfaceKHR surface, int width, int height,
                                Cherry::Window *win);
  static void CleanupVulkanWindow(Cherry::Window *win);
  static void CleanupSpecificVulkanWindow(Cherry::Window *win);
  static void CleanupVulkan();
  static void CleanupVulkan(Cherry::Window *win);
  static void SetupVulkan(const char **extensions, uint32_t extensions_count);

  bool IsKeyPressed(CherryKey key);

  Window *GetWindowByHandle(SDL_Window *window_handle);
  std::string GetRootPath() { return m_RootPath; }

  bool IsMaximized(const std::shared_ptr<Window> &win) const;

  void BoostrappWindow();

  void SingleThreadRuntime(); // Run in single thread mode. (Better for third
                              // party thread safety)
  void MultiThreadWindowRuntime(); // EXPERIMENTAL : Run in mutliple thread
                                   // mode. (Worse for third party thread safety
                                   // but more performant.)

  void PushLayer(const std::shared_ptr<Layer> &layer);
  void NewWinInstance(const std::string &name);

  void Close();
  bool IsMaximized() const;
  float GetTime();
  bool IsTitleBarHovered() const { return m_TitleBarHovered; }

  ImDrawData *RenderWindow(Window *window);
  void InitializeWindowStates();
  void SaveData();
  void SetWindowSaveDataFile(const std::string &path,
                             const bool &relative = false);
  void SynchronizeWindows();
  std::vector<std::shared_ptr<AppWindow>> GetLastSaveInstanciableAppWindows();
  std::string PutWindow(const std::shared_ptr<AppWindow> &win);
  void AddFont(const std::string &name, const std::string &ttf_file_path,
               const float &size = 20.0f);
  void SetFavIconPath(const std::string &icon_path);
  void DeleteAppWindow(const std::shared_ptr<AppWindow> &win);
  void EnableNoAppWindowSafety();
  void DisableNoAppWindowSafety();

  // Locales & Traductions
  void AddLocale(const std::string &locale_name, const std::string &data_path);
  void SetLocale(const std::string &locale_name);
  void SetDefaultLocale(const std::string &locale_name);
  std::string GetLocale(const std::string &locale_type);

  // Net & Cache
  std::string GetHttpCacheFolderName();
  void SetHttpCacheFolderName(const std::string &name);

  // Identified component
  static Component &GetAnonymousComponent(const Identifier &identifier);
  static Component &GetComponent(const Identifier &identifier);

  std::string GetComponentData(const Identifier &id, const std::string &topic);

  std::string GetName() { return m_DefaultSpecification.Name; }

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

  void RenderComponent(Component &component);
  void RenderComponent(const std::shared_ptr<Component> &component);

  // TODO:
  // SetDataToComponentPool
  // SetPopertyToComponentPool
  // GetDataFromComponentPool
  // GetPopertyFromComponentPool
  // CherryNextComponent : Set-Get/Property-Data, CreateOnly, Inline

  // WARNING
  // A component array is différent than a component group.
  // Component group : Serve as "namespace" to separe component types or nature
  // and add logic for a group Component array : Where components will be stored
  // and managed by the runtime

  template <typename T> void PushLayer() {
    static_assert(std::is_base_of<Layer, T>::value,
                  "Pushed type is not subclass of Layer!");
    m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
  }

  static Identifier GetAnonymousID() {
    return Identifier(Identifier::GetUniqueIndex());
  }

  template <typename... Args>
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
        HashAnyImpl(seed, std::string(std::any_cast<const char *>(arg)),
                    std::true_type{});
      } else {
        // Fallback pointer hashing
        HashAnyImpl(seed, reinterpret_cast<std::size_t>(&arg),
                    std::false_type{});
      }
    }
    return Cherry::Identifier(std::to_string(seed));
  }

  template <typename T>
  static void HashAnyImpl(std::size_t &seed, const T &value, std::true_type) {
    std::hash<T> hasher;
    seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  template <typename T>
  static void HashAnyImpl(std::size_t &seed, const T &value, std::false_type) {
    seed ^= reinterpret_cast<std::size_t>(&value) + 0x9e3779b9 + (seed << 6) +
            (seed >> 2);
  }

  template <typename T> static void HashAny(std::size_t &seed, const T &value) {
    HashAnyImpl(seed, value, std::is_invocable<std::hash<T>, T>{});
  }

  static Identifier GetAnonymousID(const std::string &label) {
    std::size_t label_hash = std::hash<std::string>{}(label);

    return Identifier(std::to_string(label_hash));
  }

  static void ClearAnonymousComponents() {
    // Application::Get().m_ApplicationAnonymousComponents.clear();
  }

  template <typename T>
  static std::shared_ptr<Component>
  CreateAnonymousComponent(const T &component) {
    T component_copy = component;

    if (component_copy.GetIdentifier().string().empty() ||
        component_copy.GetIdentifier().string() == "anonymous") {
      Identifier anonymous_id = component_copy.GetIdentifier();
      anonymous_id.set(Identifier::GetUniqueIndex());
      component_copy.SetIdentifier(anonymous_id);
    }

    std::shared_ptr<Component> new_component =
        std::make_shared<T>(component_copy);

    ComponentsPool *pool = Application::Get().GetComponentPool();
    pool->AnonymousComponents.push_back(new_component);

    return new_component;
  }

  template <typename T, typename... Args>
  Component &PushComponent(const Identifier &identifier, Args &&...args) {
    switch (GetRenderMode()) {
    case RenderMode::Inline: {
      auto new_button =
          std::make_shared<T>(identifier, std::forward<Args>(args)...);
      new_button->RenderWrapper();
      return *new_button;
    }
    case RenderMode::CreateOnly: {
      auto &existing_button = Application::GetComponent(identifier);
      if (existing_button.GetIdentifier().string() != "undefined") {
        CherryApp.ResetNextComponent();
        return existing_button;
      } else {
        auto new_button = Application::CreateComponent<T>(
            identifier, std::forward<Args>(args)...);
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
        auto new_button = Application::CreateComponent<T>(
            identifier, std::forward<Args>(args)...);
        new_button->RenderWrapper();
        return *new_button;
      }
    }
    }
  }

  template <typename T, typename... Args>
  static std::shared_ptr<Component> CreateComponent(const Identifier &id,
                                                    Args &&...args) {
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
  std::string GetThemeProperty(const std::string &theme_name,
                               const std::string &key);
  std::string GetActiveThemeProperty(const std::string &key);

  void PurgeNoRenderedComponents(ComponentsPool *pool = nullptr);
  void RefreshComponentsRenderFlags(ComponentsPool *pool = nullptr);
  void DestroyComponent(const Identifier &id, ComponentsPool *pool = nullptr);
  void RefreshComponent(const Identifier &id, ComponentsPool *pool = nullptr);

  void PushComponentRenderMode(Cherry::RenderMode render_mode) {
    m_RenderModeStack.push_back(render_mode);
  }

  void PopRenderMode(int number = 1) {
    for (int i = 0; i < number && !m_RenderModeStack.empty(); ++i) {
      m_RenderModeStack.pop_back();
    }
  }

  Cherry::RenderMode GetRenderMode() const {
    if (!m_RenderModeStack.empty()) {
      return m_RenderModeStack.back();
    }

    Cherry::RenderMode fallback = CherryNextComponent.GetRenderMode();

    if (IsValidRenderMode(fallback)) {
      return fallback;
    }

    return Cherry::RenderMode::None;
  }

  bool IsValidRenderMode(Cherry::RenderMode mode) const {
    return mode != Cherry::RenderMode::None;
  }

  std::vector<RenderMode> m_RenderModeStack;

  std::unordered_map<std::string, Theme> m_Themes;
  std::vector<Theme> m_ActiveThemes;

  // Different than the default theme !
  // When the user specify this, the runtime will search level 1 theme at
  // GetThemeProperty(theme, key) "manually", and not from the active theme.
  std::string m_SelectedTheme = "undefined";

  // FIXME : Groups need to be on the CherryID, not on the pool.
  // FIXME : Component Arrays need also to be on the CherryID, when we create
  // the component we see if the CherryID have a group.
  // FIXME : Add the Identifier class here, on the app.hpp
  // std::vector<std::shared_ptr<ComponentGroup>> m_ComponentGroups;  // List of
  // components in groups
  //
  std::vector<std::string> m_PushedComponentGroups; // List of groups
  std::vector<std::shared_ptr<Component>>
      m_PushedCurrentComponent; // List of groups

  // Ordre de création dans un array :
  //	-- Le CherryID a un array de composants spécifié ? Oui, alors je le crée
  // dednas, non, je continue
  //	-- Je crée le composant en faisant un GetComponentArray, il me donnera
  // un array push si il y a eu un
  // PushComponentArray sans pop, 	   sinon il l créera dans le pool
  // général.
  void SetPropertyOnGroup(const std::string &group_name, const std::string &key,
                          const std::string &value);

  void SetDataOnGroup(const std::string &group_name, const std::string &key,
                      const std::string &value);

  // Add common property/data to all group components

  // Resources
  // TODO: move out of application class since this can't be tied
  //            to application lifetime
  std::function<void()> m_MenubarCallback;
  std::function<void()> m_FramebarCallback;
  std::function<void()> m_MainRenderCallback;
  std::function<void()> m_CloseCallback;
  ApplicationSpecification m_DefaultSpecification;

  std::shared_ptr<Component> m_FocusedDebugComponent;

  std::vector<std::shared_ptr<Layer>> m_LayerStack;

#ifdef CHERRY_ENABLE_AUDIO
  ma_engine m_AudioEngine;
#endif

  std::vector<std::pair<std::string, std::string>>
      m_PermanentProperties; // Properties will be added in every component
                             // (until pop)
  void PushPermanentProperty(const std::string &property,
                             const std::string &value);
  void PopPermanentProperty(int number_of_pops = 0);

  std::unordered_map<std::string, std::string>
      m_OnTimeProperties; // Properties will be added in the next component
  void AddOneTimeProperty(const std::string &property,
                          const std::string &value);
  const std::unordered_map<std::string, std::string> &GetOneTimeProperties();
  void RemoveOneTimeProperty(const std::string &key);
  void ClearOneTimeProperties();

  std::vector<std::shared_ptr<Component>> m_ParentComponentsStack;
  void PushParentComponent(const std::shared_ptr<Component> &component);
  void PopParentComponent();
  std::shared_ptr<Component> GetParent(int parent_number = 0);

  std::string m_RootPath;

  bool m_ClosePending = false;

  std::mutex m_WindowCreationMutex;
  ImGuiContext *m_ImGuiMasterContext;

  std::string m_WindowSaveDataPath;
  bool m_SaveWindowData = false;
  bool m_IsDataSaved = true;
  bool m_IsDataInitialized = false;
  nlohmann::json m_PreviousSaveData;

  bool m_NoAppWindowSafetyEnabled = false;
  bool m_Running = false;
  bool m_Dragging = false;

  std::vector<std::shared_ptr<Window>> m_Windows;
  std::vector<std::shared_ptr<RedockRequest>> m_RedockRequests;
  std::vector<std::shared_ptr<AppWindow>> m_AppWindows;
  std::vector<std::shared_ptr<AppWindow>> m_SavedAppWindows;
  std::vector<std::pair<std::string, std::pair<std::string, float>>>
      m_CustomFonts;
  std::string m_FavIconPath;

  // std::vector<std::shared_ptr<Component>> m_ApplicationComponents;
  // std::vector<std::shared_ptr<Component>> m_ApplicationAnonymousComponents;

  ComponentsPool m_ApplicationComponentPool;
  std::vector<ComponentsPool *> m_ComponentPoolStack;

  std::unordered_map<std::string, nlohmann::json> m_Locales;
  std::string m_SelectedLocale;
  std::string m_DefaultLocale;

  std::string m_HttpCacheFolderName = "http_cache";

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
void AddAppWindow(const std::shared_ptr<AppWindow> &win);
void DeleteAppWindow(const std::shared_ptr<AppWindow> &win);
std::shared_ptr<AppWindow> GetAppWindowByName(const std::string &win_name);
std::shared_ptr<AppWindow> GetCurrentRenderedAppWindow();
std::shared_ptr<Window> GetWindowByName(const std::string &win_name);

// Images
ImTextureID GetTexture(const std::string &path);
ImVec2 GetTextureSize(const std::string &path);

// Notification
void AddNotification(const ImGuiToast &toast);

// Font
void PushFont(const std::string &font_name);
void PopFont();

// Runtime
std::string GetPath(const std::string &path);
std::string GetLocale(const std::string &topic);
#ifdef CHERRY_ENABLE_NET
std::string GetHttpPath(const std::string &url);
#endif // CHERRY_ENABLE_NET

// Data (theses functions can return JSON to string format or legacy string.)
std::string GetData(const Identifier &id, const std::string topic);
std::string GetWindowData(const std::string &id, const std::string topic);

void PushPermanentProperty(const std::string &property,
                           const std::string &value);
void PopPermanentProperty(int number_of_pops = 0);
void SetNextComponentProperty(const std::string &property,
                              const std::string &value);

void PushParentComponent(const std::shared_ptr<Component> &component);
void PopParentComponent();
std::shared_ptr<Component> GetParent(int parent_number = 0);

bool IsReady();

// Implemented by CLIENT
Application *CreateApplication(int argc, char **argv);
} // namespace Cherry

#endif