#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_H4_TITLES
#define UIKIT_V1_H4_TITLES

namespace UIKit
{
    inline void TitleFour(
        const std::string &label)
    {
        float oldsize = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= 1.50;
        ImGui::PushFont(ImGui::GetFont());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

        ImGui::Text(label.c_str());

        ImGui::PopStyleVar();

        ImGui::GetFont()->Scale = oldsize;
        ImGui::PopFont();
    }

    inline void TitleFourColored(
        const std::string &label,
        const std::string &hex_text_color = "#B1FF31FF")
    {
        float oldsize = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= 1.50;
        ImGui::PushFont(ImGui::GetFont());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

        ImGui::TextColored(HexToRGBA(hex_text_color), label.c_str());

        ImGui::PopStyleVar();

        ImGui::GetFont()->Scale = oldsize;
        ImGui::PopFont();
    }

}

#endif // UIKIT_V1_H4_TITLES
