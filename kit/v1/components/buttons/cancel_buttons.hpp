#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CANCEL_BUTTONS
#define UIKIT_V1_CANCEL_BUTTONS

namespace UIKit
{
    inline bool CustomButtonSimple(
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

    inline bool CustomButtonLowProfile(
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
