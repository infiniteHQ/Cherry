#include "./content_browser.hpp"

// To move in class members
static float padding = 30.0f;
static float thumbnailSize = 94.0f;

static std::string pathToRename = "";
static char pathRename[256];

static bool pool_add_mode = false;
static char pool_add_path[512];

static std::string _parent;
static char ProjectSearch[256];
static float threshold = 0.4;

static ImU32 folder_color = IM_COL32(150, 128, 50, 255);

static std::pair<std::string, ImU32> current_editing_folder;
static bool current_editing_folder_is_favorite;

static bool isOnlySpacesOrEmpty(const char *str)
{
    if (str == nullptr || std::strlen(str) == 0)
    {
        return true;
    }

    for (size_t i = 0; i < std::strlen(str); ++i)
    {
        if (str[i] != ' ')
        {
            return false;
        }
    }
    return true;
}

static std::string toLowerCase(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

static void DrawHighlightedText(ImDrawList *drawList, ImVec2 textPos, const char *text, const char *search, ImU32 highlightColor, ImU32 textColor, ImU32 highlightTextColor)
{
    if (!text || !search || !*search)
    {
        drawList->AddText(textPos, textColor, text);
        return;
    }

    const char *start = text;
    const char *found = strstr(start, search);
    while (found)
    {
        if (found > start)
        {
            std::string preText(start, found);
            drawList->AddText(textPos, textColor, preText.c_str());
            textPos.x += ImGui::CalcTextSize(preText.c_str()).x;
        }

        ImVec2 highlightPos = textPos;
        ImVec2 highlightSize = ImGui::CalcTextSize(search);
        drawList->AddRectFilled(highlightPos, ImVec2(highlightPos.x + highlightSize.x, highlightPos.y + highlightSize.y), highlightColor);
        drawList->AddText(textPos, highlightTextColor, search);
        textPos.x += highlightSize.x;

        start = found + strlen(search);
        found = strstr(start, search);
    }

    if (*start)
    {
        drawList->AddText(textPos, textColor, start);
    }
}

static int levenshteinDistance(const std::string &s1, const std::string &s2)
{
    const size_t m = s1.size();
    const size_t n = s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (size_t i = 0; i <= m; ++i)
    {
        for (size_t j = 0; j <= n; ++j)
        {
            if (i == 0)
            {
                dp[i][j] = j;
            }
            else if (j == 0)
            {
                dp[i][j] = i;
            }
            else
            {
                int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
                dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost});
            }
        }
    }
    return dp[m][n];
}

static bool hasCommonLetters(const std::string &s1, const std::string &s2)
{
    std::unordered_set<char> set1(s1.begin(), s1.end());
    for (char c : s2)
    {
        if (set1.find(c) != set1.end())
        {
            return true;
        }
    }
    return false;
}

static bool areStringsSimilar(const std::string &s1, const std::string &s2, double threshold)
{
    std::string lower_s1 = toLowerCase(s1);
    std::string lower_s2 = toLowerCase(s2);

    int dist = levenshteinDistance(lower_s1, lower_s2);
    int maxLength = std::max(lower_s1.size(), lower_s2.size());
    double similarity = 1.0 - (static_cast<double>(dist) / maxLength);

    if (std::strlen(ProjectSearch) < 5)
    {
        return similarity >= threshold || hasCommonLetters(lower_s1, lower_s2);
    }

    return similarity >= threshold;
}
bool Splitter(bool split_vertically, float thickness, float *sizebefore, float *size, float *size_after, float min_size1, float min_size2)
{
    using namespace ImGui;
    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = g.CurrentWindow;
    ImDrawList *draw_list = GetWindowDrawList();

    ImVec2 backup_pos = window->DC.CursorPos;
    if (split_vertically)
        window->DC.CursorPos = ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y);
    else
        window->DC.CursorPos = ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y);

    PushID("#Splitter");
    InvisibleButton("##Splitter", ImVec2(split_vertically ? thickness : window->Size.x, split_vertically ? window->Size.y : thickness));
    PopID();

    bool hovered = IsItemHovered();
    bool held = IsItemActive();
    if (hovered || held)
        SetMouseCursor(split_vertically ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS);

    bool updated = false;
    if (held)
    {
        float mouse_delta = split_vertically ? g.IO.MouseDelta.x : g.IO.MouseDelta.y;
        if (mouse_delta != 0.0f)
        {
            if (*size + mouse_delta < min_size1)
                mouse_delta = -(*size - min_size1);
            if (*sizebefore - mouse_delta < min_size2)
                mouse_delta = *sizebefore - min_size2;

            *sizebefore += mouse_delta;
            *size -= mouse_delta;
            updated = true;
        }
    }

    window->DC.CursorPos = backup_pos;

    ImU32 color = IM_COL32(45, 45, 45, 255);
    ImVec2 pos = split_vertically ? ImVec2(backup_pos.x, backup_pos.y) : ImVec2(backup_pos.x, backup_pos.y);
    ImVec2 end_pos = split_vertically ? ImVec2(pos.x + thickness, pos.y + window->Size.y) : ImVec2(pos.x + window->Size.x, pos.y + thickness);
    draw_list->AddRectFilled(pos, end_pos, color);

    return updated;
}

bool ColorPicker3U32(const char *label, ImU32 *color, ImGuiColorEditFlags flags = 0)
{
    float col[3];
    col[0] = (float)((*color >> 0) & 0xFF) / 255.0f;
    col[1] = (float)((*color >> 8) & 0xFF) / 255.0f;
    col[2] = (float)((*color >> 16) & 0xFF) / 255.0f;

    bool result = ImGui::ColorPicker3(label, col, flags);

    if (result)
    {
        *color = ((ImU32)(col[0] * 255.0f)) |
                 ((ImU32)(col[1] * 255.0f) << 8) |
                 ((ImU32)(col[2] * 255.0f) << 16) |
                 ((ImU32)(255) << 24);
    }

    ImVec4 buttonColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    ImVec4 buttonHoveredColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 buttonActiveColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 cancelButtonColor = ImVec4(0.4f, 0.2f, 0.2f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);

    if (ImGui::Button("Cancel", ImVec2(75.0f, 0.0f)))
    {
        //
    }

    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.3f, 0.7f, 1.0f));

    if (ImGui::Button("Done", ImVec2(75.0f, 0.0f)))
    {
        *color = ImColor(col[0], col[1], col[2]);
        // VortexMaker::PublishContentBrowserCustomFolder(current_editing_folder.first, VortexMaker::ImU32ToHex(*color), current_editing_folder_is_favorite);
    }

    ImGui::PopStyleColor(3);

    return result;
}

bool CollapsingHeaderWithIcon(const char *label, ImTextureID icon)
{
    ImGui::PushID(label);

    bool open = ImGui::CollapsingHeader("##hidden", ImGuiTreeNodeFlags_AllowItemOverlap);

    ImVec2 textPos = ImGui::GetCursorPos();
    ImGui::SameLine();
    ImGui::SetCursorPosX(textPos.x + ImGui::GetStyle().FramePadding.x);

    ImGui::Image(icon, ImVec2(16, 16));
    ImGui::SameLine();

    ImGui::SetCursorPosY(textPos.y);
    ImGui::TextUnformatted(label);

    ImGui::PopID();

    return open;
}

static std::uintmax_t getDirectorySize(const std::filesystem::path &directoryPath)
{
    std::uintmax_t size = 0;

    for (const auto &entry : std::filesystem::recursive_directory_iterator(directoryPath))
    {
        if (std::filesystem::is_regular_file(entry.path()))
        {
            size += std::filesystem::file_size(entry.path());
        }
    }

    return size;
}

