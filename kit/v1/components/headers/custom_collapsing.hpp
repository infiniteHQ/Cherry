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

    static bool MyCollapsingHeaderCustom(const char *label, ImTextureID my_texture, float width, std::function<void()> customContent)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
        bool *p_open = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID(label), false);

        ImGuiStyle &style = ImGui::GetStyle();
        float texture_size = ImGui::GetFontSize();
        float padding = style.ItemInnerSpacing.x;

        ImGui::BeginGroup();
        bool pressed = ImGui::ImageSizeButtonWithText(my_texture, width, label, ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));

        ImVec2 button_size = ImGui::GetItemRectSize();
        ImVec2 button_pos = ImGui::GetItemRectMin();
        ImVec2 content_pos;
        content_pos.x = button_pos.x + button_size.x - texture_size - 100;
        content_pos.y = button_pos.y;

        ImGui::SetCursorScreenPos(content_pos);
        ImVec2 child_size = ImVec2(100, button_size.y);
        ImGui::BeginChild("##customContent", child_size, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        if (customContent)
        {
            customContent();
        }

        ImGui::EndChild();

        ImGui::EndGroup();

        ImGui::PopStyleVar();

        return pressed ? (*p_open ^= 1) : *p_open;
    }

    static bool MyCollapsingHeader(const char *label, ImTextureID my_texture, float width)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
        bool *p_open = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID(label), false);

        ImGuiStyle &style = ImGui::GetStyle();

        float texture_size = ImGui::GetFontSize(); 
        float padding = style.ItemInnerSpacing.x;

        ImGui::BeginGroup();
        if (ImGui::ImageSizeButtonWithText(my_texture, width, label, ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
            *p_open ^= 1;

        ImVec2 arrow_pos = ImVec2(ImGui::GetItemRectMax().x - style.FramePadding.x - ImGui::GetFontSize(), ImGui::GetItemRectMin().y + style.FramePadding.y);
        ImGui::RenderArrow(ImGui::GetWindowDrawList(), arrow_pos, ImGui::GetColorU32(ImGuiCol_Text), *p_open ? ImGuiDir_Down : ImGuiDir_Right);
        ImGui::EndGroup();

        ImGui::PopStyleVar();
        return *p_open;
    }

    inline bool CustomCollapsingHeaderLogo(
        const std::string &label,
        const std::string &logo_path,
        const std::function<void()> &content,
        const float& sizex = 200.0f)
    {
        bool openned = false;

         ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(logo_path);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
        float oldsize = ImGui::GetFont()->Scale;

        ImGui::GetFont()->Scale *= 0.85;
        ImGui::PushFont(ImGui::GetFont());

        if (MyCollapsingHeader(label.c_str(), texture, sizex))
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

        ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(logo_path);

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
