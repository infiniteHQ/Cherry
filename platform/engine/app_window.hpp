
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
#include <SDL2/SDL_image.h>

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
        std::shared_ptr<RedockRequest> CreateRedockEvent(const std::string &parentWindow, DockEmplacement emplacement, const bool &fromSave = false, const std::string &appWindow = "none")
        {
            std::shared_ptr<RedockRequest> req = std::make_shared<RedockRequest>();
            req->m_DockPlace = emplacement;
            req->m_ParentAppWindow = appWindow;
            req->m_ParentAppWindowHost = this->m_IdName;
            req->m_ParentWindow = parentWindow;
            req->m_FromSave = fromSave;
            return req;
        }

        void AttachOnNewWindow(ApplicationSpecification spec);
        void AttachOnWindow(const std::string winname);

        ReattachRequest m_AttachRequest;

        ImGuiID m_DockID;

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
            m_EnableMenuBar = true;
        }

        void SetRightMenubarCallback(const std::function<void()> &left_menubar)
        {
            m_MenubarRight = left_menubar;
            m_EnableMenuBar = true;
        }

        void SetLeftBottombarCallback(const std::function<void()> &left_menubar)
        {
            m_BottombarLeft = left_menubar;
            m_EnableBottomBar = true;
        }

        void SetRightBottombarCallback(const std::function<void()> &right_menubar)
        {
            m_BottombarRight = right_menubar;
            m_EnableBottomBar = true;
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

        void SetInternalPaddingX(const float& new_padding)
        {
            m_InternalPaddingX = new_padding;
        }

        void SetInternalPaddingY(const float& new_padding)
        {
            m_InternalPaddingY = new_padding;
        }

        std::string m_WinParent;

    public:
        int treated = 0;

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