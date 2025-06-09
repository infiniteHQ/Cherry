#pragma once
#include "../../../platform/engine/app.hpp"
#include "../../../platform/engine/themes.hpp"

//
// DarkVortex
//

#ifndef CHERRY_KIT_DARK_VORTEX
#define CHERRY_KIT_DARK_VORTEX

namespace Cherry {
namespace Themes {
class DarkVortex : public Theme {
public:
  DarkVortex() {
    m_ThemeID = "dark_vortex";

    // Colors
    SetProperty("block_color", "#252525");
    SetProperty("block_color_hovered", "#454545");
    SetProperty("block_color_pressed", "#555555");
    SetProperty("block_border_color", "#353535");
    SetProperty("block_border_color_hovered", "#353535");
    SetProperty("block_border_color_pressed", "#555555");

    SetProperty("color_window_bg", "#FF1515");
    SetProperty("color_child_bg", "#FF1515");
    SetProperty("color_popup_bg", "#FF1515");
    SetProperty("color_border", "#FF1515");
  }
};
} // namespace Themes

} // namespace Cherry

#endif // CHERRY_KIT_DARK_VORTEX
