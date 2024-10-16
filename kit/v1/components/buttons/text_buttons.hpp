#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_TEXT_BUTTONS
#define UIKIT_V1_TEXT_BUTTONS

namespace Cherry
{
    inline bool TextButtonUnderline(
        const std::string &label,
        bool enabled = true,
        const std::string &hex_text_idle = "#999999ff",
        const std::string &hex_text_hovered = "#BBBBBBff",
        const std::string &hex_text_clicked = "#FFFFFFff",
        const std::string &hex_underline_idle = "#494949ff",
        const std::string &hex_underline_hovered = "#BBBBBBff",
        const std::string &hex_underline_clicked = "#FFFFFFff",
        const ImVec2 &size = ImVec2(0, 0),
        float y_margin = 2.0f)
    {
        bool pressed = false;
        int i = 0;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, y_margin));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, y_margin));

        ImVec4 text_idle = HexToRGBA(hex_text_idle.c_str());
        ImVec4 text_hovered = HexToRGBA(hex_text_hovered.c_str());
        ImVec4 text_clicked = HexToRGBA(hex_text_clicked.c_str());
        ImVec4 underline_idle = HexToRGBA(hex_underline_idle.c_str());
        ImVec4 underline_hovered = HexToRGBA(hex_underline_hovered.c_str());
        ImVec4 underline_clicked = HexToRGBA(hex_underline_clicked.c_str());

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, text_idle);

        if (ImGui::Button(label.c_str(), size))
        {
            pressed = true;
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, text_hovered);
            i++;
        }
        if (ImGui::IsItemActive())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, text_clicked);
            i++;
        }

        if (ImGui::IsItemHovered())
        {
            ImVec2 pos = ImGui::GetItemRectMin();
            ImVec2 rect_size = ImGui::GetItemRectSize();
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            ImVec4 underline_color = ImGui::IsItemActive() ? underline_clicked : underline_hovered;
            draw_list->AddLine(ImVec2(pos.x, pos.y + rect_size.y), ImVec2(pos.x + rect_size.x, pos.y + rect_size.y), ImColor(underline_color));
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);
        ImGui::PopStyleColor(i);

        return pressed;
    }

}

#endif // UIKIT_V1_TEXT_BUTTONS
