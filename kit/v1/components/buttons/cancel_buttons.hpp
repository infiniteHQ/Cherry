#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CANCEL_BUTTONS
#define UIKIT_V1_CANCEL_BUTTONS

namespace UIKit
{

class NCustomButtonSimple : public Component {
public:
    NCustomButtonSimple(const std::string& id) 
        : Component(id), m_Label("Button"), m_LastClickTime("Never"), m_IsPressed(false){
        m_ID = id;
    }

    bool Render(const std::string& duplication_name, const ImVec2& size = ImVec2(0, 0)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

        ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA("#3e3e3eff"));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA("#3e3e3eff"));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA("#5e5e5eff"));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA("#2e2e2eff"));

        std::string Label = m_Label + "####" + duplication_name;

        if (ImGui::Button(Label.c_str(), size)) {
            m_IsPressed = true;
            UpdateLastClickTime();
        } else {
            m_IsPressed = false;
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        return m_IsPressed;
    }

    std::string GetData(const std::string& data_type) override {
        if (data_type == "isButtonPressed") {
            return m_IsPressed ? "true" : "false";
        } else if (data_type == "lastButtonClick") {
            return m_LastClickTime;
        }
        return "Unknown data type";
    }

private:
    std::string m_Label;
    bool m_IsPressed;
    std::string m_LastClickTime;

    void UpdateLastClickTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        m_LastClickTime = std::ctime(&now_c);
        m_LastClickTime.erase(m_LastClickTime.length() - 1);
    }
};


    inline bool CancelButtonSimple(
        const std::string &label,
        const std::string &hex_bg_idle = "#3e3e3eff",
        const std::string &hex_border_idle = "#3e3e3eff",
        const std::string &hex_bg_hovered = "#5e5e5eff",
        const std::string &hex_border_hovered = "#5e5e5eff",
        const std::string &hex_bg_clicked = "#2e2e2eff",
        const std::string &hex_border_clicked = "#2e2e2eff",
        const ImVec2 &size = ImVec2(0, 0))
    {
        bool pressed = false;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

        ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(hex_border_idle));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(hex_bg_idle));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(hex_bg_hovered));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(hex_bg_clicked));

        if (ImGui::Button(label.c_str(), size))
        {
            pressed = true;
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        return pressed;
    }

    inline bool CancelButtonLowProfile(
        const std::string &label,
        const std::string &hex_bg_idle = "#3e3e3eff",
        const std::string &hex_border_idle = "#3e3e3eff",
        const std::string &hex_bg_hovered = "#5e5e5eff",
        const std::string &hex_border_hovered = "#5e5e5eff",
        const std::string &hex_bg_clicked = "#2e2e2eff",
        const std::string &hex_border_clicked = "#2e2e2eff",
        const ImVec2 &size = ImVec2(0, 0))
    {
        bool pressed = false;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 6));

        ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(hex_border_idle));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(hex_bg_idle));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(hex_bg_hovered));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(hex_bg_clicked));

        if (ImGui::Button(label.c_str(), size))
        {
            pressed = true;
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        return pressed;
    }
}

#endif // UIKIT_V1_CANCEL_BUTTONS
