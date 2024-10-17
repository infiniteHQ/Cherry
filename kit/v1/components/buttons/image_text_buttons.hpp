#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_IMAGE_TEXT_BUTTONS
#define UIKIT_V1_IMAGE_TEXT_BUTTONS

namespace Cherry
{
    class ImageTextButtonSimple : public Component
    {
    public:
        ImageTextButtonSimple(
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

        static std::shared_ptr<ImageTextButtonSimple> Create(const std::string &id, const std::string &label)
        {
            auto instance = std::shared_ptr<ImageTextButtonSimple>(new ImageTextButtonSimple(id, label));
            return instance;
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
            
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(m_InternalMarginX, m_InternalMarginY));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = m_Label + "####" + duplication_name;

            ImVec2 final_size = size;

            if(m_SizeX != 0 && m_SizeY != 0)
            {
                final_size.x = m_SizeX; 
                final_size.y = m_SizeY;
            }

            if (ImGui::ImageButtonWithText(texture, Label.c_str(), final_size))
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

        void SetLogoSize(const int& size_x, const int& size_y)
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

        void SetInternalMarginX(const float& new_margin)
        {
            m_InternalMarginX = new_margin;
        }

        void SetInternalMarginY(const float& new_margin)
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

    class ReverseImageTextButtonSimple : public Component
    {
    public:
        ReverseImageTextButtonSimple(
            const std::string &id,
            const std::string &label = "Button",
            const std::string &imgpath = "/usr/local/include/Vortex/imgs/vortex.png",
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
        
        static std::shared_ptr<ReverseImageTextButtonSimple> Create(const std::string &id, const std::string &label)
        {
            auto instance = std::shared_ptr<ReverseImageTextButtonSimple>(new ReverseImageTextButtonSimple(id, label));
            return instance;
        }

        bool Render(const std::string &duplication_name, const ImVec2 &size = ImVec2(0, 0))
        {
         ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_ImagePath);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = m_Label + "####" + duplication_name;

            if (ImGui::RightImageButtonWithText(texture, Label.c_str(), size))
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
        std::string m_ImagePath;
        std::string m_LastClickTime;
        std::string m_HexBgIdle;
        std::string m_HexBorderIdle;
        std::string m_HexBgHovered;
        std::string m_HexBorderHovered;
        std::string m_HexBgClicked;
        std::string m_HexBorderClicked;
        bool m_IsPressed;

        void UpdateLastClickTime()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            m_LastClickTime = std::ctime(&now_c);
            m_LastClickTime.erase(m_LastClickTime.length() - 1);
        }
    };

}

#endif // UIKIT_V1_IMAGE_TEXT_BUTTONS