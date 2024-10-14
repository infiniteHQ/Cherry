#include "../../../../../../platform/engine/app.hpp"
#include "../../../../../../platform/engine/ui/notifications/notifications.hpp"


#ifndef UIKIT_V1_AIO_CONTENT_CONTENTBROWSER
#define UIKIT_V1_AIO_CONTENT_CONTENTBROWSER

// UIKIT Components
#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"
#include "../../../../components/buttons/image_text_buttons.hpp"
#include "../../../../components/buttons/text_buttons.hpp"
#include "../../../../components/buttons/dropdown_buttons.hpp"
#include "../../../../components/titles/h1.hpp"
#include "../../../../components/titles/h2.hpp"
#include "../../../../components/titles/h3.hpp"
#include "../../../../components/titles/h4.hpp"
#include "../../../../components/titles/h5.hpp"
#include "../../../../components/titles/h6.hpp"
#include "../../../../components/texts/simple_text.hpp"
#include "../../../../components/combos/simple_combo.hpp"
#include "../../../../components/headers/simple_collapsing.hpp"
#include "../../../../components/headers/custom_collapsing.hpp"

#include <stack>
#include <algorithm>
#include <filesystem>
#include <cctype>
#include <unordered_set>

namespace Cherry
{

    class ContenBrowserItem
    {
    public:
        bool (*f_Detect)(const std::string &path);

        std::string m_Name;

        ImTextureID m_Logo;
        ImVec4 m_LineColor;
        std::string m_Description;

        ContenBrowserItem(bool (*detect_function)(const std::string &path), const std::string &name, const std::string &description, const ImVec4 &line_color) : m_Name(name), m_Description(description), f_Detect(detect_function), m_LineColor(line_color) {};
    };
    enum class ContentShowMode
    {
        Thumbmails,
        List,
        Columns
    };

    enum class FileTypes
    {
        // Very low level
        File_ASM,
        File_BIN,

        // Programming languages
        File_C,
        File_H,
        File_CPP,
        File_HPP,
        File_INL,
        File_RUST,
        File_ZIG,
        File_GO,
        File_JAVA,
        File_JAVASCRIPT,
        File_COBOL,
        File_PASCAL,
        File_CARBON,

        // Misc
        File_CFG,
        File_JSON,
        File_PICTURE,
        File_TXT,
        File_MD,
        File_YAML,
        File_INI,
        File_GIT,

        // Vortex
        File_VORTEX_CONFIG,

        // Other
        File_UNKNOW,

    };

    struct FileHandler
    {
        FileTypes for_type;
        std::string file_handler_name;
        std::string handler_module_name;
        std::string handler_plugin_name;
    };

    struct ContentBrowserChild
    {
        std::function<void()> m_Child;
        std::string m_Name;
        bool m_Disabled = true;
        float m_DefaultSize = 0.0f;
        float m_MinSize = 0.0f;
        float m_MaxSize = 0.0f;
        float m_Size = 200.0f;
        float m_Ratio = 0.0f;
        bool m_Resizable = true;
        bool m_ResizeDisabled = false;
        bool m_Initialized = false;
        bool m_InitializedSec = false;
        bool m_InitializedTh = false;
        ImVec4 m_BackgroundColor = ImVec4(0.0f,0.0f,0.0f,0.0f);

        void Enable()
        {
            m_Disabled = true;
        }

        void Disable()
        {
            m_Disabled = false;
        }

