#pragma once
#include "../../../platform/engine/app/app.hpp"
#include "../../../platform/engine/themes/themes.hpp"

//
// Dark
//

#ifndef CHERRY_KIT_DARK
#define CHERRY_KIT_DARK

namespace Cherry {
namespace Themes {
class Dark : public Theme {
public:
  Dark() {
    SetName("dark");

    // GENERAL THEME
    SetProperty("color_header", "#2A2D30FF");
    SetProperty("color_header_hovered", "#323538FF");
    SetProperty("color_header_active", "#3A3E41FF");
    SetProperty("color", "#34383CC8");
    SetProperty("color_hovered", "#3F4347FF");
    SetProperty("color_active", "#34383C96");
    SetProperty("color_framebg", "#101214FF");
    SetProperty("color_framebg_hovered", "#111417FF");
    SetProperty("color_framebg_active", "#121519FF");
    SetProperty("color_tab", "#1A1D1FFF");
    SetProperty("color_tab_hovered", "#24282C64");
    SetProperty("color_tab_active", "#282C30FF");
    SetProperty("color_tab_unfocused", "#1A1D1FFF");
    SetProperty("color_text", "#898989FF"); // hex
    SetProperty("color_tab_unfocused_active", "#24282C64");
    SetProperty("color_titlebg", "#1A1D1FFF");
    SetProperty("color_titlebg_active", "#1A1D1FFF");
    SetProperty("color_titlebg_collapsed", "#25282CFF");
    SetProperty("color_resizegrip", "#D0D0D040");
    SetProperty("color_resizegrip_hovered", "#C0C0C0AA");
    SetProperty("color_resizegrip_active", "#888888F2");
    SetProperty("color_scrollbar_bg", "#080A0D87");
    SetProperty("color_scrollbar_grab", "#4A4E52FF");
    SetProperty("color_scrollbar_grab_hovered", "#62666AFF");
    SetProperty("color_scrollbar_grab_active", "#7A7F83FF");
    SetProperty("color_checkmark", "#B0B0B0FF");
    SetProperty("color_slidergrab", "#787D83B3");
    SetProperty("color_slidergrab_active", "#A0A5ABFF");
    SetProperty("color_separator", "#1A1D1FFF");
    SetProperty("color_separator_active", "#27B9F2FF");
    SetProperty("color_separator_hovered", "#1A1D1FFF");
    SetProperty("color_window_bg", "#1A1D1FFF");
    SetProperty("color_child_bg", "#24282CFF");
    SetProperty("color_popup_bg", "#32363AFF");
    SetProperty("color_border", "#24282CFF");
    SetProperty("color_table_header_bg", "#2A2D30FF");
    SetProperty("color_table_border_light", "#1C1F22FF");
    SetProperty("color_menubar_bg", "#24282CFF");

    // BUTTONS
    SetProperty("button_color_border", "#3F4245FF");
    SetProperty("button_color_border_hovered", "#4A4E51FF");
    SetProperty("button_color_border_clicked", "#44484BFF");
    SetProperty("button_color_border_pressed", "#6C6F72FF");
    SetProperty("button_color_bg", "#24282CFF");
    SetProperty("button_color_bg_hovered", "#2F3337FF");
    SetProperty("button_color_bg_pressed", "#3A3E41FF");
    SetProperty("button_color_bg_clicked", "#3A3E41FF");
    SetProperty("button_color_text", "#C2C2C2FF");
    SetProperty("button_color_text_hovered", "#FFFFFFFF");
    SetProperty("button_color_text_pressed", "#FFFFFFFF");
    SetProperty("button_color_underline", "#24282CFF");
    SetProperty("button_color_underline_hovered", "#2F3337FF");
    SetProperty("button_color_underline_pressed", "#3A3E41FF");
    SetProperty("button_size_x", "0");
    SetProperty("button_size_y", "0");
    SetProperty("button_padding_x", "6");
    SetProperty("button_padding_y", "6");
    SetProperty("button_scale", "0");

    // CHECKBOX
    SetProperty("checkbox_color_border", "#3F4245B2");
    SetProperty("checkbox_color_border_hovered", "#3F4245B2");
    SetProperty("checkbox_color_border_clicked", "#3F4245B2");
    SetProperty("checkbox_color_border_pressed", "#3F4245B2");
    SetProperty("checkbox_color_bg", "#24282CFF");
    SetProperty("checkbox_color_bg_hovered", "#2F3337FF");
    SetProperty("checkbox_color_bg_pressed", "#3A3E41FF");
    SetProperty("checkbox_color_bg_clicked", "#3A3E41FF");
    SetProperty("checkbox_size_x", "6");
    SetProperty("checkbox_size_y", "6");

    // COMBO
    SetProperty("combo_color_border", "#3F4245B2");
    SetProperty("combo_color_border_hovered", "#3F4245B2");
    SetProperty("combo_color_border_clicked", "#3F4245B2");
    SetProperty("combo_color_border_pressed", "#3F4245B2");
    SetProperty("combo_color_bg", "#24282CFF");
    SetProperty("combo_color_bg_hovered", "#2F3337FF");
    SetProperty("combo_color_bg_pressed", "#3A3E41FF");
    SetProperty("combo_color_bg_clicked", "#3A3E41FF");
    SetProperty("combo_size_x", "6");
    SetProperty("combo_size_y", "6");

    // HEADER
    SetProperty("header_color_border", "#3F4245B2");
    SetProperty("header_color_border_hovered", "#3F4245B2");
    SetProperty("header_color_border_clicked", "#3F4245B2");
    SetProperty("header_color_border_pressed", "#3F4245B2");
    SetProperty("header_color_bg", "#24282CFF");
    SetProperty("header_color_bg_hovered", "#2F3337FF");
    SetProperty("header_color_bg_pressed", "#3A3E41FF");
    SetProperty("header_color_bg_clicked", "#3A3E41FF");
    SetProperty("header_size_x", "6");
    SetProperty("header_size_y", "6");
    SetProperty("header_button_color_border", "#3F4245FF");
    SetProperty("header_button_color_border_hovered", "#4A4E51FF");
    SetProperty("header_button_color_border_clicked", "#44484BFF");
    SetProperty("header_button_color_border_pressed", "#6C6F72FF");
    SetProperty("header_button_color_bg", "#24282CFF");
    SetProperty("header_button_color_bg_hovered", "#2F3337FF");
    SetProperty("header_button_color_bg_pressed", "#3A3E41FF");
    SetProperty("header_button_color_bg_clicked", "#3A3E41FF");
    SetProperty("header_button_color_text", "#C2C2C2FF");
    SetProperty("header_button_color_text_hovered", "#FFFFFFFF");
    SetProperty("header_button_color_text_pressed", "#FFFFFFFF");

    // IMAGE
    SetProperty("image_color_border", "#3F4245B2");
    SetProperty("image_color_border_hovered", "#3F4245B2");
    SetProperty("image_color_border_clicked", "#3F4245B2");
    SetProperty("image_color_border_pressed", "#3F4245B2");
    SetProperty("image_color_bg", "#24282CFF");
    SetProperty("image_color_bg_hovered", "#2F3337FF");
    SetProperty("image_color_bg_pressed", "#3A3E41FF");
    SetProperty("image_color_bg_clicked", "#3A3E41FF");
    SetProperty("image_size_x", "6");
    SetProperty("image_size_y", "6");

    // MODAL
    SetProperty("modal_padding_x", "6");
    SetProperty("modal_padding_y", "6");

    // NOTIFICATION
    SetProperty("notification_color_border", "#3F4245FF");
    SetProperty("notification_color_border_hovered", "#4A4E51FF");
    SetProperty("notification_color_bg", "#24282CFF");
    SetProperty("notification_color_bg_hovered", "#2F3337FF");

    // SEPARATOR
    SetProperty("separator_color", "#3F4245");
    SetProperty("separator_color_text", "#3F4245B2");

    // KEYVAL
    SetProperty("keyval_color_text", "#3F4245B2");

    // TABLE
    SetProperty("table_color_border", "#202325");
    SetProperty("table_cell_padding_x_header", "6.0");
    SetProperty("table_cell_padding_y_header", "6.0");
    SetProperty("table_cell_padding_x_row", "6.0");
    SetProperty("table_cell_padding_y_row", "6.0");

    // TEXT AREA
    SetProperty("text_area_color_text", "#FFFFFFFF");

    // TEXT
    SetProperty("text_color_text", "#3F4245B2");

    // TITLES
    SetProperty("title_color_text", "#FFFFFFFF");

    // TOOLTIP
    SetProperty("tooltip_color_border", "#3F4245B2");
    SetProperty("tooltip_color_border_hovered", "#3F4245B2");
    SetProperty("tooltip_color_border_clicked", "#3F4245B2");
    SetProperty("tooltip_color_bg", "#24282CFF");
    SetProperty("tooltip_color_bg_hovered", "#2F3337FF");
    SetProperty("tooltip_color_bg_clicked", "#3A3E41FF");

    // BLOCKS
    SetProperty("block_color", "#1E1E1E");
    SetProperty("block_color_hovered", "#353535");
    SetProperty("block_color_pressed", "#4A4A4A");
    SetProperty("block_border_color", "#2C2C2C");
    SetProperty("block_border_color_hovered", "#2C2C2C");
    SetProperty("block_border_color_pressed", "#4A4A4A");
    SetProperty("block_border_radius", std::to_string(0.0f));
    SetProperty("block_border_size", std::to_string(1.0f));
  }
};
} // namespace Themes

} // namespace Cherry

#endif // CHERRY_KIT_DARK
