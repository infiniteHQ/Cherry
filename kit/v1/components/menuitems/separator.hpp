#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_MENUITEM_SEPARATOR
#define UIKIT_V1_MENUITEM_SEPARATOR

namespace Cherry
{
    inline void MenuItemTextSeparator(const std::string &text, const float &text_scale = 0.8f, const float &ctx_text_scale = Application::GetCurrentRenderedWindow()->m_Specifications.FontGlobalScale)
    {
        ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
        ImGui::GetFont()->Scale *= text_scale;
        ImGui::PushFont(ImGui::GetFont());

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

        ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
        ImGui::Text(text.c_str());
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
        ImGui::Separator();
        ImGui::PopStyleColor();

        ImGui::GetFont()->Scale = ctx_text_scale;
        ImGui::PopFont();
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
    }

}

#endif // UIKIT_V1_MENUITEM_SEPARATOR
