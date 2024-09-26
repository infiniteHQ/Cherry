#include "../../../../../platform/engine/app.hpp"


#ifndef UIKIT_V1_AIO_CONTENT_CONTENTBROWSER
#define UIKIT_V1_AIO_CONTENT_CONTENTBROWSER

// UIKIT Components
#include "../../../components/buttons/custom_buttons.hpp"
#include "../../../components/buttons/image_buttons.hpp"
#include "../../../components/buttons/text_buttons.hpp"
#include "../../../components/titles/h1.hpp"
#include "../../../components/titles/h2.hpp"
#include "../../../components/titles/h3.hpp"
#include "../../../components/titles/h4.hpp"
#include "../../../components/titles/h5.hpp"
#include "../../../components/titles/h6.hpp"
#include "../../../components/texts/simple_text.hpp"
#include "../../../components/combos/simple_combo.hpp"
#include "../../../components/headers/simple_collapsing.hpp"

#include <stack>
#include <algorithm>
#include <filesystem>
#include <cctype>
#include <unordered_set>

namespace UIKit
{

class ContenBrowserItem
{
public:
  bool (*f_Detect)(const std::string &path);

  std::string m_Name;

  ImTextureID m_Logo;
  ImVec4 m_LineColor;
  std::string m_Description;

  ContenBrowserItem(bool (*detect_function)(const std::string &path), const std::string& name, const std::string& description, const ImVec4& line_color):
  m_Name(name), m_Description(description), f_Detect(detect_function), m_LineColor(line_color)
  {};
  
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

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class ContentBrowserAppWindow
    {
    public:
        ContentBrowserAppWindow(const std::string &name, const std::string &start_path);
        //ContentBrowserAppWindow(const std::string &name);

        void RefreshRender(const std::shared_ptr<ContentBrowserAppWindow> &instance);
	bool MyButton(const std::string &name, const std::string &path, const std::string &description, const std::string &size, bool selected, const std::string &logo, ImU32 bgColor, ImU32 borderColor, ImU32 lineColor, float maxTextWidth, float borderRadius);

        void ChangeDirectory(const std::filesystem::path &newDirectory);
        void GoBack();
        void GoForward();
        void DrawPathBar();

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

        ContentShowMode m_ShowMode = ContentShowMode::Thumbmails;

        std::filesystem::path m_BaseDirectory;

        std::filesystem::path m_CurrentDirectory;
        std::stack<std::filesystem::path> m_BackHistory;
        std::stack<std::filesystem::path> m_ForwardHistory;

        std::vector<std::string> m_Selected;

        std::vector<std::filesystem::path> m_Favorites;

        std::shared_ptr<UIKit::Image> m_ProjectIcon;
        std::shared_ptr<UIKit::Image> m_FileIcon;
        std::shared_ptr<UIKit::Image> m_DirectoryIcon;

        std::shared_ptr<UIKit::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_AIO_CONTENT_CONTENTBROWSER