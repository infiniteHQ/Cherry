#include "../themes.hpp"

#ifndef CHERRY_DEFTHEME_HPP
#define CHERRY_DEFTHEME_HPP

namespace Cherry {
class DefaultCherryTheme : public Theme {
public:
  DefaultCherryTheme() {
    m_ThemeID = "default";

    // Colors
    SetProperty("block_color", "#252525");
    SetProperty("block_color_hovered", "#454545");
    SetProperty("block_color_pressed", "#555555");
    SetProperty("block_border_color", "#353535");
    SetProperty("block_border_color_hovered", "#353535");
    SetProperty("block_border_color_pressed", "#555555");
  }
};

}; // namespace Cherry
#endif // CHERRY_DEFTHEME_HPP