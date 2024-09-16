#include "buttons.h"

namespace ImGui
{
bool UIKit_ImageButtonWithText(ImTextureID texId, const char* label, const ImVec2& imageSize = ImVec2(0,0), const ImVec2& uv0 = ImVec2(0,0), const ImVec2& uv1 = ImVec2(1,1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1)) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
    return false;

    ImVec2 size = imageSize;
    if (size.x<=0 && size.y<=0) {size.x=size.y=ImGui::GetTextLineHeightWithSpacing();}
    else {
        if (size.x<=0)          size.x=size.y;
        else if (size.y<=0)     size.y=size.x;
        size*=window->FontWindowScale*ImGui::GetIO().FontGlobalScale;
    }

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);
    const ImVec2 textSize = ImGui::CalcTextSize(label,NULL,true);
    const bool hasText = textSize.x>0;

    const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImVec2 totalSizeWithoutPadding(size.x+innerSpacing+textSize.x,size.y>textSize.y ? size.y : textSize.y);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding*2);
    ImVec2 start(0,0);
    start = window->DC.CursorPos + padding;if (size.y<textSize.y) start.y+=(textSize.y-size.y)*.5f;
    const ImRect image_bb(start, start + size);
    start = window->DC.CursorPos + padding;start.x+=size.x+innerSpacing;if (size.y>textSize.y) start.y+=(size.y-textSize.y)*.5f;
    ItemSize(bb);
    if (!ItemAdd(bb, id))
    return false;

    bool hovered=false, held=false;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    if (bg_col.w > 0.0f)
    window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    if (textSize.x>0) ImGui::RenderText(start,label);
    return pressed;
}

bool UIKit_ImageSizeButtonWithText(ImTextureID texId, float button_width, const char* label, const ImVec2& imageSize = ImVec2(0,0), const ImVec2& uv0 = ImVec2(0,0), const ImVec2& uv1 = ImVec2(1,1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1)) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImVec2 size = imageSize;
    if (size.x <= 0 && size.y <= 0) {
        size.x = size.y = ImGui::GetTextLineHeightWithSpacing();
    } else {
        if (size.x <= 0) size.x = size.y;
        else if (size.y <= 0) size.y = size.x;
        size *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
    }

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);
    const ImVec2 textSize = ImGui::CalcTextSize(label, NULL, true);
    const bool hasText = textSize.x > 0;

    const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y);

    // Adjust width if button_width is specified
    ImVec2 totalSize = totalSizeWithoutPadding;
    if (button_width > 0.0f) {
        totalSize.x = button_width;
    }

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSize + padding * 2);
    ImVec2 start(0, 0);
    start = window->DC.CursorPos + padding;
    if (size.y < textSize.y) start.y += (textSize.y - size.y) * .5f;
    const ImRect image_bb(start, start + size);
    start = window->DC.CursorPos + padding;
    start.x += size.x + innerSpacing;
    if (size.y > textSize.y) start.y += (size.y - textSize.y) * .5f;
    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered = false, held = false;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = ImGui::GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, ImGui::GetColorU32(bg_col));

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, ImGui::GetColorU32(tint_col));

    if (textSize.x > 0) ImGui::RenderText(start, label);
    return pressed;
}



}