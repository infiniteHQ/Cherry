
#include "../../lib/imgui/imgui.h"

#undef IMGUI_DEFINE_PLACEMENT_NEW
#define IMGUI_DEFINE_PLACEMENT_NEW
#undef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../lib/imgui/imgui_internal.h"

namespace ImGui {
  IMGUI_API bool UIKit_BeginMenuBar(float height_addition);
  IMGUI_API void UIKit_EndMenuBar();
} // namespace ImGui 
