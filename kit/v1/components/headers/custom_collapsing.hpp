#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_HEADERS_CUSTOMCOLLAPSING
#define UIKIT_V1_HEADERS_CUSTOMCOLLAPSING

namespace UIKit
{

    inline bool CustomCollapsingHeaderSimple(
        const std::string &label,
        const std::function<void()> &content)
    {
        bool openned = false;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
        if (ImGui::CollapsingHeader(label.c_str()))
        {
            openned = true;

            if (content)
            {
                content();
            }
        }
        ImGui::PopStyleVar();

        return openned;
    }
    static float EstimateContentWidth(std::function<void()> customContent)
    {
        ImVec2 initialCursorPos = ImGui::GetCursorPos();
        ImGui::PushClipRect(ImVec2(0, 0), ImVec2(0, 0), false);
        ImGuiID id = ImGui::GetID("TempID");

        ImGui::PushID(id);
        ImGui::SetCursorPosX(0.0f);
        if (customContent)
        {
            customContent();
        }
        float width = ImGui::GetCursorPosX();
        ImGui::PopID();

        ImGui::PopClipRect();
        ImGui::SetCursorPos(initialCursorPos);

        return width;
    }

    static bool MyCollapsingHeaderCustom(const char *label, ImTextureID my_texture, float width, std::function<void()> customContent)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
        bool *p_open = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID(label), false);

        ImGuiStyle &style = ImGui::GetStyle();

        float texture_size = ImGui::GetFontSize();
        float padding = style.ItemInnerSpacing.x;

        float contentWidth = EstimateContentWidth(customContent);

        std::cout << contentWidth << std::endl;

        ImGui::BeginGroup();

        if (ImGui::ImageSizeButtonWithText(my_texture, width, label, ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
            *p_open ^= 1;

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + width + padding - contentWidth);

        if (customContent)
        {
            customContent();
        }

        ImGui::EndGroup();

        ImGui::PopStyleVar();
        return *p_open;
    }

    static bool MyCollapsingHeader(const char *label, ImTextureID my_texture, float width)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
        bool *p_open = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID(label), false);

        ImGuiStyle &style = ImGui::GetStyle();

        // Calculate the size of the texture and padding
        float texture_size = ImGui::GetFontSize(); // Adjust the size of the texture as needed
        float padding = style.ItemInnerSpacing.x;

        // Create a horizontal layout
        ImGui::BeginGroup();
        if (ImGui::ImageSizeButtonWithText(my_texture, width, label, ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
            *p_open ^= 1;

        // Adjust arrow position considering the texture
        ImVec2 arrow_pos = ImVec2(ImGui::GetItemRectMax().x - style.FramePadding.x - ImGui::GetFontSize(), ImGui::GetItemRectMin().y + style.FramePadding.y);
        ImGui::RenderArrow(ImGui::GetWindowDrawList(), arrow_pos, ImGui::GetColorU32(ImGuiCol_Text), *p_open ? ImGuiDir_Down : ImGuiDir_Right);
        ImGui::EndGroup();

        ImGui::PopStyleVar();
        return *p_open;
    }

    inline bool CustomCollapsingHeaderLogo(
        const std::string &label,
        const std::string &logo_path,
        const std::function<void()> &content)
    {
        bool openned = false;

        static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get(logo_path)->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
        float oldsize = ImGui::GetFont()->Scale;

        ImGui::GetFont()->Scale *= 0.85;
        ImGui::PushFont(ImGui::GetFont());

        if (MyCollapsingHeader(label.c_str(), texture, 500.0f))
        {
            openned = true;
            if (content)
            {
                content();
            }
        }

        ImGui::GetFont()->Scale = oldsize;
        ImGui::PopFont();
        ImGui::PopStyleVar();

        return openned;
    }

    inline bool CustomCollapsingHeaderLogoCallback(
        const std::string &label,
        const std::string &logo_path,
        const std::function<void()> &inside,
        const std::function<void()> &content)
    {
        bool openned = false;

        static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get(logo_path)->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
        float oldsize = ImGui::GetFont()->Scale;

        ImGui::GetFont()->Scale *= 0.85;
        ImGui::PushFont(ImGui::GetFont());

        if (MyCollapsingHeaderCustom(label.c_str(), texture, 500.0f, inside))
        {
            openned = true;
            if (content)
            {
                content();
            }
        }

        ImGui::GetFont()->Scale = oldsize;
        ImGui::PopFont();
        ImGui::PopStyleVar();

        return openned;
    }
}

#endif // UIKIT_V1_HEADERS_CUSTOMCOLLAPSING
