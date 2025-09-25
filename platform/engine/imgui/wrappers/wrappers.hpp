#pragma once
#include "../../base.hpp"

// This ImGui wrapper helps Cherry framework to easely add custom behaviors,
// control and ui callbacks and other cool stuff for the end-user API.

namespace Cherry {
namespace GUI {
// Fonts
CHERRY_API ImVec2 CalcTextSizeA(ImFont* ptr, float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL); // utf8
CHERRY_API const char* CalcWordWrapPositionA(ImFont* ptr, float scale, const char* text, const char* text_end, float wrap_width) ;
CHERRY_API void  RenderChar(ImFont* ptr, ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, ImWchar c);
CHERRY_API void  RenderText(ImFont* ptr, ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false);
CHERRY_API void ClearInputQueueCharacters(ImGuiIO& io);

// Drawlist
CHERRY_API void  AddLine(ImDrawList* ptr, const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f);
CHERRY_API void  AddRect(ImDrawList* ptr, const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f);   // a: upper-left, b: lower-right (== upper-left + size)
CHERRY_API void  AddRectFilled(ImDrawList* ptr, const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0);                     // a: upper-left, b: lower-right (== upper-left + size)
CHERRY_API void  AddRectFilledMultiColor(ImDrawList* ptr, const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
CHERRY_API void  AddQuad(ImDrawList* ptr, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness = 1.0f);
CHERRY_API void  AddQuadFilled(ImDrawList* ptr, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col);
CHERRY_API void  AddTriangle(ImDrawList* ptr, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.0f);
CHERRY_API void  AddTriangleFilled(ImDrawList* ptr, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col);
CHERRY_API void  AddCircle(ImDrawList* ptr, const ImVec2& center, float radius, ImU32 col, int num_segments = 0, float thickness = 1.0f);
CHERRY_API void  AddCircleFilled(ImDrawList* ptr, const ImVec2& center, float radius, ImU32 col, int num_segments = 0);
CHERRY_API void  AddNgon(ImDrawList* ptr, const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness = 1.0f);
CHERRY_API void  AddNgonFilled(ImDrawList* ptr, const ImVec2& center, float radius, ImU32 col, int num_segments);
CHERRY_API void  AddText(ImDrawList* ptr, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
CHERRY_API void  AddText(ImDrawList* ptr, const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
CHERRY_API void  AddPolyline(ImDrawList* ptr, const ImVec2* points, int num_points, ImU32 col, ImDrawFlags flags, float thickness);
CHERRY_API void  AddConvexPolyFilled(ImDrawList* ptr, const ImVec2* points, int num_points, ImU32 col); // Note: Anti-aliased filling requires points to be in clockwise order.
CHERRY_API void  AddBezierCubic(ImDrawList* ptr, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0); // Cubic Bezier (4 control points)
CHERRY_API void  AddBezierQuadratic(ImDrawList* ptr, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness, int num_segments = 0);               // Quadratic Bezier (3 control points)

// Legacy ImGui style variables
CHERRY_API void PushStyleVar(ImGuiStyleVar idx, float val);
CHERRY_API void PushStyleVar(ImGuiCol idx, const ImVec2 &val);
CHERRY_API void PopStyleVar(int count = 1);

// Legacy ImGui style colors
CHERRY_API void PushStyleColor(ImGuiCol idx, const ImVec4 &col);
CHERRY_API void PushStyleColor(ImGuiCol idx, ImU32 col);
CHERRY_API void PopStyleColor(int count = 1);

CHERRY_API bool BeginMenuBar(); // append to menu-bar of current window (requires
                     // ImGuiWindowFlags_MenuBar flag set on parent window).
CHERRY_API bool BeginMenuBar(
    float offsetY);    // append to menu-bar of current window (requires
                       // ImGuiWindowFlags_MenuBar flag set on parent window).
CHERRY_API bool BeginBottomBar(); // append to menu-bar of current window (requires
                       // ImGuiWindowFlags_MenuBar flag set on parent window).

CHERRY_API void EndMenuBar();
CHERRY_API void EndBottomBar(); // only call EndMenuBar() if BeginMenuBar() returns true!
CHERRY_API bool BeginMainMenuBar(); // create and append to a full screen menu-bar.
CHERRY_API void EndMainMenuBar();

// Legacy ImGui ui elements
CHERRY_API bool Button(const char *label, const ImVec2 &size = ImVec2(0, 0)); // button
CHERRY_API bool ImageSizeButtonWithText(ImTextureID texId, float button_width,
                             const char *label,
                             const ImVec2 &imageSize = ImVec2(0, 0),
                             const ImVec2 &uv0 = ImVec2(0, 0),
                             const ImVec2 &uv1 = ImVec2(1, 1),
                             int frame_padding = -1,
                             const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                             const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
CHERRY_API bool SmallButton(const char *label); // button with FramePadding=(0,0) to easily
                                     // embed within text
CHERRY_API bool InvisibleButton(
    const char *str_id, const ImVec2 &size,
    ImGuiButtonFlags flags =
        0); // flexible button behavior without the visuals, frequently useful
            // to build custom behaviors using the public api (along with
            // IsItemActive, IsItemHovered, etc.)
CHERRY_API bool ArrowButton(const char *str_id,
                 ImGuiDir dir); // square button with an arrow shape
CHERRY_API void Image(ImTextureID user_texture_id, const ImVec2 &size,
           const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1),
           const ImVec4 &tint_col = ImVec4(1, 1, 1, 1),
           const ImVec4 &border_col = ImVec4(0, 0, 0, 0));
CHERRY_API bool ImageButton(ImTextureID user_texture_id, const ImVec2 &size,
                 const ImVec2 &uv0 = ImVec2(0, 0),
                 const ImVec2 &uv1 = ImVec2(1, 1), int frame_padding = -1,
                 const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                 const ImVec4 &tint_col =
                     ImVec4(1, 1, 1, 1)); // <0 frame_padding uses default frame
                                          // padding settings. 0 for no padding
CHERRY_API bool Checkbox(const char *label, bool *v);
CHERRY_API bool CheckboxFlags(const char *label, int *flags, int flags_value);
CHERRY_API bool CheckboxFlags(const char *label, unsigned int *flags,
                   unsigned int flags_value);
CHERRY_API bool RadioButton(const char *label,
                 bool active); // use with e.g. if (RadioButton("one",
                               // my_value==1)) { my_value = 1; }
CHERRY_API bool RadioButton(const char *label, int *v,
                 int v_button); // shortcut to handle the above pattern when
                                // value is an integer
CHERRY_API void ProgressBar(float fraction, const ImVec2 &size_arg = ImVec2(-FLT_MIN, 0),
                 const char *overlay = NULL);
CHERRY_API void Bullet(); // draw a small circle + keep the cursor on the same line.
               // advance cursor x position by GetTreeNodeToLabelSpacing(), same
               // distance that TreeNode() uses
CHERRY_API bool TextButtonWithImageWithIcon(ImTextureID texIdFirst,
                                 ImTextureID texIdSecond, const char *label,
                                 const ImVec2 &imageSizeFirst = ImVec2(0, 0),
                                 const ImVec2 &imageSizeSecond = ImVec2(0, 0),
                                 const ImVec2 &uv0 = ImVec2(0, 0),
                                 const ImVec2 &uv1 = ImVec2(1, 1),
                                 int frame_padding = -1,
                                 const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                                 const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
CHERRY_API bool ImageButtonWithTextWithIcon(ImTextureID texIdFirst,
                                 ImTextureID texIdSecond, const char *label,
                                 const ImVec2 &imageSizeFirst = ImVec2(0, 0),
                                 const ImVec2 &imageSizeSecond = ImVec2(0, 0),
                                 const ImVec2 &uv0 = ImVec2(0, 0),
                                 const ImVec2 &uv1 = ImVec2(1, 1),
                                 int frame_padding = -1,
                                 const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                                 const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
CHERRY_API bool ImageButtonWithText(ImTextureID texId, const char *label,
                         const ImVec2 &imageSize = ImVec2(0, 0),
                         const ImVec2 &uv0 = ImVec2(0, 0),
                         const ImVec2 &uv1 = ImVec2(1, 1),
                         int frame_padding = -1,
                         const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                         const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));
CHERRY_API bool RightImageButtonWithText(ImTextureID texId, const char *label,
                              const ImVec2 &imageSize = ImVec2(0, 0),
                              const ImVec2 &uv0 = ImVec2(0, 0),
                              const ImVec2 &uv1 = ImVec2(1, 1),
                              int frame_padding = -1,
                              const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                              const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));

CHERRY_API bool BeginMenu(const char *label,
               bool enabled = true); // create a sub-menu entry. only call
                                     // EndMenu() if this returns true!
CHERRY_API bool BeginMenuImage(const char *label, ImTextureID *texture,
                    bool enabled = true); // create a sub-menu entry. only call
                                          // EndMenu() if this returns true!
CHERRY_API void EndMenu();

// Legacy menu items
CHERRY_API bool MenuItem(const char *label, const char *shortcut = NULL,
              bool selected = false,
              bool enabled = true); // return true when activated.
CHERRY_API bool MenuItem(const char *label, const char *shortcut, bool *p_selected,
              bool enabled = true); // return true when activated + toggle
                                    // (*p_selected) if p_selected != NULL
CHERRY_API bool MenuItem(const char *label, const char *shortcut, ImTextureID texture,
              bool selected,
              bool enabled = true); // return true when activated + toggle
                                    // (*p_selected) if p_selected != NULL

// Legacy popups
CHERRY_API bool BeginPopup(
    const char *str_id,
    ImGuiWindowFlags flags = 0); // return true if the popup is open, and you
                                 // can start outputting to it.
CHERRY_API bool BeginPopupModal(
    const char *name, bool *p_open = NULL,
    ImGuiWindowFlags flags = 0); // return true if the modal is open, and you
                                 // can start outputting to it.
CHERRY_API void EndPopup();                 // did mouse button double-clicked? Same as
                 // GetMouseClickedCount() == 2. (note that a double-click will
                 // also report IsMouseClicked() == true)
CHERRY_API void OpenPopup(const char *str_id,
               ImGuiPopupFlags popup_flags =
                   0); // call to mark popup as open (don't call every frame!).
CHERRY_API void OpenPopup(ImGuiID id,
               ImGuiPopupFlags popup_flags =
                   0); // id overload to facilitate calling from nested stacks
CHERRY_API void OpenPopupOnItemClick(
    const char *str_id = NULL,
    ImGuiPopupFlags popup_flags =
        1); // helper to open popup when clicked on last item. Default to
            // ImGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers
            // on the mouse _released_ event to be consistent with popup
            // behaviors)
CHERRY_API void CloseCurrentPopup();
CHERRY_API bool BeginPopupContextItem(
    const char *str_id = NULL,
    ImGuiPopupFlags popup_flags =
        1); // open+begin popup when clicked on last item. Use str_id==NULL to
            // associate the popup to previous item. If you want to use that on
            // a non-interactive item such as Text() you need to pass in an
            // explicit ID here. read comments in .cpp!
CHERRY_API bool BeginPopupContextWindow(
    const char *str_id = NULL,
    ImGuiPopupFlags popup_flags =
        1); // open+begin popup when clicked on current window.
CHERRY_API bool BeginPopupContextVoid(
    const char *str_id = NULL,
    ImGuiPopupFlags popup_flags = 1); // open+begin popup when clicked in void
                                      // (where there are no windows).

CHERRY_API ImFont *GetFont();   // get current font
CHERRY_API float GetFontSize(); // get current font size (= height in pixels) of current
                     // font with current scale applied
CHERRY_API ImVec2 GetFontTexUvWhitePixel(); // get UV coordinate for a while pixel, useful
                                 // to draw custom shapes via the ImDrawList API
CHERRY_API ImU32 GetColorU32(
    ImGuiCol idx,
    float alpha_mul = 1.0f); // retrieve given style color with style alpha
                             // applied and optional extra alpha multiplier,
                             // packed as a 32-bit value suitable for ImDrawList
CHERRY_API ImU32 GetColorU32(
    const ImVec4 &col); // retrieve given color with style alpha applied, packed
                        // as a 32-bit value suitable for ImDrawList
CHERRY_API ImU32 GetColorU32(
    ImU32 col); // retrieve given color with style alpha applied, packed as a
                // 32-bit value suitable for ImDrawList
CHERRY_API const ImVec4 &GetStyleColorVec4(
    ImGuiCol
        idx); // retrieve style color as stored in ImGuiStyle structure. use to
              // feed back into PushStyleColor(), otherwise use GetColorU32() to
              // get style color with style alpha baked in.

CHERRY_API void PushFont(ImFont *font); // use NULL as a shortcut to push default font
CHERRY_API void PopFont();

// Widgets: Text
CHERRY_API void TextUnformatted(
    const char *text,
    const char *text_end =
        NULL); // raw text without formatting. Roughly equivalent to Text("%s",
               // text) but: A) doesn't require null terminated string if
               // 'text_end' is specified, B) it's faster, no memory copy is
               // done, no buffer size limits, recommended for long chunks of
               // text.
CHERRY_API void Text(const char *fmt, ...) IM_FMTARGS(1); // formatted text
CHERRY_API void TextV(const char *fmt, va_list args) IM_FMTLIST(1);
CHERRY_API void TextColored(const ImVec4 &col, const char *fmt, ...)
    IM_FMTARGS(2); // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt,
                   // ...); PopStyleColor();
CHERRY_API void TextColoredV(const ImVec4 &col, const char *fmt, va_list args)
    IM_FMTLIST(2);
CHERRY_API void TextDisabled(const char *fmt, ...)
    IM_FMTARGS(1); // shortcut for PushStyleColor(ImGuiCol_Text,
                   // style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...);
                   // PopStyleColor();
CHERRY_API void TextDisabledV(const char *fmt, va_list args) IM_FMTLIST(1);
CHERRY_API void TextWrapped(const char *fmt, ...) IM_FMTARGS(
    1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...);
        // PopTextWrapPos();. Note that this won't work on an auto-resizing
        // window if there's no other widgets to extend the window width, yoy
        // may need to set a size using SetNextWindowSize().
CHERRY_API void TextWrappedV(const char *fmt, va_list args) IM_FMTLIST(1);
CHERRY_API void LabelText(const char *label, const char *fmt, ...) IM_FMTARGS(
    2); // display text+label aligned the same way as value+label widgets
CHERRY_API void LabelTextV(const char *label, const char *fmt, va_list args) IM_FMTLIST(2);
CHERRY_API void BulletText(const char *fmt, ...)
    IM_FMTARGS(1); // shortcut for Bullet()+Text()
CHERRY_API void BulletTextV(const char *fmt, va_list args) IM_FMTLIST(1);

// Font
CHERRY_API void SetCurrentFont(ImFont *font);

// Items interaction
CHERRY_API bool IsItemHovered(
    ImGuiHoveredFlags flags =
        0); // is the last item hovered? (and usable, aka not blocked by a
            // popup, etc.). See ImGuiHoveredFlags for more options.
CHERRY_API bool IsItemActive();  // is the last item active? (e.g. button being held, text
                      // field being edited. This will continuously return true
                      // while holding mouse button on an item. Items that don't
                      // interact will always return false)
CHERRY_API bool IsItemFocused(); // is the last item focused for keyboard/gamepad
                      // navigation?
CHERRY_API bool IsItemClicked(
    ImGuiMouseButton mouse_button =
        0); // is the last item hovered and mouse clicked on? (**)  ==
            // IsMouseClicked(mouse_button) && IsItemHovered()Important. (**)
            // this it NOT equivalent to the behavior of e.g. Button(). Read
            // comments in function definition.
CHERRY_API bool IsItemVisible(); // is the last item visible? (items may be out of sight
                      // because of clipping/scrolling)
CHERRY_API bool IsItemEdited();  // did the last item modify its underlying value this
                     // frame? or was pressed? This is generally the same as the
                     // "bool" return value of many widgets.
CHERRY_API bool IsItemActivated();   // was the last item just made active (item was
                          // previously inactive).
CHERRY_API bool IsItemDeactivated(); // was the last item just made inactive (item was
                          // previously active). Useful for Undo/Redo patterns
                          // with widgets that requires continuous editing.
CHERRY_API bool IsItemDeactivatedAfterEdit(); // was the last item just made inactive and
                                   // made a value change when it was active?
                                   // (e.g. Slider/Drag moved). Useful for
                                   // Undo/Redo patterns with widgets that
                                   // requires continuous editing. Note that you
                                   // may get false positives (some widgets such
                                   // as Combo()/ListBox()/Selectable() will
                                   // return true even when clicking an already
                                   // selected item).
CHERRY_API bool IsItemToggledOpen(); // was the last item open state toggled? set by
                          // TreeNode().
CHERRY_API bool IsAnyItemHovered();  // is any item hovered?
CHERRY_API bool IsAnyItemActive();   // is any item active?
CHERRY_API bool IsAnyItemFocused();  // is any item focused?
CHERRY_API ImVec2 GetItemRectMin();  // get upper-left bounding rectangle of the last item
                          // (screen space)
CHERRY_API ImVec2 GetItemRectMax();  // get lower-right bounding rectangle of the last item
                          // (screen space)
CHERRY_API ImVec2 GetItemRectSize(); // get size of last item
CHERRY_API void SetItemAllowOverlap();

// Layout
CHERRY_API void Separator();
CHERRY_API void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
CHERRY_API void NewLine();
CHERRY_API void Spacing();
CHERRY_API void Dummy(const ImVec2 &size);
CHERRY_API void Indent(float indent_w = 0.0f);
CHERRY_API void Unindent(float indent_w = 0.0f);
CHERRY_API void BeginGroup();
CHERRY_API void EndGroup();
CHERRY_API ImVec2 GetCursorPos();
CHERRY_API float GetCursorPosX();
CHERRY_API float GetCursorPosY();
CHERRY_API void SetCursorPos(const ImVec2 &local_pos);
CHERRY_API void SetCursorPosX(float local_x);
CHERRY_API void SetCursorPosY(float local_y);
CHERRY_API ImVec2 GetCursorStartPos();
CHERRY_API ImVec2 GetCursorScreenPos();
CHERRY_API void SetCursorScreenPos(const ImVec2 &pos);
CHERRY_API void AlignTextToFramePadding();
CHERRY_API void AlignTextToFramePadding(float offsetY);
CHERRY_API float GetTextLineHeight();
CHERRY_API float GetTextLineHeightWithSpacing();
CHERRY_API float GetFrameHeight();
CHERRY_API float GetFrameHeightWithSpacing();
CHERRY_API void BeginHorizontal(const char *str_id, const ImVec2 &size = ImVec2(0, 0),
                     float align = -1.0f);
CHERRY_API void BeginHorizontal(const void *ptr_id, const ImVec2 &size = ImVec2(0, 0),
                     float align = -1.0f);
CHERRY_API void BeginHorizontal(int id, const ImVec2 &size = ImVec2(0, 0),
                     float align = -1);
CHERRY_API void EndHorizontal();
CHERRY_API void BeginVertical(const char *str_id, const ImVec2 &size = ImVec2(0, 0),
                   float align = -1.0f);
CHERRY_API void BeginVertical(const void *ptr_id, const ImVec2 &size = ImVec2(0, 0),
                   float align = -1.0f);
CHERRY_API void BeginVertical(int id, const ImVec2 &size = ImVec2(0, 0), float align = -1);
CHERRY_API void EndVertical();
CHERRY_API void Spring(float weight = 1.0f, float spacing = -1.0f);
CHERRY_API void SuspendLayout();
CHERRY_API void ResumeLayout();

// Inputs (Please consider using the Cherry input api.)
CHERRY_API bool IsMouseDown(ImGuiMouseButton button);
CHERRY_API bool IsMouseClicked(ImGuiMouseButton button, bool repeat = false);
CHERRY_API bool IsMouseReleased(ImGuiMouseButton button);
CHERRY_API bool IsMouseDoubleClicked(ImGuiMouseButton button);
CHERRY_API int GetMouseClickedCount(ImGuiMouseButton button);
CHERRY_API bool IsMouseHoveringRect(const ImVec2 &r_min, const ImVec2 &r_max,
                         bool clip = true);
CHERRY_API bool IsMousePosValid(const ImVec2 *mouse_pos = NULL);
CHERRY_API bool IsAnyMouseDown();
CHERRY_API ImVec2 GetMousePos();
CHERRY_API ImVec2 GetMousePosOnOpeningCurrentPopup();
CHERRY_API bool IsMouseDragging(ImGuiMouseButton button, float lock_threshold = -1.0f);
CHERRY_API ImVec2 GetMouseDragDelta(ImGuiMouseButton button = 0,
                         float lock_threshold = -1.0f);
CHERRY_API void ResetMouseDragDelta(ImGuiMouseButton button = 0);
CHERRY_API ImGuiMouseCursor GetMouseCursor();
CHERRY_API void SetMouseCursor(ImGuiMouseCursor cursor_type);
CHERRY_API void CaptureMouseFromApp(bool want_capture_mouse_value = true);

// Text
CHERRY_API ImVec2 CalcTextSize(const char *text, const char *text_end = NULL,
                    bool hide_text_after_double_hash = false,
                    float wrap_width = -1.0f);

// ID
CHERRY_API void PushID(const char *str_id);
CHERRY_API void PushID(const char *str_id_begin, const char *str_id_end);
CHERRY_API void PushID(const void *ptr_id);
CHERRY_API void PushID(int int_id);
CHERRY_API void PopID();
CHERRY_API ImGuiID GetID(const char *str_id);
CHERRY_API ImGuiID GetID(const char *str_id_begin, const char *str_id_end);
CHERRY_API ImGuiID GetID(const void *ptr_id);

CHERRY_API bool BeginCombo(const char *label, const char *preview_value,
                ImGuiComboFlags flags = 0);
CHERRY_API bool BeginCombo(const char *label, const std::function<void()> &preview,
                ImGuiComboFlags flags = 0);
CHERRY_API void EndCombo(); // only call EndCombo() if BeginCombo() returns true!
CHERRY_API bool Combo(const char *label, int *current_item, const char *const items[],
           int items_count, int popup_max_height_in_items = -1);
CHERRY_API bool Combo(const char *label, int *current_item,
           const char *items_separated_by_zeros,
           int popup_max_height_in_items =
               -1); // Separate items with \0 within a string, end item-list
                    // with \0\0. e.g. "One\0Two\0Three\0"
CHERRY_API bool Combo(const char *label, int *current_item,
           bool (*items_getter)(void *data, int idx, const char **out_text),
           void *data, int items_count, int popup_max_height_in_items = -1);

CHERRY_API bool BeginChild(const char *str_id, const ImVec2 &size = ImVec2(0, 0),
                bool border = false, ImGuiWindowFlags flags = 0);
CHERRY_API bool BeginChild(ImGuiID id, const ImVec2 &size = ImVec2(0, 0),
                bool border = false, ImGuiWindowFlags flags = 0);
CHERRY_API void EndChild();

// Windows Utilities
// - 'current window' = the window we are appending into while inside a
// Begin()/End() block. 'next window' = next window we will Begin() into.
CHERRY_API bool IsWindowAppearing();
CHERRY_API bool IsWindowCollapsed();
CHERRY_API bool IsWindowFocused(ImGuiFocusedFlags flags =
                         0); // is current window focused? or its root/child,
                             // depending on flags. see flags for options.
CHERRY_API bool IsWindowHovered(
    ImGuiHoveredFlags flags =
        0); // is current window hovered (and typically: not blocked by a
            // popup/modal)? see flags for options. NB: If you are trying to
            // check whether your mouse should be dispatched to imgui or to your
            // app, you should use the 'io.WantCaptureMouse' boolean for that!
            // Please read the FAQ!
CHERRY_API ImDrawList *
GetWindowDrawList();       // get draw list associated to the current window, to
                           // append your own drawing primitives
CHERRY_API float GetWindowDpiScale(); // get DPI scale currently associated to the current
                           // window's viewport.
CHERRY_API ImVec2 GetWindowPos(); // get current window position in screen space (useful if
                       // you want to do your own drawing via the DrawList API)
CHERRY_API ImVec2 GetWindowSize();  // get current window size
CHERRY_API float GetWindowWidth();  // get current window width (shortcut for
                         // GetWindowSize().x)
CHERRY_API float GetWindowHeight(); // get current window height (shortcut for
                         // GetWindowSize().y)
CHERRY_API ImGuiViewport *
GetWindowViewport(); // get viewport currently associated to the current window.

// Window manipulation
// - Prefer using SetNextXXX functions (before Begin) rather that SetXXX
// functions (after Begin).
CHERRY_API void SetNextWindowPos(
    const ImVec2 &pos, ImGuiCond cond = 0,
    const ImVec2 &pivot =
        ImVec2(0,
               0)); // set next window position. call before Begin(). use
                    // pivot=(0.5f,0.5f) to center on given point, etc.
CHERRY_API void SetNextWindowSize(
    const ImVec2 &size,
    ImGuiCond cond = 0); // set next window size. set axis to 0.0f to force an
                         // auto-fit on this axis. call before Begin()
CHERRY_API void SetNextWindowSizeConstraints(
    const ImVec2 &size_min, const ImVec2 &size_max,
    ImGuiSizeCallback custom_callback = NULL,
    void *custom_callback_data =
        NULL); // set next window size limits. use -1,-1 on either X/Y axis to
               // preserve the current size. Sizes will be rounded down. Use
               // callback to apply non-trivial programmatic constraints.
CHERRY_API void SetNextWindowContentSize(
    const ImVec2 &
        size); // set next window content size (~ scrollable client area, which
               // enforce the range of scrollbars). Not including window
               // decorations (title bar, menu bar, etc.) nor WindowPadding. set
               // an axis to 0.0f to leave it automatic. call before Begin()
CHERRY_API void SetNextWindowCollapsed(
    bool collapsed,
    ImGuiCond cond = 0); // set next window collapsed state. call before Begin()
CHERRY_API void SetNextWindowFocus(); // set next window to be focused / top-most. call
                           // before Begin()
CHERRY_API void SetNextWindowBgAlpha(
    float alpha); // set next window background color alpha. helper to easily
                  // override the Alpha component of
                  // ImGuiCol_WindowBg/ChildBg/PopupBg. you may also use
                  // ImGuiWindowFlags_NoBackground.
CHERRY_API void SetNextWindowViewport(ImGuiID viewport_id); // set next window viewport
CHERRY_API void SetWindowPos(
    const ImVec2 &pos,
    ImGuiCond cond =
        0); // (not recommended) set current window position - call within
            // Begin()/End(). prefer using SetNextWindowPos(), as this may incur
            // tearing and side-effects.
CHERRY_API void SetWindowSize(
    const ImVec2 &size,
    ImGuiCond cond = 0); // (not recommended) set current window size - call
                         // within Begin()/End(). set to ImVec2(0, 0) to force
                         // an auto-fit. prefer using SetNextWindowSize(), as
                         // this may incur tearing and minor side-effects.
CHERRY_API void SetWindowCollapsed(
    bool collapsed,
    ImGuiCond cond = 0); // (not recommended) set current window collapsed
                         // state. prefer using SetNextWindowCollapsed().
CHERRY_API void SetWindowFocus();   // (not recommended) set current window to be focused /
                         // top-most. prefer using SetNextWindowFocus().
CHERRY_API void SetWindowFontScale(
    float scale); // [OBSOLETE] set font scale. Adjust IO.FontGlobalScale if you
                  // want to scale all windows. This is an old API! For correct
                  // scaling, prefer to reload font + rebuild ImFontAtlas + call
                  // style.ScaleAllSizes().
CHERRY_API void SetWindowPos(const char *name, const ImVec2 &pos,
                  ImGuiCond cond = 0); // set named window position.
CHERRY_API void SetWindowSize(
    const char *name, const ImVec2 &size,
    ImGuiCond cond = 0); // set named window size. set axis to 0.0f to force an
                         // auto-fit on this axis.
CHERRY_API void SetWindowCollapsed(const char *name, bool collapsed,
                        ImGuiCond cond = 0); // set named window collapsed state
CHERRY_API void SetWindowFocus(const char *name);       // set named window to be focused /
                                       // top-most. use NULL to remove focus.

// Content region
// - Retrieve available space from a given point. GetContentRegionAvail() is
// frequently useful.
// - Those functions are bound to be redesigned (they are confusing, incomplete
// and the Min/Max return values are in local window coordinates which increases
// confusion)
CHERRY_API ImVec2 GetContentRegionAvail(); // == GetContentRegionMax() - GetCursorPos()
CHERRY_API ImVec2 GetContentRegionMax();   // current content boundaries (typically window
                                // boundaries including scrolling, or current
                                // column boundaries), in windows coordinates
CHERRY_API ImVec2
GetWindowContentRegionMin(); // content boundaries min for the full window
                             // (roughly (0,0)-Scroll), in window coordinates
CHERRY_API ImVec2
GetWindowContentRegionMax(); // content boundaries max for the full window
                             // (roughly (0,0)+Size-Scroll) where Size can be
                             // override with SetNextWindowContentSize(), in
                             // window coordinates

// Windows Scrolling
CHERRY_API float GetScrollX();              // get scrolling amount [0 .. GetScrollMaxX()]
CHERRY_API float GetScrollY();              // get scrolling amount [0 .. GetScrollMaxY()]
CHERRY_API void SetScrollX(float scroll_x); // set scrolling amount [0 .. GetScrollMaxX()]
CHERRY_API void SetScrollY(float scroll_y); // set scrolling amount [0 .. GetScrollMaxY()]
CHERRY_API float GetScrollMaxX(); // get maximum scrolling amount ~~ ContentSize.x -
                       // WindowSize.x - DecorationsSize.x
CHERRY_API float GetScrollMaxY(); // get maximum scrolling amount ~~ ContentSize.y -
                       // WindowSize.y - DecorationsSize.y
CHERRY_API void SetScrollHereX(
    float center_x_ratio =
        0.5f); // adjust scrolling amount to make current cursor position
               // visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right.
               // When using to make a "default/current item" visible, consider
               // using SetItemDefaultFocus() instead.
CHERRY_API void SetScrollHereY(
    float center_y_ratio =
        0.5f); // adjust scrolling amount to make current cursor position
               // visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom.
               // When using to make a "default/current item" visible, consider
               // using SetItemDefaultFocus() instead.
CHERRY_API void SetScrollFromPosX(
    float local_x,
    float center_x_ratio =
        0.5f); // adjust scrolling amount to make given position visible.
               // Generally GetCursorStartPos() + offset to compute a valid
               // position.
CHERRY_API void SetScrollFromPosY(
    float local_y,
    float center_y_ratio =
        0.5f); // adjust scrolling amount to make given position visible.
               // Generally GetCursorStartPos() + offset to compute a valid
               // position.

// Color Utilities
CHERRY_API ImVec4 ColorConvertU32ToFloat4(ImU32 in);
CHERRY_API ImU32 ColorConvertFloat4ToU32(const ImVec4 &in);
CHERRY_API void ColorConvertRGBtoHSV(float r, float g, float b, float &out_h, float &out_s,
                          float &out_v);
CHERRY_API void ColorConvertHSVtoRGB(float h, float s, float v, float &out_r, float &out_g,
                          float &out_b);

CHERRY_API ImGuiIO &GetIO(); // access the IO structure (mouse/keyboard/gamepad inputs,
                  // time, various configuration options/flags)
CHERRY_API ImGuiStyle &
GetStyle(); // access the Style structure (colors, sizes). Always use
            // PushStyleCol(), PushStyleVar() to modify style mid-frame!
CHERRY_API ImDrawData *GetDrawData(); // valid after Render() and until the next call to
                           // NewFrame(). this is what you have to render.
CHERRY_API void PlotLines(const char *label, const float *values, int values_count,
               int values_offset = 0, const char *overlay_text = NULL,
               float scale_min = FLT_MAX, float scale_max = FLT_MAX,
               ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
CHERRY_API void PlotLines(const char *label, float (*values_getter)(void *data, int idx),
               void *data, int values_count, int values_offset = 0,
               const char *overlay_text = NULL, float scale_min = FLT_MAX,
               float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
CHERRY_API void PlotHistogram(const char *label, const float *values, int values_count,
                   int values_offset = 0, const char *overlay_text = NULL,
                   float scale_min = FLT_MAX, float scale_max = FLT_MAX,
                   ImVec2 graph_size = ImVec2(0, 0),
                   int stride = sizeof(float));
CHERRY_API void PlotHistogram(const char *label,
                   float (*values_getter)(void *data, int idx), void *data,
                   int values_count, int values_offset = 0,
                   const char *overlay_text = NULL, float scale_min = FLT_MAX,
                   float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
CHERRY_API bool Selectable(
    const char *label, bool selected = false, ImGuiSelectableFlags flags = 0,
    const ImVec2 &size = ImVec2(
        0, 0)); // "bool selected" carry the selection state (read-only).
                // Selectable() is clicked is returns true so you can modify
                // your selection state. size.x==0.0: use remaining width,
                // size.x>0.0: specify width. size.y==0.0: use label height,
                // size.y>0.0: specify height
CHERRY_API bool Selectable(const char *label, bool *p_selected,
                ImGuiSelectableFlags flags = 0,
                const ImVec2 &size = ImVec2(
                    0, 0)); // "bool* p_selected" point to the selection state
                            // (read-write), as a convenient helper.
CHERRY_API bool TreeNode(const char *label);
CHERRY_API bool TreeNode(const char *str_id, const char *fmt, ...) IM_FMTARGS(
    2); // helper variation to easily decorelate the id from the displayed
        // string. Read the FAQ about why and how to use ID. to align arbitrary
        // text at the same level as a TreeNode() you can use Bullet().
CHERRY_API bool TreeNode(const void *ptr_id, const char *fmt, ...) IM_FMTARGS(2); // "
CHERRY_API bool TreeNodeV(const char *str_id, const char *fmt, va_list args) IM_FMTLIST(2);
CHERRY_API bool TreeNodeV(const void *ptr_id, const char *fmt, va_list args) IM_FMTLIST(2);
CHERRY_API bool TreeNodeEx(const char *label, ImGuiTreeNodeFlags flags = 0);
CHERRY_API bool TreeNodeEx(const char *str_id, ImGuiTreeNodeFlags flags, const char *fmt,
                ...) IM_FMTARGS(3);
CHERRY_API bool TreeNodeEx(const void *ptr_id, ImGuiTreeNodeFlags flags, const char *fmt,
                ...) IM_FMTARGS(3);
CHERRY_API bool TreeNodeExV(const char *str_id, ImGuiTreeNodeFlags flags, const char *fmt,
                 va_list args) IM_FMTLIST(3);
CHERRY_API bool TreeNodeExV(const void *ptr_id, ImGuiTreeNodeFlags flags, const char *fmt,
                 va_list args) IM_FMTLIST(3);
CHERRY_API void TreePush(
    const char *str_id); // ~ Indent()+PushId(). Already called by TreeNode()
                         // when returning true, but you can call
                         // TreePush/TreePop yourself if desired.
CHERRY_API void TreePush(const void *ptr_id = NULL); // "
CHERRY_API void TreePop();                           // ~ Unindent()+PopId()
CHERRY_API float GetTreeNodeToLabelSpacing(); // horizontal distance preceding label when
                                   // using TreeNode*() or Bullet() ==
                                   // (g.FontSize + style.FramePadding.x*2) for
                                   // a regular unframed TreeNode

CHERRY_API bool CollapsingHeader(const char *label, ImTextureID *texture,
                      ImGuiTreeNodeFlags flags);
CHERRY_API bool CollapsingHeader(
    const char *label,
    ImGuiTreeNodeFlags flags =
        0); // if returning 'true' the header is open. doesn't indent nor push
            // on ID stack. user doesn't have to call TreePop().
CHERRY_API bool CollapsingHeader(
    const char *label, bool *p_visible,
    ImGuiTreeNodeFlags flags =
        0); // when 'p_visible != NULL': if '*p_visible==true' display an
            // additional small close button on upper right of the header which
            // will set the bool to false when clicked, if '*p_visible==false'
            // don't display the header.
CHERRY_API void SetNextItemOpen(
    bool is_open,
    ImGuiCond cond = 0); // set next TreeNode/CollapsingHeader open state.
CHERRY_API void BeginTooltip(); // begin/append a tooltip window. to create full-featured
                     // tooltip (with any kind of items).
CHERRY_API void EndTooltip();
CHERRY_API void SetTooltip(const char *fmt, ...) IM_FMTARGS(
    1); // set a text-only tooltip, typically use with ImGui::IsItemHovered().
        // override any previous call to SetTooltip().
CHERRY_API void SetTooltipV(const char *fmt, va_list args) IM_FMTLIST(1);

CHERRY_API bool BeginListBox(
    const char *label,
    const ImVec2 &size = ImVec2(0, 0)); // open a framed scrolling region
CHERRY_API void EndListBox(); // only call EndListBox() if BeginListBox() returned true!
CHERRY_API bool ListBox(const char *label, int *current_item, const char *const items[],
             int items_count, int height_in_items = -1);
CHERRY_API bool ListBox(const char *label, int *current_item,
             bool (*items_getter)(void *data, int idx, const char **out_text),
             void *data, int items_count, int height_in_items = -1);
CHERRY_API void PushItemWidth(
    float item_width); // push width of items for common large "item+label"
                       // widgets. >0.0f: width in pixels, <0.0f align xx pixels
                       // to the right of window (so -FLT_MIN always align width
                       // to the right side).
CHERRY_API void PopItemWidth();
CHERRY_API void SetNextItemWidth(
    float item_width); // set width of the _next_ common large "item+label"
                       // widget. >0.0f: width in pixels, <0.0f align xx pixels
                       // to the right of window (so -FLT_MIN always align width
                       // to the right side)
CHERRY_API float CalcItemWidth(); // width of item given pushed settings and current cursor
                       // position. NOT necessarily the width of last item
                       // unlike most 'Item' functions.
CHERRY_API void PushTextWrapPos(
    float wrap_local_pos_x =
        0.0f); // push word-wrapping position for Text*() commands. < 0.0f: no
               // wrapping; 0.0f: wrap to end of window (or column); > 0.0f:
               // wrap at 'wrap_pos_x' position in window local space
CHERRY_API void PopTextWrapPos();
CHERRY_API bool Begin(const char *name, bool *p_open = NULL, ImGuiWindowFlags flags = 0);
CHERRY_API bool Begin(const char *name, ImTextureID logo, bool *p_open = NULL,
           ImGuiWindowFlags flags = 0, ImVec2 internal_padding = ImVec2(0, 0));

CHERRY_API void End();
CHERRY_API bool IsKeyDown(ImGuiKey key); // is key being held.
CHERRY_API bool IsKeyPressed(
    ImGuiKey key,
    bool repeat = true); // was key pressed (went from !Down to Down)? if
                         // repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
CHERRY_API bool IsKeyReleased(ImGuiKey key); // was key released (went from Down to !Down)?
CHERRY_API int GetKeyPressedAmount(
    ImGuiKey key, float repeat_delay,
    float rate); // uses provided repeat rate/delay. return a count, most often
                 // 0 or 1 but might be >1 if RepeatRate is small enough that
                 // DeltaTime > RepeatRate
CHERRY_API const char *
GetKeyName(ImGuiKey key); // [DEBUG] returns English name of the key. Those
                          // names a provided for debugging purpose and are not
                          // meant to be saved persistently not compared.
CHERRY_API void CaptureKeyboardFromApp(
    bool want_capture_keyboard_value =
        true); // attention: misleading name! manually override
               // io.WantCaptureKeyboard flag next frame (said flag is entirely
               // left for your application to handle). e.g. force capture
               // keyboard when your widget is being hovered. This is equivalent
               // to setting "io.WantCaptureKeyboard =
               // want_capture_keyboard_value"; after the next NewFrame() call.
CHERRY_API bool InputText(const char *label, char *buf, size_t buf_size,
               ImGuiInputTextFlags flags = 0,
               ImGuiInputTextCallback callback = NULL, void *user_data = NULL);
CHERRY_API bool InputTextWithImage(const char *label, ImTextureID texture, char *buf,
                        size_t buf_size, ImGuiInputTextFlags flags = 0,
                        ImGuiInputTextCallback callback = NULL,
                        void *user_data = NULL);
CHERRY_API bool InputTextMultiline(const char *label, char *buf, size_t buf_size,
                        const ImVec2 &size = ImVec2(0, 0),
                        ImGuiInputTextFlags flags = 0,
                        ImGuiInputTextCallback callback = NULL,
                        void *user_data = NULL);
CHERRY_API bool InputTextWithHint(const char *label, const char *hint, char *buf,
                       size_t buf_size, ImGuiInputTextFlags flags = 0,
                       ImGuiInputTextCallback callback = NULL,
                       void *user_data = NULL);
CHERRY_API bool InputFloat(const char *label, float *v, float step = 0.0f,
                float step_fast = 0.0f, const char *format = "%.3f",
                ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputColorFloat(const char *label, float *v,
                     ImVec4 color = ImVec4(0, 0, 0, 0), float step = 0.0f,
                     float step_fast = 0.0f, const char *format = "%.3f",
                     ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputFloat2(const char *label, float v[2], const char *format = "%.3f",
                 ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputFloat3(const char *label, float v[3], const char *format = "%.3f",
                 ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputFloat4(const char *label, float v[4], const char *format = "%.3f",
                 ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputInt(const char *label, int *v, int step = 1, int step_fast = 100,
              ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputInt2(const char *label, int v[2], ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputInt3(const char *label, int v[3], ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputInt4(const char *label, int v[4], ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputDouble(const char *label, double *v, double step = 0.0,
                 double step_fast = 0.0, const char *format = "%.6f",
                 ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputColorScalar(const char *label, ImGuiDataType data_type, ImVec4 color,
                      void *p_data, const void *p_step = NULL,
                      const void *p_step_fast = NULL, const char *format = NULL,
                      ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputScalar(const char *label, ImGuiDataType data_type, void *p_data,
                 const void *p_step = NULL, const void *p_step_fast = NULL,
                 const char *format = NULL, ImGuiInputTextFlags flags = 0);
CHERRY_API bool InputScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                  int components, const void *p_step = NULL,
                  const void *p_step_fast = NULL, const char *format = NULL,
                  ImGuiInputTextFlags flags = 0);

CHERRY_API void SetItemDefaultFocus(); // make last item the default focused item of a
                            // window.
CHERRY_API void SetKeyboardFocusHere(
    int offset = 0); // focus keyboard on the next widget. Use positive 'offset'
                     // to access sub components of a multiple component widget.
                     // Use -1 to access previous widget.
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
CHERRY_API int GetKeyIndex(ImGuiKey key); // map ImGuiKey_* values into legacy native key
                               // index. == io.KeyMap[key]
#else
CHERRY_API static inline int GetKeyIndex(ImGuiKey key) {
  IM_ASSERT(key >= ImGuiKey_NamedKey_BEGIN && key < ImGuiKey_NamedKey_END &&
            "ImGuiKey and native_index was merged together and native_index is "
            "disabled by IMGUI_DISABLE_OBSOLETE_KEYIO. "
            "Please switch to ImGuiKey.");
  return key;
}
#endif
CHERRY_API bool DragScalar(const char *label, ImGuiDataType data_type, void *p_data,
                float v_speed, const void *p_min, const void *p_max,
                const char *format, float power);
CHERRY_API bool DragScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                 int components, float v_speed, const void *p_min,
                 const void *p_max, const char *format, float power);
CHERRY_API bool SliderScalar(const char *label, ImGuiDataType data_type, void *p_data,
                  const void *p_min, const void *p_max, const char *format,
                  float power);
CHERRY_API bool SliderScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                   int components, const void *p_min, const void *p_max,
                   const char *format, float power);

CHERRY_API bool BeginChildFrame(
    ImGuiID id, const ImVec2 &size,
    ImGuiWindowFlags flags = 0); // helper to create a child window / scrolling
                                 // region that looks like a normal widget frame
CHERRY_API void EndChildFrame();            // always call EndChildFrame() regardless of
                      // BeginChildFrame() return values (which indicates a
                      // collapsed/clipped window)

CHERRY_API bool ColorEdit3(const char *label, float col[3], ImGuiColorEditFlags flags = 0);
CHERRY_API bool ColorEdit4(const char *label, float col[4], ImGuiColorEditFlags flags = 0);
CHERRY_API bool ColorPicker3(const char *label, float col[3],
                  ImGuiColorEditFlags flags = 0);
CHERRY_API bool ColorPicker4(const char *label, float col[4],
                  ImGuiColorEditFlags flags = 0, const float *ref_col = NULL);
CHERRY_API bool ColorButton(
    const char *desc_id, const ImVec4 &col, ImGuiColorEditFlags flags = 0,
    ImVec2 size = ImVec2(0, 0)); // display a color square/button, hover for
                                 // details, return true when pressed.
CHERRY_API void SetColorEditOptions(
    ImGuiColorEditFlags
        flags); // initialize current options (generally on application startup)
                // if you want to select a default format, picker type, etc.
                // User will be able to change many settings, unless you pass
                // the _NoOptions flag to your calls.
CHERRY_API void BeginDisabled(bool disabled = true);
CHERRY_API void EndDisabled();
CHERRY_API void RenderArrow(ImDrawList *draw_list, ImTextureID *texture, ImVec2 pos,
                 ImU32 col, ImGuiDir dir, float scale);
CHERRY_API void RenderArrow(ImDrawList *draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir,
                 float scale = 1.0f);
CHERRY_API void RenderBullet(ImDrawList *draw_list, ImTextureID *texture, ImVec2 pos,
                  ImU32 col);
CHERRY_API void RenderBullet(ImDrawList *draw_list, ImVec2 pos, ImU32 col);
CHERRY_API void RenderCheckMark(ImDrawList *draw_list, ImVec2 pos, ImU32 col, float sz);
CHERRY_API void RenderArrowPointingAt(ImDrawList *draw_list, ImVec2 pos, ImVec2 half_sz,
                           ImGuiDir direction, ImU32 col);
CHERRY_API void RenderArrowDockMenu(ImDrawList *draw_list, ImVec2 p_min, float sz,
                         ImU32 col);
CHERRY_API void RenderRectFilledRangeH(ImDrawList *draw_list, const ImRect &rect,
                            ImU32 col, float x_start_norm, float x_end_norm,
                            float rounding);
CHERRY_API void RenderRectFilledWithHole(ImDrawList *draw_list, ImRect outer, ImRect inner,
                              ImU32 col, float rounding);
CHERRY_API ImDrawFlags CalcRoundingFlagsForRectInRect(const ImRect &r_in,
                                           const ImRect &r_outer,
                                           float threshold);
CHERRY_API int TableGetColumnCount(); // return number of columns (value passed to
                           // BeginTable)
CHERRY_API int TableGetColumnIndex(); // return current column index.
CHERRY_API int TableGetRowIndex();    // return current row index.
CHERRY_API const char *TableGetColumnName(
    int column_n = -1); // return "" if column didn't have a name declared by
                        // TableSetupColumn(). Pass -1 to use current column.
CHERRY_API ImGuiTableColumnFlags TableGetColumnFlags(
    int column_n = -1); // return column flags so you can query their
                        // Enabled/Visible/Sorted/Hovered status flags. Pass -1
                        // to use current column.
CHERRY_API void TableSetColumnEnabled(
    int column_n,
    bool v); // change user accessible enabled/disabled state of a column. Set
             // to false to hide the column. User can use the context menu to
             // change this themselves (right-click in headers, or right-click
             // in columns body with ImGuiTableFlags_ContextMenuInBody)
CHERRY_API void TableSetBgColor(
    ImGuiTableBgTarget target, ImU32 color,
    int column_n = -1); // change the color of a cell, row, or column. See
                        // ImGuiTableBgTarget_ flags for details.

CHERRY_API bool DragFloat(
    const char *label, float *v, float v_speed = 1.0f, float v_min = 0.0f,
    float v_max = 0.0f, const char *format = "%.3f",
    ImGuiSliderFlags flags = 0); // If v_min >= v_max we have no bound
CHERRY_API bool DragFloat2(const char *label, float v[2], float v_speed = 1.0f,
                float v_min = 0.0f, float v_max = 0.0f,
                const char *format = "%.3f", ImGuiSliderFlags flags = 0);
CHERRY_API bool DragFloat3(const char *label, float v[3], float v_speed = 1.0f,
                float v_min = 0.0f, float v_max = 0.0f,
                const char *format = "%.3f", ImGuiSliderFlags flags = 0);
CHERRY_API bool DragFloat4(const char *label, float v[4], float v_speed = 1.0f,
                float v_min = 0.0f, float v_max = 0.0f,
                const char *format = "%.3f", ImGuiSliderFlags flags = 0);
CHERRY_API bool DragFloatRange2(const char *label, float *v_current_min,
                     float *v_current_max, float v_speed = 1.0f,
                     float v_min = 0.0f, float v_max = 0.0f,
                     const char *format = "%.3f", const char *format_max = NULL,
                     ImGuiSliderFlags flags = 0);
CHERRY_API bool DragInt(const char *label, int *v, float v_speed = 1.0f, int v_min = 0,
             int v_max = 0, const char *format = "%d",
             ImGuiSliderFlags flags = 0); // If v_min >= v_max we have no bound
CHERRY_API bool DragInt2(const char *label, int v[2], float v_speed = 1.0f, int v_min = 0,
              int v_max = 0, const char *format = "%d",
              ImGuiSliderFlags flags = 0);
CHERRY_API bool DragInt3(const char *label, int v[3], float v_speed = 1.0f, int v_min = 0,
              int v_max = 0, const char *format = "%d",
              ImGuiSliderFlags flags = 0);
CHERRY_API bool DragInt4(const char *label, int v[4], float v_speed = 1.0f, int v_min = 0,
              int v_max = 0, const char *format = "%d",
              ImGuiSliderFlags flags = 0);
CHERRY_API bool DragIntRange2(const char *label, int *v_current_min, int *v_current_max,
                   float v_speed = 1.0f, int v_min = 0, int v_max = 0,
                   const char *format = "%d", const char *format_max = NULL,
                   ImGuiSliderFlags flags = 0);
CHERRY_API bool DragScalar(const char *label, ImGuiDataType data_type, void *p_data,
                float v_speed = 1.0f, const void *p_min = NULL,
                const void *p_max = NULL, const char *format = NULL,
                ImGuiSliderFlags flags = 0);
CHERRY_API bool DragScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                 int components, float v_speed = 1.0f, const void *p_min = NULL,
                 const void *p_max = NULL, const char *format = NULL,
                 ImGuiSliderFlags flags = 0);

// Legacy Columns API (prefer using Tables!)
// - You can also use SameLine(pos_x) to mimic simplified columns.
CHERRY_API void Columns(int count = 1, const char *id = NULL, bool border = true);
CHERRY_API void NextColumn();    // next column, defaults to current row or next row if the
                      // current row is finished
CHERRY_API int GetColumnIndex(); // get current column index
CHERRY_API float GetColumnWidth(int column_index = -1); // get column width (in pixels).
                                             // pass -1 to use current column
CHERRY_API void SetColumnWidth(
    int column_index,
    float width); // set column width (in pixels). pass -1 to use current column
CHERRY_API float GetColumnOffset(
    int column_index =
        -1); // get position of column line (in pixels, from the left side of
             // the contents region). pass -1 to use current column, otherwise
             // 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
CHERRY_API void SetColumnOffset(int column_index,
                     float offset_x); // set position of column line (in pixels,
                                      // from the left side of the contents
                                      // region). pass -1 to use current column
CHERRY_API int GetColumnsCount();

CHERRY_API void TableSetupColumn(const char *label, ImGuiTableColumnFlags flags = 0,
                      float init_width_or_weight = 0.0f, ImGuiID user_id = 0);
CHERRY_API void TableSetupScrollFreeze(
    int cols,
    int rows);          // lock columns/rows so they stay visible when scrolled.
CHERRY_API void TableHeadersRow(); // submit all headers cells based on data provided to
                        // TableSetupColumn() + submit context menu
CHERRY_API void TableHeader(
    const char *label); // submit one header cell manually (rarely used)

CHERRY_API bool BeginTable(const char *str_id, int column, ImGuiTableFlags flags = 0,
                const ImVec2 &outer_size = ImVec2(0.0f, 0.0f),
                float inner_width = 0.0f);
CHERRY_API void EndTable(); // only call EndTable() if BeginTable() returns true!
CHERRY_API void TableNextRow(
    ImGuiTableRowFlags row_flags = 0,
    float min_row_height = 0.0f); // append into the first cell of a new row.
CHERRY_API bool TableNextColumn(); // append into the next column (or first column of next
                        // row if currently in last column). Return true when
                        // column is visible.
CHERRY_API bool TableSetColumnIndex(int column_n); // append into the specified column.
                                        // Return true when column is visible.
CHERRY_API void TableOpenContextMenu(int column_n = -1);
CHERRY_API void TableSetColumnWidth(int column_n, float width);
CHERRY_API void TableSetColumnSortDirection(int column_n,
                                 ImGuiSortDirection sort_direction,
                                 bool append_to_sort_specs);
CHERRY_API int TableGetHoveredColumn(); // May use (TableGetColumnFlags() &
                             // ImGuiTableColumnFlags_IsHovered) instead. Return
                             // hovered column. return -1 when table is not
                             // hovered. return columns_count if the unused
                             // space at the right of visible columns is
                             // hovered.
CHERRY_API float TableGetHeaderRowHeight();
CHERRY_API void TablePushBackgroundChannel();
CHERRY_API void TablePopBackgroundChannel();

CHERRY_API void PushClipRect(const ImVec2 &clip_rect_min, const ImVec2 &clip_rect_max,
                  bool intersect_with_current_clip_rect);
CHERRY_API void PopClipRect();

CHERRY_API const char*   GetClipboardText();
CHERRY_API void          SetClipboardText(const char* text);
CHERRY_API double        GetTime();   
CHERRY_API void          PushAllowKeyboardFocus(bool allow_keyboard_focus);              // == tab stop enable. Allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
CHERRY_API void          PopAllowKeyboardFocus(); 
} // namespace GUI
} // namespace Cherry

namespace CherryGUI = Cherry::GUI;