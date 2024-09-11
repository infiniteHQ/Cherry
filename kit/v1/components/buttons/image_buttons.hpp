#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_IMAGE_BUTTONS
#define UIKIT_V1_IMAGE_BUTTONS

namespace UIKit
{
    inline bool ImageButtonSimple(
        const std::string &label = "Button",
        const std::string &imgpath = "undefined",
        const std::string &hex_bg_idle = "#CC1111ff",
        const std::string &hex_border_idle = "#CC4444ff",
        const std::string &hex_bg_hovered = "#CC4444ff",
        const std::string &hex_border_hovered = "#CC1111ff",
        const std::string &hex_bg_clicked = "#CC1111ff",
        const std::string &hex_border_clicked = "#CC1111ff",
        const ImVec2 &size = ImVec2(0, 0))
    {
        bool pressed = false;
        static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        ImVec2 text_size = ImGui::CalcTextSize(label.c_str());

        float vertical_padding = (size.y - text_size.y) * 0.5f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding)); // 10 pixels sur X, padding dynamique sur Y

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
        const std::string &hex_bg_idle = "#CC1111ff",
        const std::string &hex_border_idle = "#CC4444ff",
        const std::string &hex_bg_hovered = "#CC4444ff",
        const std::string &hex_border_hovered = "#CC1111ff",
        const std::string &hex_bg_clicked = "#CC1111ff",
        const std::string &hex_border_clicked = "#CC1111ff",
        const ImVec2 &size = ImVec2(0, 20))
    {
        bool pressed = false;
        static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        ImVec2 text_size = ImGui::CalcTextSize(label.c_str());

        float vertical_padding = (size.y - text_size.y) * 0.5f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, vertical_padding)); // 10 pixels sur X, padding dynamique sur Y

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