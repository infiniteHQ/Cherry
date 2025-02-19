#include "wrappers.hpp"

namespace Cherry
{
    namespace GUI
    {
        void PushStyleVar(ImGuiStyleVar idx, float val)
        {
            ImGui::PushStyleVar(idx, val);
        }

        void PushStyleVar(ImGuiCol idx, const ImVec2 &val)
        {
            ImGui::PushStyleVar(idx, val);
        }

        void PopStyleVar(int count)
        {
            ImGui::PopStyleVar(count);
        }

        void PushStyleColor(ImGuiCol idx, const ImVec4 &col)
        {
            ImGui::PushStyleColor(idx, col);
        }

        void PushStyleColor(ImGuiCol idx, ImU32 col)
        {
            ImGui::PushStyleColor(idx, col);
        }

        void PopStyleColor(int count)
        {
            ImGui::PopStyleColor(count);
        }

        bool Button(const char *label, const ImVec2 &size)
        {
            return ImGui::Button(label, size);
        }

        bool ImageButtonWithTextWithIcon(ImTextureID texIdFirst, ImTextureID texIdSecond, const char *label, const ImVec2 &imageSizeFirst, const ImVec2 &imageSizeSecond, const ImVec2 &uv0, const ImVec2 &uv1, int frame_padding, const ImVec4 &bg_col, const ImVec4 &tint_col)
        {
            return ImGui::ImageButtonWithTextWithIcon(texIdFirst, texIdSecond, label, imageSizeFirst, imageSizeSecond, uv0, uv1, frame_padding, bg_col, tint_col);
        }

        bool ImageButtonWithText(ImTextureID texId, const char *label, const ImVec2 &imageSize, const ImVec2 &uv0, const ImVec2 &uv1, int frame_padding, const ImVec4 &bg_col, const ImVec4 &tint_col)
        {
            return ImGui::ImageButtonWithText(texId, label, imageSize, uv0, uv1, frame_padding, bg_col, tint_col);
        }

        bool RightImageButtonWithText(ImTextureID texId, const char *label, const ImVec2 &imageSize, const ImVec2 &uv0, const ImVec2 &uv1, int frame_padding, const ImVec4 &bg_col, const ImVec4 &tint_col)
        {
            return ImGui::RightImageButtonWithText(texId, label, imageSize, uv0, uv1, frame_padding, bg_col, tint_col);
        }

        // Legacy menu items
        bool MenuItem(const char *label, const char *shortcut, bool selected, bool enabled)
        {
            return ImGui::MenuItem(label, shortcut, selected, enabled);
        }

        bool MenuItem(const char *label, const char *shortcut, bool *p_selected, bool enabled)
        {
            return ImGui::MenuItem(label, shortcut, p_selected, enabled);
        }

        bool MenuItem(const char *label, const char *shortcut, ImTextureID texture, bool selected, bool enabled)
        {
            return ImGui::MenuItem(label, shortcut, texture, selected, enabled);
        }

        // Legacy popups
        bool BeginPopup(const char *str_id, ImGuiWindowFlags flags)
        {
            return ImGui::BeginPopup(str_id, flags);
        }

        bool BeginPopupModal(const char *name, bool *p_open, ImGuiWindowFlags flags)
        {
            return ImGui::BeginPopupModal(name, p_open, flags);
        }

        void EndPopup()
        {
            ImGui::EndPopup();
        }

        void OpenPopup(const char *str_id, ImGuiPopupFlags popup_flags)
        {
            ImGui::OpenPopup(str_id, popup_flags);
        }

        void OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags)
        {
            ImGui::OpenPopup(id, popup_flags);
        }

        void OpenPopupOnItemClick(const char *str_id, ImGuiPopupFlags popup_flags)
        {
            ImGui::OpenPopupOnItemClick(str_id, popup_flags);
        }

        void CloseCurrentPopup()
        {
            ImGui::CloseCurrentPopup();
        }

        bool BeginPopupContextItem(const char *str_id, ImGuiPopupFlags popup_flags)
        {
            return ImGui::BeginPopupContextItem(str_id, popup_flags);
        }

        bool BeginPopupContextWindow(const char *str_id, ImGuiPopupFlags popup_flags)
        {
            return ImGui::BeginPopupContextWindow(str_id, popup_flags);
        }

        bool BeginPopupContextVoid(const char *str_id, ImGuiPopupFlags popup_flags)
        {
            return ImGui::BeginPopupContextVoid(str_id, popup_flags);
        }

        // Legacy mouse detection (consider using the Cherry inputs API)
        bool IsMouseDown(ImGuiMouseButton button)
        {
            return ImGui::IsMouseDown(button);
        }

