#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_IMAGE_BUTTONS
#define UIKIT_V1_IMAGE_BUTTONS

namespace UIKit
{

    class ImageButtonSimple : public Component
    {
    public:
        ImageButtonSimple(
            const std::string &id,
            const std::string &imgpath = ".png",
            const std::string &hex_bg_idle = "#242424FF",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
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
            m_ID = id;
        }

        bool Render(const std::string &duplication_name, const ImVec2 &size = ImVec2(0, 0))
        {
         ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_ImagePath);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label =  "####" + duplication_name;

            if (ImGui::ImageButtonWithText(texture, Label.c_str(), size))
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


    class ImageButtonLowProfile : public Component
    {
    public:
        ImageButtonLowProfile(
            const std::string &id,
            const std::string &label = "Button",
            const std::string &imgpath = "/usr/local/include/Vortex/imgs/vortex.png",
            const std::string &hex_bg_idle = "#242424FF",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2",
            const ImVec2 &size = ImVec2(0, 18))
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
              m_IsPressed(false),
              m_Size(size)
        {
            m_ID = id;
        }

        bool Render(const std::string &duplication_name, const ImVec2 &size = ImVec2(0, 0))
        {
            ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_ImagePath);

            ImVec2 text_size = ImGui::CalcTextSize(m_Label.c_str());

            float vertical_padding = (m_Size.y - text_size.y) * 0.5f;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding));


            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = m_Label + "####" + duplication_name;

            if (ImGui::ImageButtonWithText(texture, Label.c_str(), size))
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
}

#endif // UIKIT_V1_IMAGE_BUTTONS