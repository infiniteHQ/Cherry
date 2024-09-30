#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CUSTOM_DROPDOWN_BUTTONS
#define UIKIT_V1_CUSTOM_DROPDOWN_BUTTONS

namespace UIKit
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
            m_ID = id;
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

            if(m_IsMenuActivated)
            {
                ImGui::OpenPopup("TabContextMenu");
            }

            if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                m_IsMenuActivated = false;
            }

            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();

            return m_IsPressed;
        }

        void SetBackgroundColor() {}

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
            m_ID = id;
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
