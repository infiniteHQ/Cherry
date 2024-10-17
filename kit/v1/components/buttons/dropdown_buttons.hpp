#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CUSTOM_DROPDOWN_BUTTONS
#define UIKIT_V1_CUSTOM_DROPDOWN_BUTTONS

namespace Cherry
{
    class CustomDrowpdownButtonSimple : public Component
    {
    public:
        CustomDrowpdownButtonSimple(
            const std::string &id,
            const std::string &label = "Button",
            const std::string &dropdown_logo = "/usr/local/include/Vortex/imgs/vortex.png",
            const std::string &hex_bg_idle = "#242424FF",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
              m_Label(label),
              m_DropdownLogo(dropdown_logo),
              m_LastClickTime("never"),
              m_HexBgIdle(hex_bg_idle),
              m_HexBorderIdle(hex_border_idle),
              m_HexBgHovered(hex_bg_hovered),
              m_HexBorderHovered(hex_border_hovered),
              m_HexBgClicked(hex_bg_clicked),
              m_HexBorderClicked(hex_border_clicked),
              m_IsPressed(false)
        {
            SetID(id);
        }


        static std::shared_ptr<CustomDrowpdownButtonSimple> Create(const std::string &id, const std::string &label)
        {
            auto instance = std::shared_ptr<CustomDrowpdownButtonSimple>(new CustomDrowpdownButtonSimple(id, label));
            return instance;
        }

        bool Render(const std::string &duplication_name, const ImVec2 &size = ImVec2(0, 0))
        {
            ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_DropdownLogo);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = m_Label + "####" + duplication_name;

            if (ImGui::RightImageButtonWithText(texture, Label.c_str(), size))
            {
                m_IsPressed = true;
                m_IsMenuActivated = true;
                UpdateLastClickTime();
            }
            else
            {
                m_IsPressed = false;
            }

            if (m_IsMenuActivated)
            {
                if (ImGui::BeginPopupContextItem("TabContextMenu"))
                {

                    ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
                    ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                    ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
                    ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

                    ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);
                    ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);
                    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);