static std::string formatFileSize(size_t size)
{
    const char *units[] = {"o", "ko", "Mo", "Go", "To"};
    int unitIndex = 0;
    double displaySize = static_cast<double>(size);

    while (displaySize >= 1024 && unitIndex < 4)
    {
        displaySize /= 1024;
        ++unitIndex;
    }

    char formattedSize[20];
    snprintf(formattedSize, sizeof(formattedSize), "%.2f %s", displaySize, units[unitIndex]);
    return std::string(formattedSize);
}

static ImU32 DarkenColor(ImU32 color, float amount)
{
    int r = (color & 0xFF000000) >> 24;
    int g = (color & 0x00FF0000) >> 16;
    int b = (color & 0x0000FF00) >> 8;
    int a = color & 0x000000FF;

    r = static_cast<int>(r * (1.0f - amount));
    g = static_cast<int>(g * (1.0f - amount));
    b = static_cast<int>(b * (1.0f - amount));

    return IM_COL32(r, g, b, a);
}

namespace Cherry
{
    std::string get_extension(const std::string &path)
    {
        size_t dot_pos = path.find_last_of('.');
        if (dot_pos == std::string::npos)
            return "";
        return path.substr(dot_pos + 1);
    }

    FileTypes detect_file(const std::string &path)
    {
        static const std::unordered_map<std::string, FileTypes> extension_map = {
            {"asm", FileTypes::File_ASM},
            {"bin", FileTypes::File_BIN},
            {"c", FileTypes::File_C},
            {"h", FileTypes::File_H},
            {"cpp", FileTypes::File_CPP},
            {"hpp", FileTypes::File_HPP},
            {"inl", FileTypes::File_INL},
            {"rs", FileTypes::File_RUST},
            {"zig", FileTypes::File_ZIG},
            {"go", FileTypes::File_GO},
            {"cfg", FileTypes::File_CFG},
            {"json", FileTypes::File_JSON},
            {"txt", FileTypes::File_TXT},
            {"md", FileTypes::File_MD},
            {"yaml", FileTypes::File_YAML},
            {"ini", FileTypes::File_INI},
            {"gitignore", FileTypes::File_GIT},
            {"gitmodules", FileTypes::File_GIT},
            {"git", FileTypes::File_GIT},
            {"png", FileTypes::File_PICTURE},
            {"jpg", FileTypes::File_PICTURE},
            {"jpeg", FileTypes::File_PICTURE},
        };

        std::string extension = get_extension(path);
        auto it = extension_map.find(extension);
        if (it != extension_map.end())
        {
            return it->second;
        }
        else
        {
            return FileTypes::File_UNKNOW;
        }
    }

    static std::vector<std::pair<std::shared_ptr<ContenBrowserItem>, std::string>> recognized_modules_items;

