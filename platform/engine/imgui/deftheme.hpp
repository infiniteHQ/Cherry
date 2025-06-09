#include "../themes.hpp"

#ifndef CHERRY_DEFTHEME_HPP
#define CHERRY_DEFTHEME_HPP

namespace Cherry {
class DefaultCherryTheme : public Theme {
public:
  DefaultCherryTheme() {
    m_ThemeID = "default";

    // Colors
    SetProperty("color_header", "#2F2F2FFF");
    SetProperty("color_header_hovered", "#2F2F2FFF");
    SetProperty("color_header_active", "#2F2F2FFF");

    SetProperty("color_button", "#383838C8");
    SetProperty("color_button_hovered", "#464646FF");
    SetProperty("color_button_active", "#38383896");

    SetProperty("color_framebg", "#0F0F0FFF");
    SetProperty("color_framebg_hovered", "#0F0F0FFF");
    SetProperty("color_framebg_active", "#0F0F0FFF");

    SetProperty("color_tab", "#151515FF");
    SetProperty("color_tab_hovered", "#24242464");
    SetProperty("color_tab_active", "#242424FF");
    SetProperty("color_tab_unfocused", "#151515FF");
    SetProperty("color_tab_unfocused_active", "#24242464");

    SetProperty("color_titlebg", "#151515FF");
    SetProperty("color_titlebg_active", "#151515FF");
    SetProperty("color_titlebg_collapsed", "#262626FF");

    SetProperty("color_resizegrip", "#E8E8E840");
    SetProperty("color_resizegrip_hovered", "#CFCFCFAA");
    SetProperty("color_resizegrip_active", "#757575F2");

    SetProperty("color_scrollbar_bg", "#05050587");
    SetProperty("color_scrollbar_grab", "#4F4F4FFF");
    SetProperty("color_scrollbar_grab_hovered", "#696969FF");
    SetProperty("color_scrollbar_grab_active", "#828282FF");

    SetProperty("color_checkmark", "#C8C8C8FF");

    SetProperty("color_slidergrab", "#828282B3");
    SetProperty("color_slidergrab_active", "#A8A8A8FF");

    SetProperty("color_separator", "#151515FF");
    SetProperty("color_separator_active", "#27B9F2FF");
    SetProperty("color_separator_hovered", "#151515FF");

    SetProperty("color_window_bg", "#151515FF");
    SetProperty("color_child_bg", "#242424FF");
    SetProperty("color_popup_bg", "#383838FF");
    SetProperty("color_border", "#242424FF");

    SetProperty("color_table_header_bg", "#2F2F2FFF");
    SetProperty("color_table_border_light", "#1A1A1AFF");

    SetProperty("color_menubar_bg", "#242424FF");
  }
};

}; // namespace Cherry
#endif // CHERRY_DEFTHEME_HPP