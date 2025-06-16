#pragma once
#include "../../../platform/engine/app/app.hpp"
#include "../../../platform/engine/themes/themes.hpp"

//
// DarkHighContrast
//

#ifndef CHERRY_KIT_DARK_HIGH_CONSTRAT
#define CHERRY_KIT_DARK_HIGH_CONSTRAT

namespace Cherry {
namespace Themes {
class DarkHighContrast : public Theme {
public:
  DarkHighContrast() {
    SetName("dark_high_contrast");

    // General theme, windows
    SetProperty("color_header", "#000000FF");
    SetProperty("color_header_hovered", "#1A1A1AFF");
    SetProperty("color_header_active", "#262626FF");
    SetProperty("color", "#0F0F0FFF");
    SetProperty("color_hovered", "#1A1A1AFF");
    SetProperty("color_active", "#262626FF");
    SetProperty("color_framebg", "#000000FF");
    SetProperty("color_framebg_hovered", "#1F1F1FFF");
    SetProperty("color_framebg_active", "#2A2A2AFF");
    SetProperty("color_tab", "#0A0A0AFF");
    SetProperty("color_tab_hovered", "#3A3A3AFF");
    SetProperty("color_tab_active", "#505050FF");
    SetProperty("color_tab_unfocused", "#1A1A1AFF");
    SetProperty("color_tab_unfocused_active", "#2F2F2FFF");
    SetProperty("color_text", "#FFFFFFFF");
    SetProperty("color_titlebg", "#000000FF");
    SetProperty("color_titlebg_active", "#101010FF");
    SetProperty("color_titlebg_collapsed", "#202020FF");
    SetProperty("color_resizegrip", "#AAAAAAFF");
    SetProperty("color_resizegrip_hovered", "#CCCCCCFF");
    SetProperty("color_resizegrip_active", "#FFFFFFFF");
    SetProperty("color_scrollbar_bg", "#000000FF");
    SetProperty("color_scrollbar_grab", "#555555FF");
    SetProperty("color_scrollbar_grab_hovered", "#AAAAAAFF");
    SetProperty("color_scrollbar_grab_active", "#FFFFFFFF");
    SetProperty("color_checkmark", "#FFFFFFFF");
    SetProperty("color_slidergrab", "#AAAAAAFF");
    SetProperty("color_slidergrab_active", "#FFFFFFFF");
    SetProperty("color_separator", "#404040FF");
    SetProperty("color_separator_active", "#00FFFF");
    SetProperty("color_separator_hovered", "#AAAAAAFF");
    SetProperty("color_window_bg", "#000000FF");
    SetProperty("color_child_bg", "#121212FF");
    SetProperty("color_popup_bg", "#1E1E1EFF");
    SetProperty("color_border", "#333333FF");
    SetProperty("color_table_header_bg", "#111111FF");
    SetProperty("color_table_border_light", "#2A2A2AFF");
    SetProperty("color_menubar_bg", "#121212FF");

    // Buttons
    SetProperty("button_color_border", "#555555FF");
    SetProperty("button_color_border_hovered", "#777777FF");
    SetProperty("button_color_border_clicked", "#999999FF");
    SetProperty("button_color_border_pressed", "#FFFFFFFF");
    SetProperty("button_color_bg", "#000000FF");
    SetProperty("button_color_bg_hovered", "#1A1A1AFF");
    SetProperty("button_color_bg_pressed", "#2A2A2AFF");
    SetProperty("button_color_bg_clicked", "#2A2A2AFF");
    SetProperty("button_color_text", "#FFFFFF");
    SetProperty("button_color_text_hovered", "#FFFFFFFF");
    SetProperty("button_color_text_pressed", "#FFFFFFFF");
    SetProperty("button_color_underline", "#FFFFFF");
    SetProperty("button_color_underline_hovered", "#00FFFF");
    SetProperty("button_color_underline_pressed", "#00AAFFFF");
    SetProperty("button_size_x", "0");
    SetProperty("button_size_y", "0");
    SetProperty("button_padding_x", "6");
    SetProperty("button_padding_y", "6");
    SetProperty("button_scale", "0");

    // Checkbox
    SetProperty("checkbox_color_border", "#CCCCCCFF");
    SetProperty("checkbox_color_border_hovered", "#FFFFFF");
    SetProperty("checkbox_color_border_clicked", "#FFFFFF");
    SetProperty("checkbox_color_border_pressed", "#FFFFFF");
    SetProperty("checkbox_color_bg", "#000000FF");
    SetProperty("checkbox_color_bg_hovered", "#1A1A1AFF");
    SetProperty("checkbox_color_bg_pressed", "#2A2A2AFF");
    SetProperty("checkbox_color_bg_clicked", "#2A2A2AFF");
    SetProperty("checkbox_size_x", "6");
    SetProperty("checkbox_size_y", "6");

    // Combo
    SetProperty("combo_color_border", "#888888FF");
    SetProperty("combo_color_border_hovered", "#AAAAAAFF");
    SetProperty("combo_color_border_clicked", "#CCCCCCFF");
    SetProperty("combo_color_border_pressed", "#FFFFFF");
    SetProperty("combo_color_bg", "#000000FF");
    SetProperty("combo_color_bg_hovered", "#1A1A1AFF");
    SetProperty("combo_color_bg_pressed", "#2A2A2AFF");
    SetProperty("combo_color_bg_clicked", "#2A2A2AFF");
    SetProperty("combo_size_x", "6");
    SetProperty("combo_size_y", "6");

    // Header
    SetProperty("header_color_border", "#444444FF");
    SetProperty("header_color_border_hovered", "#666666FF");
    SetProperty("header_color_border_clicked", "#888888FF");
    SetProperty("header_color_border_pressed", "#FFFFFF");
    SetProperty("header_color_bg", "#000000FF");
    SetProperty("header_color_bg_hovered", "#1A1A1AFF");
    SetProperty("header_color_bg_pressed", "#2A2A2AFF");
    SetProperty("header_color_bg_clicked", "#2A2A2AFF");
    SetProperty("header_size_x", "6");
    SetProperty("header_size_y", "6");
    SetProperty("header_button_color_border", "#555555FF");
    SetProperty("header_button_color_border_hovered", "#777777FF");
    SetProperty("header_button_color_border_clicked", "#999999FF");
    SetProperty("header_button_color_border_pressed", "#FFFFFF");
    SetProperty("header_button_color_bg", "#000000FF");
    SetProperty("header_button_color_bg_hovered", "#1A1A1AFF");
    SetProperty("header_button_color_bg_pressed", "#2A2A2AFF");
    SetProperty("header_button_color_bg_clicked", "#2A2A2AFF");
    SetProperty("header_button_color_text", "#FFFFFF");
    SetProperty("header_button_color_text_hovered", "#FFFFFFFF");
    SetProperty("header_button_color_text_pressed", "#FFFFFFFF");

    // Image
    SetProperty("image_color_border", "#555555FF");
    SetProperty("image_color_border_hovered", "#777777FF");
    SetProperty("image_color_border_clicked", "#999999FF");
    SetProperty("image_color_border_pressed", "#FFFFFF");
    SetProperty("image_color_bg", "#000000FF");
    SetProperty("image_color_bg_hovered", "#1A1A1AFF");
    SetProperty("image_color_bg_pressed", "#2A2A2AFF");
    SetProperty("image_color_bg_clicked", "#2A2A2AFF");
    SetProperty("image_size_x", "6");
    SetProperty("image_size_y", "6");

    // Modal
    SetProperty("modal_padding_x", "6");
    SetProperty("modal_padding_y", "6");

    // Notification
    SetProperty("notification_color_border", "#FFFFFF");
    SetProperty("notification_color_border_hovered", "#00FFFF");
    SetProperty("notification_color_bg", "#000000FF");
    SetProperty("notification_color_bg_hovered", "#1A1A1AFF");

    // Separator
    SetProperty("separator_color", "#666666FF");
    SetProperty("separator_color_text", "#CCCCCCFF");

    // Keyval
    SetProperty("keyval_color_text", "#FFFFFF");

    // Table
    SetProperty("table_color_border", "#444444FF");
    SetProperty("table_cell_padding_x_header", "6.0");
    SetProperty("table_cell_padding_y_header", "6.0");
    SetProperty("table_cell_padding_x_row", "6.0");
    SetProperty("table_cell_padding_y_row", "6.0");

    // Text Area
    SetProperty("text_area_color_text", "#FFFFFFFF");

    // Text
    SetProperty("text_color_text", "#FFFFFFFF");

    // Titles
    SetProperty("title_color_text", "#FFFFFFFF");

    // Tooltip
    SetProperty("tooltip_color_border", "#777777FF");
    SetProperty("tooltip_color_border_hovered", "#AAAAAAFF");
    SetProperty("tooltip_color_border_clicked", "#FFFFFF");
    SetProperty("tooltip_color_bg", "#000000FF");
    SetProperty("tooltip_color_bg_hovered", "#1A1A1AFF");
    SetProperty("tooltip_color_bg_clicked", "#2A2A2AFF");

    // Blocks
    SetProperty("block_color", "#0A0A0AFF");
    SetProperty("block_color_hovered", "#2A2A2AFF");
    SetProperty("block_color_pressed", "#444444FF");
    SetProperty("block_border_color", "#555555FF");
    SetProperty("block_border_color_hovered", "#777777FF");
    SetProperty("block_border_color_pressed", "#FFFFFF");
    SetProperty("block_border_radius", "0.0");
    SetProperty("block_border_size", "1.0");
  }
};
} // namespace Themes

} // namespace Cherry

#endif // CHERRY_KIT_DARK_HIGH_CONSTRAT
