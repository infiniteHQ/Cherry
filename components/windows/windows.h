#include "../../lib/imgui/imgui.h"

#undef IMGUI_DEFINE_PLACEMENT_NEW
#define IMGUI_DEFINE_PLACEMENT_NEW
#undef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../lib/imgui/imgui_internal.h"

namespace ImGui
{
    IMGUI_API bool UIKit_BeginLogoWindow(const char* name, ImTextureID* logo, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
}