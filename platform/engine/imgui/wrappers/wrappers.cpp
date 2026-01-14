#include "wrappers.hpp"

namespace Cherry {
namespace GUI {

// Drawlist
void AddLine(ImDrawList *ptr, const ImVec2 &p1, const ImVec2 &p2, ImU32 col,
             float thickness) {
  ptr->AddLine(p1, p2, col, thickness);
}

void AddRect(ImDrawList *ptr, const ImVec2 &p_min, const ImVec2 &p_max,
             ImU32 col, float rounding, ImDrawFlags flags, float thickness) {
  ptr->AddRect(p_min, p_max, col, rounding, flags, thickness);
}

void AddRectFilled(ImDrawList *ptr, const ImVec2 &p_min, const ImVec2 &p_max,
                   ImU32 col, float rounding, ImDrawFlags flags) {
  ptr->AddRectFilled(p_min, p_max, col, rounding, flags);
}

void AddRectFilledMultiColor(ImDrawList *ptr, const ImVec2 &p_min,
                             const ImVec2 &p_max, ImU32 col_upr_left,
                             ImU32 col_upr_right, ImU32 col_bot_right,
                             ImU32 col_bot_left) {
  ptr->AddRectFilledMultiColor(p_min, p_max, col_upr_left, col_upr_right,
                               col_bot_right, col_bot_left);
}

void AddQuad(ImDrawList *ptr, const ImVec2 &p1, const ImVec2 &p2,
             const ImVec2 &p3, const ImVec2 &p4, ImU32 col, float thickness) {
  ptr->AddQuad(p1, p2, p3, p4, col, thickness);
}

void AddQuadFilled(ImDrawList *ptr, const ImVec2 &p1, const ImVec2 &p2,
                   const ImVec2 &p3, const ImVec2 &p4, ImU32 col) {
  ptr->AddQuadFilled(p1, p2, p3, p4, col);
}

void AddTriangle(ImDrawList *ptr, const ImVec2 &p1, const ImVec2 &p2,
                 const ImVec2 &p3, ImU32 col, float thickness) {
  ptr->AddTriangle(p1, p2, p3, col, thickness);
}

void AddTriangleFilled(ImDrawList *ptr, const ImVec2 &p1, const ImVec2 &p2,
                       const ImVec2 &p3, ImU32 col) {
  ptr->AddTriangleFilled(p1, p2, p3, col);
}

void AddCircle(ImDrawList *ptr, const ImVec2 &center, float radius, ImU32 col,
               int num_segments, float thickness) {
  ptr->AddCircle(center, radius, col, num_segments, thickness);
}

void AddCircleFilled(ImDrawList *ptr, const ImVec2 &center, float radius,
                     ImU32 col, int num_segments) {
  ptr->AddCircleFilled(center, radius, col, num_segments);
}

void AddNgon(ImDrawList *ptr, const ImVec2 &center, float radius, ImU32 col,
             int num_segments, float thickness) {
  ptr->AddNgon(center, radius, col, num_segments, thickness);
}

void AddNgonFilled(ImDrawList *ptr, const ImVec2 &center, float radius,
                   ImU32 col, int num_segments) {
  ptr->AddNgonFilled(center, radius, col, num_segments);
}

void AddText(ImDrawList *ptr, const ImVec2 &pos, ImU32 col,
             const char *text_begin, const char *text_end) {
  ptr->AddText(pos, col, text_begin, text_end);
}

void AddText(ImDrawList *ptr, const ImFont *font, float font_size,
             const ImVec2 &pos, ImU32 col, const char *text_begin,
             const char *text_end, float wrap_width,
             const ImVec4 *cpu_fine_clip_rect) {
  ptr->AddText(font, font_size, pos, col, text_begin, text_end, wrap_width,
               cpu_fine_clip_rect);
}

void AddPolyline(ImDrawList *ptr, const ImVec2 *points, int num_points,
                 ImU32 col, ImDrawFlags flags, float thickness) {
  ptr->AddPolyline(points, num_points, col, flags, thickness);
}

void AddConvexPolyFilled(ImDrawList *ptr, const ImVec2 *points, int num_points,
                         ImU32 col) {
  ptr->AddConvexPolyFilled(points, num_points, col);
}

void AddBezierCubic(ImDrawList *ptr, const ImVec2 &p1, const ImVec2 &p2,
                    const ImVec2 &p3, const ImVec2 &p4, ImU32 col,
                    float thickness, int num_segments) {
  ptr->AddBezierCubic(p1, p2, p3, p4, col, thickness, num_segments);
}

void AddBezierQuadratic(ImDrawList *ptr, const ImVec2 &p1, const ImVec2 &p2,
                        const ImVec2 &p3, ImU32 col, float thickness,
                        int num_segments) {
  ptr->AddBezierQuadratic(p1, p2, p3, col, thickness, num_segments);
}

ImVec2 CalcTextSizeA(ImFont *ptr, float size, float max_width, float wrap_width,
                     const char *text_begin, const char *text_end,
                     const char **remaining) {
  return ptr->CalcTextSizeA(size, max_width, wrap_width, text_begin, text_end,
                            remaining);
}

const char *CalcWordWrapPositionA(ImFont *ptr, float scale, const char *text,
                                  const char *text_end, float wrap_width) {
  return ptr->CalcWordWrapPositionA(scale, text, text_end, wrap_width);
}

void RenderChar(ImFont *ptr, ImDrawList *draw_list, float size, ImVec2 pos,
                ImU32 col, ImWchar c) {
  ptr->RenderChar(draw_list, size, pos, col, c);
}

void RenderText(ImFont *ptr, ImDrawList *draw_list, float size, ImVec2 pos,
                ImU32 col, const ImVec4 &clip_rect, const char *text_begin,
                const char *text_end, float wrap_width, bool cpu_fine_clip) {
  ptr->RenderText(draw_list, size, pos, col, clip_rect, text_begin, text_end,
                  wrap_width, cpu_fine_clip);
}

void ClearInputQueueCharacters(ImGuiIO &io) { io.InputQueueCharacters.clear(); }

void PushStyleVar(ImGuiStyleVar idx, float val) {
  ImGui::PushStyleVar(idx, val);
}

void PushStyleVar(ImGuiCol idx, const ImVec2 &val) {
  ImGui::PushStyleVar(idx, val);
}

void PopStyleVar(int count) { ImGui::PopStyleVar(count); }

void PushStyleColor(ImGuiCol idx, const ImVec4 &col) {
  ImGui::PushStyleColor(idx, col);
}

void PushStyleColor(ImGuiCol idx, ImU32 col) {
  ImGui::PushStyleColor(idx, col);
}

void PopStyleColor(int count) { ImGui::PopStyleColor(count); }

bool BeginMenu(const char *label, bool enabled) {
  return ImGui::BeginMenu(label, enabled);
}

bool BeginMenuImage(const char *label, ImTextureID *texture, bool enabled) {
  return ImGui::BeginMenuImage(label, texture, enabled);
}

void EndMenu() { ImGui::EndMenu(); }

bool BeginChildFrame(ImGuiID id, const ImVec2 &size, ImGuiWindowFlags flags) {
  return ImGui::BeginChildFrame(id, size, flags);
} // helper to create a child window / scrolling region that looks like a normal
  // widget frame
void EndChildFrame() { ImGui::EndChildFrame(); }

bool BeginMenuBar() {
  return ImGui::BeginMenuBar();

} // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar
  // flag set on parent window).
bool BeginMenuBar(float offsetY) {
  return ImGui::BeginMenuBar(offsetY);
} // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar
  // flag set on parent window).
bool BeginBottomBar() {
  return ImGui::BeginBottomBar();
} // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar
  // flag set on parent window).

void EndMenuBar() {}
void EndBottomBar() {} // only call EndMenuBar() if BeginMenuBar() returns true!
bool BeginMainMenuBar() {
  return ImGui::BeginMainMenuBar();

} // create and append to a full screen menu-bar.
void EndMainMenuBar() { ImGui::EndMainMenuBar(); }

bool ImageButtonWithTextWithIcon(ImTextureID texIdFirst,
                                 ImTextureID texIdSecond, const char *label,
                                 const ImVec2 &imageSizeFirst,
                                 const ImVec2 &imageSizeSecond,
                                 const ImVec2 &uv0, const ImVec2 &uv1,
                                 int frame_padding, const ImVec4 &bg_col,
                                 const ImVec4 &tint_col) {
  return ImGui::ImageButtonWithTextWithIcon(
      texIdFirst, texIdSecond, label, imageSizeFirst, imageSizeSecond, uv0, uv1,
      frame_padding, bg_col, tint_col);
}

bool ImageButtonWithText(ImTextureID texId, const char *label,
                         const ImVec2 &imageSize, const ImVec2 &uv0,
                         const ImVec2 &uv1, int frame_padding,
                         const ImVec4 &bg_col, const ImVec4 &tint_col) {
  return ImGui::ImageButtonWithText(texId, label, imageSize, uv0, uv1,
                                    frame_padding, bg_col, tint_col);
}

bool RightImageButtonWithText(ImTextureID texId, const char *label,
                              const ImVec2 &imageSize, const ImVec2 &uv0,
                              const ImVec2 &uv1, int frame_padding,
                              const ImVec4 &bg_col, const ImVec4 &tint_col) {
  return ImGui::RightImageButtonWithText(texId, label, imageSize, uv0, uv1,
                                         frame_padding, bg_col, tint_col);
}

// Legacy menu items
bool MenuItem(const char *label, const char *shortcut, bool selected,
              bool enabled) {
  return ImGui::MenuItem(label, shortcut, selected, enabled);
}

bool MenuItem(const char *label, const char *shortcut, bool *p_selected,
              bool enabled) {
  return ImGui::MenuItem(label, shortcut, p_selected, enabled);
}

bool MenuItem(const char *label, const char *shortcut, ImTextureID texture,
              bool selected, bool enabled) {
  return ImGui::MenuItem(label, shortcut, texture, selected, enabled);
}

// Legacy popups
bool BeginPopup(const char *str_id, ImGuiWindowFlags flags) {
  return ImGui::BeginPopup(str_id, flags);
}

bool BeginPopupModal(const char *name, bool *p_open, ImGuiWindowFlags flags) {
  return ImGui::BeginPopupModal(name, p_open, flags);
}

void EndPopup() { ImGui::EndPopup(); }

void OpenPopup(const char *str_id, ImGuiPopupFlags popup_flags) {
  ImGui::OpenPopup(str_id, popup_flags);
}

void OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags) {
  ImGui::OpenPopup(id, popup_flags);
}

void OpenPopupOnItemClick(const char *str_id, ImGuiPopupFlags popup_flags) {
  ImGui::OpenPopupOnItemClick(str_id, popup_flags);
}

void CloseCurrentPopup() { ImGui::CloseCurrentPopup(); }

bool BeginPopupContextItem(const char *str_id, ImGuiPopupFlags popup_flags) {
  return ImGui::BeginPopupContextItem(str_id, popup_flags);
}

bool BeginPopupContextWindow(const char *str_id, ImGuiPopupFlags popup_flags) {
  return ImGui::BeginPopupContextWindow(str_id, popup_flags);
}

bool BeginPopupContextVoid(const char *str_id, ImGuiPopupFlags popup_flags) {
  return ImGui::BeginPopupContextVoid(str_id, popup_flags);
}

ImFont *GetFont() { return ImGui::GetFont(); }

float GetFontSize() { return ImGui::GetFontSize(); }

ImVec2 GetFontTexUvWhitePixel() { return ImGui::GetFontTexUvWhitePixel(); }

ImU32 GetColorU32(ImGuiCol idx, float alpha_mul) {
  return ImGui::GetColorU32(idx, alpha_mul);
}

ImU32 GetColorU32(const ImVec4 &col) { return ImGui::GetColorU32(col); }

ImU32 GetColorU32(ImU32 col) { return ImGui::GetColorU32(col); }

const ImVec4 &GetStyleColorVec4(ImGuiCol idx) {
  return ImGui::GetStyleColorVec4(idx);
}

void PushFont(ImFont *font) { ImGui::PushFont(font); }

void PopFont() { return ImGui::PopFont(); }

// Widgets: Text
void TextUnformatted(const char *text, const char *text_end) {
  ImGui::TextUnformatted(text, text_end);
}

void Text(const char *fmt, ...) {
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

void TextColored(const ImVec4 &col, const char *fmt, ...) {
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

void TextDisabled(const char *fmt, ...) {
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

void TextWrapped(const char *fmt, ...) {
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

void BulletText(const char *fmt, ...) {
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

void LabelText(const char *label, const char *fmt, ...) {
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

void TextV(const char *fmt, va_list args) { ImGui::TextV(fmt, args); }

void TextColoredV(const ImVec4 &col, const char *fmt, va_list args) {
  ImGui::TextColoredV(col, fmt, args);
}

void TextDisabledV(const char *fmt, va_list args) {
  ImGui::TextDisabledV(fmt, args);
}

void TextWrappedV(const char *fmt, va_list args) {
  ImGui::TextWrappedV(fmt, args);
}

void LabelTextV(const char *label, const char *fmt, va_list args) {
  ImGui::LabelTextV(label, fmt, args);
}

void BulletTextV(const char *fmt, va_list args) {
  ImGui::BulletTextV(fmt, args);
}

void SetCurrentFont(ImFont *font) { ImGui::SetCurrentFont(font); }

ImVec2 GetCursorPos() { return ImGui::GetCursorPos(); }

float GetCursorPosX() { return ImGui::GetCursorPosX(); }

float GetCursorPosY() { return ImGui::GetCursorPosY(); }

ImVec2 GetWindowMousePos() {
  ImVec2 mousePos = ImGui::GetMousePos();
  ImVec2 windowPos = ImGui::GetWindowPos();
  return ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);
}

ImVec2 GetScreenMousePos() { return ImGui::GetMousePos(); }

void SetCursorPos(const ImVec2 &local_pos) { ImGui::SetCursorPos(local_pos); }

void SetCursorPosX(float local_x) { ImGui::SetCursorPosX(local_x); }

void SetCursorPosY(float local_y) { ImGui::SetCursorPosY(local_y); }

bool IsItemHovered(ImGuiHoveredFlags flags) {
  return ImGui::IsItemHovered(flags);
}

bool IsItemActive() { return ImGui::IsItemActive(); }

bool IsItemFocused() { return ImGui::IsItemFocused(); }

bool IsItemClicked(ImGuiMouseButton mouse_button) {
  return ImGui::IsItemClicked(mouse_button);
}

bool IsItemVisible() { return ImGui::IsItemVisible(); }

bool IsItemEdited() { return ImGui::IsItemEdited(); }

bool IsItemActivated() { return ImGui::IsItemActivated(); }

bool IsItemDeactivated() { return ImGui::IsItemDeactivated(); }

bool IsItemDeactivatedAfterEdit() {
  return ImGui::IsItemDeactivatedAfterEdit();
}

bool IsItemToggledOpen() { return ImGui::IsItemToggledOpen(); }

bool IsAnyItemHovered() { return ImGui::IsAnyItemHovered(); }

bool IsAnyItemActive() { return ImGui::IsAnyItemActive(); }

bool IsAnyItemFocused() { return ImGui::IsAnyItemFocused(); }

ImVec2 GetItemRectMin() { return ImGui::GetItemRectMin(); }

ImVec2 GetItemRectMax() { return ImGui::GetItemRectMax(); }

ImVec2 GetItemRectSize() { return ImGui::GetItemRectSize(); }

void SetItemAllowOverlap() { ImGui::SetItemAllowOverlap(); }

void Separator() { ImGui::Separator(); }

void SameLine(float offset_from_start_x, float spacing) {
  ImGui::SameLine(offset_from_start_x, spacing);
}

void NewLine() { ImGui::NewLine(); }

void Spacing() { ImGui::Spacing(); }

void Dummy(const ImVec2 &size) { ImGui::Dummy(size); }

void Indent(float indent_w) { ImGui::Indent(indent_w); }

void Unindent(float indent_w) { ImGui::Unindent(indent_w); }

void BeginGroup() { ImGui::BeginGroup(); }

void EndGroup() { ImGui::EndGroup(); }

ImVec2 GetCursorStartPos() { return ImGui::GetCursorStartPos(); }

ImVec2 GetCursorScreenPos() { return ImGui::GetCursorScreenPos(); }

void SetCursorScreenPos(const ImVec2 &pos) { ImGui::SetCursorScreenPos(pos); }

void AlignTextToFramePadding() { ImGui::AlignTextToFramePadding(); }

void AlignTextToFramePadding(float offsetY) {
  ImGui::AlignTextToFramePadding(offsetY);
}

float GetTextLineHeight() { return ImGui::GetTextLineHeight(); }

float GetTextLineHeightWithSpacing() {
  return ImGui::GetTextLineHeightWithSpacing();
}

float GetFrameHeight() { return ImGui::GetFrameHeight(); }

float GetFrameHeightWithSpacing() { return ImGui::GetFrameHeightWithSpacing(); }

void BeginHorizontal(const char *str_id, const ImVec2 &size, float align) {
  ImGui::BeginHorizontal(str_id, size, align);
}

void BeginHorizontal(const void *ptr_id, const ImVec2 &size, float align) {
  ImGui::BeginHorizontal(ptr_id, size, align);
}

void BeginHorizontal(int id, const ImVec2 &size, float align) {
  ImGui::BeginHorizontal(id, size, align);
}

void EndHorizontal() { ImGui::EndHorizontal(); }

void BeginVertical(const char *str_id, const ImVec2 &size, float align) {
  ImGui::BeginVertical(str_id, size, align);
}

void BeginVertical(const void *ptr_id, const ImVec2 &size, float align) {
  ImGui::BeginVertical(ptr_id, size, align);
}

void BeginVertical(int id, const ImVec2 &size, float align) {
  ImGui::BeginVertical(id, size, align);
}

void EndVertical() { ImGui::EndVertical(); }

void Spring(float weight, float spacing) { ImGui::Spring(weight, spacing); }

void SuspendLayout() { ImGui::SuspendLayout(); }

void ResumeLayout() { ImGui::SuspendLayout(); }

bool IsMouseDown(ImGuiMouseButton button) { return ImGui::IsMouseDown(button); }

bool IsMouseClicked(ImGuiMouseButton button, bool repeat) {
  return ImGui::IsMouseClicked(button, repeat);
}

bool IsMouseReleased(ImGuiMouseButton button) {
  return ImGui::IsMouseReleased(button);
}

bool IsMouseDoubleClicked(ImGuiMouseButton button) {
  return ImGui::IsMouseDoubleClicked(button);
}

int GetMouseClickedCount(ImGuiMouseButton button) {
  return ImGui::GetMouseClickedCount(button);
}

bool IsMouseHoveringRect(const ImVec2 &r_min, const ImVec2 &r_max, bool clip) {
  return ImGui::IsMouseHoveringRect(r_min, r_max, clip);
}

bool IsMousePosValid(const ImVec2 *mouse_pos) {
  return ImGui::IsMousePosValid(mouse_pos);
}

bool IsAnyMouseDown() { return ImGui::IsAnyMouseDown(); }

ImVec2 GetMousePos() { return ImGui::GetMousePos(); }

ImVec2 GetMousePosOnOpeningCurrentPopup() {
  return ImGui::GetMousePosOnOpeningCurrentPopup();
}

bool IsMouseDragging(ImGuiMouseButton button, float lock_threshold) {
  return ImGui::IsMouseDragging(button, lock_threshold);
}

ImVec2 GetMouseDragDelta(ImGuiMouseButton button, float lock_threshold) {
  return ImGui::GetMouseDragDelta(button, lock_threshold);
}

void ResetMouseDragDelta(ImGuiMouseButton button) {
  ImGui::ResetMouseDragDelta(button);
}

ImGuiMouseCursor GetMouseCursor() { return ImGui::GetMouseCursor(); }

void SetMouseCursor(ImGuiMouseCursor cursor_type) {
  ImGui::SetMouseCursor(cursor_type);
}

void CaptureMouseFromApp(bool want_capture_mouse_value) {
  ImGui::CaptureMouseFromApp(want_capture_mouse_value);
}

ImVec2 CalcTextSize(const char *text, const char *text_end,
                    bool hide_text_after_double_hash, float wrap_width) {
  return ImGui::CalcTextSize(text, text_end, hide_text_after_double_hash,
                             wrap_width);
}

void PushID(const char *str_id) { ImGui::PushID(str_id); }

void PushID(const char *str_id_begin, const char *str_id_end) {
  ImGui::PushID(str_id_begin, str_id_end);
}

void PushID(const void *ptr_id) { ImGui::PushID(ptr_id); }

void PushID(int int_id) { ImGui::PushID(int_id); }

void PopID() { ImGui::PopID(); }

ImGuiID GetID(const char *str_id) { return ImGui::GetID(str_id); }

ImGuiID GetID(const char *str_id_begin, const char *str_id_end) {
  return ImGui::GetID(str_id_begin, str_id_end);
}

ImGuiID GetID(const void *ptr_id) { return ImGui::GetID(ptr_id); }

bool Button(const char *label, const ImVec2 &size) {
  return ImGui::Button(label, size);
}

bool ImageSizeButtonWithText(ImTextureID texId, float button_width,
                             const char *label, const ImVec2 &imageSize,
                             const ImVec2 &uv0, const ImVec2 &uv1,
                             int frame_padding, const ImVec4 &bg_col,
                             const ImVec4 &tint_col) {
  return ImGui::ImageSizeButtonWithText(texId, button_width, label, imageSize,
                                        uv0, uv1, frame_padding, bg_col,
                                        tint_col);
}

bool SmallButton(const char *label) { return ImGui::SmallButton(label); }

bool InvisibleButton(const char *str_id, const ImVec2 &size,
                     ImGuiButtonFlags flags) {
  return ImGui::InvisibleButton(str_id, size, flags);
}

bool ArrowButton(const char *str_id, ImGuiDir dir) {
  return ImGui::ArrowButton(str_id, dir);
}

void Image(ImTextureID user_texture_id, const ImVec2 &size, const ImVec2 &uv0,
           const ImVec2 &uv1, const ImVec4 &tint_col,
           const ImVec4 &border_col) {
  ImGui::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
}

bool ImageButton(ImTextureID user_texture_id, const ImVec2 &size,
                 const ImVec2 &uv0, const ImVec2 &uv1, int frame_padding,
                 const ImVec4 &bg_col, const ImVec4 &tint_col) {
  return ImGui::ImageButton(user_texture_id, size, uv0, uv1, frame_padding,
                            bg_col, tint_col);
}

bool Checkbox(const char *label, bool *v) { return ImGui::Checkbox(label, v); }

bool CheckboxFlags(const char *label, int *flags, int flags_value) {
  return ImGui::CheckboxFlags(label, flags, flags_value);
}

bool CheckboxFlags(const char *label, unsigned int *flags,
                   unsigned int flags_value) {
  return ImGui::CheckboxFlags(label, flags, flags_value);
}

bool RadioButton(const char *label, bool active) {
  return ImGui::RadioButton(label, active);
}

bool RadioButton(const char *label, int *v, int v_button) {
  return ImGui::RadioButton(label, v, v_button);
}

void ProgressBar(float fraction, const ImVec2 &size_arg, const char *overlay) {
  ImGui::ProgressBar(fraction, size_arg, overlay);
}

void Bullet() { ImGui::Bullet(); }

bool BeginCombo(const char *label, const char *preview_value,
                ImGuiComboFlags flags) {
  return ImGui::BeginCombo(label, preview_value, flags);
}

bool BeginCombo(const char *label, const std::function<void()> &preview,
                ImGuiComboFlags flags) {
  return ImGui::BeginCombo(label, preview, flags);
}

void EndCombo() { ImGui::EndCombo(); }

bool Combo(const char *label, int *current_item, const char *const items[],
           int items_count, int popup_max_height_in_items) {
  return ImGui::Combo(label, current_item, items, items_count,
                      popup_max_height_in_items);
}

bool Combo(const char *label, int *current_item,
           const char *items_separated_by_zeros,
           int popup_max_height_in_items) {
  return ImGui::Combo(label, current_item, items_separated_by_zeros,
                      popup_max_height_in_items);
}

bool Combo(const char *label, int *current_item,
           bool (*items_getter)(void *data, int idx, const char **out_text),
           void *data, int items_count, int popup_max_height_in_items) {
  return ImGui::Combo(label, current_item, items_getter, data, items_count,
                      popup_max_height_in_items);
}

bool BeginChild(const char *str_id, const ImVec2 &size, bool border,
                ImGuiWindowFlags flags) {
  return ImGui::BeginChild(str_id, size, border, flags);
}

bool BeginChild(ImGuiID id, const ImVec2 &size, bool border,
                ImGuiWindowFlags flags) {
  return ImGui::BeginChild(id, size, border, flags);
}

void EndChild() { ImGui::EndChild(); }

bool IsWindowAppearing() { return ImGui::IsWindowAppearing(); }

bool IsWindowCollapsed() { return ImGui::IsWindowCollapsed(); }

bool IsWindowFocused(ImGuiFocusedFlags flags) {
  return ImGui::IsWindowFocused(flags);
}

bool IsWindowHovered(ImGuiHoveredFlags flags) {
  return ImGui::IsWindowHovered(flags);
}

ImDrawList *GetWindowDrawList() { return ImGui::GetWindowDrawList(); }

float GetWindowDpiScale() { return ImGui::GetWindowDpiScale(); }

ImVec2 GetWindowPos() { return ImGui::GetWindowPos(); }

ImVec2 GetWindowSize() { return ImGui::GetWindowSize(); }

float GetWindowWidth() { return ImGui::GetWindowWidth(); }

float GetWindowHeight() { return ImGui::GetWindowHeight(); }

ImGuiViewport *GetWindowViewport() { return ImGui::GetWindowViewport(); }

void SetNextWindowPos(const ImVec2 &pos, ImGuiCond cond, const ImVec2 &pivot) {
  ImGui::SetNextWindowPos(pos, cond, pivot);
}

void SetNextWindowSize(const ImVec2 &size, ImGuiCond cond) {
  ImGui::SetNextWindowSize(size, cond);
}

void SetNextWindowSizeConstraints(const ImVec2 &size_min,
                                  const ImVec2 &size_max,
                                  ImGuiSizeCallback custom_callback,
                                  void *custom_callback_data) {
  ImGui::SetNextWindowSizeConstraints(size_min, size_max, custom_callback,
                                      custom_callback_data);
}

void SetNextWindowContentSize(const ImVec2 &size) {
  ImGui::SetNextWindowContentSize(size);
}

void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond) {
  ImGui::SetNextWindowCollapsed(collapsed, cond);
}

void SetNextWindowFocus() { ImGui::SetNextWindowFocus(); }

void SetNextWindowBgAlpha(float alpha) { ImGui::SetNextWindowBgAlpha(alpha); }

void SetNextWindowViewport(ImGuiID viewport_id) {
  ImGui::SetNextWindowViewport(viewport_id);
}

void SetWindowPos(const ImVec2 &pos, ImGuiCond cond) {
  ImGui::SetWindowPos(pos, cond);
}

void SetWindowSize(const ImVec2 &size, ImGuiCond cond) {
  ImGui::SetWindowSize(size, cond);
}

void SetWindowCollapsed(bool collapsed, ImGuiCond cond) {
  ImGui::SetWindowCollapsed(collapsed, cond);
}

void SetWindowFocus() { ImGui::SetWindowFocus(); }

void SetWindowFontScale(float scale) { ImGui::SetWindowFontScale(scale); }

void SetWindowPos(const char *name, const ImVec2 &pos, ImGuiCond cond) {
  ImGui::SetWindowPos(name, pos, cond);
}

void SetWindowSize(const char *name, const ImVec2 &size, ImGuiCond cond) {
  ImGui::SetWindowSize(name, size, cond);
}

void SetWindowCollapsed(const char *name, bool collapsed, ImGuiCond cond) {
  ImGui::SetWindowCollapsed(name, collapsed, cond);
}

void SetWindowFocus(const char *name) { ImGui::SetWindowFocus(name); }

ImVec2 GetContentRegionAvail() { return ImGui::GetContentRegionAvail(); }

ImVec2 GetContentRegionMax() { return ImGui::GetContentRegionMax(); }

ImVec2 GetWindowContentRegionMin() {
  return ImGui::GetWindowContentRegionMin();
}

ImVec2 GetWindowContentRegionMax() {
  return ImGui::GetWindowContentRegionMax();
}

float GetScrollX() { return ImGui::GetScrollX(); }

float GetScrollY() { return ImGui::GetScrollY(); }

void SetScrollX(float scroll_x) { ImGui::SetScrollX(scroll_x); }

void SetScrollY(float scroll_y) { ImGui::SetScrollY(scroll_y); }

float GetScrollMaxX() { return ImGui::GetScrollMaxX(); }

float GetScrollMaxY() { return ImGui::GetScrollMaxY(); }

void SetScrollHereX(float center_x_ratio) {
  ImGui::SetScrollHereX(center_x_ratio);
}

void SetScrollHereY(float center_y_ratio) {
  ImGui::SetScrollHereY(center_y_ratio);
}

void SetScrollFromPosX(float local_x, float center_x_ratio) {
  ImGui::SetScrollFromPosX(local_x, center_x_ratio);
}

void SetScrollFromPosY(float local_y, float center_y_ratio) {
  ImGui::SetScrollFromPosY(local_y, center_y_ratio);
}

ImVec4 ColorConvertU32ToFloat4(ImU32 in) {
  return ImGui::ColorConvertU32ToFloat4(in);
}

ImU32 ColorConvertFloat4ToU32(const ImVec4 &in) {
  return ImGui::ColorConvertFloat4ToU32(in);
}

void ColorConvertRGBtoHSV(float r, float g, float b, float &out_h, float &out_s,
                          float &out_v) {
  ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
}

void ColorConvertHSVtoRGB(float h, float s, float v, float &out_r, float &out_g,
                          float &out_b) {
  ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
}

ImGuiIO &GetIO() { return ImGui::GetIO(); }

ImGuiStyle &GetStyle() { return ImGui::GetStyle(); }

ImDrawData *GetDrawData() { return ImGui::GetDrawData(); }

void PlotLines(const char *label, const float *values, int values_count,
               int values_offset, const char *overlay_text, float scale_min,
               float scale_max, ImVec2 graph_size, int stride) {
  ImGui::PlotLines(label, values, values_count, values_offset, overlay_text,
                   scale_min, scale_max, graph_size, stride);
}

void PlotLines(const char *label, float (*values_getter)(void *data, int idx),
               void *data, int values_count, int values_offset,
               const char *overlay_text, float scale_min, float scale_max,
               ImVec2 graph_size) {
  ImGui::PlotLines(label, values_getter, data, values_count, values_offset,
                   overlay_text, scale_min, scale_max, graph_size);
}

void PlotHistogram(const char *label, const float *values, int values_count,
                   int values_offset, const char *overlay_text, float scale_min,
                   float scale_max, ImVec2 graph_size, int stride) {
  ImGui::PlotHistogram(label, values, values_count, values_offset, overlay_text,
                       scale_min, scale_max, graph_size, stride);
}

void PlotHistogram(const char *label,
                   float (*values_getter)(void *data, int idx), void *data,
                   int values_count, int values_offset,
                   const char *overlay_text, float scale_min, float scale_max,
                   ImVec2 graph_size) {
  ImGui::PlotHistogram(label, values_getter, data, values_count, values_offset,
                       overlay_text, scale_min, scale_max, graph_size);
}

bool Selectable(const char *label, bool selected, ImGuiSelectableFlags flags,
                const ImVec2 &size) {
  return ImGui::Selectable(label, selected, flags, size);
}

bool Selectable(const char *label, bool *p_selected, ImGuiSelectableFlags flags,
                const ImVec2 &size) {
  return ImGui::Selectable(label, p_selected, flags, size);
}

bool TreeNode(const char *label) { return ImGui::TreeNode(label); }

bool TreeNode(const char *str_id, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  bool result = ImGui::TreeNodeV(str_id, fmt, args);
  va_end(args);
  return result;
}

bool TreeNode(const void *ptr_id, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  bool result = ImGui::TreeNodeV(ptr_id, fmt, args);
  va_end(args);
  return result;
}

bool TreeNodeV(const char *str_id, const char *fmt, va_list args) {
  return ImGui::TreeNodeV(str_id, fmt, args);
}

bool TreeNodeV(const void *ptr_id, const char *fmt, va_list args) {
  return ImGui::TreeNodeV(ptr_id, fmt, args);
}

bool TreeNodeEx(const char *label, ImGuiTreeNodeFlags flags) {
  return ImGui::TreeNodeEx(label, flags);
}

bool TreeNodeEx(const char *str_id, ImGuiTreeNodeFlags flags, const char *fmt,
                ...) {
  va_list args;
  va_start(args, fmt);
  bool result = ImGui::TreeNodeExV(str_id, flags, fmt, args);
  va_end(args);
  return result;
}

bool TreeNodeEx(const void *ptr_id, ImGuiTreeNodeFlags flags, const char *fmt,
                ...) {
  va_list args;
  va_start(args, fmt);
  bool result = ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
  va_end(args);
  return result;
}

bool TreeNodeExV(const char *str_id, ImGuiTreeNodeFlags flags, const char *fmt,
                 va_list args) {
  return ImGui::TreeNodeExV(str_id, flags, fmt, args);
}

bool TreeNodeExV(const void *ptr_id, ImGuiTreeNodeFlags flags, const char *fmt,
                 va_list args) {
  return ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
}

void TreePush(const char *str_id) { ImGui::TreePush(str_id); }

void TreePop() { ImGui::TreePop(); }

bool CollapsingHeader(const char *label, ImGuiTreeNodeFlags flags) {
  return ImGui::CollapsingHeader(label, flags);
}

void SetNextItemOpen(bool is_open, ImGuiCond cond) {
  ImGui::SetNextItemOpen(is_open, cond);
}

void BeginTooltip() { ImGui::BeginTooltip(); }

void EndTooltip() { ImGui::EndTooltip(); }

void SetTooltip(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  ImGui::SetTooltipV(fmt, args);
  va_end(args);
}

bool BeginListBox(const char *label, const ImVec2 &size) {
  return ImGui::BeginListBox(label, size);
}

void EndListBox() { ImGui::EndListBox(); }

bool ListBox(const char *label, int *current_item, const char *const items[],
             int items_count, int height_in_items) {
  return ImGui::ListBox(label, current_item, items, items_count,
                        height_in_items);
}

bool ListBox(const char *label, int *current_item,
             bool (*items_getter)(void *data, int idx, const char **out_text),
             void *data, int items_count, int height_in_items) {
  return ImGui::ListBox(label, current_item, items_getter, data, items_count,
                        height_in_items);
}

void PushItemWidth(float item_width) { ImGui::PushItemWidth(item_width); }

void PopItemWidth() { ImGui::PopItemWidth(); }

void SetNextItemWidth(float item_width) { ImGui::SetNextItemWidth(item_width); }

float CalcItemWidth() { return ImGui::CalcItemWidth(); }

void PushTextWrapPos(float wrap_local_pos_x) {
  ImGui::PushTextWrapPos(wrap_local_pos_x);
}

void PopTextWrapPos() { ImGui::PopTextWrapPos(); }

bool Begin(const char *name, bool *p_open, ImGuiWindowFlags flags) {
  return ImGui::Begin(name, p_open, flags);
}

bool Begin(const char *name, ImTextureID logo, bool *p_open,
           ImGuiWindowFlags flags, ImVec2 internal_padding) {
  return ImGui::Begin(name, logo, p_open, flags, internal_padding);
}

void End() { ImGui::End(); }

bool IsKeyDown(ImGuiKey key) { return ImGui::IsKeyDown(key); }

bool IsKeyPressed(ImGuiKey key, bool repeat) {
  return ImGui::IsKeyPressed(key, repeat);
}

bool IsKeyReleased(ImGuiKey key) { return ImGui::IsKeyReleased(key); }

int GetKeyPressedAmount(ImGuiKey key, float repeat_delay, float rate) {
  return ImGui::GetKeyPressedAmount(key, repeat_delay, rate);
}

const char *GetKeyName(ImGuiKey key) { return ImGui::GetKeyName(key); }

void CaptureKeyboardFromApp(bool want_capture_keyboard_value) {
  ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value);
}

bool InputText(const char *label, char *buf, size_t buf_size,
               ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void *user_data) {
  return ImGui::InputText(label, buf, buf_size, flags, callback, user_data);
}

bool InputTextWithImage(const char *label, ImTextureID texture, char *buf,
                        size_t buf_size, ImGuiInputTextFlags flags,
                        ImGuiInputTextCallback callback, void *user_data) {
  return ImGui::InputTextWithImage(label, texture, buf, buf_size, flags,
                                   callback, user_data);
}

bool InputTextMultiline(const char *label, char *buf, size_t buf_size,
                        const ImVec2 &size, ImGuiInputTextFlags flags,
                        ImGuiInputTextCallback callback, void *user_data) {
  return ImGui::InputTextMultiline(label, buf, buf_size, size, flags, callback,
                                   user_data);
}

bool InputTextWithHint(const char *label, const char *hint, char *buf,
                       size_t buf_size, ImGuiInputTextFlags flags,
                       ImGuiInputTextCallback callback, void *user_data) {
  return ImGui::InputTextWithHint(label, hint, buf, buf_size, flags, callback,
                                  user_data);
}

bool InputFloat(const char *label, float *v, float step, float step_fast,
                const char *format, ImGuiInputTextFlags flags) {
  return ImGui::InputFloat(label, v, step, step_fast, format, flags);
}

bool InputColorFloat(const char *label, float *v, ImVec4 color, float step,
                     float step_fast, const char *format,
                     ImGuiInputTextFlags flags) {
  return ImGui::InputColorFloat(label, v, color, step, step_fast, format,
                                flags);
}

bool InputFloat2(const char *label, float v[2], const char *format,
                 ImGuiInputTextFlags flags) {
  return ImGui::InputFloat2(label, v, format, flags);
}

bool InputFloat3(const char *label, float v[3], const char *format,
                 ImGuiInputTextFlags flags) {
  return ImGui::InputFloat3(label, v, format, flags);
}

bool InputFloat4(const char *label, float v[4], const char *format,
                 ImGuiInputTextFlags flags) {
  return ImGui::InputFloat4(label, v, format, flags);
}

bool InputInt(const char *label, int *v, int step, int step_fast,
              ImGuiInputTextFlags flags) {
  return ImGui::InputInt(label, v, step, step_fast, flags);
}

bool InputInt2(const char *label, int v[2], ImGuiInputTextFlags flags) {
  return ImGui::InputInt2(label, v, flags);
}

bool InputInt3(const char *label, int v[3], ImGuiInputTextFlags flags) {
  return ImGui::InputInt3(label, v, flags);
}

bool InputInt4(const char *label, int v[4], ImGuiInputTextFlags flags) {
  return ImGui::InputInt4(label, v, flags);
}

bool InputDouble(const char *label, double *v, double step, double step_fast,
                 const char *format, ImGuiInputTextFlags flags) {
  return ImGui::InputDouble(label, v, step, step_fast, format, flags);
}

bool InputColorScalar(const char *label, ImGuiDataType data_type, ImVec4 color,
                      void *p_data, const void *p_step, const void *p_step_fast,
                      const char *format, ImGuiInputTextFlags flags) {
  return ImGui::InputColorScalar(label, data_type, color, p_data, p_step,
                                 p_step_fast, format, flags);
}

bool InputScalar(const char *label, ImGuiDataType data_type, void *p_data,
                 const void *p_step, const void *p_step_fast,
                 const char *format, ImGuiInputTextFlags flags) {
  return ImGui::InputScalar(label, data_type, p_data, p_step, p_step_fast,
                            format, flags);
}

bool InputScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                  int components, const void *p_step, const void *p_step_fast,
                  const char *format, ImGuiInputTextFlags flags) {
  return ImGui::InputScalarN(label, data_type, p_data, components, p_step,
                             p_step_fast, format, flags);
}

void SetItemDefaultFocus() { ImGui::SetItemDefaultFocus(); }

void SetKeyboardFocusHere(int offset) { ImGui::SetKeyboardFocusHere(offset); }

#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
int GetKeyIndex(ImGuiKey key) { return ImGui::GetKeyIndex(key); }
#endif

bool DragScalar(const char *label, ImGuiDataType data_type, void *p_data,
                float v_speed, const void *p_min, const void *p_max,
                const char *format, float power) {
  return ImGui::DragScalar(label, data_type, p_data, v_speed, p_min, p_max,
                           format, power);
}

bool DragScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                 int components, float v_speed, const void *p_min,
                 const void *p_max, const char *format, float power) {
  return ImGui::DragScalarN(label, data_type, p_data, components, v_speed,
                            p_min, p_max, format, power);
}

bool SliderScalar(const char *label, ImGuiDataType data_type, void *p_data,
                  const void *p_min, const void *p_max, const char *format,
                  float power) {
  return ImGui::SliderScalar(label, data_type, p_data, p_min, p_max, format,
                             power);
}

bool SliderScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                   int components, const void *p_min, const void *p_max,
                   const char *format, float power) {
  return ImGui::SliderScalarN(label, data_type, p_data, components, p_min,
                              p_max, format, power);
}

bool ColorEdit3(const char *label, float col[3], ImGuiColorEditFlags flags) {
  return ImGui::ColorEdit3(label, col, flags);
}

bool ColorEdit4(const char *label, float col[4], ImGuiColorEditFlags flags) {
  return ImGui::ColorEdit4(label, col, flags);
}

bool ColorPicker3(const char *label, float col[3], ImGuiColorEditFlags flags) {
  return ImGui::ColorPicker3(label, col, flags);
}

bool ColorPicker4(const char *label, float col[4], ImGuiColorEditFlags flags,
                  const float *ref_col) {
  return ImGui::ColorPicker4(label, col, flags, ref_col);
}

bool ColorButton(const char *desc_id, const ImVec4 &col,
                 ImGuiColorEditFlags flags, ImVec2 size) {
  return ImGui::ColorButton(desc_id, col, flags, size);
}

void SetColorEditOptions(ImGuiColorEditFlags flags) {
  ImGui::SetColorEditOptions(flags);
}

void BeginDisabled(bool disabled) { ImGui::BeginDisabled(disabled); }

void EndDisabled() { ImGui::EndDisabled(); }

void RenderArrow(ImDrawList *draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir,
                 float scale) {
  ImGui::RenderArrow(draw_list, pos, col, dir, scale);
}

void RenderBullet(ImDrawList *draw_list, ImVec2 pos, ImU32 col) {
  ImGui::RenderBullet(draw_list, pos, col);
}

void RenderCheckMark(ImDrawList *draw_list, ImVec2 pos, ImU32 col, float sz) {
  ImGui::RenderCheckMark(draw_list, pos, col, sz);
}

void RenderArrowPointingAt(ImDrawList *draw_list, ImVec2 pos, ImVec2 half_sz,
                           ImGuiDir direction, ImU32 col) {
  ImGui::RenderArrowPointingAt(draw_list, pos, half_sz, direction, col);
}

void RenderArrowDockMenu(ImDrawList *draw_list, ImVec2 p_min, float sz,
                         ImU32 col) {
  ImGui::RenderArrowDockMenu(draw_list, p_min, sz, col);
}

void RenderRectFilledRangeH(ImDrawList *draw_list, const ImRect &rect,
                            ImU32 col, float x_start_norm, float x_end_norm,
                            float rounding) {
  ImGui::RenderRectFilledRangeH(draw_list, rect, col, x_start_norm, x_end_norm,
                                rounding);
}

void RenderRectFilledWithHole(ImDrawList *draw_list, ImRect outer, ImRect inner,
                              ImU32 col, float rounding) {
  ImGui::RenderRectFilledWithHole(draw_list, outer, inner, col, rounding);
}

ImDrawFlags CalcRoundingFlagsForRectInRect(const ImRect &r_in,
                                           const ImRect &r_outer,
                                           float threshold) {
  return ImGui::CalcRoundingFlagsForRectInRect(r_in, r_outer, threshold);
}

bool DragFloat(const char *label, float *v, float v_speed, float v_min,
               float v_max, const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragFloat2(const char *label, float v[2], float v_speed, float v_min,
                float v_max, const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragFloat3(const char *label, float v[3], float v_speed, float v_min,
                float v_max, const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragFloat4(const char *label, float v[4], float v_speed, float v_min,
                float v_max, const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragFloatRange2(const char *label, float *v_current_min,
                     float *v_current_max, float v_speed, float v_min,
                     float v_max, const char *format, const char *format_max,
                     ImGuiSliderFlags flags) {
  return ImGui::DragFloatRange2(label, v_current_min, v_current_max, v_speed,
                                v_min, v_max, format, format_max, flags);
}

bool DragInt(const char *label, int *v, float v_speed, int v_min, int v_max,
             const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragInt(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragInt2(const char *label, int v[2], float v_speed, int v_min, int v_max,
              const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragInt2(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragInt3(const char *label, int v[3], float v_speed, int v_min, int v_max,
              const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragInt3(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragInt4(const char *label, int v[4], float v_speed, int v_min, int v_max,
              const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragInt4(label, v, v_speed, v_min, v_max, format, flags);
}

bool DragIntRange2(const char *label, int *v_current_min, int *v_current_max,
                   float v_speed, int v_min, int v_max, const char *format,
                   const char *format_max, ImGuiSliderFlags flags) {
  return ImGui::DragIntRange2(label, v_current_min, v_current_max, v_speed,
                              v_min, v_max, format, format_max, flags);
}

bool DragScalar(const char *label, ImGuiDataType data_type, void *p_data,
                float v_speed, const void *p_min, const void *p_max,
                const char *format, ImGuiSliderFlags flags) {
  return ImGui::DragScalar(label, data_type, p_data, v_speed, p_min, p_max,
                           format, flags);
}

bool DragScalarN(const char *label, ImGuiDataType data_type, void *p_data,
                 int components, float v_speed, const void *p_min,
                 const void *p_max, const char *format,
                 ImGuiSliderFlags flags) {
  return ImGui::DragScalarN(label, data_type, p_data, components, v_speed,
                            p_min, p_max, format, flags);
}

void TableSetupColumn(const char *label, ImGuiTableColumnFlags flags,
                      float init_width_or_weight, ImGuiID user_id) {
  ImGui::TableSetupColumn(label, flags, init_width_or_weight, user_id);
}

void TableSetupScrollFreeze(int cols, int rows) {
  ImGui::TableSetupScrollFreeze(cols, rows);
}

void TableHeadersRow() { ImGui::TableHeadersRow(); }

void TableHeader(const char *label) { ImGui::TableHeader(label); }

int TableGetColumnCount() { return ImGui::TableGetColumnCount(); }

int TableGetColumnIndex() { return ImGui::TableGetColumnIndex(); }

int TableGetRowIndex() { return ImGui::TableGetRowIndex(); }

const char *TableGetColumnName(int column_n) {
  return ImGui::TableGetColumnName(column_n);
}

ImGuiTableColumnFlags TableGetColumnFlags(int column_n) {
  return ImGui::TableGetColumnFlags(column_n);
}

void TableSetColumnEnabled(int column_n, bool v) {
  ImGui::TableSetColumnEnabled(column_n, v);
}

void TableSetBgColor(ImGuiTableBgTarget target, ImU32 color, int column_n) {
  ImGui::TableSetBgColor(target, color, column_n);
}

// API des colonnes héritée (legacy)

void Columns(int count, const char *id, bool border) {
  ImGui::Columns(count, id, border);
}

void NextColumn() { ImGui::NextColumn(); }

int GetColumnIndex() { return ImGui::GetColumnIndex(); }

float GetColumnWidth(int column_index) {
  return ImGui::GetColumnWidth(column_index);
}

void SetColumnWidth(int column_index, float width) {
  ImGui::SetColumnWidth(column_index, width);
}

float GetColumnOffset(int column_index) {
  return ImGui::GetColumnOffset(column_index);
}

void SetColumnOffset(int column_index, float offset_x) {
  ImGui::SetColumnOffset(column_index, offset_x);
}

int GetColumnsCount() { return ImGui::GetColumnsCount(); }
bool BeginTable(const char *str_id, int column, ImGuiTableFlags flags,
                const ImVec2 &outer_size, float inner_width) {
  return ImGui::BeginTable(str_id, column, flags, outer_size, inner_width);
}

void EndTable() { ImGui::EndTable(); }

void TableNextRow(ImGuiTableRowFlags row_flags, float min_row_height) {
  ImGui::TableNextRow(row_flags, min_row_height);
}

bool TableNextColumn() { return ImGui::TableNextColumn(); }

bool TableSetColumnIndex(int column_n) {
  return ImGui::TableSetColumnIndex(column_n);
}
void TableOpenContextMenu(int column_n) {
  ImGui::TableOpenContextMenu(column_n);
}
void TableSetColumnWidth(int column_n, float width) {
  ImGui::TableSetColumnWidth(column_n, width);
}
void TableSetColumnSortDirection(int column_n,
                                 ImGuiSortDirection sort_direction,
                                 bool append_to_sort_specs) {
  ImGui::TableSetColumnSortDirection(column_n, sort_direction,
                                     append_to_sort_specs);
}

int TableGetHoveredColumn() { return ImGui::TableGetHoveredColumn(); }
float TableGetHeaderRowHeight() { return ImGui::TableGetHeaderRowHeight(); }
void TablePushBackgroundChannel() { ImGui::TablePushBackgroundChannel(); }
void TablePopBackgroundChannel() { ImGui::TablePopBackgroundChannel(); }
bool TextButtonWithImageWithIcon(ImTextureID texIdFirst,
                                 ImTextureID texIdSecond, const char *label,
                                 const ImVec2 &imageSizeFirst,
                                 const ImVec2 &imageSizeSecond,
                                 const ImVec2 &uv0, const ImVec2 &uv1,
                                 int frame_padding, const ImVec4 &bg_col,
                                 const ImVec4 &tint_col) {
  return false;
  // return ImGui::TextButtonWithImageWithIcon(texIdFirst, texIdSecond, label,
  // imageSizeFirst, imageSizeSecond, uv0, uv1, frame_padding, bg_col,
  // tint_col);
}

void PushClipRect(const ImVec2 &clip_rect_min, const ImVec2 &clip_rect_max,
                  bool intersect_with_current_clip_rect) {
  ImGui::PushClipRect(clip_rect_min, clip_rect_max,
                      intersect_with_current_clip_rect);
}

void PopClipRect() { ImGui::PopClipRect(); }

const char *GetClipboardText() {
  ImGuiContext &g = *GImGui;
  return g.IO.GetClipboardTextFn
             ? g.IO.GetClipboardTextFn(g.IO.ClipboardUserData)
             : "";
}

void SetClipboardText(const char *text) {
  ImGuiContext &g = *GImGui;
  if (g.IO.SetClipboardTextFn)
    g.IO.SetClipboardTextFn(g.IO.ClipboardUserData, text);
}

double GetTime() { return GImGui->Time; }

void PushAllowKeyboardFocus(bool allow_keyboard_focus) {
  ImGui::PushItemFlag(ImGuiItemFlags_NoTabStop, !allow_keyboard_focus);
}

void PopAllowKeyboardFocus() { ImGui::PopItemFlag(); }
bool IsPopupOpen(const char *str_id, ImGuiPopupFlags flags) {
  return ImGui::IsPopupOpen(str_id, flags);
} // return true if the popup is open.

} // namespace GUI

} // namespace Cherry