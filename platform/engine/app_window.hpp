
#pragma once

#include "../../src/core/color.hpp"
#include "../../src/layer.hpp"
#include "base.hpp"
#include "components/components.hpp"
#include "image.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "imgui/ImGuiTheme.h"
#include "ui/ui.hpp"
#include "vulkan/vulkan.h"

namespace Cherry {
enum class DefaultAppWindowBehaviors { DefaultDocking, DefaultSize };

enum class AppWindowTypes { InstanciableWindow, StaticWindow };

class AppWindow {
public:
  AppWindow();
  AppWindow(const std::string &id, const std::string &name);
  AppWindow(const std::string &id, const std::string &name,
            const std::string &icon);
  ~AppWindow();

  void push_event();
  void CtxRender(std::vector<std::shared_ptr<RedockRequest>> *reqs,
                 const std::string &winname);
  std::shared_ptr<RedockRequest>
  CreateRedockEvent(const std::string &parentWindow,
                    DockEmplacement emplacement, const bool &fromSave = false,
                    const std::string &appWindow = "none",
                    const bool &newwin = false);
  void AttachOnNewWindow(ApplicationSpecification spec);
  void AttachOnWindow(const std::string winname);
  std::shared_ptr<Cherry::Image> GetImage(const std::string &path);
  ImTextureID *GetTexture(const std::string &path);
  float EstimateMenubarRightWidth();
  void SetSimpleStorage(const std::string &key, const std::string &data,
                        const bool &persistant);
  void SetWindowStorage(const std::string &key, const nlohmann::json &data,
                        const bool &persistant);
  std::shared_ptr<SimpleStorageItem> GetSimpleStorage(const std::string &key);
  std::shared_ptr<WindowStorageItem> GetWindowStorage(const std::string &key);
  std::unordered_map<std::string, std::shared_ptr<WindowStorageItem>>
  DumpWindowStorage();
  std::unordered_map<std::string, std::shared_ptr<SimpleStorageItem>>
  DumpSimpleStorage();
  void SetDefaultBehavior(DefaultAppWindowBehaviors behavior,
                          const std::string &value);
  std::string GetDefaultBehavior(DefaultAppWindowBehaviors behavior);
  void SpawnInNewWindow();
  void SetRenderCallback(const std::function<void()> &render);
  void SetLeftMenubarCallback(const std::function<void()> &right_menubar);
  void SetRightMenubarCallback(const std::function<void()> &left_menubar);
  void SetLeftBottombarCallback(const std::function<void()> &left_menubar);
  void SetRightBottombarCallback(const std::function<void()> &right_menubar);
  void SetIcon(const std::string &name);
  void SetDockingMode(const bool &use_docking);
  void SetFetchedSaveData(const std::string &key, const std::string &value);
  void SetInstanciable();
  std::string GetFetchedSaveData(const std::string &key);
  void SetParent(const std::shared_ptr<AppWindow> &parent);
  bool CheckWinParent(const std::string &parentname);
  void SetParentWindow(const std::string &parentname);
  void SetClosable(const bool &is_closable);
  void SetCloseCallback(std::function<void()> close_callback);
  void SetSaveMode(const bool &use_save_mode);
  void SetSaved(const bool &new_state);
  void SetOpened(const bool &new_state);
  void SetDisableDragging(const bool &new_state);
  void SetDisableContextMenu(const bool &new_state);
  void SetInternalPaddingX(const float &new_padding);
  void SetInternalPaddingY(const float &new_padding);
  void SetVisibility(const bool &new_visibility);
  void SetDeleteCallback(const std::function<void()> &callback);
  void DeleteSignal();
  std::string GetName();
  std::string GetThemeProperty(const std::string &key);
  void SetTheme(const std::string &theme_name);
  void RemoveTheme();

public:
  ImGuiID m_DockID;
  ImGuiID m_DockSpaceID;
  ImGuiID m_ParentDockSpaceID;

  std::string m_WinParent;
  std::string m_DockParent = "unknow";
  std::string m_DockPlace = "unknow";
  std::string m_Name = "unknow";
  std::string m_IdName = "unknow";
  std::string m_ID = "unknow";
  std::string m_Icon = "none";

  bool m_Visible = true;
  bool m_Closable = true;
  bool m_CloseSignal = true;
  bool m_SaveMode = false;
  bool m_Saved = false;
  bool m_UnsaveMarkerApplied = false;
  bool m_Opened = true;
  bool m_IsRendering = true;
  bool m_DisableDragging = false;
  bool m_DisableContextMenu = false;
  bool m_EnableMenuBar = false;
  bool m_EnableBottomBar = false;
  bool m_IsDragging;
  bool m_DockIsDraggingStarted;
  bool m_Pressed;
  bool m_DockingMode = false;
  bool m_WindowRebuilded = false;
  bool m_WindowJustRebuilded = false;
  bool m_HaveParentAppWindow = false;

  float m_InternalPaddingY = 0;
  float m_InternalPaddingX = 0;

  std::function<void()> m_CloseCallback;
  std::function<void()> m_TabMenuCallback;
  std::function<void()> m_DeleteCallback;
  std::function<void()> m_Render;
  std::function<void()> m_MenubarRight;
  std::function<void()> m_MenubarLeft;
  std::function<void()> m_BottombarRight;
  std::function<void()> m_BottombarLeft;
  std::function<void()> m_CloseEvent;

  std::shared_ptr<AppWindow> m_ParentAppWindow;
  std::vector<std::shared_ptr<AppWindow>> m_SubAppWindows;

  AppWindowTypes m_AppWindowType = AppWindowTypes::StaticWindow;
  ReattachRequest m_AttachRequest;

  // Different than the default theme !
  // When the user specify this, the runtime will search level 3 theme at
  // GetThemeProperty(theme, key) "manually", and not from the active theme.
  std::string m_SelectedTheme = "undefined";

  // Notifications
  // Number of unread notifs
  // ImGuiWindow *m_ImGuiWindow;
  // Default behavior

  // Renders functions

private:
  // Simple storage (only Key/Value), can be replicated in saves
  std::unordered_map<std::string, std::shared_ptr<SimpleStorageItem>> m_Storage;

  // Advanced storage (json & metrics), can be also replicated in saves
  std::unordered_map<std::string, std::shared_ptr<WindowStorageItem>>
      m_WindowStorage;

  // Fetched datas from last save
  std::unordered_map<std::string, std::string> m_LastSaveData;

  // Default behaviors (docking behaviors, default themes, internal docking
  // emplacements.)
  std::unordered_map<DefaultAppWindowBehaviors, std::string, EnumClassHash>
      m_DefaultBehaviors;
};

class AppWindowWrapper : public std::enable_shared_from_this<AppWindowWrapper> {
public:
  AppWindowWrapper();
  AppWindowWrapper(const std::string &name);
  AppWindowWrapper(
      const std::string &name, const std::function<void()> callback = []() {});

  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<AppWindowWrapper> Create(
      const std::string &name, const std::function<void()> callback = []() {});
  void SetRender(const std::function<void()> &callback);
  void SetupRenderCallback();
  void Render();

  // Internal & private managment of Components
  std::vector<std::shared_ptr<Component>> *GetComponents() {
    return &m_PrivateComponents;
  }

private:
  std::function<void()> m_RenderCallback;
  std::vector<std::shared_ptr<Component>> m_PrivateComponents;
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};
} // namespace Cherry