        ContentBrowserChild(const std::string &name, const std::function<void()> &child, const float &default_size = 0.0f, const bool &resize_disabled = false, const float &min_size = 0.0f, const float &max_size = 0.0f)
            : m_Name(name),
              m_Child(child),
              m_ResizeDisabled(resize_disabled),
              m_DefaultSize(default_size),
              m_MinSize(min_size),
              m_MaxSize(max_size) {}
    };

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class ContentBrowserAppWindow : public std::enable_shared_from_this<ContentBrowserAppWindow>
    {
    public:
        ContentBrowserAppWindow(const std::string &name, const std::string &start_path);
        // ContentBrowserAppWindow(const std::string &name);

        std::shared_ptr<Cherry::AppWindow> &GetAppWindow()
        {
            return m_AppWindow;
        }

        static std::shared_ptr<ContentBrowserAppWindow> Create(const std::string &name, const std::string &start_path)
        {
            auto instance = std::shared_ptr<ContentBrowserAppWindow>(new ContentBrowserAppWindow(name, start_path));
            instance->SetupRenderCallback();
            return instance;
        }
        void SetupRenderCallback();


        void Render();
        bool MyButton(const std::string &name, const std::string &path, const std::string &description, const std::string &size, bool selected, const std::string &logo, ImU32 bgColor, ImU32 borderColor, ImU32 lineColor, float maxTextWidth, float borderRadius);
        void AddChild(const ContentBrowserChild &child);
        void ChangeDirectory(const std::filesystem::path &newDirectory);
        void GoBack();
        void GoForward();
        void DrawPathBar();

        void RenderSideBar();
        void RenderFiltersBar();
        void RenderContentBar();
        void RenderDetailsBar();

        void Select(const std::string &path)
        {
            for (auto already : m_Selected)
            {
                if (path == already)
                {
                    return;
                }
            }
            m_Selected.push_back(path);
        }

        void ChangeShowMode(ContentShowMode mode)
        {
            this->m_ShowMode = mode;
        }

        // To move in components
        void DrawFolderIcon(ImVec2 pos, ImVec2 size, ImU32 color);
        void MyFolderButton(const char *id, ImVec2 size, ImU32 color, const std::string &path);
        void DrawHierarchy(std::filesystem::path path, bool isDir, const std::string &label);

        void SetDefaultFolderColor(const std::string &hex);

        std::string GetContentBrowserFolderColor(const std::string &path)
        {
            for (auto &colored_folder : m_FolderColors)
            {
                if (colored_folder.first == path)
                {
                    return colored_folder.second;
                }
            }

            return "#997D44FF";
        }

        void AddReconizedItem(const std::shared_ptr<ContenBrowserItem> &item) {};
        bool IsPathFavorite(const std::string &path) { return false; };
        void SetColoredFolder(const std::string &path, const std::string &hex_color) {};
            
        std::vector<ContentBrowserChild> m_Childs;

    private:
        bool opened;

        bool m_ThumbnailMode = true;
        bool m_ListMode = false;

        bool m_ShowTypes = false;
        bool m_ShowSizes = false;

        // !!!
        bool m_ShowFolderPannel = true;
        bool m_ShowFilterPannel = false;
        bool m_ShowThumbnailVisualizer = false;
        bool m_ShowSelectionQuantifier = false;
        bool m_IsInitialized;

        ContentShowMode m_ShowMode = ContentShowMode::Thumbmails;

        std::filesystem::path m_BaseDirectory;

        std::filesystem::path m_CurrentDirectory;
        std::stack<std::filesystem::path> m_BackHistory;
        std::stack<std::filesystem::path> m_ForwardHistory;

        std::vector<std::string> m_Selected;
        std::vector<std::string> m_CopySelection;
        std::vector<std::string> m_CutSelection;

        std::string m_DefaultFolderColor;

        // Path/Color
        std::vector<std::pair<std::string, std::string>> m_FolderColors;
        std::vector<std::string> m_FavoriteFolders;
        std::vector<std::string> m_Pools;
        std::vector<std::shared_ptr<ContenBrowserItem>> m_ItemToReconize;

        std::vector<std::filesystem::path> m_Favorites;

        std::shared_ptr<Cherry::Image> m_ProjectIcon;
        std::shared_ptr<Cherry::Image> m_FileIcon;
        std::shared_ptr<Cherry::Image> m_DirectoryIcon;

        std::shared_ptr<Cherry::AppWindow> m_AppWindow;

        std::function<void(const std::string &)> m_DeletePathCallback;
        std::function<void(const std::vector<std::string> &)> m_CopyPathsCallback;
        std::function<void(const std::vector<std::string> &)> m_PastePathsCallback;

        std::shared_ptr<ImageTextButtonSimple> cp_SaveButton;
        std::shared_ptr<ImageTextButtonSimple> cp_ImportButton;
        std::shared_ptr<ImageTextButtonSimple> cp_AddButton;

        std::shared_ptr<CustomDrowpdownImageButtonSimple> cp_SettingsButton;

        std::shared_ptr<ImageButtonSimple> cp_DirectoryUndo;
        std::shared_ptr<ImageButtonSimple> cp_DirectoryRedo;
    };
}

#endif // UIKIT_V1_AIO_CONTENT_CONTENTBROWSER