                    if (ImGui::MenuItem("Close window", "Close this current window"))
                    {
                    }

                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor(2);
                    ImGui::EndPopup();
                }
            }

            if (m_IsMenuActivated)
            {
                ImGui::OpenPopup("TabContextMenu");
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                m_IsMenuActivated = false;
            }

            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();

            return m_IsPressed;
        }

        void SetBackgroundColor() {}

        void SetDropDownImage(const std::string &path)
        {
            m_DropdownLogo = path;
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "isButtonPressed")
            {
                return m_IsPressed ? "true" : "false";
            }
            else if (data_type == "lastButtonClick")
            {
                return m_LastClickTime;
            }
            return "Unknown data type";
        }

    private:
        std::string m_Label;
        std::string m_LastClickTime;
        std::string m_DropdownLogo;
        std::string m_HexBgIdle;
        std::string m_HexBorderIdle;
        std::string m_HexBgHovered;
        std::string m_HexBorderHovered;
        std::string m_HexBgClicked;
        std::string m_HexBorderClicked;
        bool m_IsPressed;
        bool m_IsMenuActivated = false;

        void UpdateLastClickTime()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            m_LastClickTime = std::ctime(&now_c);
            m_LastClickTime.erase(m_LastClickTime.length() - 1);
        }
    };

    class CustomDrowpdownImageButtonSimple : public Component
    {
    public:
        CustomDrowpdownImageButtonSimple(
            const std::string &id,
            const std::string &label = "Button",
            const std::string &imgpath = ".png",
            const std::string &hex_bg_idle = "#242424FF",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
              m_Label(label),
              m_ImagePath(imgpath),
              m_LastClickTime("never"),
              m_HexBgIdle(hex_bg_idle),
              m_HexBorderIdle(hex_border_idle),
              m_HexBgHovered(hex_bg_hovered),
              m_HexBorderHovered(hex_border_hovered),
              m_HexBgClicked(hex_bg_clicked),
              m_HexBorderClicked(hex_border_clicked),
              m_IsPressed(false)
        {
            SetID(id);
        }

        bool Render(const std::string &duplication_name = "first", const ImVec2 &size = ImVec2(0, 0))
        {
            if (m_Scale != 0.0f)
            {
                m_OldScale = ImGui::GetFont()->Scale;
                ImGui::GetFont()->Scale *= m_Scale;
                ImGui::PushFont(ImGui::GetFont());
            }

            ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_ImagePath);
            ImTextureID icon = Application::Get().GetCurrentRenderedWindow()->get_texture(m_IconPath);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(m_InternalMarginX, m_InternalMarginY));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = m_Label + "####" + duplication_name;

            ImVec2 final_size = size;

            if (m_SizeX != 0 && m_SizeY != 0)
            {
                final_size.x = m_SizeX;
                final_size.y = m_SizeY;
            }

            if (ImGui::ImageButtonWithTextWithIcon(texture, icon, Label.c_str(), final_size, final_size))
            {
                m_IsPressed = true;
                UpdateLastClickTime();
            }
            else
            {
                m_IsPressed = false;
            }

            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();

            if (m_Scale != 0.0f)
            {
                ImGui::GetFont()->Scale = m_OldScale;
                ImGui::PopFont();
            }

            return m_IsPressed;
        }

        void SetScale(const float &new_scale)
        {
            m_Scale = new_scale;
        }

        void SetLogoSize(const int &size_x, const int &size_y)
        {
            m_SizeX = size_x;
            m_SizeY = size_y;
        }

        void SetLabel(const std::string &new_label)
        {
            m_Label = new_label;
        }

        void SetImagePath(const std::string &new_path)
        {
            m_ImagePath = new_path;
        }

        void SetIconPath(const std::string &new_path)
        {
            m_IconPath = new_path;
        }

        void SetDropDownImage(const std::string &path)
        {
            m_IconPath = path;
        }

        void SetBorderColorIdle(const std::string &hex_color)
        {
            m_HexBorderIdle = hex_color;
        }

        void SetBackgroundColorIdle(const std::string &hex_color)
        {
            m_HexBgIdle = hex_color;
        }

        void SetBackgroundColorHovered(const std::string &hex_color)
        {
            m_HexBgHovered = hex_color;
        }

        void SetBackgroundColorClicked(const std::string &hex_color)
        {
            m_HexBgClicked = hex_color;
        }

        void SetInternalMarginX(const float &new_margin)
        {
            m_InternalMarginX = new_margin;
        }

        void SetInternalMarginY(const float &new_margin)
        {
            m_InternalMarginY = new_margin;
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "isButtonPressed")
            {
                return m_IsPressed ? "true" : "false";
            }
            else if (data_type == "lastButtonClick")
            {
                return m_LastClickTime;
            }
            return "Unknown data type";
        }

    private:
        std::string m_Label;
        std::string m_ImagePath;
        std::string m_LastClickTime;
        std::string m_HexBgIdle;
        std::string m_HexBorderIdle;
        std::string m_HexBgHovered;
        std::string m_HexBorderHovered;
        std::string m_HexBgClicked;
        std::string m_HexBorderClicked;
        std::string m_IconPath;


        bool m_IsPressed;

        float m_Scale = 0.0f;

        int m_SizeX = 0.0f;
        int m_SizeY = 0.0f;

        int m_InternalMarginX = 10;
        int m_InternalMarginY = 6;

        float m_OldScale;

        void
        UpdateLastClickTime()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            m_LastClickTime = std::ctime(&now_c);
            m_LastClickTime.erase(m_LastClickTime.length() - 1);
        }
    };

    class CustomDrowpdownImageOnlyButtonSimple : public Component
    {
    public:
        CustomDrowpdownImageOnlyButtonSimple(
            const std::string &id,
            const std::string &label = "Button",
            const std::string &imgpath = ".png",
            const std::string &hex_bg_idle = "#242424FF",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
              m_Label(label),
              m_ImagePath(imgpath),
              m_LastClickTime("never"),
              m_HexBgIdle(hex_bg_idle),
              m_HexBorderIdle(hex_border_idle),
              m_HexBgHovered(hex_bg_hovered),
              m_HexBorderHovered(hex_border_hovered),
              m_HexBgClicked(hex_bg_clicked),
              m_HexBorderClicked(hex_border_clicked),
              m_IsPressed(false)
        {
            SetID(id);
        }

        bool Render(const std::string &duplication_name = "first", const ImVec2 &size = ImVec2(0, 0))
        {
            if (m_Scale != 0.0f)
            {
                m_OldScale = ImGui::GetFont()->Scale;
                ImGui::GetFont()->Scale *= m_Scale;
                ImGui::PushFont(ImGui::GetFont());
            }

            ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_ImagePath);
            ImTextureID icon = Application::Get().GetCurrentRenderedWindow()->get_texture(m_IconPath);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(m_InternalMarginX, m_InternalMarginY));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = "####" + duplication_name;

            ImVec2 final_size = size;

            if (m_SizeX != 0 && m_SizeY != 0)
            {
                final_size.x = m_SizeX;
                final_size.y = m_SizeY;
            }

            if (ImGui::ImageButtonWithTextWithIcon(texture, icon, Label.c_str(), final_size, final_size))
            {
                m_IsPressed = true;
                UpdateLastClickTime();
            }
            else
            {
                m_IsPressed = false;
            }

            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();

            if (m_Scale != 0.0f)
            {
                ImGui::GetFont()->Scale = m_OldScale;
                ImGui::PopFont();
            }

            return m_IsPressed;
        }

        void SetScale(const float &new_scale)
        {
            m_Scale = new_scale;
        }

        void SetLogoSize(const int &size_x, const int &size_y)
        {
            m_SizeX = size_x;
            m_SizeY = size_y;
        }

        void SetLabel(const std::string &new_label)
        {
            m_Label = new_label;
        }

        void SetImagePath(const std::string &new_path)
        {
            m_ImagePath = new_path;
        }

        void SetIconPath(const std::string &new_path)
        {
            m_IconPath = new_path;
        }

        void SetDropDownImage(const std::string &path)
        {
            m_IconPath = path;
        }

        void SetBorderColorIdle(const std::string &hex_color)
        {
            m_HexBorderIdle = hex_color;
        }

        void SetBackgroundColorIdle(const std::string &hex_color)
        {
            m_HexBgIdle = hex_color;
        }

        void SetBackgroundColorHovered(const std::string &hex_color)
        {
            m_HexBgHovered = hex_color;
        }

        void SetBackgroundColorClicked(const std::string &hex_color)
        {
            m_HexBgClicked = hex_color;
        }

        void SetInternalMarginX(const float &new_margin)
        {
            m_InternalMarginX = new_margin;
        }

        void SetInternalMarginY(const float &new_margin)
        {
            m_InternalMarginY = new_margin;
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "isButtonPressed")
            {
                return m_IsPressed ? "true" : "false";
            }
            else if (data_type == "lastButtonClick")
            {
                return m_LastClickTime;
            }
            return "Unknown data type";
        }

    private:
        std::string m_Label;
        std::string m_ImagePath;
        std::string m_LastClickTime;
        std::string m_HexBgIdle;
        std::string m_HexBorderIdle;
        std::string m_HexBgHovered;
        std::string m_HexBorderHovered;
        std::string m_HexBgClicked;
        std::string m_HexBorderClicked;
        std::string m_IconPath;


        bool m_IsPressed;

        float m_Scale = 0.0f;

        int m_SizeX = 0.0f;
        int m_SizeY = 0.0f;

        int m_InternalMarginX = 10;
        int m_InternalMarginY = 6;

        float m_OldScale;

        void
        UpdateLastClickTime()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            m_LastClickTime = std::ctime(&now_c);
            m_LastClickTime.erase(m_LastClickTime.length() - 1);
        }
    };

    /*class CustomButtonLowProfile : public Component
    {
    public:
        CustomButtonLowProfile(
            const std::string &id,
            const std::string &label = "Button",
            const std::string &hex_bg_idle = "#242424FF",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#242424FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#242424FF",
            const std::string &hex_border_clicked = "#454545B2",
            const ImVec2 &size = ImVec2(0, 18))
            : Component(id),
              m_Label(label),
              m_LastClickTime("never"),
              m_HexBgIdle(hex_bg_idle),
              m_HexBorderIdle(hex_border_idle),
              m_HexBgHovered(hex_bg_hovered),
              m_HexBorderHovered(hex_border_hovered),
              m_HexBgClicked(hex_bg_clicked),
              m_HexBorderClicked(hex_border_clicked),
              m_IsPressed(false),
              m_Size(size)
        {
            SetID(id);
        }

        inline static bool RenderInl()
        {
            // Render without component...
            return false;
        }

        bool Render(const std::string &duplication_name, const ImVec2 &size = ImVec2(0, 0))
        {
            ImVec2 text_size = ImGui::CalcTextSize(m_Label.c_str());
            float vertical_padding = (m_Size.y - text_size.y) * 0.5f;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = m_Label + "####" + duplication_name;

            if (ImGui::Button(Label.c_str(), size))
            {
                m_IsPressed = true;
                UpdateLastClickTime();
            }
            else
            {
                m_IsPressed = false;
            }

            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();

            return m_IsPressed;
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "isButtonPressed")
            {
                return m_IsPressed ? "true" : "false";
            }
            else if (data_type == "lastButtonClick")
            {
                return m_LastClickTime;
            }
            return "Unknown data type";
        }

    private:
        std::string m_Label;
        std::string m_LastClickTime;
        std::string m_HexBgIdle;
        std::string m_HexBorderIdle;
        std::string m_HexBgHovered;
        std::string m_HexBorderHovered;
        std::string m_HexBgClicked;
        std::string m_HexBorderClicked;
        ImVec2 m_Size;
        bool m_IsPressed;

        void UpdateLastClickTime()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            m_LastClickTime = std::ctime(&now_c);
            m_LastClickTime.erase(m_LastClickTime.length() - 1);
        }
    };
*/
}

#endif // UIKIT_V1_CUSTOM_DROPDOWN_BUTTONS
