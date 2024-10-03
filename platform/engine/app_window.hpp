
#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"
#include "image.hpp"
#include "base.hpp"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#include <memory>
#include <functional>
#include <fstream>
#include <filesystem>

#include "ui/ui.hpp"
#include "imgui/ImGuiTheme.h"
#include "vulkan/vulkan.h"

namespace UIKit
{
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

    class AppWindow
    {
    public:
        AppWindow();
        AppWindow(const std::string &id, const std::string &name);
        AppWindow(const std::string &id, const std::string &name, const std::string &icon);

        void push_event();
        void CtxRender(std::vector<std::shared_ptr<RedockRequest>> *reqs, const std::string &winname);
        std::shared_ptr<RedockRequest> CreateRedockEvent(const std::string &parentWindow, DockEmplacement emplacement, const bool &fromSave = false, const std::string &appWindow = "none",  const bool& newwin = false);
        void AttachOnNewWindow(ApplicationSpecification spec);
        void AttachOnWindow(const std::string winname);
        std::shared_ptr<UIKit::Image> GetImage(const std::string &path);
        ImTextureID *GetTexture(const std::string &path);
        float EstimateMenubarRightWidth();
        void SetSimpleStorage(const std::string &key, const std::string &data, const bool &persistant);
        void SetWindowStorage(const std::string &key, const nlohmann::json &data, const bool &persistant);
        std::shared_ptr<SimpleStorageItem> GetSimpleStorage(const std::string &key);
        std::shared_ptr<WindowStorageItem> GetWindowStorage(const std::string &key);
        std::unordered_map<std::string, std::shared_ptr<WindowStorageItem>> DumpWindowStorage();
        std::unordered_map<std::string, std::shared_ptr<SimpleStorageItem>> DumpSimpleStorage();
        void SetDefaultBehavior(DefaultAppWindowBehaviors behavior, const std::string &value);
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
        void SetInternalPaddingX(const float& new_padding);
        void SetInternalPaddingY(const float& new_padding);

    public:
        ImGuiID m_DockID;
        ReattachRequest m_AttachRequest;
        std::string m_WinParent;

        // Main informations
        std::string m_DockParent = "unknow";
        std::string m_DockPlace = "unknow";
        std::string m_Name = "unknow";
        std::string m_IdName = "unknow";
        std::string m_ID = "unknow";
        // ImGuiWindow *m_ImGuiWindow;
        ImGuiID m_DockSpaceID;
        ImGuiID m_ParentDockSpaceID;

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

        bool m_EnableMenuBar = false;
        bool m_EnableBottomBar = false;

        bool m_IsDragging;
        bool m_DockIsDraggingStarted;

        bool m_Pressed;

        bool m_DockingMode = false;

        float m_InternalPaddingY = 0;
        float m_InternalPaddingX = 0;

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
        std::function<void()> m_BottombarRight;
        std::function<void()> m_BottombarLeft;

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
}