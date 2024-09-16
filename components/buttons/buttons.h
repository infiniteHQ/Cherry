
#include "../../lib/imgui/imgui.h"

#undef IMGUI_DEFINE_PLACEMENT_NEW
#define IMGUI_DEFINE_PLACEMENT_NEW
#undef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../lib/imgui/imgui_internal.h"

namespace ImGui {
  IMGUI_API bool UIKit_ImageSizeButtonWithText(ImTextureID texId, float button_width, const char* label, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col);
IMGUI_API bool UIKit_ImageButtonWithText(ImTextureID texId, const char* label,const ImVec2& imageSize, const ImVec2& uv0,  const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col);
} // namespace ImGui 
