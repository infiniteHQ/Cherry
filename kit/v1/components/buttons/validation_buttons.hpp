#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_VALIDATION_BUTTONS
#define UIKIT_V1_VALIDATION_BUTTONS

namespace UIKit
{
    inline bool ValidationButtonLowProfile(
        const std::string &label = "Cancel",
        const std::string &hex_bg_idle = "#0074ffce",
        const std::string &hex_border_idle = "#51bcffce",
        const std::string &hex_bg_hovered = "#2d8dffce",
        const std::string &hex_border_hovered = "#7cccff",
        const std::string &hex_bg_clicked = "#67acfdce",
        const std::string &hex_border_clicked = "#caebff",
        const ImVec2 &size = ImVec2(0, 20))
    {
        bool pressed = false;

        ImVec2 text_size = ImGui::CalcTextSize(label.c_str());

        float vertical_padding = (size.y - text_size.y) * 0.5f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding));
ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

        ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(hex_border_idle));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(hex_bg_idle));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(hex_bg_hovered));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(hex_bg_clicked));

        if (ImGui::Button(label.c_str(), size))
        {
            pressed = true;
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        return pressed;
    }
}

#endif // UIKIT_V1_VALIDATION_BUTTONS