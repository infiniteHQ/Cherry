#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_H1_TITLES
#define UIKIT_V1_H1_TITLES

namespace UIKit
{
    inline void TitleOne(
        const std::string &label)
    {
        float oldsize = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= 2.10;
        ImGui::PushFont(ImGui::GetFont());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

        ImGui::Text(label.c_str());

        ImGui::PopStyleVar();

        ImGui::GetFont()->Scale = oldsize;
        ImGui::PopFont();
    }

    inline void TitleOneColored(
        const std::string &label,
        const std::string &hex_text_color = "#B1FF31FF")
    {
        float oldsize = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= 2.10;
        ImGui::PushFont(ImGui::GetFont());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

        ImGui::TextColored(HexToRGBA(hex_text_color), label.c_str());

        ImGui::PopStyleVar();

        ImGui::GetFont()->Scale = oldsize;
        ImGui::PopFont();
    }

}

#endif // UIKIT_V1_H1_TITLES
