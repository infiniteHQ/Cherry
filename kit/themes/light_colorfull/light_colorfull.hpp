#pragma once
#include "../../../platform/engine/app/app.hpp"
#include "../../../platform/engine/themes/themes.hpp"

//
// Dark Colorfull
//

#ifndef CHERRY_KIT_THEME_LIGHT_COLORFULL
#define CHERRY_KIT_THEME_LIGHT_COLORFULL

namespace Cherry {
namespace Themes {
class LightColorfull : public Theme {
public:
  LightColorfull() {
    SetName("light_colorfull");

    // Main
    SetProperty("color_window_bg", "#F5F7FA");
    SetProperty("color_child_bg", "#F0F2F5");
    SetProperty("color_popup_bg", "#E6E9F0");
    SetProperty("color_border", "#C8CCD8");
    SetProperty("color_table_border_light", "#D8DCE6");
    SetProperty("color_menubar_bg", "#F0F2F5");

    // Headers / Titlebars
    SetProperty("color_header", "#E1E6F0");
    SetProperty("color_header_hovered", "#D3D9E6");
    SetProperty("color_header_active", "#C3C9DB");
    SetProperty("color_titlebg", "#F5F7FA");
    SetProperty("color_titlebg_active", "#E1E6F0");
    SetProperty("color_titlebg_collapsed", "#E9ECF3");

    // Tabs
    SetProperty("color_tab", "#E2E5F0");
    SetProperty("color_tab_hovered", "#D3D9E6");
    SetProperty("color_tab_active", "#C3C9DB");
    SetProperty("color_tab_unfocused", "#E2E5F0");
    SetProperty("color_tab_unfocused_active", "#D3D9E6");

    // Frame backgrounds
    SetProperty("color_framebg", "#FFFFFF");
    SetProperty("color_framebg_hovered", "#E8ECF3");
    SetProperty("color_framebg_active", "#DDE3EE");

    // Sliders, checkmarks, grabbers
    SetProperty("color_checkmark", "#4173C8");
    SetProperty("color_slidergrab", "#5C80FF");
    SetProperty("color_slidergrab_active", "#3763E1");
    SetProperty("color_scrollbar_bg", "#E0E4EB");
    SetProperty("color_scrollbar_grab", "#C2C8D8");
    SetProperty("color_scrollbar_grab_hovered", "#AEB6CC");
    SetProperty("color_scrollbar_grab_active", "#96A0BB");

    // Buttons
    SetProperty("button_color_bg", "#E1E6F0");
    SetProperty("button_color_bg_hovered", "#D3D9E6");
    SetProperty("button_color_bg_pressed", "#C3C9DB");
    SetProperty("button_color_bg_clicked", "#C3C9DB");
    SetProperty("button_color_text", "#2E3440");
    SetProperty("button_color_text_hovered", "#000000");
    SetProperty("button_color_text_pressed", "#000000");
    SetProperty("button_color_border", "#B6BDD1");
    SetProperty("button_color_border_hovered", "#5C80FF");
    SetProperty("button_color_border_clicked", "#3763E1");

    // Resize grips
    SetProperty("color_resizegrip", "#5C80FF20");
    SetProperty("color_resizegrip_hovered", "#5C80FF88");
    SetProperty("color_resizegrip_active", "#3763E1");

    // Separators
    SetProperty("color_separator", "#D1D6E1");
    SetProperty("color_separator_hovered", "#5C80FF");
    SetProperty("color_separator_active", "#3763E1");

    // Tooltip
    SetProperty("tooltip_color_border", "#5C80FF");
    SetProperty("tooltip_color_bg", "#E9ECF3");
    SetProperty("tooltip_color_bg_hovered", "#DDE3EE");
    SetProperty("tooltip_color_bg_clicked", "#C3C9DB");

    // Text
    SetProperty("text_color_text", "#2E3440");
    SetProperty("text_area_color_text", "#1A1D25");
    SetProperty("title_color_text", "#000000");
    SetProperty("keyval_color_text", "#4C5566");

    // Notifications
    SetProperty("notification_color_border", "#5C80FF");
    SetProperty("notification_color_border_hovered", "#3763E1");
    SetProperty("notification_color_bg", "#E1E6F0");
    SetProperty("notification_color_bg_hovered", "#D3D9E6");

    // Blocks
    SetProperty("block_color", "#F0F2F5");
    SetProperty("block_color_hovered", "#E1E6F0");
    SetProperty("block_color_pressed", "#D3D9E6");
    SetProperty("block_border_color", "#C8CCD8");
    SetProperty("block_border_color_hovered", "#5C80FF");
    SetProperty("block_border_color_pressed", "#3763E1");
    SetProperty("block_border_radius", std::to_string(4.0f));
    SetProperty("block_border_size", std::to_string(1.0f));

    // Table Header
    SetProperty("color_table_header_bg", "#D3D9E6");

    // Combos & Checkboxes
    SetProperty("checkbox_color_border", "#5C80FF");
    SetProperty("checkbox_color_border_hovered", "#4173C8");
    SetProperty("checkbox_color_border_pressed", "#3763E1");
    SetProperty("checkbox_color_bg", "#FFFFFF");
    SetProperty("checkbox_color_bg_hovered", "#E8ECF3");
    SetProperty("checkbox_color_bg_pressed", "#DDE3EE");
    SetProperty("checkbox_color_bg_clicked", "#DDE3EE");

    SetProperty("combo_color_border", "#5C80FF");
    SetProperty("combo_color_border_hovered", "#4173C8");
    SetProperty("combo_color_border_pressed", "#3763E1");
    SetProperty("combo_color_bg", "#FFFFFF");
    SetProperty("combo_color_bg_hovered", "#E8ECF3");
    SetProperty("combo_color_bg_pressed", "#DDE3EE");
    SetProperty("combo_color_bg_clicked", "#DDE3EE");

    // Table padding
    SetProperty("table_cell_padding_x_header", "6.0");
    SetProperty("table_cell_padding_y_header", "6.0");
    SetProperty("table_cell_padding_x_row", "6.0");
    SetProperty("table_cell_padding_y_row", "6.0");
  }
};
} // namespace Themes
} // namespace Cherry

#endif // CHERRY_KIT_THEME_LIGHT_COLORFULL
