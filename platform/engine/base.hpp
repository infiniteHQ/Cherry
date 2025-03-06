#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"
#include "image.hpp"

#include <mutex>
#include <atomic>
#include <memory>
#include <string>

namespace Cherry
{
    enum class WindowType
    {
        Docking,
        Tabs,
        Unique
    };

    enum class WindowRenderingMethod
    {
        DockingWindows, // Unlimited windows with docking
        TabWidows,      // Unlimited windows without docking.
        SimpleWindow,   // Need to Put one single window
        SimpleRender    // Need to use the MainRenderChannel
    };

    enum class WindowMenubar
    {
        CustomTitleBar
    };

    enum class DockEmplacement
    {
        // Unknow/Unset position
        DockBlank,

        // Entire place of main dock node
        DockFull,

        // Main positions
        DockUp,
        DockDown,
        DockRight,
        DockLeft,

        // Secondary, Only used for default behaviors.
        DockUpLeft,    // AppWindow at up + DockLeft
        DockUpRight,   // AppWindow at up + DockRight
        DockDownLeft,  // AppWindow at down + DockLeft
        DockDownRight, // AppWindow at down + DockRight
        DockLeftDown,  // AppWindow at left + DockDown
        DockLeftUp,    // AppWindow at left + DockUp
        DockRightUp,   // AppWindow at right + DockDown
        DockRightDown  // AppWindow at right + DockUp
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

    struct WindowDragDropState
    {
        std::string LastDraggingWindow = "unknown";
        std::string LastDraggingAppWindowHost = "unknown";
        std::string LastDraggingAppWindow = "unknown";
        bool LastDraggingAppWindowHaveParent = false;
        bool DockIsDragging = false;
        Cherry::DockEmplacement LastDraggingPlace = Cherry::DockEmplacement::DockBlank;
        bool CreateNewWindow = false;
        bool FromSave = false;
        std::string DragOwner = "none";
        int mouseX;
        int mouseY;
    };

    enum class WindowThemeColorEntry
    {
        FrameColor,
        BackgroundColor,
        BorderColor
    };

    struct WindowThemeColorObject
    {
        ImGuiCol_ m_Entry;
        ImVec4 m_Value;

        WindowThemeColorObject(ImGuiCol_ entry, const ImVec4 val) : m_Entry(entry),
                                                                    m_Value(val)
        {
        }
    };

    struct WindowThemeColor
    {
        std::vector<WindowThemeColorObject> Colors;

        void SetColor(ImGuiCol_ key, std::string val)
        {
            Colors.push_back(WindowThemeColorObject(key, Cherry::HexToRGBA(val)));
        }
    };

    struct ApplicationSpecification
    {
        std::string Name = "Cherry App";
        std::string IconPath;
        std::string FavIconPath;
        std::string LastFavIconPath;
        std::string UniqueAppWindowName;
        std::string DefaultWindowName;

        uint32_t Width = 1850;
        uint32_t Height = 1000;
        uint32_t MinWidth = 100;
        uint32_t MinHeight = 100;

        float GlobalScale = 0.84f;
        float FontGlobalScale = 0.83f;

        bool CenterWindow = false; // Window will be created in the center of primary monitor
        bool DisableTitle = false;
        bool DisableTitleBar = false;
        bool DisableResize = false;
        bool WindowResizeable = true;
        bool WindowOnlyClosable = false;
        bool WindowSaves = false;
        bool DisableWindowManagerTitleBar = false;
        bool UsingCloseCallback = false;
        bool CustomTitlebar = false;
        bool DebugMode = false;

        std::function<void()> MenubarCallback;
        std::function<void()> FramebarCallback;
        std::function<void()> MainRenderCallback;
        std::function<void()> CloseCallback;

        WindowRenderingMethod RenderMode;

    public:
        WindowThemeColor ColorTheme;
        void UseCustomTitleBar()
        {
            CustomTitlebar = true;
        }

        void SetRenderMode(WindowRenderingMethod method)
        {
            RenderMode = method;
        }

        void SetDebugMode(bool enabled)
        {
            DebugMode = enabled;
        }

        void EnableDebugMode()
        {
            DebugMode = true;
        }

        void SetCloseCallback(const std::function<void()> &closeCallback)
        {
            CloseCallback = closeCallback;
        }

        void SetMenubarCallback(const std::function<void()> &menubarCallback)
        {
            MenubarCallback = menubarCallback;
        }

        void SetFramebarCallback(const std::function<void()> &framebarCallback)
        {
            FramebarCallback = framebarCallback;
        }

        void SetCloseCallback(const std::function<bool()> &closeCallback)
        {
            CloseCallback = closeCallback;
        }

        void SetMainRenderCallback(const std::function<void()> &mainRenderCallback)
        {
            MainRenderCallback = mainRenderCallback;
        }

        void UseWindowSaves()
        {
            WindowSaves = true;
        }

        void DisableWindowTitle()
        {
            DisableTitle = true;
        }

        void DisableDesktopTitleBar()
        {
            DisableWindowManagerTitleBar = true;
        }

        void SetIconPath(const std::string &path)
        {
            IconPath = path;
        }

        void SetFavIconPath(const std::string &path)
        {
            FavIconPath = path;
        }

        void SetUniqueAppWindowName(const std::string &name)
        {
            UniqueAppWindowName = name;
        }

        void SetCustomTitlebar(bool custom_titlebar)
        {
            CustomTitlebar = custom_titlebar;
        }

        void SetName(const std::string &name)
        {
            Name = name;
        }

        void SetDefaultWindowName(const std::string &name)
        {
            DefaultWindowName = name;
        }

        void SetMinimumWidth(int minimum_width)
        {
            MinWidth = minimum_width;
        }

        void SetMinimumHeight(int minimum_height)
        {
            MinHeight = minimum_height;
        }

        void SetDefaultHeight(int default_height)
        {
            Height = default_height;
        }

        void SetDefaultWidth(int default_width)
        {
            Width = default_width;
        }

        void SetGlobalScale(float global_scale)
        {
            GlobalScale = global_scale;
        }

        void SetFontGlobalScale(float font_scale)
        {
            FontGlobalScale = font_scale;
        }
    };

    struct ReattachRequest
    {
        ApplicationSpecification m_Specification;
        std::string m_AppWindowName;
        bool m_IsFinished = true;
    };

    struct RedockRequest
    {
        std::string m_ParentWindow;
        std::string m_ParentAppWindowHost;
        std::string m_ParentAppWindow;
        DockEmplacement m_DockPlace;
        bool m_IsHandled;
        bool m_FromSave;
        bool m_FromNewWindow;
        bool m_IsObsolete = false;
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

    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

}