    ContentBrowserAppWindow::ContentBrowserAppWindow(const std::string &name, const std::string &start_path)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        cp_SaveButton = Application::Get().CreateComponent<ImageTextButtonSimple>("save_button", Application::Get().GetLocale("loc.content_browser.save_all") + "####content_browser.save_all", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_save.png"));
        cp_SaveButton->SetScale(0.85f);
        cp_SaveButton->SetLogoSize(15, 15);
        cp_SaveButton->SetBackgroundColorIdle("#00000000");
        cp_SaveButton->SetBorderColorIdle("#00000000");

        cp_ImportButton = Application::Get().CreateComponent<ImageTextButtonSimple>("import_button", Application::Get().GetLocale("loc.content_browser.import") + "####content_browser.import", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_import.png"));
        cp_ImportButton->SetScale(0.85f);
        cp_ImportButton->SetLogoSize(15, 15);
        cp_ImportButton->SetBackgroundColorIdle("#00000000");
        cp_ImportButton->SetBorderColorIdle("#00000000");

        cp_AddButton = Application::Get().CreateComponent<ImageTextButtonSimple>("add_button", Application::Get().GetLocale("loc.content_browser.add") + "####content_browser.add", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_add.png"));
        cp_AddButton->SetScale(0.85f);
        cp_AddButton->SetInternalMarginX(10.0f);
        cp_AddButton->SetLogoSize(15, 15);

        cp_SettingsButton = Application::Get().CreateComponent<CustomDrowpdownImageButtonSimple>("setgings_button", Application::Get().GetLocale("loc.content_browser.add") + "####content_browser.settings", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_add.png"));
        cp_SettingsButton->SetScale(0.85f);
        cp_SettingsButton->SetInternalMarginX(10.0f);
        cp_SettingsButton->SetLogoSize(15, 15);

        cp_DirectoryUndo = Application::Get().CreateComponent<ImageButtonSimple>("directory_undo", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_arrow_l_disabled.png"));
        cp_DirectoryRedo = Application::Get().CreateComponent<ImageButtonSimple>("directory_redo", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_arrow_r_disabled.png"));

        cp_DirectoryRedo->SetBackgroundColorIdle("#00000000");
        cp_DirectoryRedo->SetBorderColorIdle("#00000000");
        cp_DirectoryRedo->SetScale(0.85f);

        cp_DirectoryUndo->SetBackgroundColorIdle("#00000000");
        cp_DirectoryUndo->SetBorderColorIdle("#00000000");
        cp_DirectoryUndo->SetScale(0.85f);

        m_AppWindow->SetLeftMenubarCallback([this]()
                                            {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
            
        if(cp_AddButton->Render())
        {
            //
            ImGui::InsertNotification({ImGuiToastType::Success, 3000, "That is a success! %s", "(Format here)"});
        }
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(62, 62, 62, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(62, 62, 62, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(62, 62, 62, 0));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(62, 62, 62, 0));
        
        if(cp_SaveButton->Render())
        {
            //
        }

        if(cp_ImportButton->Render())
        {
            //
        }
        
		ImGui::PopStyleColor(4);

		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 12));

		if (m_BackHistory.empty())
		{
            cp_DirectoryUndo->SetImagePath(Application::Get().CookPath("ressources/imgs/icons/misc/icon_arrow_l_disabled.png"));
			if (cp_DirectoryUndo->Render("normal"))
			{
				//
			}
		}
		else
		{
            cp_DirectoryUndo->SetImagePath(Application::Get().CookPath("ressources/imgs/icons/misc/icon_arrow_l_enabled.png"));
			if (cp_DirectoryUndo->Render("normal"))
			{
				GoBack();
			}

			if (ImGui::IsMouseClicked(3))
			{
				GoBack();
			}
		}

		if (m_ForwardHistory.empty())
		{
            cp_DirectoryUndo->SetImagePath(Application::Get().CookPath("ressources/imgs/icons/misc/icon_arrow_r_disabled.png"));
			if (cp_DirectoryRedo->Render("normal"))
			{
				//
			}
		}
		else
		{
            cp_DirectoryUndo->SetImagePath(Application::Get().CookPath("ressources/imgs/icons/misc/icon_arrow_r_enabled.png"));
			if (cp_DirectoryRedo->Render("normal"))
			{
				GoForward();
			}

			if (ImGui::IsMouseClicked(4))
			{
				GoForward();
			}
		}
		ImGui::PopStyleVar();

		ImGui::PopStyleColor();

		ImGui::Separator();

		if (m_Selected.size() > 0)
		{
			std::string label = std::to_string(m_Selected.size()) + " copies";
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), label.c_str());
		}

		if (m_CutSelection.size() > 0)
		{
			std::string label = std::to_string(m_CutSelection.size()) + " cuts";
			ImGui::Text(label.c_str());
		}

		if (m_Selected.size() > 0)
		{
			std::string label = std::to_string(m_Selected.size()) + " selected.";
			ImGui::Text(label.c_str());
		} });

        m_AppWindow->SetRightMenubarCallback([this]()
                                             {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
            
        if(cp_SettingsButton->Render())
        {
            //
        }
		ImGui::PopStyleColor(); });

        m_BaseDirectory = start_path;
        m_CurrentDirectory = m_BaseDirectory;

        ContentBrowserChild sidebar("RenderSideBar", [this]()
                                    { RenderSideBar(); });
        sidebar.Enable();
        sidebar.m_BackgroundColor = ImVec4(0.0f,0.6f,0.6f,1.0f);
        AddChild(sidebar);

        ContentBrowserChild filterbar("RenderFiltersBar", [this]()
                                      { RenderFiltersBar(); });
        filterbar.Disable();
        AddChild(filterbar);

        ContentBrowserChild contentbar("RenderContentBar", [this]()
                                       { RenderContentBar(); });
        contentbar.Enable();
        AddChild(ContentBrowserChild(contentbar));

        ContentBrowserChild detailsbar("RenderDetailsBar", [this]()
                                       { RenderDetailsBar(); });
        detailsbar.Disable();
        AddChild(ContentBrowserChild(detailsbar));
    }

    void ContentBrowserAppWindow::AddChild(const ContentBrowserChild &child)
    {
        m_Childs.push_back(child);
    }

    void ContentBrowserAppWindow::GoBack()
    {
        if (!m_BackHistory.empty())
        {
            m_ForwardHistory.push(m_CurrentDirectory);
            m_CurrentDirectory = m_BackHistory.top();
            m_BackHistory.pop();
        }
    }

    void ContentBrowserAppWindow::GoForward()
    {
        if (!m_ForwardHistory.empty())
        {
            m_BackHistory.push(m_CurrentDirectory);
            m_CurrentDirectory = m_ForwardHistory.top();
            m_ForwardHistory.pop();
        }
    }

    void ContentBrowserAppWindow::ChangeDirectory(const std::filesystem::path &newDirectory)
    {
        if (newDirectory != m_CurrentDirectory)
        {
            if (!m_CurrentDirectory.empty())
            {
                m_BackHistory.push(m_CurrentDirectory);

                while (!m_ForwardHistory.empty())
                {
                    m_ForwardHistory.pop();
                }
            }
            m_CurrentDirectory = newDirectory;
        }
    }

    bool ContentBrowserAppWindow::MyButton(const std::string &name, const std::string &path, const std::string &description, const std::string &size, bool selected, const std::string &logo, ImU32 bgColor = IM_COL32(100, 100, 100, 255), ImU32 borderColor = IM_COL32(150, 150, 150, 255), ImU32 lineColor = IM_COL32(255, 255, 0, 255), float maxTextWidth = 100.0f, float borderRadius = 5.0f)
    {
        bool pressed = false;

        float logoSize = 60.0f;
        float extraHeight = 80.0f;
        float padding = 10.0f;
        float separatorHeight = 2.0f;
        float textOffsetY = 5.0f;
        float versionBoxWidth = 10.0f;
        float versionBoxHeight = 20.0f;
        float thumbnailIconOffsetY = 30.0f;

        if (selected)
        {
            bgColor = IM_COL32(80, 80, 240, 255);
            borderColor = IM_COL32(150, 150, 255, 255);
        }

        ImVec2 squareSize(logoSize, logoSize);

        const char *originalText = name.c_str();
        std::string truncatedText = name;

        if (ImGui::CalcTextSize(originalText).x > maxTextWidth)
        {
            truncatedText = name.substr(0, 20);
            if (ImGui::CalcTextSize(truncatedText.c_str()).x > maxTextWidth)
            {
                truncatedText = name.substr(0, 10) + "\n" + name.substr(10, 10);
            }
        }
        else
        {
            truncatedText = name + "\n";
        }

        ImVec2 fixedSize(maxTextWidth + padding * 2, logoSize + extraHeight + padding * 2);

        ImVec2 cursorPos = ImGui::GetCursorScreenPos();

        std::string button_id = name + "squareButtonWithText" + name;
        if (ImGui::InvisibleButton(button_id.c_str(), fixedSize))
        {
            pressed = true;
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
        ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

        ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);

        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);

        static bool open_deletion_modal = false;

        static bool delete_single_file = false;
        static std::string delete_single_file_path = "";

        if (open_deletion_modal)
        {
            ImGui::SetNextWindowSize(ImVec2(300, 200));

            static ImGuiTableFlags window_flags = ImGuiWindowFlags_NoResize;
            if (ImGui::BeginPopupModal("Delete file(s)", NULL, window_flags))
            {
                static char path_input_all[512];

                if (delete_single_file)
                {
                    ImGui::TextWrapped("WARNING, one file");
                }
                else
                {
                    ImGui::TextWrapped("WARNING, if you click on the Delete button, the project will be erase forever.");
                }

                ImGui::SetItemDefaultFocus();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    open_deletion_modal = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.2f, 0.2f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.2f, 0.2f, 1.8f));
                if (ImGui::Button("Delete", ImVec2(120, 0)))
                {
                    if (delete_single_file)
                    {
                        if (m_DeletePathCallback)
                        {
                            m_DeletePathCallback(delete_single_file_path);
                        }
                        delete_single_file_path = "";
                        delete_single_file = false;
                    }
                    else
                    {
                        for (auto item : m_Selected)
                        {
                            m_DeletePathCallback(item);
                        }
                    }

                    open_deletion_modal = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::PopStyleColor(3);
                ImGui::EndPopup();
            }
        }
        if (open_deletion_modal)
            ImGui::OpenPopup("Delete file(s)");

        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            m_Selected.push_back(path);
        }

        if (ImGui::BeginPopupContextItem("ContextPopup"))
        {
            ImGui::GetFont()->Scale *= 0.9;
            ImGui::PushFont(ImGui::GetFont());

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

            ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
            ImGui::Text("Main");
            ImGui::PopStyleColor();

            if (ImGui::MenuItem("Rename", "Ctrl + R"))
            {
                pathToRename = path;
                strncpy(pathRename, name.c_str(), sizeof(pathRename));
                pathRename[sizeof(pathRename) - 1] = '\0';
            }

            std::string cpy_label = "Copy (" + std::to_string(m_Selected.size()) + ") selected";

            if (ImGui::MenuItem(cpy_label.c_str(), "Ctrl + C"))
            {
                if (m_CopyPathsCallback)
                {
                    m_CopyPathsCallback(m_Selected);
                    for (auto &path : m_Selected)
                    {
                        m_CopySelection.push_back(path);
                    }
                }

                m_Selected.clear();

                ImGui::CloseCurrentPopup();
            }

            if (m_CopySelection.size() > 0)
            {
                std::string label = "Copy in addition (" + std::to_string(m_CopySelection.size()) + " copies)";
                if (ImGui::MenuItem(label.c_str(), "Ctrl + Alt + C"))
                {
                    if (m_CopyPathsCallback)
                    {
                        m_CopyPathsCallback(m_Selected);

                        for (auto &path : m_Selected)
                        {
                            m_CopySelection.push_back(path);
                        }
                    }
                    m_Selected.clear();
                    ImGui::CloseCurrentPopup();
                }
            }

            if (ImGui::MenuItem("Delete", "Suppr"))
            {
                delete_single_file = true;
                delete_single_file_path = path;
                open_deletion_modal = true;
                ImGui::CloseCurrentPopup();
            }

            if (m_Selected.size() > 1)
            {
                std::string label = "Delete (" + std::to_string(m_Selected.size()) + " selected)";
                if (ImGui::MenuItem(label.c_str(), "Alt + Suppr"))
                {
                    open_deletion_modal = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
            ImGui::Separator();
            ImGui::PopStyleColor();

            ImGui::GetFont()->Scale *= 0.9;
            ImGui::PushFont(ImGui::GetFont());

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

            ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
            ImGui::Text("Customization");
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
            ImGui::Separator();
            ImGui::PopStyleColor();

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImDrawList *drawList = ImGui::GetWindowDrawList();

        drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + fixedSize.x, cursorPos.y + fixedSize.y), bgColor, borderRadius);
        drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + fixedSize.x, cursorPos.y + thumbnailIconOffsetY + squareSize.y), IM_COL32(26, 26, 26, 255), borderRadius, ImDrawFlags_RoundCornersTop);
        drawList->AddRect(cursorPos, ImVec2(cursorPos.x + fixedSize.x, cursorPos.y + fixedSize.y), borderColor, borderRadius, 0, 1.0f);

        ImVec2 logoPos = ImVec2(cursorPos.x + (fixedSize.x - squareSize.x) / 2, cursorPos.y + padding);

        ImVec2 sizePos = ImVec2(cursorPos.x + padding, cursorPos.y + squareSize.y + thumbnailIconOffsetY - 20 + textOffsetY);
        ImGui::SetCursorScreenPos(sizePos);

        static ImTextureID logotexture = Application::GetCurrentRenderedWindow()->get_texture(logo);
        drawList->AddImage(logotexture, logoPos, ImVec2(logoPos.x + squareSize.x, logoPos.y + squareSize.y));

        ImGui::GetFont()->Scale *= 0.7;
        ImGui::PushFont(ImGui::GetFont());
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::PushItemWidth(maxTextWidth);
        ImGui::TextWrapped(size.c_str());
        ImGui::PopItemWidth();
        ImGui::PopStyleColor();

        ImGui::GetFont()->Scale = 1.0f;
        ImGui::PopFont();

        ImVec2 lineStart = ImVec2(cursorPos.x, cursorPos.y + squareSize.y + thumbnailIconOffsetY + separatorHeight);
        ImVec2 lineEnd = ImVec2(cursorPos.x + fixedSize.x, cursorPos.y + squareSize.y + thumbnailIconOffsetY + separatorHeight);
        drawList->AddLine(lineStart, lineEnd, lineColor, separatorHeight);

        ImGui::GetFont()->Scale *= 0.9;
        ImGui::PushFont(ImGui::GetFont());

        ImVec2 textPos = ImVec2(cursorPos.x + padding, cursorPos.y + squareSize.y + thumbnailIconOffsetY + textOffsetY);
        ImGui::SetCursorScreenPos(textPos);
        ImGui::PushItemWidth(maxTextWidth);
        ImU32 textColor = IM_COL32(255, 255, 255, 255);
        ImU32 highlightColor = IM_COL32(255, 255, 0, 255);
        ImU32 highlightTextColor = IM_COL32(0, 0, 0, 255);

        if (pathToRename == path)
        {
            ImGui::SetItemAllowOverlap();
            ImGui::PushID(path.c_str());
            if (ImGui::InputText("", pathRename, sizeof(pathRename), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::cout << "Renamed file to: " << pathRename << std::endl;
                pathToRename = "";
            }
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                std::cout << "Renamed file to: " << pathRename << std::endl;
                pathToRename = "";
            }
            ImGui::PopID();
        }
        else
        {
            DrawHighlightedText(drawList, textPos, truncatedText.c_str(), ProjectSearch, highlightColor, textColor, highlightTextColor);
        }

        ImGui::PopItemWidth();

        ImGui::GetFont()->Scale = 1.0f;
        ImGui::PopFont();

        ImVec2 descriptionPos = ImVec2(cursorPos.x + padding, cursorPos.y + squareSize.y + thumbnailIconOffsetY + 35 + textOffsetY);
        ImGui::SetCursorScreenPos(descriptionPos);

        ImGui::GetFont()->Scale *= 0.7;
        ImGui::PushFont(ImGui::GetFont());
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::PushItemWidth(maxTextWidth);
        ImGui::TextWrapped(description.c_str());
        ImGui::PopItemWidth();
        ImGui::PopStyleColor();

        ImGui::GetFont()->Scale = 1.0f;
        ImGui::PopFont();

        ImVec2 smallRectPos = ImVec2(cursorPos.x + fixedSize.x - versionBoxWidth - padding, cursorPos.y + fixedSize.y - versionBoxHeight - padding);
        drawList->AddRectFilled(smallRectPos, ImVec2(smallRectPos.x + versionBoxWidth, smallRectPos.y + versionBoxHeight), IM_COL32(0, 0, 0, 255), borderRadius);

        float windowVisibleX2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        if (cursorPos.x + fixedSize.x < windowVisibleX2)
            ImGui::SameLine();

        ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y + fixedSize.y + padding));

        return pressed;
    }

    void ContentBrowserAppWindow::DrawFolderIcon(ImVec2 pos, ImVec2 size, ImU32 color)
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();

        float folderFlapHeight = size.y * 0.2f;
        float flapSlopeWidth = size.x * 0.15f;
        float borderRadius = size.y * 0.1f;

        ImVec2 flapTopLeft = pos;
        ImVec2 flapBottomRight = ImVec2(pos.x + size.x * 0.6f, pos.y + folderFlapHeight);
        ImVec2 flapSlopeEnd = ImVec2(flapBottomRight.x + flapSlopeWidth, flapBottomRight.y);

        ImVec2 bodyTopLeft = ImVec2(pos.x, pos.y + folderFlapHeight);
        ImVec2 bodyBottomRight = ImVec2(pos.x + size.x, pos.y + size.y);

        drawList->AddRectFilled(bodyTopLeft, bodyBottomRight, color);

        ImVec2 centerRectTopLeft = ImVec2(pos.x + size.x * 0.2f, pos.y + 0.2f + size.y * 0.2f + 6.8f);
        ImVec2 centerRectBottomRight = ImVec2(pos.x + size.x * 0.8f, pos.y + size.y * 0.8f - 2.8f);
        drawList->AddRectFilled(centerRectTopLeft, centerRectBottomRight, IM_COL32_WHITE, 0.0f, 0);

        ImVec2 secondRectTopLeft = ImVec2(pos.x + size.x * 0.2f, pos.y + 0.2f + size.y * 0.2f + 11.8f);
        ImVec2 secondRectBottomRight = ImVec2(pos.x + size.x, pos.y + size.y);
        drawList->AddRectFilled(secondRectTopLeft, secondRectBottomRight, color, 0.0f, 0);

        drawList->AddRectFilled(flapTopLeft, flapBottomRight, color, borderRadius, ImDrawFlags_RoundCornersTopLeft);
        drawList->AddTriangleFilled(flapBottomRight, flapSlopeEnd, ImVec2(flapBottomRight.x - 3, flapTopLeft.y), color);
    }

    void ContentBrowserAppWindow::MyFolderButton(const char *id, ImVec2 size, ImU32 color, const std::string &path)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton(id, size);
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            ChangeDirectory(path);
        }

        float thumbmailWidth = thumbnailSize;
        float folderWidth = size.x;
        float offsetX = (thumbmailWidth - folderWidth) / 2.0f;

        ImVec2 iconPos = ImVec2(pos.x + offsetX + 7.5f, pos.y);

        DrawFolderIcon(iconPos, size, color);

        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
    }

    void ContentBrowserAppWindow::DrawHierarchy(std::filesystem::path path, bool isDir, const std::string &label = "")
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12.0f);
        std::string tree_label = "???";
        if (label.empty())
        {
            tree_label = path.filename().string() + "###treenode";
        }
        else
        {
            tree_label = label + "###treenode";
        }

        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SetItemAllowOverlap();

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImU32 col;

        DrawFolderIcon(pos, ImVec2(12, 12), HexToImU32(GetContentBrowserFolderColor(path)));

        if (ImGui::TreeNode(tree_label.c_str()))
        {
            for (auto &dirEntry : std::filesystem::directory_iterator(path))
            {
                const std::filesystem::path &otherPath = dirEntry.path();

                DrawHierarchy(otherPath, dirEntry.is_directory());
            }

            ImGui::TreePop();
        }
        ImVec2 finalCursorPos = ImGui::GetCursorPos();
        ImVec2 size = ImGui::GetItemRectSize();
    }

    void ContentBrowserAppWindow::RenderSideBar()
    {
        CustomCollapsingHeaderLogo("Favorite", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_star.png"), [this]()
                                   {
                                       for (auto custom_dir : m_FavoriteFolders)
                                       {
                                           DrawHierarchy(custom_dir, true);
                                       } });

        CustomCollapsingHeaderLogo("Main", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_home.png"), [this]()
                                   { DrawHierarchy(m_BaseDirectory, true, "Main"); });

        CustomCollapsingHeaderLogo("Pools & Collections", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_collection.png"), [this]()
                                   {
                                       ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 2.0f));

                                       ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
                                       if (!pool_add_mode)
                                       {
                                           if (ImGui::ImageButtonWithText(Application::Get().GetCurrentRenderedWindow()->get_texture("/usr/local/include/Vortex/imgs/vortex.png"), "Add pool", ImVec2(0, 0), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
                                           {
                                               pool_add_mode = true;
                                           }
                                       }
                                       else
                                       {
                                           ImGui::Text("Please enter a path");
                                           ImGui::SetNextItemWidth(-FLT_MIN);
                                           ImGui::InputText("###AddPool", pool_add_path, sizeof(pool_add_path));
                                           if (ImGui::ImageButtonWithText(Application::Get().GetCurrentRenderedWindow()->get_texture("/usr/local/include/Vortex/imgs/vortex.png"), "Add", ImVec2(0, 0), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
                                           {
                                               // FIX VortexMaker::PublishPool(pool_add_path);
                                               pool_add_mode = false;
                                           }
                                           ImGui::SameLine();
                                           if (ImGui::ImageButtonWithText(Application::Get().GetCurrentRenderedWindow()->get_texture("/usr/local/include/Vortex/imgs/vortex.png"), "Cancel", ImVec2(0, 0), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
                                           {
                                               pool_add_mode = false;
                                           }
                                       }
                                       ImGui::PopStyleVar();
                                       ImGui::PopStyleColor(); });

        for (auto custom_dir : m_Pools)
        {
            std::size_t lastSlashPos = custom_dir.find_last_of("/\\");

            std::string name = custom_dir.substr(lastSlashPos + 1);

            DrawHierarchy(custom_dir, true, name);
        }
    }

    void ContentBrowserAppWindow::RenderFiltersBar()
    {
        CustomCollapsingHeaderLogo("Favorite", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_star.png"), [this]()
                                   {
                                       for (auto custom_dir : m_FavoriteFolders)
                                       {
                                           DrawHierarchy(custom_dir, true);
                                       } });
    }

    void ContentBrowserAppWindow::RenderContentBar()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 originalChildBgColor = style.Colors[ImGuiCol_ChildBg];
        ImVec4 originalBorderColor = style.Colors[ImGuiCol_Border];
        ImVec4 originalBorderShadowColor = style.Colors[ImGuiCol_BorderShadow];
        float oldsize = ImGui::GetFont()->Scale;

        style.Colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;

        ImGui::InputText("Search", ProjectSearch, sizeof(ProjectSearch));

        std::vector<std::filesystem::directory_entry> directories;
        std::vector<std::filesystem::directory_entry> files;

        recognized_modules_items.clear();

        for (auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            bool isItem = false;
            for (auto item : m_ItemToReconize)
            {
                std::string path = directoryEntry.path().string();
                if (item->f_Detect(path))
                {
                    recognized_modules_items.push_back({item, path});
                    isItem = true;
                }
            }

            if (isItem)
            {
                continue;
            }

            if (directoryEntry.is_directory())
            {
                directories.push_back(directoryEntry);
            }
            else
            {
                files.push_back(directoryEntry);
            }
        }

        std::sort(directories.begin(), directories.end(), [](const auto &a, const auto &b)
                  { return a.path().filename().string() < b.path().filename().string(); });

        if (m_ShowMode == ContentShowMode::Thumbmails)
        {
            ImGui::Columns(columnCount, 0, false);

            ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
            ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
            ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
            ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

            ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

            ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);

            ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);

            if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            {
                m_Selected.clear();
                ImGui::OpenPopup("EmptySpacePopup");
            }

            if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                m_Selected.clear();
            }

            if (ImGui::BeginPopup("EmptySpacePopup"))
            {
                ImGui::GetFont()->Scale *= 0.9;
                ImGui::PushFont(ImGui::GetFont());

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                ImGui::Text("Main");
                ImGui::PopStyleColor();

                if (ImGui::Selectable("Paste"))
                {
                    if (m_PastePathsCallback)
                    {
                        m_PastePathsCallback({m_CurrentDirectory});
                    }
                }
                ImGui::GetFont()->Scale = 1.0f;
                ImGui::PushFont(ImGui::GetFont());
                ImGui::EndPopup();
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
            for (auto &directoryEntry : directories)
            {
                const auto &path = directoryEntry.path();
                std::string filenameString = path.filename().string();

                if (areStringsSimilar(filenameString, ProjectSearch, threshold) || isOnlySpacesOrEmpty(ProjectSearch))
                {
                    ImGui::PushID(filenameString.c_str());

                    float reducedThumbnailSize = thumbnailSize * 0.9f;

                    float availableWidth = ImGui::GetContentRegionAvail().x;
                    float imageOffsetX = (availableWidth - reducedThumbnailSize) * 0.5f;

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

                    // Old folder logo
                    /*ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imageOffsetX);
                    addTexture(Folder_Logo, Folder_Logo);
                    ImGui::ImageButton(getTexture(Folder_Logo), {reducedThumbnailSize, reducedThumbnailSize}, {-1, 0}, {0, 1});

                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        ChangeDirectory(path);
                    }*/

                    ImVec2 folderSize(reducedThumbnailSize, reducedThumbnailSize);
                    std::string folderPath = "path/to/folder";

                    if (current_editing_folder.first == path.string())
                    {
                        MyFolderButton("folder_icon", folderSize, current_editing_folder.second, path);
                    }
                    else
                    {
                        MyFolderButton("folder_icon", folderSize, HexToImU32(GetContentBrowserFolderColor(path)), path);
                    }

                    float oldsize = ImGui::GetFont()->Scale;

                    ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
                    ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                    ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
                    ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

                    ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

                    ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);

                    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
                    if (ImGui::BeginPopupContextItem("ContextPopup"))
                    {
                        ImGui::GetFont()->Scale *= 0.9;
                        ImGui::PushFont(ImGui::GetFont());

                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                        ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                        ImGui::Text("Main");
                        ImGui::PopStyleColor();

                        ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                        ImGui::Separator();
                        ImGui::PopStyleColor();

                        ImGui::GetFont()->Scale = oldsize;
                        ImGui::PopFont();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                        if (ImGui::MenuItem("Open", "Ctrl + O"))
                        {
                            ChangeDirectory(path);
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::MenuItem("Copy folder", "Ctrl + C"))
                        {
                            ChangeDirectory(path);
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::MenuItem("Cut folder", "Ctrl + X"))
                        {
                            ChangeDirectory(path);
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::GetFont()->Scale *= 0.9;
                        ImGui::PushFont(ImGui::GetFont());

                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

                        ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                        ImGui::Text("Customization");
                        ImGui::PopStyleColor();

                        ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                        ImGui::Separator();
                        ImGui::PopStyleColor();

                        ImGui::GetFont()->Scale = oldsize;
                        ImGui::PopFont();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                        static bool EditingColor = false;
                        static bool ColorChanged = false;

                        current_editing_folder_is_favorite = IsPathFavorite(directoryEntry.path().string());

                        if (ImGui::BeginMenu("Change color"))
                        {
                            if (!EditingColor)
                            {
                                current_editing_folder = {directoryEntry.path().string(), folder_color};

                                current_editing_folder = {directoryEntry.path().string(), HexToImU32(GetContentBrowserFolderColor(path.string()))};

                                current_editing_folder_is_favorite = IsPathFavorite(directoryEntry.path().string());
                            }

                            EditingColor = true;

                            static bool alpha_preview = true;
                            static bool alpha_half_preview = false;
                            static bool drag_and_drop = true;
                            static bool options_menu = true;
                            static bool hdr = false;

                            ColorPicker3U32("MyColor", &current_editing_folder.second);

                            if (current_editing_folder.second != folder_color)
                            {
                                ColorChanged = true;
                            }

                            ImGui::EndMenu();
                        }
                        else
                        {
                            EditingColor = false;
                        }

                        if (ImGui::MenuItem("Mark as favorite", "", current_editing_folder_is_favorite))
                        {
                            current_editing_folder = {directoryEntry.path().string(), current_editing_folder.second};

                            current_editing_folder_is_favorite = !current_editing_folder_is_favorite;
                            SetColoredFolder(current_editing_folder.first, ImU32ToHex(current_editing_folder.second));
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor(2);
                    ImGui::PopStyleVar(2);
                    ImGui::PopStyleColor(3);

                    float textWidth = ImGui::CalcTextSize(filenameString.c_str()).x;
                    float textOffsetX = (availableWidth - textWidth) * 0.5f;

                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffsetX);
                    ImGui::TextWrapped(filenameString.c_str());

                    ImGui::PopID();
                    ImGui::NextColumn();
                }
            }

            for (auto &itemEntry : recognized_modules_items)
            {
                const auto &path = itemEntry.second;
                std::filesystem::path fsPath(path);
                std::string filenameString = fsPath.filename().string();

                bool selected = false;

                if (std::find(m_Selected.begin(), m_Selected.end(), path) != m_Selected.end())
                {
                    selected = true;
                }

                if (areStringsSimilar(filenameString, ProjectSearch, threshold) || isOnlySpacesOrEmpty(ProjectSearch))
                {
                    std::uintmax_t folderSize = getDirectorySize(path);
                    std::string folderSizeString = formatFileSize(folderSize);
                    ImGui::PushID(filenameString.c_str());

                    if (MyButton(filenameString, path, itemEntry.first->m_Description, folderSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_picture_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(itemEntry.first->m_LineColor.x, itemEntry.first->m_LineColor.y, itemEntry.first->m_LineColor.z, itemEntry.first->m_LineColor.w)))
                    {
                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            // itemEntry.first->f_Execute(path);
                            // VXERROR("te", "tyr");
                        }

                        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                        {
                            m_Selected.push_back(path);
                        }
                        else
                        {
                            m_Selected.clear();
                            m_Selected.push_back(path);
                        }
                    }

                    ImGui::PopID();
                    ImGui::NextColumn();
                }
            }

            for (auto &fileEntry : files)
            {
                const auto &path = fileEntry.path();
                std::string filenameString = path.filename().string();

                bool selected = false;

                if (std::find(m_Selected.begin(), m_Selected.end(), path) != m_Selected.end())
                {
                    selected = true;
                }

                if (areStringsSimilar(filenameString, ProjectSearch, threshold) || isOnlySpacesOrEmpty(ProjectSearch))
                {
                    size_t fileSize = std::filesystem::file_size(path);
                    std::string fileSizeString = formatFileSize(fileSize);
                    ImGui::PushID(filenameString.c_str());

                    FileTypes fileType = detect_file(path.string());

                    switch (fileType)
                    {
                    case FileTypes::File_PICTURE:
                    {
                        if (MyButton(filenameString, path, "Picture file", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_picture_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(255, 100, 150, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }
                    case FileTypes::File_GIT:
                    {
                        if (MyButton(filenameString, path, "Git File", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_git_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(100, 100, 255, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }
                    case FileTypes::File_H:
                    {
                        if (MyButton(filenameString, path, "C Header File", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_c_h_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(220, 100, 220, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }
                    case FileTypes::File_C:
                    {
                        if (MyButton(filenameString, path, "C Source File", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_c_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(100, 100, 255, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }
                    case FileTypes::File_HPP:
                    {
                        if (MyButton(filenameString, path, "C++ Header File", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_cpp_h_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(100, 100, 255, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }
                    case FileTypes::File_CPP:
                    {
                        if (MyButton(filenameString, path, "C++ Source File", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_cpp_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(100, 100, 255, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }
                    case FileTypes::File_INI:
                    {
                        if (MyButton(filenameString, path, "Init File", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_ini_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(150, 150, 150, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }
                    default:
                    {
                        if (MyButton(filenameString, path, "File", fileSizeString, selected, Application::CookPath("ressources/imgs/icons/files/icon_unknow_file.png"), IM_COL32(56, 56, 56, 150), IM_COL32(50, 50, 50, 255), IM_COL32(150, 150, 150, 255)))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }
                        break;
                    }

                        ImGui::PopStyleVar(2);
                        ImGui::PopStyleColor(3);
                    }
                    float oldsize = ImGui::GetFont()->Scale;

                    if (ImGui::BeginPopupContextItem("ItemContextPopup"))
                    {
                        ImGui::GetFont()->Scale *= 0.9;
                        ImGui::PushFont(ImGui::GetFont());

                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                        ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
                        ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                        ImGui::PushStyleColor(ImGuiCol_Text, grayColor);

                        ImGui::Text("Main");

                        ImGui::PopStyleColor();

                        ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                        ImGui::Separator();
                        ImGui::PopStyleColor();

                        ImGui::GetFont()->Scale = oldsize;
                        ImGui::PopFont();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                        if (ImGui::MenuItem("Open", "Ctrl + O"))
                        {
                            ChangeDirectory(path);
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::MenuItem("Copy folder", "Ctrl + C"))
                        {
                            if (m_CopyPathsCallback)
                            {
                                m_CopyPathsCallback(m_Selected);
                            }

                            m_Selected.clear();
                            ImGui::CloseCurrentPopup();
                        }

                        if (m_Selected.size() > 0)
                        {
                            std::string label = "Copy in addition (" + m_Selected.size() + ' copies)';
                            if (ImGui::MenuItem(label.c_str(), "Ctrl + C"))
                            {
                                if (m_CopyPathsCallback)
                                {
                                    m_CopyPathsCallback(m_Selected);
                                }

                                m_Selected.clear();
                                ImGui::CloseCurrentPopup();
                            }
                        }
                        if (ImGui::MenuItem("Cut folder", "Ctrl + X"))
                        {
                            ChangeDirectory(path);
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::GetFont()->Scale *= 0.9;
                        ImGui::PushFont(ImGui::GetFont());

                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

                        ImGui::PushStyleColor(ImGuiCol_Text, grayColor);

                        ImGui::Text("Main");

                        ImGui::PopStyleColor();

                        ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                        ImGui::Separator();
                        ImGui::PopStyleColor();

                        ImGui::GetFont()->Scale = oldsize;
                        ImGui::PopFont();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                        if (ImGui::MenuItem("Change color"))
                        {
                            //
                        }
                        if (ImGui::MenuItem("Mark as favorite"))
                        {
                            //
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::PopID();
                    ImGui::NextColumn();
                }
            }

            ImGui::Columns(1);
        }

        ImGui::Text(std::to_string(m_Selected.size()).c_str());

        if (m_ShowMode == ContentShowMode::Columns)
        {
            static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

            if (ImGui::BeginTable("tablhjke_", 5, flags))
            {
                ImGui::TableSetupColumn("Icon", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Last Updated", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableHeadersRow();

                ImGui::PopStyleVar(4);

                for (auto &directoryEntry : directories)
                {
                    const auto &path = directoryEntry.path();
                    std::string filenameString = path.filename().string();

                    if (areStringsSimilar(filenameString, ProjectSearch, threshold) || isOnlySpacesOrEmpty(ProjectSearch))
                    {
                        bool selected = false;
                        bool rename = false;
                        if (path == pathToRename)
                        {
                            rename = true;
                        }

                        if (std::find(m_Selected.begin(), m_Selected.end(), path) != m_Selected.end())
                        {
                            selected = true;
                        }

                        std::cout << "Selected : " << m_Selected.size() << std::endl;

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        if (selected)
                        {
                            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.0f, 1.0f, 0.5f));
                            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.0f, 1.0f, 0.5f));
                            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 0.0f, 1.0f, 0.5f));
                        }
                        if (ImGui::Selectable("", &selected, ImGuiSelectableFlags_SpanAllColumns))
                        {
                            if (selected)
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.erase(std::remove(m_Selected.begin(), m_Selected.end(), path), m_Selected.end());
                            }
                        }

                        if (selected)
                        {
                            ImGui::PopStyleColor(3);
                        }

                        std::string label = "RowContextMenu###" + filenameString;
                        ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
                        ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                        ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
                        ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
                        ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

                        ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);
                        /*	if (ImGui::IsMouseDoubleClicked(0))
                            {
                                ChangeDirectory(path);
                            }*/

                        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
                            {
                                m_Selected.push_back(path);
                            }
                            else
                            {
                                m_Selected.clear();
                                m_Selected.push_back(path);
                            }
                        }

                        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
                        if (ImGui::BeginPopupContextItem(label.c_str()))
                        {
                            ImGui::GetFont()->Scale *= 0.9;
                            ImGui::PushFont(ImGui::GetFont());

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                            ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                            ImGui::Text("Main");
                            ImGui::PopStyleColor();

                            ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                            ImGui::Separator();
                            ImGui::PopStyleColor();

                            ImGui::GetFont()->Scale = oldsize;
                            ImGui::PopFont();
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                            if (ImGui::MenuItem("Open", "Ctrl + O"))
                            {
                                ChangeDirectory(path);
                                ImGui::CloseCurrentPopup();
                            }
                            if (ImGui::MenuItem("Copy folder", "Ctrl + C"))
                            {
                                ChangeDirectory(path);
                                ImGui::CloseCurrentPopup();
                            }
                            if (ImGui::MenuItem("Cut folder", "Ctrl + X"))
                            {
                                ChangeDirectory(path);
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::GetFont()->Scale *= 0.9;
                            ImGui::PushFont(ImGui::GetFont());

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

                            ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                            ImGui::Text("Customization");
                            ImGui::PopStyleColor();

                            ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                            ImGui::Separator();
                            ImGui::PopStyleColor();

                            ImGui::GetFont()->Scale = oldsize;
                            ImGui::PopFont();
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                            static bool EditingColor = false;
                            static bool ColorChanged = false;

                            current_editing_folder_is_favorite = IsPathFavorite(directoryEntry.path().string());

                            if (ImGui::BeginMenu("Change color"))
                            {
                                if (!EditingColor)
                                {
                                    current_editing_folder = {directoryEntry.path().string(), folder_color};

                                    ImU32 col;

                                    current_editing_folder = {directoryEntry.path().string(), HexToImU32(GetContentBrowserFolderColor(directoryEntry.path().string()))};

                                    current_editing_folder_is_favorite = IsPathFavorite(directoryEntry.path().string());
                                }

                                EditingColor = true;

                                static bool alpha_preview = true;
                                static bool alpha_half_preview = false;
                                static bool drag_and_drop = true;
                                static bool options_menu = true;
                                static bool hdr = false;

                                ColorPicker3U32("MyColor", &current_editing_folder.second);

                                if (current_editing_folder.second != folder_color)
                                {
                                    ColorChanged = true;
                                }

                                ImGui::EndMenu();
                            }
                            else
                            {
                                EditingColor = false;
                            }

                            if (ImGui::MenuItem("Mark as favorite", "", current_editing_folder_is_favorite))
                            {
                                current_editing_folder = {directoryEntry.path().string(), current_editing_folder.second};

                                current_editing_folder_is_favorite = !current_editing_folder_is_favorite;
                                // VortexMaker::PublishContentBrowserCustomFolder(current_editing_folder.first, VortexMaker::ImU32ToHex(current_editing_folder.second), current_editing_folder_is_favorite);
                            }

                            ImGui::EndPopup();
                        }

                        ImGui::PopStyleVar();
                        ImGui::PopStyleColor(2);

                        for (int column = 0; column < 4; column++)
                        {
                            ImGui::TableSetColumnIndex(column);

                            if (column == 0)
                            {
                                ImVec2 pos = ImGui::GetCursorScreenPos();
                                ImU32 col;

                                DrawFolderIcon(pos, ImVec2(12, 12), HexToImU32(GetContentBrowserFolderColor(path.string())));
                            }
                            else if (column == 1)
                            {
                                ImGui::Text(filenameString.c_str());
                            }
                            else if (column == 2)
                            {
                                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "Folder");
                            }
                            else if (column == 3)
                            {
                                ImGui::Text("---");
                            }
                            else if (column == 4)
                            {
                                ImGui::Text("---");
                            }
                        }
                    }
                }

                for (auto &fileEntry : files)
                {
                    const auto &path = fileEntry.path();
                    std::string filenameString = path.filename().string();

                    bool selected = false;

                    if (std::find(m_Selected.begin(), m_Selected.end(), path) != m_Selected.end())
                    {
                        selected = true;
                    }

                    if (areStringsSimilar(filenameString, ProjectSearch, threshold) || isOnlySpacesOrEmpty(ProjectSearch))
                    {
                        size_t fileSize = std::filesystem::file_size(path);
                        std::string fileSizeString = formatFileSize(fileSize);
                        ImGui::PushID(filenameString.c_str());

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        FileTypes fileType = detect_file(path.string());

                        switch (fileType)
                        {
                        case FileTypes::File_PICTURE:
                        {
                            for (int column = 0; column < 4; column++)
                            {
                                ImGui::TableSetColumnIndex(column);

                                if (column == 0)
                                {
                                    ImGui::Image(Application::GetCurrentRenderedWindow()->get_texture(Application::CookPath("ressources/imgs/icons/files/icon_picture_file.png")), ImVec2(15, 15));
                                }
                                else if (column == 1)
                                {
                                    ImGui::Text(filenameString.c_str());
                                }
                                else if (column == 2)
                                {
                                    ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "Folder");
                                }
                                else if (column == 3)
                                {
                                    ImGui::Text("---");
                                }
                                else if (column == 4)
                                {
                                    ImGui::Text("---");
                                }
                            }
                        }
                        case FileTypes::File_GIT:
                        {
                            //
                            break;
                        }
                        case FileTypes::File_H:
                        {
                            //
                            break;
                        }
                        case FileTypes::File_C:
                        {
                            //
                            break;
                        }
                        case FileTypes::File_HPP:
                        {
                            //
                            break;
                        }
                        case FileTypes::File_CPP:
                        {
                            //                            
                            break;
                        }
                        case FileTypes::File_INI:
                        {
                            //
                            break;
                        }
                        default:
                        {
                            //
                            break;
                        }

                            ImGui::PopStyleVar(2);
                            ImGui::PopStyleColor(3);
                        }
                        float oldsize = ImGui::GetFont()->Scale;

                        if (ImGui::BeginPopupContextItem("ItemContextPopup"))
                        {
                            ImGui::GetFont()->Scale *= 0.9;
                            ImGui::PushFont(ImGui::GetFont());

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                            ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
                            ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                            ImGui::PushStyleColor(ImGuiCol_Text, grayColor);

                            ImGui::Text("Main");

                            ImGui::PopStyleColor();

                            ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                            ImGui::Separator();
                            ImGui::PopStyleColor();

                            ImGui::GetFont()->Scale = oldsize;
                            ImGui::PopFont();
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                            if (ImGui::MenuItem("Open", "Ctrl + O"))
                            {
                                ChangeDirectory(path);
                                ImGui::CloseCurrentPopup();
                            }
                            if (ImGui::MenuItem("Copy folder", "Ctrl + C"))
                            {
                                m_Selected.clear();
                                ImGui::CloseCurrentPopup();
                            }

                            if (m_Selected.size() > 0)
                            {
                                std::string label = "Copy in addition (" + m_Selected.size() + ' copies)';
                                if (ImGui::MenuItem(label.c_str(), "Ctrl + C"))
                                {
                                    m_Selected.clear();
                                    ImGui::CloseCurrentPopup();
                                }
                            }

                            if (ImGui::MenuItem("Cut folder", "Ctrl + X"))
                            {
                                ChangeDirectory(path);
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::GetFont()->Scale *= 0.9;
                            ImGui::PushFont(ImGui::GetFont());

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

                            ImGui::PushStyleColor(ImGuiCol_Text, grayColor);

                            ImGui::Text("Main");

                            ImGui::PopStyleColor();

                            ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                            ImGui::Separator();
                            ImGui::PopStyleColor();

                            ImGui::GetFont()->Scale = oldsize;
                            ImGui::PopFont();
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                            if (ImGui::MenuItem("Change color"))
                            {
                                //
                            }
                            if (ImGui::MenuItem("Mark as favorite"))
                            {
                                //
                            }

                            ImGui::EndPopup();
                        }

                        ImGui::PopID();
                        ImGui::NextColumn();
                    }
                }

                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);

                ImGui::EndTable();
            }
        }
    }

    void ContentBrowserAppWindow::RenderDetailsBar()
    {
    }

    void ContentBrowserAppWindow::RefreshRender(const std::shared_ptr<ContentBrowserAppWindow> &instance)
    {
        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                           const float splitterWidth = 2.5f;
                                           const float margin = 10.0f;

                                           auto &children = instance->m_Childs;
                                           static float lastTotalWidth = 0.0f;
                                           
                                           ImVec2 availableSize = ImGui::GetContentRegionAvail();
                                           float totalAvailableSize = availableSize.x - (children.size() - 1) * splitterWidth - 40.0f;

                                           float usedSize = 0.0f;
                                           int childrenWithoutDefaultSize = 0;

                                           if (totalAvailableSize != lastTotalWidth && lastTotalWidth > 0.0f)
                                           {
                                               float scale = totalAvailableSize / lastTotalWidth;

                                               for (auto &child : children)
                                               {
                                                   child.m_Size *= scale;
                                               }

                                               lastTotalWidth = totalAvailableSize;
                                           }

                                           if (lastTotalWidth == 0.0f)
                                           {
                                               float totalSizeAssigned = 0.0f;

                                               for (auto &child : children)
                                               {
                                                if(child.m_Disabled)
                                                {
                                                    continue;
                                                }
                                                
                                                   if (!child.m_Initialized || totalAvailableSize != lastTotalWidth)
                                                   {
                                                       if (child.m_DefaultSize > 0.0f)
                                                       {
                                                           child.m_Size = child.m_DefaultSize;
                                                       }
                                                       else
                                                       {
                                                           child.m_Size = totalAvailableSize / children.size();
                                                       }
                                                       child.m_Initialized = true;
                                                   }
                                                   totalSizeAssigned += child.m_Size;
                                               }

                                               if (totalSizeAssigned < totalAvailableSize)
                                               {
                                                   float remainingSize = totalAvailableSize - totalSizeAssigned;
                                                   for (auto &child : children)
                                                   {
                                                       child.m_Size += remainingSize / children.size();
                                                   }
                                               }

                                               lastTotalWidth = totalAvailableSize;
                                           }

                                           for (size_t i = 0; i < children.size(); ++i)
                                           {
                                               auto &child = children[i];
                                               
                                               ImGui::PushStyleColor(ImGuiCol_ChildBg, child.m_BackgroundColor);
                                               ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

                                               std::string childname = child.m_Name + "##cbchildnh" + instance->m_AppWindow->m_Name;
                                               ImGui::BeginChild(childname.c_str(), ImVec2(child.m_Size, availableSize.y), true);

                                               child.m_Child();

                                               ImGui::EndChild();
                                               ImGui::PopStyleColor(2);

                                               if (i + 1 < children.size())
                                               {
                                                   auto &next_child = children[i + 1];
                                                   ImGui::SameLine();

                                                   std::string lab = "##cbsplitter" + child.m_Name + instance->m_AppWindow->m_Name;

                                                   ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                                                   ImGui::Button(lab.c_str(), ImVec2(splitterWidth, -1));
                                                   ImGui::PopStyleColor();

                                                   if (ImGui::IsItemHovered())
                                                   {
                                                       ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                                                   }

                                                   if (ImGui::IsItemActive())
                                                   {
                                                       float delta = ImGui::GetIO().MouseDelta.x;

                                                       if (child.m_Size + delta < child.m_MinSize + margin)
                                                       {
                                                           delta = child.m_MinSize + margin - child.m_Size;
                                                       }
                                                       if (next_child.m_Size - delta < next_child.m_MinSize + margin)
                                                       {
                                                           delta = next_child.m_Size - next_child.m_MinSize - margin;
                                                       }

                                                       if (child.m_Size + delta >= child.m_MinSize + margin && next_child.m_Size - delta >= next_child.m_MinSize + margin)
                                                       {
                                                           child.m_Size += delta;
                                                           next_child.m_Size -= delta;
                                                           lastTotalWidth = totalAvailableSize;
                                                       }
                                                   }

                                                   ImGui::SameLine();
                                               }
                                           } });
    }

}