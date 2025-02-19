#pragma once
#include "../base.hpp"

// This ImGui wrapper helps Cherry framework to easely add custom behaviors,
// control and ui callbacks and other cool stuff for the end-user API.

namespace Cherry
{
    namespace GUI
    {
        // Legacy ImGui style variables
        void PushStyleVar(ImGuiStyleVar idx, float val);
        void PushStyleVar(ImGuiCol idx, const ImVec2 &val);
        void PopStyleVar(int count = 1);

        // Legacy ImGui style colors
        void PushStyleColor(ImGuiCol idx, const ImVec4 &col);
        void PushStyleColor(ImGuiCol idx, ImU32 col);
        void PopStyleColor(int count = 1);

        // Legacy ImGui buttons
        bool Button(const char *label, const ImVec2 &size = ImVec2(0, 0));
        bool ImageButtonWithTextWithIcon(ImTextureID texIdFirst, ImTextureID texIdSecond, const char *label, const ImVec2 &imageSizeFirst = ImVec2(0, 0), const ImVec2 &imageSizeSecond = ImVec2(0, 0), const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4 &bg_col = ImVec4(0, 0, 0, 0), const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
        bool ImageButtonWithText(ImTextureID texId, const char *label, const ImVec2 &imageSize = ImVec2(0, 0), const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4 &bg_col = ImVec4(0, 0, 0, 0), const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
        bool RightImageButtonWithText(ImTextureID texId, const char *label, const ImVec2 &imageSize = ImVec2(0, 0), const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4 &bg_col = ImVec4(0, 0, 0, 0), const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));

        // Legacy menu items
        bool MenuItem(const char *label, const char *shortcut = NULL, bool selected = false, bool enabled = true);       // return true when activated.
        bool MenuItem(const char *label, const char *shortcut, bool *p_selected, bool enabled = true);                   // return true when activated + toggle (*p_selected) if p_selected != NULL
        bool MenuItem(const char *label, const char *shortcut, ImTextureID texture, bool selected, bool enabled = true); // return true when activated + toggle (*p_selected) if p_selected != NULL

        // Legacy popups
        bool BeginPopup(const char *str_id, ImGuiWindowFlags flags = 0);                         // return true if the popup is open, and you can start outputting to it.
        bool BeginPopupModal(const char *name, bool *p_open = NULL, ImGuiWindowFlags flags = 0); // return true if the modal is open, and you can start outputting to it.
        void EndPopup();                                                                         // did mouse button double-clicked? Same as GetMouseClickedCount() == 2. (note that a double-click will also report IsMouseClicked() == true)
        void OpenPopup(const char *str_id, ImGuiPopupFlags popup_flags = 0);                     // call to mark popup as open (don't call every frame!).
        void OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0);                             // id overload to facilitate calling from nested stacks
        void OpenPopupOnItemClick(const char *str_id = NULL, ImGuiPopupFlags popup_flags = 1);   // helper to open popup when clicked on last item. Default to ImGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors)
        void CloseCurrentPopup();
        bool BeginPopupContextItem(const char *str_id = NULL, ImGuiPopupFlags popup_flags = 1);   // open+begin popup when clicked on last item. Use str_id==NULL to associate the popup to previous item. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
        bool BeginPopupContextWindow(const char *str_id = NULL, ImGuiPopupFlags popup_flags = 1); // open+begin popup when clicked on current window.
        bool BeginPopupContextVoid(const char *str_id = NULL, ImGuiPopupFlags popup_flags = 1);   // open+begin popup when clicked in void (where there are no windows).

        // Legacy mouse detection (consider using the Cherry inputs API)
        bool IsMouseDown(ImGuiMouseButton button);                         // is mouse button held?
        bool IsMouseClicked(ImGuiMouseButton button, bool repeat = false); // did mouse button clicked? (went from !Down to Down). Same as GetMouseClickedCount() == 1.
        bool IsMouseReleased(ImGuiMouseButton button);                     // did mouse button released? (went from Down to !Down)
        bool IsMouseDoubleClicked(ImGuiMouseButton button);                // manually close the popup we have begin-ed into.

        ImFont *GetFont();                                       // get current font
        float GetFontSize();                                     // get current font size (= height in pixels) of current font with current scale applied
        ImVec2 GetFontTexUvWhitePixel();                         // get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API
        ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f); // retrieve given style color with style alpha applied and optional extra alpha multiplier, packed as a 32-bit value suitable for ImDrawList
        ImU32 GetColorU32(const ImVec4 &col);                    // retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
        ImU32 GetColorU32(ImU32 col);                            // retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
        const ImVec4 &GetStyleColorVec4(ImGuiCol idx);           // retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.

        void PushFont(ImFont *font); // use NULL as a shortcut to push default font
        void PopFont();

        // Widgets: Text
        void TextUnformatted(const char *text, const char *text_end = NULL); // raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.
        void Text(const char *fmt, ...) IM_FMTARGS(1);                       // formatted text
        void TextV(const char *fmt, va_list args) IM_FMTLIST(1);
        void TextColored(const ImVec4 &col, const char *fmt, ...) IM_FMTARGS(2); // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
        void TextColoredV(const ImVec4 &col, const char *fmt, va_list args) IM_FMTLIST(2);
        void TextDisabled(const char *fmt, ...) IM_FMTARGS(1); // shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
        void TextDisabledV(const char *fmt, va_list args) IM_FMTLIST(1);
        void TextWrapped(const char *fmt, ...) IM_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
        void TextWrappedV(const char *fmt, va_list args) IM_FMTLIST(1);
        void LabelText(const char *label, const char *fmt, ...) IM_FMTARGS(2); // display text+label aligned the same way as value+label widgets
        void LabelTextV(const char *label, const char *fmt, va_list args) IM_FMTLIST(2);
        void BulletText(const char *fmt, ...) IM_FMTARGS(1); // shortcut for Bullet()+Text()
        void BulletTextV(const char *fmt, va_list args) IM_FMTLIST(1);

    }
}

namespace CherryGUI = Cherry::GUI;