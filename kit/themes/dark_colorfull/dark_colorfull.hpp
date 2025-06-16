#pragma once
#include "../../../platform/engine/app/app.hpp"
#include "../../../platform/engine/themes/themes.hpp"

//
// Dark Colorfull
//

#ifndef CHERRY_KIT_THEME_DARK_COLORFULL
#define CHERRY_KIT_THEME_DARK_COLORFULL

namespace Cherry {
namespace Themes {
class DarkColorfull : public Theme {
public:
  DarkColorfull() {
    SetName("dark_colorfull");

    // Main
    SetProperty("color_window_bg", "#1E1F2B");
    SetProperty("color_child_bg", "#232435");
    SetProperty("color_popup_bg", "#2B2D3C");
    SetProperty("color_border", "#2F3242");
    SetProperty("color_table_border_light", "#373B4B");
    SetProperty("color_menubar_bg", "#232435");

    // Headers & Titlebars
    SetProperty("color_header", "#2B2D3C");
    SetProperty("color_header_hovered", "#3A3D4F");
    SetProperty("color_header_active", "#4C5065");
    SetProperty("color_titlebg", "#1E1F2B");
    SetProperty("color_titlebg_active", "#2B2D3C");
    SetProperty("color_titlebg_collapsed", "#20222F");

    // Tabs
    SetProperty("color_tab", "#262738");
    SetProperty("color_tab_hovered", "#3A3D4F");
    SetProperty("color_tab_active", "#4C5065");
    SetProperty("color_tab_unfocused", "#262738");
    SetProperty("color_tab_unfocused_active", "#3A3D4F");

    // Frame Backgrounds
    SetProperty("color_framebg", "#1F202C");
    SetProperty("color_framebg_hovered", "#2B2D3C");
    SetProperty("color_framebg_active", "#3A3D4F");

    // Sliders, Checkmarks, Grabbers
    SetProperty("color_checkmark", "#7DB4FF");
    SetProperty("color_slidergrab", "#5C80FF");
    SetProperty("color_slidergrab_active", "#84A8FF");
    SetProperty("color_scrollbar_bg", "#1A1B26");
    SetProperty("color_scrollbar_grab", "#3C3F53");
    SetProperty("color_scrollbar_grab_hovered", "#50546C");
    SetProperty("color_scrollbar_grab_active", "#697092");

    // Buttons
    SetProperty("button_color_bg", "#2B2D3C");
    SetProperty("button_color_bg_hovered", "#3C3F53");
    SetProperty("button_color_bg_pressed", "#4A4F6C");
    SetProperty("button_color_bg_clicked", "#4A4F6C");
    SetProperty("button_color_text", "#D1D9FF");
    SetProperty("button_color_text_hovered", "#FFFFFF");
    SetProperty("button_color_text_pressed", "#FFFFFF");
    SetProperty("button_color_border", "#3C3F53");
    SetProperty("button_color_border_hovered", "#5C80FF");
    SetProperty("button_color_border_clicked", "#84A8FF");
    SetProperty("button_color_underline", "#2B2D3C");
    SetProperty("button_color_underline_hovered", "#3C3F53");
    SetProperty("button_color_underline_pressed", "#4A4F6C");
    SetProperty("button_size_x", "0");
    SetProperty("button_size_y", "0");
    SetProperty("button_padding_x", "6");
    SetProperty("button_padding_y", "6");
    SetProperty("button_scale", "0");

    // Resize Grips
    SetProperty("color_resizegrip", "#CCCCCC22");
    SetProperty("color_resizegrip_hovered", "#7DB4FFAA");
    SetProperty("color_resizegrip_active", "#5C80FF");

    // Separators
    SetProperty("color_separator", "#313344");
    SetProperty("color_separator_hovered", "#5C80FF");
    SetProperty("color_separator_active", "#84A8FF");
    SetProperty("separator_color_text", "#7D8499");

    // Tooltip
    SetProperty("tooltip_color_border", "#5C80FF");
    SetProperty("tooltip_color_border_hovered", "#7DB4FF");
    SetProperty("tooltip_color_border_clicked", "#84A8FF");
    SetProperty("tooltip_color_bg", "#2A2C3D");
    SetProperty("tooltip_color_bg_hovered", "#353853");
    SetProperty("tooltip_color_bg_clicked", "#4A4F6C");

    // Text
    SetProperty("text_color_text", "#D1D9FF");
    SetProperty("text_area_color_text", "#E0E6FF");
    SetProperty("title_color_text", "#FFFFFF");
    SetProperty("keyval_color_text", "#9BA6C9");

    // Notifications
    SetProperty("notification_color_border", "#5C80FF");
    SetProperty("notification_color_border_hovered", "#84A8FF");
    SetProperty("notification_color_bg", "#2B2D3C");
    SetProperty("notification_color_bg_hovered", "#353853");

    // Blocks
    SetProperty("block_color", "#2B2D3C");
    SetProperty("block_color_hovered", "#3A3D4F");
    SetProperty("block_color_pressed", "#4C5065");
    SetProperty("block_border_color", "#3A3D4F");
    SetProperty("block_border_color_hovered", "#5C80FF");
    SetProperty("block_border_color_pressed", "#84A8FF");
    SetProperty("block_border_radius", std::to_string(4.0f));
    SetProperty("block_border_size", std::to_string(1.0f));

    // Table Header
    SetProperty("color_table_header_bg", "#353853");

    // Checkbox
    SetProperty("checkbox_color_border", "#5C80FF");
    SetProperty("checkbox_color_border_hovered", "#7DB4FF");
    SetProperty("checkbox_color_border_pressed", "#84A8FF");
    SetProperty("checkbox_color_bg", "#2B2D3C");
    SetProperty("checkbox_color_bg_hovered", "#3A3D4F");
    SetProperty("checkbox_color_bg_pressed", "#4C5065");
    SetProperty("checkbox_color_bg_clicked", "#4C5065");
    SetProperty("checkbox_size_x", "6");
    SetProperty("checkbox_size_y", "6");

    // Combo
    SetProperty("combo_color_border", "#5C80FF");
    SetProperty("combo_color_border_hovered", "#7DB4FF");
    SetProperty("combo_color_border_pressed", "#84A8FF");
    SetProperty("combo_color_bg", "#2B2D3C");
    SetProperty("combo_color_bg_hovered", "#3A3D4F");
    SetProperty("combo_color_bg_pressed", "#4C5065");
    SetProperty("combo_color_bg_clicked", "#4C5065");
    SetProperty("combo_size_x", "6");
    SetProperty("combo_size_y", "6");

    // Table Padding
    SetProperty("table_cell_padding_x_header", "6.0");
    SetProperty("table_cell_padding_y_header", "6.0");
    SetProperty("table_cell_padding_x_row", "6.0");
    SetProperty("table_cell_padding_y_row", "6.0");

    // Header Buttons
    SetProperty("header_color_border", "#3A3D4F");
    SetProperty("header_color_border_hovered", "#5C80FF");
    SetProperty("header_color_border_clicked", "#7DB4FF");
    SetProperty("header_color_border_pressed", "#84A8FF");
    SetProperty("header_color_bg", "#2B2D3C");
    SetProperty("header_color_bg_hovered", "#3A3D4F");
    SetProperty("header_color_bg_pressed", "#4C5065");
    SetProperty("header_color_bg_clicked", "#4C5065");
    SetProperty("header_size_x", "6");
    SetProperty("header_size_y", "6");
    SetProperty("header_button_color_border", "#3C3F53");
    SetProperty("header_button_color_border_hovered", "#5C80FF");
    SetProperty("header_button_color_border_clicked", "#84A8FF");
    SetProperty("header_button_color_border_pressed", "#84A8FF");
    SetProperty("header_button_color_bg", "#2B2D3C");
    SetProperty("header_button_color_bg_hovered", "#3C3F53");
    SetProperty("header_button_color_bg_pressed", "#4A4F6C");
    SetProperty("header_button_color_bg_clicked", "#4A4F6C");
    SetProperty("header_button_color_text", "#D1D9FF");
    SetProperty("header_button_color_text_hovered", "#FFFFFF");
    SetProperty("header_button_color_text_pressed", "#FFFFFF");

    // Image
    SetProperty("image_color_border", "#3C3F53");
    SetProperty("image_color_border_hovered", "#5C80FF");
    SetProperty("image_color_border_clicked", "#7DB4FF");
    SetProperty("image_color_border_pressed", "#84A8FF");
    SetProperty("image_color_bg", "#2B2D3C");
    SetProperty("image_color_bg_hovered", "#3A3D4F");
    SetProperty("image_color_bg_pressed", "#4C5065");
    SetProperty("image_color_bg_clicked", "#4C5065");
    SetProperty("image_size_x", "6");
    SetProperty("image_size_y", "6");

    // Modal
    SetProperty("modal_padding_x", "6");
    SetProperty("modal_padding_y", "6");
  }
};
} // namespace Themes
} // namespace Cherry

#endif // CHERRY_KIT_THEME_DARK_COLORFULL
