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

    }
}

namespace CherryGUI = Cherry::GUI;