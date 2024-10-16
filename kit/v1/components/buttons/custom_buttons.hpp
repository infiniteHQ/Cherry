#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CUSTOM_BUTTONS
#define UIKIT_V1_CUSTOM_BUTTONS

namespace Cherry
{
    class CustomButtonSimple : public Component
    {
    public:
        CustomButtonSimple(const std::string &id, const std::string &label)
            : Component(id)
        {
            SetID(id);

            // String Props
            SetProp("label", label);
            SetProp("last_time_clicked", "never");
            SetProp("bg", "#242424FF");
            SetProp("bg_hovered", "#343434FF");
            SetProp("bg_clicked", "#444444FF");
            SetProp("border", "#454545B2");
            SetProp("border_hovered", "#454545B2");
            SetProp("border_clicked", "#454545B2");
            SetProp("is_pressed", "false");
        }

        static std::shared_ptr<CustomButtonSimple> Create(const std::string &id, const std::string &label)
        {
            auto instance = std::shared_ptr<CustomButtonSimple>(new CustomButtonSimple(id, label));
            return instance;
        }

        void Refresh()
        {
            if (NeedRefreshing())
            {
                m_IsPressed = GetProp("is_pressed") == "true" ? true : false;
                m_HexBgIdle = GetProp("bg");
                m_HexBgHovered = GetProp("bg_hovered");
                m_HexBgClicked = GetProp("bg_clicked");
                m_HexBorderIdle = GetProp("border");
                m_HexBorderHovered = GetProp("border_hovered");
                m_HexBorderClicked = GetProp("border_clicked");
                m_Label = GetProp("label");
            }
        }

        bool Render(const std::string &duplication_name = "__blank", const ImVec2 &size = ImVec2(0, 0))
        {
            Refresh();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

            ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(m_HexBorderIdle));
            ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(m_HexBgIdle));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(m_HexBgHovered));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(m_HexBgClicked));

            std::string Label = m_Label + "####" + duplication_name;

            if (ImGui::Button(Label.c_str(), size))
            {
                SetProp("is_pressed", "true");
                UpdateLastClickTime();
            }
            else
            {
                SetProp("is_pressed", "false");
            }

            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();

            Refresh();
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
        std::string m_HexBgIdle;
        std::string m_HexBgHovered;
        std::string m_HexBgClicked;
        std::string m_HexBorderIdle;
        std::string m_HexBorderHovered;
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

#endif // UIKIT_V1_CUSTOM_BUTTONS
