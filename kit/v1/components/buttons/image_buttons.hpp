#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_IMAGE_BUTTONS
#define UIKIT_V1_IMAGE_BUTTONS

namespace UIKit
{
    inline bool ImageButtonSimple(
        const std::string &label = "Button",
        const std::string &imgpath = "undefined",
        const std::string &hex_bg_idle = "#242424FF",
        const std::string &hex_border_idle = "#454545B2",
        const std::string &hex_bg_hovered = "#242424FF",
        const std::string &hex_border_hovered = "#454545B2",
        const std::string &hex_bg_clicked = "#242424FF",
        const std::string &hex_border_clicked = "#454545B2",
        const ImVec2 &size = ImVec2(0, 22))
    {
        bool pressed = false;
        static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        ImVec2 text_size = ImGui::CalcTextSize(label.c_str());

        float vertical_padding = (size.y - text_size.y) * 0.5f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding)); 

        ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(hex_border_idle));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(hex_bg_idle));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(hex_bg_hovered));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(hex_bg_clicked));

        if (ImGui::ImageButtonWithText(texture, label.c_str(), size))
        {
            pressed = true;
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        return pressed;
    }

    inline bool ImageButtonLowProfile(
        const std::string &label = "Button",
        const std::string &imgpath = "undefined",
        const std::string &hex_bg_idle = "#242424FF",
        const std::string &hex_border_idle = "#454545B2",
        const std::string &hex_bg_hovered = "#242424FF",
        const std::string &hex_border_hovered = "#454545B2",
        const std::string &hex_bg_clicked = "#242424FF",
        const std::string &hex_border_clicked = "#454545B2",
        const ImVec2 &size = ImVec2(0, 18))
    {
        bool pressed = false;
        static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        ImVec2 text_size = ImGui::CalcTextSize(label.c_str());

        float vertical_padding = (size.y - text_size.y) * 0.5f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding)); 

        ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(hex_border_idle));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(hex_bg_idle));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(hex_bg_hovered));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(hex_bg_clicked));

        if (ImGui::ImageButtonWithText(texture, label.c_str(), size))
        {
            pressed = true;
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        return pressed;
    }
}

#endif // UIKIT_V1_IMAGE_BUTTONS