        bool IsMouseClicked(ImGuiMouseButton button, bool repeat)
        {
            return ImGui::IsMouseClicked(button, repeat);
        }

        bool IsMouseReleased(ImGuiMouseButton button)
        {
            return ImGui::IsMouseReleased(button);
        }

        bool IsMouseDoubleClicked(ImGuiMouseButton button)
        {
            return ImGui::IsMouseDoubleClicked(button);
        }

        ImFont *GetFont()
        {
            return ImGui::GetFont();
        }

        float GetFontSize()
        {
            return ImGui::GetFontSize();
        }

        ImVec2 GetFontTexUvWhitePixel()
        {
            return ImGui::GetFontTexUvWhitePixel();
        }

        ImU32 GetColorU32(ImGuiCol idx, float alpha_mul)
        {
            return ImGui::GetColorU32(idx, alpha_mul);
        }

        ImU32 GetColorU32(const ImVec4 &col)
        {
            return ImGui::GetColorU32(col);
        }

        ImU32 GetColorU32(ImU32 col)
        {
            return ImGui::GetColorU32(col);
        }

        const ImVec4 &GetStyleColorVec4(ImGuiCol idx)
        {
            return ImGui::GetStyleColorVec4(idx);
        }

        void PushFont(ImFont *font)
        {
            ImGui::PushFont(font);
        }

        void PopFont()
        {
            return ImGui::PopFont();
        }

        // Widgets: Text
        void TextUnformatted(const char *text, const char *text_end)
        {
            ImGui::TextUnformatted(text, text_end);
        }

        void Text(const char *fmt, ...)
        {
            va_list args;
            va_start(args, fmt);

            int size = vsnprintf(nullptr, 0, fmt, args) + 1;
            va_end(args);

            if (size <= 0)
                return;

            std::vector<char> buffer(size);

            va_start(args, fmt);
            vsnprintf(buffer.data(), size, fmt, args);
            va_end(args);

            ImGui::TextUnformatted(buffer.data());
        }
        void TextColored(const ImVec4 &col, const char *fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            int size = vsnprintf(nullptr, 0, fmt, args) + 1;
            va_end(args);

            if (size <= 0)
                return;

            std::vector<char> buffer(size);
            va_start(args, fmt);
            vsnprintf(buffer.data(), size, fmt, args);
            va_end(args);

            ImGui::TextColored(col, "%s", buffer.data());
        }

        void TextDisabled(const char *fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            int size = vsnprintf(nullptr, 0, fmt, args) + 1;
            va_end(args);

            if (size <= 0)
                return;

            std::vector<char> buffer(size);
            va_start(args, fmt);
            vsnprintf(buffer.data(), size, fmt, args);
            va_end(args);

            ImGui::TextDisabled("%s", buffer.data());
        }

        void TextWrapped(const char *fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            int size = vsnprintf(nullptr, 0, fmt, args) + 1;
            va_end(args);

            if (size <= 0)
                return;

            std::vector<char> buffer(size);
            va_start(args, fmt);
            vsnprintf(buffer.data(), size, fmt, args);
            va_end(args);

            ImGui::TextWrapped("%s", buffer.data());
        }

        void BulletText(const char *fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            int size = vsnprintf(nullptr, 0, fmt, args) + 1;
            va_end(args);

            if (size <= 0)
                return;

            std::vector<char> buffer(size);
            va_start(args, fmt);
            vsnprintf(buffer.data(), size, fmt, args);
            va_end(args);

            ImGui::BulletText("%s", buffer.data());
        }

        void LabelText(const char *label, const char *fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            int size = vsnprintf(nullptr, 0, fmt, args) + 1;
            va_end(args);

            if (size <= 0)
                return;

            std::vector<char> buffer(size);
            va_start(args, fmt);
            vsnprintf(buffer.data(), size, fmt, args);
            va_end(args);

            ImGui::LabelText(label, "%s", buffer.data());
        }

        void TextV(const char *fmt, va_list args)
        {
            ImGui::TextV(fmt, args);
        }

        void TextColoredV(const ImVec4 &col, const char *fmt, va_list args)
        {
            ImGui::TextColoredV(col, fmt, args);
        }

        void TextDisabledV(const char *fmt, va_list args)
        {
            ImGui::TextDisabledV(fmt, args);
        }

        void TextWrappedV(const char *fmt, va_list args)
        {
            ImGui::TextWrappedV(fmt, args);
        }

        void LabelTextV(const char *label, const char *fmt, va_list args)
        {
            ImGui::LabelTextV(label, fmt, args);
        }

        void BulletTextV(const char *fmt, va_list args)
        {
            ImGui::BulletTextV(fmt, args);
        }

    }
}