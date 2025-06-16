#pragma once
#include "../../../platform/engine/app/app.hpp"
#include "../../../platform/engine/themes/themes.hpp"

//
// LightHighContrast
//

#ifndef CHERRY_KIT_THEME_LIGHT_HIGH_CONTRAST
#define CHERRY_KIT_THEME_LIGHT_HIGH_CONTRAST

namespace Cherry {
namespace Themes {
class LightHighContrast : public Theme {
public:
  LightHighContrast() {
    SetName("light_high_contrast");

    // General theme, windows
    SetProperty("color_header", "#E0E0E0FF");
    SetProperty("color_header_hovered", "#D0D0D0FF");
    SetProperty("color_header_active", "#C0C0C0FF");
    SetProperty("color", "#F2F2F2FF");
    SetProperty("color_hovered", "#E6E6E6FF");
    SetProperty("color_active", "#DCDCDCFF");
    SetProperty("color_framebg", "#FFFFFFFF");
    SetProperty("color_framebg_hovered", "#F0F0F0FF");
    SetProperty("color_framebg_active", "#E0E0E0FF");
    SetProperty("color_tab", "#F8F8F8FF");
    SetProperty("color_tab_hovered", "#E8E8E8FF");
    SetProperty("color_tab_active", "#DADADAFF");
    SetProperty("color_tab_unfocused", "#F8F8F8FF");
    SetProperty("color_tab_unfocused_active", "#E8E8E8FF");
    SetProperty("color_text", "#1A1A1AFF");
    SetProperty("color_titlebg", "#F8F8F8FF");
    SetProperty("color_titlebg_active", "#E0E0E0FF");
    SetProperty("color_titlebg_collapsed", "#CCCCCCFF");
    SetProperty("color_resizegrip", "#5A5A5A80");
    SetProperty("color_resizegrip_hovered", "#5A5A5ACC");
    SetProperty("color_resizegrip_active", "#5A5A5AFF");
    SetProperty("color_scrollbar_bg", "#DDDDDDC8");
    SetProperty("color_scrollbar_grab", "#999999FF");
    SetProperty("color_scrollbar_grab_hovered", "#666666FF");
    SetProperty("color_scrollbar_grab_active", "#444444FF");
    SetProperty("color_checkmark", "#1A1A1AFF");
    SetProperty("color_slidergrab", "#444444B3");
    SetProperty("color_slidergrab_active", "#222222FF");
    SetProperty("color_separator", "#CCCCCCFF");
    SetProperty("color_separator_active", "#005C8CFF");
    SetProperty("color_separator_hovered", "#AAAAAAFF");
    SetProperty("color_window_bg", "#FFFFFFFF");
    SetProperty("color_child_bg", "#F6F6F6FF");
    SetProperty("color_popup_bg", "#F0F0F0FF");
    SetProperty("color_border", "#BFBFBFFF");
    SetProperty("color_table_header_bg", "#E6E6E6FF");
    SetProperty("color_table_border_light", "#CCCCCCFF");
    SetProperty("color_menubar_bg", "#EFEFEFFF");

    // Buttons
    SetProperty("button_color_border", "#999999FF");
    SetProperty("button_color_border_hovered", "#666666FF");
    SetProperty("button_color_border_clicked", "#444444FF");
    SetProperty("button_color_border_pressed", "#222222FF");
    SetProperty("button_color_bg", "#F0F0F0FF");
    SetProperty("button_color_bg_hovered", "#E0E0E0FF");
    SetProperty("button_color_bg_pressed", "#C0C0C0FF");
    SetProperty("button_color_bg_clicked", "#C0C0C0FF");
    SetProperty("button_color_text", "#1A1A1AFF");
    SetProperty("button_color_text_hovered", "#000000FF");
    SetProperty("button_color_text_pressed", "#000000FF");
    SetProperty("button_color_underline", "#B0B0B0FF");
    SetProperty("button_color_underline_hovered", "#909090FF");
    SetProperty("button_color_underline_pressed", "#707070FF");
    SetProperty("button_size_x", "0");
    SetProperty("button_size_y", "0");
    SetProperty("button_padding_x", "6");
    SetProperty("button_padding_y", "6");
    SetProperty("button_scale", "0");

    // Checkbox
    SetProperty("checkbox_color_border", "#555555FF");
    SetProperty("checkbox_color_border_hovered", "#444444FF");
    SetProperty("checkbox_color_border_clicked", "#333333FF");
    SetProperty("checkbox_color_border_pressed", "#222222FF");
    SetProperty("checkbox_color_bg", "#FFFFFF");
    SetProperty("checkbox_color_bg_hovered", "#F0F0F0");
    SetProperty("checkbox_color_bg_pressed", "#E0E0E0");
    SetProperty("checkbox_color_bg_clicked", "#D0D0D0");
    SetProperty("checkbox_size_x", "6");
    SetProperty("checkbox_size_y", "6");

    // Combo
    SetProperty("combo_color_border", "#555555FF");
    SetProperty("combo_color_border_hovered", "#444444FF");
    SetProperty("combo_color_border_clicked", "#333333FF");
    SetProperty("combo_color_border_pressed", "#222222FF");
    SetProperty("combo_color_bg", "#FFFFFF");
    SetProperty("combo_color_bg_hovered", "#F0F0F0");
    SetProperty("combo_color_bg_pressed", "#E0E0E0");
    SetProperty("combo_color_bg_clicked", "#D0D0D0");
    SetProperty("combo_size_x", "6");
    SetProperty("combo_size_y", "6");

    // Header
    SetProperty("header_color_border", "#AAAAAAFF");
    SetProperty("header_color_border_hovered", "#999999FF");
    SetProperty("header_color_border_clicked", "#666666FF");
    SetProperty("header_color_border_pressed", "#444444FF");
    SetProperty("header_color_bg", "#F0F0F0FF");
    SetProperty("header_color_bg_hovered", "#E0E0E0FF");
    SetProperty("header_color_bg_pressed", "#CCCCCCFF");
    SetProperty("header_color_bg_clicked", "#BBBBBBFF");
    SetProperty("header_size_x", "6");
    SetProperty("header_size_y", "6");
    SetProperty("header_button_color_border", "#999999FF");
    SetProperty("header_button_color_border_hovered", "#777777FF");
    SetProperty("header_button_color_border_clicked", "#555555FF");
    SetProperty("header_button_color_border_pressed", "#333333FF");
    SetProperty("header_button_color_bg", "#F0F0F0FF");
    SetProperty("header_button_color_bg_hovered", "#E0E0E0FF");
    SetProperty("header_button_color_bg_pressed", "#CCCCCCFF");
    SetProperty("header_button_color_bg_clicked", "#BBBBBBFF");
    SetProperty("header_button_color_text", "#1A1A1AFF");
    SetProperty("header_button_color_text_hovered", "#000000FF");
    SetProperty("header_button_color_text_pressed", "#000000FF");

    // Image
    SetProperty("image_color_border", "#CCCCCCFF");
    SetProperty("image_color_border_hovered", "#999999FF");
    SetProperty("image_color_border_clicked", "#777777FF");
    SetProperty("image_color_border_pressed", "#555555FF");
    SetProperty("image_color_bg", "#FFFFFFFF");
    SetProperty("image_color_bg_hovered", "#F0F0F0FF");
    SetProperty("image_color_bg_pressed", "#E0E0E0FF");
    SetProperty("image_color_bg_clicked", "#CCCCCCFF");
    SetProperty("image_size_x", "6");
    SetProperty("image_size_y", "6");

    // Modal
    SetProperty("modal_padding_x", "6");
    SetProperty("modal_padding_y", "6");

    // Notification
    SetProperty("notification_color_border", "#888888FF");
    SetProperty("notification_color_border_hovered", "#555555FF");
    SetProperty("notification_color_bg", "#FFF8DCFF");
    SetProperty("notification_color_bg_hovered", "#FFEBCDFF");

    // Separator
    SetProperty("separator_color", "#CCCCCCFF");
    SetProperty("separator_color_text", "#444444FF");

    // Keyval
    SetProperty("keyval_color_text", "#222222FF");

    // Table
    SetProperty("table_color_border", "#BBBBBBFF");
    SetProperty("table_cell_padding_x_header", "6.0");
    SetProperty("table_cell_padding_y_header", "6.0");
    SetProperty("table_cell_padding_x_row", "6.0");
    SetProperty("table_cell_padding_y_row", "6.0");

    // Text Area
    SetProperty("text_area_color_text", "#000000FF");

    // Text
    SetProperty("text_color_text", "#111111FF");

    // Titles
    SetProperty("title_color_text", "#000000FF");

    // Tooltip
    SetProperty("tooltip_color_border", "#AAAAAAFF");
    SetProperty("tooltip_color_border_hovered", "#888888FF");
    SetProperty("tooltip_color_border_clicked", "#666666FF");
    SetProperty("tooltip_color_bg", "#FFFFF0FF");
    SetProperty("tooltip_color_bg_hovered", "#FFF5E1FF");
    SetProperty("tooltip_color_bg_clicked", "#FFEBCCFF");

    // Blocks
    SetProperty("block_color", "#EAEAEAFF");
    SetProperty("block_color_hovered", "#DDDDDDFF");
    SetProperty("block_color_pressed", "#CCCCCCFF");
    SetProperty("block_border_color", "#BBBBBBFF");
    SetProperty("block_border_color_hovered", "#999999FF");
    SetProperty("block_border_color_pressed", "#777777FF");
    SetProperty("block_border_radius", std::to_string(0.0f));
    SetProperty("block_border_size", std::to_string(1.0f));
  }
};
} // namespace Themes
} // namespace Cherry

#endif // CHERRY_KIT_THEME_LIGHT_HIGH_CONTRAST
