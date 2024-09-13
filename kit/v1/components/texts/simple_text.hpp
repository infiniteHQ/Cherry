#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_SIMPLE_TEXTS
#define UIKIT_V1_SIMPLE_TEXTS

namespace UIKit
{
inline void SimpleTextCentred(const std::string &text)
{
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(text.c_str()).x;

    float textPosX = (windowWidth - textWidth) * 0.5f;

    ImGui::SetCursorPosX(textPosX);

    ImGui::TextWrapped(text.c_str());
}


inline void SimpleTextLeft(const std::string &text)
{
    ImGui::TextWrapped(text.c_str());
}


inline void SimpleTextRight(const std::string &text)
{
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX(windowWidth - textWidth);

    ImGui::TextWrapped(text.c_str());
}


inline void SimpleTextFull(const std::string &text)
{
    float windowWidth = ImGui::GetWindowSize().x;
    ImGui::TextWrapped(text.c_str());
}


}

#endif // UIKIT_V1_SIMPLE_TEXTS
