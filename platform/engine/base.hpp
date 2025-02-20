#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"
#include "image.hpp"

#include <mutex>

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
        uint32_t Width = 1850;
        uint32_t Height = 1000;
        uint32_t MinWidth = 100;
        uint32_t MinHeight = 100;

        std::string IconPath;
        std::string FavIconPath;
        std::string LastFavIconPath;

        std::string DefaultWindowName;
        float GlobalScale = 0.84f;
        float FontGlobalScale = 0.83f;

        WindowThemeColor ColorTheme;

        bool WindowResizeable = true;
        bool WindowOnlyClosable = false;
        bool WindowSaves = false;
        bool DisableWindowManagerTitleBar = false;
        WindowRenderingMethod RenderMode;
        std::string UniqueAppWindowName;

        bool UsingCloseCallback = false;
        std::function<void()> CloseCallback;
        std::function<void()> MenubarCallback;
        bool CustomTitlebar = false;

        // Window will be created in the center
        // of primary monitor
        bool CenterWindow = false;

        bool DisableTitle = false;
        bool DisableTitleBar = false;
        bool DisableResize = false;

    public:
        void UseCustomTitleBar()
        {
            CustomTitlebar = true;
        }

        void SetRenderMode(WindowRenderingMethod method)
        {
            RenderMode = method;
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