#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CUSTOM_BUTTONS
#define UIKIT_V1_CUSTOM_BUTTONS

namespace UIKit
{
    inline bool CustomButtonSimple(
        const std::string &label,
        bool enabled = true,
        const std::string &hex_bg_idle = "#242424FF",
        const std::string &hex_border_idle = "#454545B2",
        const std::string &hex_bg_hovered = "#242424FF",
        const std::string &hex_border_hovered = "#454545B2",
        const std::string &hex_bg_clicked = "#242424FF",
        const std::string &hex_border_clicked = "#454545B2",
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
        const std::string &hex_bg_idle = "#242424FF",
        const std::string &hex_border_idle = "#454545B2",
        const std::string &hex_bg_hovered = "#242424FF",
        const std::string &hex_border_hovered = "#454545B2",
        const std::string &hex_bg_clicked = "#242424FF",
        const std::string &hex_border_clicked = "#454545B2",
        const ImVec2 &size = ImVec2(0, 15))
    {
        bool pressed = false;

        ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
        float vertical_padding = (size.y - text_size.y) * 0.5f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding));


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

#endif // UIKIT_V1_CUSTOM_BUTTONS
