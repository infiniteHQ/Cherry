#pragma once
#include "../base.hpp"

// This ImGui wrapper helps Cherry framework to easely add custom behaviors,
// control and ui callbacks and other cool stuff for the end-user API.

namespace Cherry {
  namespace GUI {
    // Legacy ImGui style variables
    void PushStyleVar(ImGuiStyleVar idx, float val);
    void PushStyleVar(ImGuiCol idx, const ImVec2 &val);
    void PopStyleVar(int count = 1);

    // Legacy ImGui style colors
    void PushStyleColor(ImGuiCol idx, const ImVec4 &col);
    void PushStyleColor(ImGuiCol idx, ImU32 col);
    void PopStyleColor(int count = 1);

         bool          BeginMenuBar();                                                     // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window).
     bool          BeginMenuBar(float offsetY);                                                     // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window).
     bool          BeginBottomBar();                                                     // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window).

     void          EndMenuBar(); 
     void          EndBottomBar();                                                      // only call EndMenuBar() if BeginMenuBar() returns true!
     bool          BeginMainMenuBar();                                                 // create and append to a full screen menu-bar.
     void          EndMainMenuBar();     

    // Legacy ImGui ui elements
    bool Button(const char *label, const ImVec2 &size = ImVec2(0, 0));  // button
    bool ImageSizeButtonWithText(
        ImTextureID texId,
        float button_width,
        const char *label,
        const ImVec2 &imageSize = ImVec2(0, 0),
        const ImVec2 &uv0 = ImVec2(0, 0),
        const ImVec2 &uv1 = ImVec2(1, 1),
        int frame_padding = -1,
        const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
    bool SmallButton(const char *label);  // button with FramePadding=(0,0) to easily embed within text
    bool InvisibleButton(
        const char *str_id,
        const ImVec2 &size,
        ImGuiButtonFlags flags = 0);  // flexible button behavior without the visuals, frequently useful to build custom
                                      // behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)
    bool ArrowButton(const char *str_id, ImGuiDir dir);  // square button with an arrow shape
    void Image(
        ImTextureID user_texture_id,
        const ImVec2 &size,
        const ImVec2 &uv0 = ImVec2(0, 0),
        const ImVec2 &uv1 = ImVec2(1, 1),
        const ImVec4 &tint_col = ImVec4(1, 1, 1, 1),
        const ImVec4 &border_col = ImVec4(0, 0, 0, 0));
    bool ImageButton(
        ImTextureID user_texture_id,
        const ImVec2 &size,
        const ImVec2 &uv0 = ImVec2(0, 0),
        const ImVec2 &uv1 = ImVec2(1, 1),
        int frame_padding = -1,
        const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4 &tint_col =
            ImVec4(1, 1, 1, 1));  // <0 frame_padding uses default frame padding settings. 0 for no padding
    bool Checkbox(const char *label, bool *v);
    bool CheckboxFlags(const char *label, int *flags, int flags_value);
    bool CheckboxFlags(const char *label, unsigned int *flags, unsigned int flags_value);
    bool RadioButton(
        const char *label,
        bool active);  // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
    bool
    RadioButton(const char *label, int *v, int v_button);  // shortcut to handle the above pattern when value is an integer
    void ProgressBar(float fraction, const ImVec2 &size_arg = ImVec2(-FLT_MIN, 0), const char *overlay = NULL);
    void Bullet();  // draw a small circle + keep the cursor on the same line. advance cursor x position by
                    // GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses
    bool TextButtonWithImageWithIcon(
        ImTextureID texIdFirst,
        ImTextureID texIdSecond,
        const char *label,
        const ImVec2 &imageSizeFirst = ImVec2(0, 0),
        const ImVec2 &imageSizeSecond = ImVec2(0, 0),
        const ImVec2 &uv0 = ImVec2(0, 0),
        const ImVec2 &uv1 = ImVec2(1, 1),
        int frame_padding = -1,
        const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
    bool ImageButtonWithTextWithIcon(
        ImTextureID texIdFirst,
        ImTextureID texIdSecond,
        const char *label,
        const ImVec2 &imageSizeFirst = ImVec2(0, 0),
        const ImVec2 &imageSizeSecond = ImVec2(0, 0),
        const ImVec2 &uv0 = ImVec2(0, 0),
        const ImVec2 &uv1 = ImVec2(1, 1),
        int frame_padding = -1,
        const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
    bool ImageButtonWithText(
        ImTextureID texId,
        const char *label,
        const ImVec2 &imageSize = ImVec2(0, 0),
        const ImVec2 &uv0 = ImVec2(0, 0),
        const ImVec2 &uv1 = ImVec2(1, 1),
        int frame_padding = -1,
        const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
    bool RightImageButtonWithText(
        ImTextureID texId,
        const char *label,
        const ImVec2 &imageSize = ImVec2(0, 0),
        const ImVec2 &uv0 = ImVec2(0, 0),
        const ImVec2 &uv1 = ImVec2(1, 1),
        int frame_padding = -1,
        const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));

    bool          BeginMenu(const char* label, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
     bool          BeginMenuImage(const char* label, ImTextureID* texture, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
     void          EndMenu();      

    // Legacy menu items
    bool MenuItem(
        const char *label,
        const char *shortcut = NULL,
        bool selected = false,
        bool enabled = true);  // return true when activated.
    bool MenuItem(
        const char *label,
        const char *shortcut,
        bool *p_selected,
        bool enabled = true);  // return true when activated + toggle (*p_selected) if p_selected != NULL
    bool MenuItem(
        const char *label,
        const char *shortcut,
        ImTextureID texture,
        bool selected,
        bool enabled = true);  // return true when activated + toggle (*p_selected) if p_selected != NULL

    // Legacy popups
    bool BeginPopup(
        const char *str_id,
        ImGuiWindowFlags flags = 0);  // return true if the popup is open, and you can start outputting to it.
    bool BeginPopupModal(
        const char *name,
        bool *p_open = NULL,
        ImGuiWindowFlags flags = 0);  // return true if the modal is open, and you can start outputting to it.
    void EndPopup();  // did mouse button double-clicked? Same as GetMouseClickedCount() == 2. (note that a double-click will
                      // also report IsMouseClicked() == true)
    void OpenPopup(
        const char *str_id,
        ImGuiPopupFlags popup_flags = 0);                         // call to mark popup as open (don't call every frame!).
    void OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0);  // id overload to facilitate calling from nested stacks
    void OpenPopupOnItemClick(
        const char *str_id = NULL,
        ImGuiPopupFlags popup_flags =
            1);  // helper to open popup when clicked on last item. Default to ImGuiPopupFlags_MouseButtonRight == 1. (note:
                 // actually triggers on the mouse _released_ event to be consistent with popup behaviors)
    void CloseCurrentPopup();
    bool BeginPopupContextItem(
        const char *str_id = NULL,
        ImGuiPopupFlags popup_flags = 1);  // open+begin popup when clicked on last item. Use str_id==NULL to associate the
                                           // popup to previous item. If you want to use that on a non-interactive item such
                                           // as Text() you need to pass in an explicit ID here. read comments in .cpp!
    bool BeginPopupContextWindow(
        const char *str_id = NULL,
        ImGuiPopupFlags popup_flags = 1);  // open+begin popup when clicked on current window.
    bool BeginPopupContextVoid(
        const char *str_id = NULL,
        ImGuiPopupFlags popup_flags = 1);  // open+begin popup when clicked in void (where there are no windows).

    ImFont *GetFont();    // get current font
    float GetFontSize();  // get current font size (= height in pixels) of current font with current scale applied
    ImVec2
    GetFontTexUvWhitePixel();  // get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API
    ImU32 GetColorU32(
        ImGuiCol idx,
        float alpha_mul = 1.0f);  // retrieve given style color with style alpha applied and optional extra alpha multiplier,
                                  // packed as a 32-bit value suitable for ImDrawList
    ImU32 GetColorU32(const ImVec4 &col);  // retrieve given color with style alpha applied, packed as a 32-bit value
                                           // suitable for ImDrawList
    ImU32 GetColorU32(
        ImU32 col);  // retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
    const ImVec4 &GetStyleColorVec4(
        ImGuiCol idx);  // retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(),
                        // otherwise use GetColorU32() to get style color with style alpha baked in.

    void PushFont(ImFont *font);  // use NULL as a shortcut to push default font
    void PopFont();

    // Widgets: Text
    void TextUnformatted(
        const char *text,
        const char *text_end = NULL);  // raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't
                                       // require null terminated string if 'text_end' is specified, B) it's faster, no
                                       // memory copy is done, no buffer size limits, recommended for long chunks of text.
    void Text(const char *fmt, ...) IM_FMTARGS(1);  // formatted text
    void TextV(const char *fmt, va_list args) IM_FMTLIST(1);
    void TextColored(const ImVec4 &col, const char *fmt, ...)
        IM_FMTARGS(2);  // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
    void TextColoredV(const ImVec4 &col, const char *fmt, va_list args) IM_FMTLIST(2);
    void TextDisabled(const char *fmt, ...)
        IM_FMTARGS(1);  // shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...);
                        // PopStyleColor();
    void TextDisabledV(const char *fmt, va_list args) IM_FMTLIST(1);
    void TextWrapped(const char *fmt, ...)
        IM_FMTARGS(1);  // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work
                        // on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to
                        // set a size using SetNextWindowSize().
    void TextWrappedV(const char *fmt, va_list args) IM_FMTLIST(1);
    void LabelText(const char *label, const char *fmt, ...)
        IM_FMTARGS(2);  // display text+label aligned the same way as value+label widgets
    void LabelTextV(const char *label, const char *fmt, va_list args) IM_FMTLIST(2);
    void BulletText(const char *fmt, ...) IM_FMTARGS(1);  // shortcut for Bullet()+Text()
    void BulletTextV(const char *fmt, va_list args) IM_FMTLIST(1);

    // Font
    void SetCurrentFont(ImFont *font);

    // Items interaction
    bool IsItemHovered(ImGuiHoveredFlags flags = 0);  // is the last item hovered? (and usable, aka not blocked by a popup,
                                                      // etc.). See ImGuiHoveredFlags for more options.
    bool IsItemActive();   // is the last item active? (e.g. button being held, text field being edited. This will
                           // continuously return true while holding mouse button on an item. Items that don't interact will
                           // always return false)
    bool IsItemFocused();  // is the last item focused for keyboard/gamepad navigation?
    bool IsItemClicked(ImGuiMouseButton mouse_button = 0);  // is the last item hovered and mouse clicked on? (**)  ==
                                                            // IsMouseClicked(mouse_button) && IsItemHovered()Important. (**)
                                                            // this it NOT equivalent to the behavior of e.g. Button(). Read
                                                            // comments in function definition.
    bool IsItemVisible();  // is the last item visible? (items may be out of sight because of clipping/scrolling)
    bool IsItemEdited();   // did the last item modify its underlying value this frame? or was pressed? This is generally the
                           // same as the "bool" return value of many widgets.
    bool IsItemActivated();    // was the last item just made active (item was previously inactive).
    bool IsItemDeactivated();  // was the last item just made inactive (item was previously active). Useful for Undo/Redo
                               // patterns with widgets that requires continuous editing.
    bool IsItemDeactivatedAfterEdit();  // was the last item just made inactive and made a value change when it was active?
                                        // (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires
                                        // continuous editing. Note that you may get false positives (some widgets such as
                                        // Combo()/ListBox()/Selectable() will return true even when clicking an already
                                        // selected item).
    bool IsItemToggledOpen();           // was the last item open state toggled? set by TreeNode().
    bool IsAnyItemHovered();            // is any item hovered?
    bool IsAnyItemActive();             // is any item active?
    bool IsAnyItemFocused();            // is any item focused?
    ImVec2 GetItemRectMin();            // get upper-left bounding rectangle of the last item (screen space)
    ImVec2 GetItemRectMax();            // get lower-right bounding rectangle of the last item (screen space)
    ImVec2 GetItemRectSize();           // get size of last item
    void SetItemAllowOverlap();

    // Layout
    void Separator();
    void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
    void NewLine();
    void Spacing();
    void Dummy(const ImVec2 &size);
    void Indent(float indent_w = 0.0f);
    void Unindent(float indent_w = 0.0f);
    void BeginGroup();
    void EndGroup();
    ImVec2 GetCursorPos();
    float GetCursorPosX();
    float GetCursorPosY();
    void SetCursorPos(const ImVec2 &local_pos);
    void SetCursorPosX(float local_x);
    void SetCursorPosY(float local_y);
    ImVec2 GetCursorStartPos();
    ImVec2 GetCursorScreenPos();
    void SetCursorScreenPos(const ImVec2 &pos);
    void AlignTextToFramePadding();
    void AlignTextToFramePadding(float offsetY);
    float GetTextLineHeight();
    float GetTextLineHeightWithSpacing();
    float GetFrameHeight();
    float GetFrameHeightWithSpacing();
    void BeginHorizontal(const char *str_id, const ImVec2 &size = ImVec2(0, 0), float align = -1.0f);
    void BeginHorizontal(const void *ptr_id, const ImVec2 &size = ImVec2(0, 0), float align = -1.0f);
    void BeginHorizontal(int id, const ImVec2 &size = ImVec2(0, 0), float align = -1);
    void EndHorizontal();
    void BeginVertical(const char *str_id, const ImVec2 &size = ImVec2(0, 0), float align = -1.0f);
    void BeginVertical(const void *ptr_id, const ImVec2 &size = ImVec2(0, 0), float align = -1.0f);
    void BeginVertical(int id, const ImVec2 &size = ImVec2(0, 0), float align = -1);
    void EndVertical();
    void Spring(float weight = 1.0f, float spacing = -1.0f);
    void SuspendLayout();
    void ResumeLayout();

    // Inputs (Please consider using the Cherry input api.)
    bool IsMouseDown(ImGuiMouseButton button);
    bool IsMouseClicked(ImGuiMouseButton button, bool repeat = false);
    bool IsMouseReleased(ImGuiMouseButton button);
    bool IsMouseDoubleClicked(ImGuiMouseButton button);
    int GetMouseClickedCount(ImGuiMouseButton button);
    bool IsMouseHoveringRect(const ImVec2 &r_min, const ImVec2 &r_max, bool clip = true);
    bool IsMousePosValid(const ImVec2 *mouse_pos = NULL);
    bool IsAnyMouseDown();
    ImVec2 GetMousePos();
    ImVec2 GetMousePosOnOpeningCurrentPopup();
    bool IsMouseDragging(ImGuiMouseButton button, float lock_threshold = -1.0f);
    ImVec2 GetMouseDragDelta(ImGuiMouseButton button = 0, float lock_threshold = -1.0f);
    void ResetMouseDragDelta(ImGuiMouseButton button = 0);
    ImGuiMouseCursor GetMouseCursor();
    void SetMouseCursor(ImGuiMouseCursor cursor_type);
    void CaptureMouseFromApp(bool want_capture_mouse_value = true);

    // Text
    ImVec2 CalcTextSize(
        const char *text,
        const char *text_end = NULL,
        bool hide_text_after_double_hash = false,
        float wrap_width = -1.0f);

    // ID
    void PushID(const char *str_id);
    void PushID(const char *str_id_begin, const char *str_id_end);
    void PushID(const void *ptr_id);
    void PushID(int int_id);
    void PopID();
    ImGuiID GetID(const char *str_id);
    ImGuiID GetID(const char *str_id_begin, const char *str_id_end);
    ImGuiID GetID(const void *ptr_id);

    bool BeginCombo(const char *label, const char *preview_value, ImGuiComboFlags flags = 0);
    bool BeginCombo(const char *label, const std::function<void()> &preview, ImGuiComboFlags flags = 0);
    void EndCombo();  // only call EndCombo() if BeginCombo() returns true!
    bool Combo(
        const char *label,
        int *current_item,
        const char *const items[],
        int items_count,
        int popup_max_height_in_items = -1);
    bool Combo(
        const char *label,
        int *current_item,
        const char *items_separated_by_zeros,
        int popup_max_height_in_items =
            -1);  // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
    bool Combo(
        const char *label,
        int *current_item,
        bool (*items_getter)(void *data, int idx, const char **out_text),
        void *data,
        int items_count,
        int popup_max_height_in_items = -1);

    bool BeginChild(const char *str_id, const ImVec2 &size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    bool BeginChild(ImGuiID id, const ImVec2 &size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    void EndChild();

    // Windows Utilities
    // - 'current window' = the window we are appending into while inside a Begin()/End() block. 'next window' = next window
    // we will Begin() into.
    bool IsWindowAppearing();
    bool IsWindowCollapsed();
    bool IsWindowFocused(ImGuiFocusedFlags flags = 0);  // is current window focused? or its root/child, depending on flags.
                                                        // see flags for options.
    bool IsWindowHovered(
        ImGuiHoveredFlags flags =
            0);  // is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. NB: If
                 // you are trying to check whether your mouse should be dispatched to imgui or to your app, you should use
                 // the 'io.WantCaptureMouse' boolean for that! Please read the FAQ!
    ImDrawList *
    GetWindowDrawList();        // get draw list associated to the current window, to append your own drawing primitives
    float GetWindowDpiScale();  // get DPI scale currently associated to the current window's viewport.
    ImVec2 GetWindowPos();  // get current window position in screen space (useful if you want to do your own drawing via the
                            // DrawList API)
    ImVec2 GetWindowSize();              // get current window size
    float GetWindowWidth();              // get current window width (shortcut for GetWindowSize().x)
    float GetWindowHeight();             // get current window height (shortcut for GetWindowSize().y)
    ImGuiViewport *GetWindowViewport();  // get viewport currently associated to the current window.

    // Window manipulation
    // - Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
    void SetNextWindowPos(
        const ImVec2 &pos,
        ImGuiCond cond = 0,
        const ImVec2 &pivot = ImVec2(
            0,
            0));  // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
    void SetNextWindowSize(const ImVec2 &size, ImGuiCond cond = 0);  // set next window size. set axis to 0.0f to force an
                                                                     // auto-fit on this axis. call before Begin()
    void SetNextWindowSizeConstraints(
        const ImVec2 &size_min,
        const ImVec2 &size_max,
        ImGuiSizeCallback custom_callback = NULL,
        void *custom_callback_data =
            NULL);  // set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Sizes will be
                    // rounded down. Use callback to apply non-trivial programmatic constraints.
    void SetNextWindowContentSize(
        const ImVec2 &size);  // set next window content size (~ scrollable client area, which enforce the range of
                              // scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding.
                              // set an axis to 0.0f to leave it automatic. call before Begin()
    void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0);  // set next window collapsed state. call before Begin()
    void SetNextWindowFocus();  // set next window to be focused / top-most. call before Begin()
    void SetNextWindowBgAlpha(
        float alpha);  // set next window background color alpha. helper to easily override the Alpha component of
                       // ImGuiCol_WindowBg/ChildBg/PopupBg. you may also use ImGuiWindowFlags_NoBackground.
    void SetNextWindowViewport(ImGuiID viewport_id);  // set next window viewport
    void SetWindowPos(
        const ImVec2 &pos,
        ImGuiCond cond = 0);  // (not recommended) set current window position - call within Begin()/End(). prefer using
                              // SetNextWindowPos(), as this may incur tearing and side-effects.
    void SetWindowSize(
        const ImVec2 &size,
        ImGuiCond cond =
            0);  // (not recommended) set current window size - call within Begin()/End(). set to ImVec2(0, 0) to force an
                 // auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
    void SetWindowCollapsed(
        bool collapsed,
        ImGuiCond cond = 0);  // (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
    void
    SetWindowFocus();  // (not recommended) set current window to be focused / top-most. prefer using SetNextWindowFocus().
    void SetWindowFontScale(float scale);  // [OBSOLETE] set font scale. Adjust IO.FontGlobalScale if you want to scale all
                                           // windows. This is an old API! For correct scaling, prefer to reload font +
                                           // rebuild ImFontAtlas + call style.ScaleAllSizes().
    void SetWindowPos(const char *name, const ImVec2 &pos, ImGuiCond cond = 0);  // set named window position.
    void SetWindowSize(
        const char *name,
        const ImVec2 &size,
        ImGuiCond cond = 0);  // set named window size. set axis to 0.0f to force an auto-fit on this axis.
    void SetWindowCollapsed(const char *name, bool collapsed, ImGuiCond cond = 0);  // set named window collapsed state
    void SetWindowFocus(const char *name);  // set named window to be focused / top-most. use NULL to remove focus.

    // Content region
    // - Retrieve available space from a given point. GetContentRegionAvail() is frequently useful.
    // - Those functions are bound to be redesigned (they are confusing, incomplete and the Min/Max return values are in
    // local window coordinates which increases confusion)
    ImVec2 GetContentRegionAvail();  // == GetContentRegionMax() - GetCursorPos()
    ImVec2 GetContentRegionMax();  // current content boundaries (typically window boundaries including scrolling, or current
                                   // column boundaries), in windows coordinates
    ImVec2
    GetWindowContentRegionMin();  // content boundaries min for the full window (roughly (0,0)-Scroll), in window coordinates
    ImVec2 GetWindowContentRegionMax();  // content boundaries max for the full window (roughly (0,0)+Size-Scroll) where Size
                                         // can be override with SetNextWindowContentSize(), in window coordinates

    // Windows Scrolling
    float GetScrollX();               // get scrolling amount [0 .. GetScrollMaxX()]
    float GetScrollY();               // get scrolling amount [0 .. GetScrollMaxY()]
    void SetScrollX(float scroll_x);  // set scrolling amount [0 .. GetScrollMaxX()]
    void SetScrollY(float scroll_y);  // set scrolling amount [0 .. GetScrollMaxY()]
    float GetScrollMaxX();            // get maximum scrolling amount ~~ ContentSize.x - WindowSize.x - DecorationsSize.x
    float GetScrollMaxY();            // get maximum scrolling amount ~~ ContentSize.y - WindowSize.y - DecorationsSize.y
    void SetScrollHereX(
        float center_x_ratio = 0.5f);  // adjust scrolling amount to make current cursor position visible.
                                       // center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a
                                       // "default/current item" visible, consider using SetItemDefaultFocus() instead.
    void SetScrollHereY(
        float center_y_ratio = 0.5f);  // adjust scrolling amount to make current cursor position visible.
                                       // center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a
                                       // "default/current item" visible, consider using SetItemDefaultFocus() instead.
    void SetScrollFromPosX(
        float local_x,
        float center_x_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally
                                       // GetCursorStartPos() + offset to compute a valid position.
    void SetScrollFromPosY(
        float local_y,
        float center_y_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally
                                       // GetCursorStartPos() + offset to compute a valid position.

    // Color Utilities
    ImVec4 ColorConvertU32ToFloat4(ImU32 in);
    ImU32 ColorConvertFloat4ToU32(const ImVec4 &in);
    void ColorConvertRGBtoHSV(float r, float g, float b, float &out_h, float &out_s, float &out_v);
    void ColorConvertHSVtoRGB(float h, float s, float v, float &out_r, float &out_g, float &out_b);

    ImGuiIO &GetIO();  // access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags)
    ImGuiStyle &GetStyle();  // access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to
                             // modify style mid-frame!
    ImDrawData *
    GetDrawData();  // valid after Render() and until the next call to NewFrame(). this is what you have to render.
    void PlotLines(
        const char *label,
        const float *values,
        int values_count,
        int values_offset = 0,
        const char *overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0),
        int stride = sizeof(float));
    void PlotLines(
        const char *label,
        float (*values_getter)(void *data, int idx),
        void *data,
        int values_count,
        int values_offset = 0,
        const char *overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0));
    void PlotHistogram(
        const char *label,
        const float *values,
        int values_count,
        int values_offset = 0,
        const char *overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0),
        int stride = sizeof(float));
    void PlotHistogram(
        const char *label,
        float (*values_getter)(void *data, int idx),
        void *data,
        int values_count,
        int values_offset = 0,
        const char *overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0));
    bool Selectable(
        const char *label,
        bool selected = false,
        ImGuiSelectableFlags flags = 0,
        const ImVec2 &size =
            ImVec2(0, 0));  // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true
                            // so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify
                            // width. size.y==0.0: use label height, size.y>0.0: specify height
    bool Selectable(
        const char *label,
        bool *p_selected,
        ImGuiSelectableFlags flags = 0,
        const ImVec2 &size =
            ImVec2(0, 0));  // "bool* p_selected" point to the selection state (read-write), as a convenient helper.
    bool TreeNode(const char *label);
    bool TreeNode(const char *str_id, const char *fmt, ...)
        IM_FMTARGS(2);  // helper variation to easily decorelate the id from the displayed string. Read the FAQ about why and
                        // how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
    bool TreeNode(const void *ptr_id, const char *fmt, ...) IM_FMTARGS(2);  // "
    bool TreeNodeV(const char *str_id, const char *fmt, va_list args) IM_FMTLIST(2);
    bool TreeNodeV(const void *ptr_id, const char *fmt, va_list args) IM_FMTLIST(2);
    bool TreeNodeEx(const char *label, ImGuiTreeNodeFlags flags = 0);
    bool TreeNodeEx(const char *str_id, ImGuiTreeNodeFlags flags, const char *fmt, ...) IM_FMTARGS(3);
    bool TreeNodeEx(const void *ptr_id, ImGuiTreeNodeFlags flags, const char *fmt, ...) IM_FMTARGS(3);
    bool TreeNodeExV(const char *str_id, ImGuiTreeNodeFlags flags, const char *fmt, va_list args) IM_FMTLIST(3);
    bool TreeNodeExV(const void *ptr_id, ImGuiTreeNodeFlags flags, const char *fmt, va_list args) IM_FMTLIST(3);
    void TreePush(const char *str_id);  // ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can
                                        // call TreePush/TreePop yourself if desired.
    void TreePush(const void *ptr_id = NULL);  // "
    void TreePop();                            // ~ Unindent()+PopId()
    float GetTreeNodeToLabelSpacing();         // horizontal distance preceding label when using TreeNode*() or Bullet() ==
                                               // (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode

    bool CollapsingHeader(const char *label, ImTextureID *texture, ImGuiTreeNodeFlags flags);
    bool CollapsingHeader(
        const char *label,
        ImGuiTreeNodeFlags flags = 0);  // if returning 'true' the header is open. doesn't indent nor push on ID stack. user
                                        // doesn't have to call TreePop().
    bool CollapsingHeader(
        const char *label,
        bool *p_visible,
        ImGuiTreeNodeFlags flags = 0);  // when 'p_visible != NULL': if '*p_visible==true' display an additional small close
                                        // button on upper right of the header which will set the bool to false when clicked,
                                        // if '*p_visible==false' don't display the header.
    void SetNextItemOpen(bool is_open, ImGuiCond cond = 0);  // set next TreeNode/CollapsingHeader open state.
    void BeginTooltip();  // begin/append a tooltip window. to create full-featured tooltip (with any kind of items).
    void EndTooltip();
    void SetTooltip(const char *fmt, ...)
        IM_FMTARGS(1);  // set a text-only tooltip, typically use with ImGui::IsItemHovered(). override any previous call to
                        // SetTooltip().
    void SetTooltipV(const char *fmt, va_list args) IM_FMTLIST(1);

    bool BeginListBox(const char *label, const ImVec2 &size = ImVec2(0, 0));  // open a framed scrolling region
    void EndListBox();  // only call EndListBox() if BeginListBox() returned true!
    bool ListBox(const char *label, int *current_item, const char *const items[], int items_count, int height_in_items = -1);
    bool ListBox(
        const char *label,
        int *current_item,
        bool (*items_getter)(void *data, int idx, const char **out_text),
        void *data,
        int items_count,
        int height_in_items = -1);
    void PushItemWidth(
        float item_width);  // push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align
                            // xx pixels to the right of window (so -FLT_MIN always align width to the right side).
    void PopItemWidth();
    void SetNextItemWidth(
        float item_width);  // set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align
                            // xx pixels to the right of window (so -FLT_MIN always align width to the right side)
    float CalcItemWidth();  // width of item given pushed settings and current cursor position. NOT necessarily the width of
                            // last item unlike most 'Item' functions.
    void PushTextWrapPos(float wrap_local_pos_x = 0.0f);  // push word-wrapping position for Text*() commands. < 0.0f: no
                                                          // wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap
                                                          // at 'wrap_pos_x' position in window local space
    void PopTextWrapPos();
    bool Begin(const char *name, bool *p_open = NULL, ImGuiWindowFlags flags = 0);
    bool Begin(
        const char *name,
        ImTextureID logo,
        bool *p_open = NULL,
        ImGuiWindowFlags flags = 0,
        ImVec2 internal_padding = ImVec2(0, 0));

    void End();
    bool IsKeyDown(ImGuiKey key);                         // is key being held.
    bool IsKeyPressed(ImGuiKey key, bool repeat = true);  // was key pressed (went from !Down to Down)? if repeat=true, uses
                                                          // io.KeyRepeatDelay / KeyRepeatRate
    bool IsKeyReleased(ImGuiKey key);                     // was key released (went from Down to !Down)?
    int GetKeyPressedAmount(
        ImGuiKey key,
        float repeat_delay,
        float rate);  // uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is
                      // small enough that DeltaTime > RepeatRate
    const char *GetKeyName(ImGuiKey key);  // [DEBUG] returns English name of the key. Those names a provided for debugging
                                           // purpose and are not meant to be saved persistently not compared.
    void CaptureKeyboardFromApp(
        bool want_capture_keyboard_value =
            true);  // attention: misleading name! manually override io.WantCaptureKeyboard flag next frame (said flag is
                    // entirely left for your application to handle). e.g. force capture keyboard when your widget is being
                    // hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard_value"; after
                    // the next NewFrame() call.
    bool InputText(
        const char *label,
        char *buf,
        size_t buf_size,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void *user_data = NULL);
    bool InputTextWithImage(
        const char *label,
        ImTextureID texture,
        char *buf,
        size_t buf_size,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void *user_data = NULL);
    bool InputTextMultiline(
        const char *label,
        char *buf,
        size_t buf_size,
        const ImVec2 &size = ImVec2(0, 0),
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void *user_data = NULL);
    bool InputTextWithHint(
        const char *label,
        const char *hint,
        char *buf,
        size_t buf_size,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void *user_data = NULL);
    bool InputFloat(
        const char *label,
        float *v,
        float step = 0.0f,
        float step_fast = 0.0f,
        const char *format = "%.3f",
        ImGuiInputTextFlags flags = 0);
    bool InputColorFloat(
        const char *label,
        float *v,
        ImVec4 color = ImVec4(0, 0, 0, 0),
        float step = 0.0f,
        float step_fast = 0.0f,
        const char *format = "%.3f",
        ImGuiInputTextFlags flags = 0);
    bool InputFloat2(const char *label, float v[2], const char *format = "%.3f", ImGuiInputTextFlags flags = 0);
    bool InputFloat3(const char *label, float v[3], const char *format = "%.3f", ImGuiInputTextFlags flags = 0);
    bool InputFloat4(const char *label, float v[4], const char *format = "%.3f", ImGuiInputTextFlags flags = 0);
    bool InputInt(const char *label, int *v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
    bool InputInt2(const char *label, int v[2], ImGuiInputTextFlags flags = 0);
    bool InputInt3(const char *label, int v[3], ImGuiInputTextFlags flags = 0);
    bool InputInt4(const char *label, int v[4], ImGuiInputTextFlags flags = 0);
    bool InputDouble(
        const char *label,
        double *v,
        double step = 0.0,
        double step_fast = 0.0,
        const char *format = "%.6f",
        ImGuiInputTextFlags flags = 0);
    bool InputColorScalar(
        const char *label,
        ImGuiDataType data_type,
        ImVec4 color,
        void *p_data,
        const void *p_step = NULL,
        const void *p_step_fast = NULL,
        const char *format = NULL,
        ImGuiInputTextFlags flags = 0);
    bool InputScalar(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        const void *p_step = NULL,
        const void *p_step_fast = NULL,
        const char *format = NULL,
        ImGuiInputTextFlags flags = 0);
    bool InputScalarN(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        int components,
        const void *p_step = NULL,
        const void *p_step_fast = NULL,
        const char *format = NULL,
        ImGuiInputTextFlags flags = 0);

    void SetItemDefaultFocus();  // make last item the default focused item of a window.
    void SetKeyboardFocusHere(
        int offset = 0);  // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple
                          // component widget. Use -1 to access previous widget.
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    int GetKeyIndex(ImGuiKey key);  // map ImGuiKey_* values into legacy native key index. == io.KeyMap[key]
#else
    static inline int GetKeyIndex(ImGuiKey key) {
      IM_ASSERT(
          key >= ImGuiKey_NamedKey_BEGIN && key < ImGuiKey_NamedKey_END &&
          "ImGuiKey and native_index was merged together and native_index is disabled by IMGUI_DISABLE_OBSOLETE_KEYIO. "
          "Please switch to ImGuiKey.");
      return key;
    }
#endif
    bool DragScalar(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        float v_speed,
        const void *p_min,
        const void *p_max,
        const char *format,
        float power);
    bool DragScalarN(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        int components,
        float v_speed,
        const void *p_min,
        const void *p_max,
        const char *format,
        float power);
    bool SliderScalar(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        const void *p_min,
        const void *p_max,
        const char *format,
        float power);
    bool SliderScalarN(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        int components,
        const void *p_min,
        const void *p_max,
        const char *format,
        float power);
    bool ColorEdit3(const char *label, float col[3], ImGuiColorEditFlags flags = 0);
    bool ColorEdit4(const char *label, float col[4], ImGuiColorEditFlags flags = 0);
    bool ColorPicker3(const char *label, float col[3], ImGuiColorEditFlags flags = 0);
    bool ColorPicker4(const char *label, float col[4], ImGuiColorEditFlags flags = 0, const float *ref_col = NULL);
    bool ColorButton(
        const char *desc_id,
        const ImVec4 &col,
        ImGuiColorEditFlags flags = 0,
        ImVec2 size = ImVec2(0, 0));  // display a color square/button, hover for details, return true when pressed.
    void SetColorEditOptions(
        ImGuiColorEditFlags flags);  // initialize current options (generally on application startup) if you want to select a
                                     // default format, picker type, etc. User will be able to change many settings, unless
                                     // you pass the _NoOptions flag to your calls.
    void BeginDisabled(bool disabled = true);
    void EndDisabled();
    void RenderArrow(ImDrawList *draw_list, ImTextureID *texture, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale);
    void RenderArrow(ImDrawList *draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale = 1.0f);
    void RenderBullet(ImDrawList *draw_list, ImTextureID *texture, ImVec2 pos, ImU32 col);
    void RenderBullet(ImDrawList *draw_list, ImVec2 pos, ImU32 col);
    void RenderCheckMark(ImDrawList *draw_list, ImVec2 pos, ImU32 col, float sz);
    void RenderArrowPointingAt(ImDrawList *draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col);
    void RenderArrowDockMenu(ImDrawList *draw_list, ImVec2 p_min, float sz, ImU32 col);
    void RenderRectFilledRangeH(
        ImDrawList *draw_list,
        const ImRect &rect,
        ImU32 col,
        float x_start_norm,
        float x_end_norm,
        float rounding);
    void RenderRectFilledWithHole(ImDrawList *draw_list, ImRect outer, ImRect inner, ImU32 col, float rounding);
    ImDrawFlags CalcRoundingFlagsForRectInRect(const ImRect &r_in, const ImRect &r_outer, float threshold);
    int TableGetColumnCount();                          // return number of columns (value passed to BeginTable)
    int TableGetColumnIndex();                          // return current column index.
    int TableGetRowIndex();                             // return current row index.
    const char *TableGetColumnName(int column_n = -1);  // return "" if column didn't have a name declared by
                                                        // TableSetupColumn(). Pass -1 to use current column.
    ImGuiTableColumnFlags TableGetColumnFlags(
        int column_n = -1);  // return column flags so you can query their Enabled/Visible/Sorted/Hovered status flags. Pass
                             // -1 to use current column.
    void TableSetColumnEnabled(
        int column_n,
        bool v);  // change user accessible enabled/disabled state of a column. Set to false to hide the column. User can use
                  // the context menu to change this themselves (right-click in headers, or right-click in columns body with
                  // ImGuiTableFlags_ContextMenuInBody)
    void TableSetBgColor(
        ImGuiTableBgTarget target,
        ImU32 color,
        int column_n = -1);  // change the color of a cell, row, or column. See ImGuiTableBgTarget_ flags for details.

    bool DragFloat(
        const char *label,
        float *v,
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char *format = "%.3f",
        ImGuiSliderFlags flags = 0);  // If v_min >= v_max we have no bound
    bool DragFloat2(
        const char *label,
        float v[2],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char *format = "%.3f",
        ImGuiSliderFlags flags = 0);
    bool DragFloat3(
        const char *label,
        float v[3],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char *format = "%.3f",
        ImGuiSliderFlags flags = 0);
    bool DragFloat4(
        const char *label,
        float v[4],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char *format = "%.3f",
        ImGuiSliderFlags flags = 0);
    bool DragFloatRange2(
        const char *label,
        float *v_current_min,
        float *v_current_max,
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char *format = "%.3f",
        const char *format_max = NULL,
        ImGuiSliderFlags flags = 0);
    bool DragInt(
        const char *label,
        int *v,
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char *format = "%d",
        ImGuiSliderFlags flags = 0);  // If v_min >= v_max we have no bound
    bool DragInt2(
        const char *label,
        int v[2],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char *format = "%d",
        ImGuiSliderFlags flags = 0);
    bool DragInt3(
        const char *label,
        int v[3],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char *format = "%d",
        ImGuiSliderFlags flags = 0);
    bool DragInt4(
        const char *label,
        int v[4],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char *format = "%d",
        ImGuiSliderFlags flags = 0);
    bool DragIntRange2(
        const char *label,
        int *v_current_min,
        int *v_current_max,
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char *format = "%d",
        const char *format_max = NULL,
        ImGuiSliderFlags flags = 0);
    bool DragScalar(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        float v_speed = 1.0f,
        const void *p_min = NULL,
        const void *p_max = NULL,
        const char *format = NULL,
        ImGuiSliderFlags flags = 0);
    bool DragScalarN(
        const char *label,
        ImGuiDataType data_type,
        void *p_data,
        int components,
        float v_speed = 1.0f,
        const void *p_min = NULL,
        const void *p_max = NULL,
        const char *format = NULL,
        ImGuiSliderFlags flags = 0);

    // Legacy Columns API (prefer using Tables!)
    // - You can also use SameLine(pos_x) to mimic simplified columns.
    void Columns(int count = 1, const char *id = NULL, bool border = true);
    void NextColumn();     // next column, defaults to current row or next row if the current row is finished
    int GetColumnIndex();  // get current column index
    float GetColumnWidth(int column_index = -1);         // get column width (in pixels). pass -1 to use current column
    void SetColumnWidth(int column_index, float width);  // set column width (in pixels). pass -1 to use current column
    float GetColumnOffset(int column_index = -1);        // get position of column line (in pixels, from the left side of the
                                                         // contents region). pass -1 to use current column, otherwise
                                                         // 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
    void SetColumnOffset(int column_index, float offset_x);  // set position of column line (in pixels, from the left side of
                                                             // the contents region). pass -1 to use current column
    int GetColumnsCount();

    void TableSetupColumn(
        const char *label,
        ImGuiTableColumnFlags flags = 0,
        float init_width_or_weight = 0.0f,
        ImGuiID user_id = 0);
    void TableSetupScrollFreeze(int cols, int rows);  // lock columns/rows so they stay visible when scrolled.
    void TableHeadersRow();  // submit all headers cells based on data provided to TableSetupColumn() + submit context menu
    void TableHeader(const char *label);  // submit one header cell manually (rarely used)

    bool BeginTable(
        const char *str_id,
        int column,
        ImGuiTableFlags flags = 0,
        const ImVec2 &outer_size = ImVec2(0.0f, 0.0f),
        float inner_width = 0.0f);
    void EndTable();  // only call EndTable() if BeginTable() returns true!
    void TableNextRow(
        ImGuiTableRowFlags row_flags = 0,
        float min_row_height = 0.0f);  // append into the first cell of a new row.
    bool TableNextColumn();  // append into the next column (or first column of next row if currently in last column). Return
                             // true when column is visible.
    bool TableSetColumnIndex(int column_n);  // append into the specified column. Return true when column is visible.

  }  // namespace GUI
}  // namespace Cherry

namespace CherryGUI = Cherry::GUI;