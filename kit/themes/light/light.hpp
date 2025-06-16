#pragma once
#include "../../../platform/engine/app/app.hpp"
#include "../../../platform/engine/themes/themes.hpp"

//
// Light
//

#ifndef CHERRY_KIT_THEME_LIGHT
#define CHERRY_KIT_THEME_LIGHT

namespace Cherry {
namespace Themes {
class Light : public Theme {
public:
  Light() {
    SetName("light");

    // General theme, windows
    SetProperty("color_header", "#EAEAEA");
    SetProperty("color_header_hovered", "#DCDCDC");
    SetProperty("color_header_active", "#D0D0D0");
    SetProperty("color", "#F5F5F5");
    SetProperty("color_hovered", "#EBEBEB");
    SetProperty("color_active", "#DADADA");
    SetProperty("color_framebg", "#FFFFFF");
    SetProperty("color_framebg_hovered", "#F0F0F0");
    SetProperty("color_framebg_active", "#E0E0E0");
    SetProperty("color_tab", "#F2F2F2");
    SetProperty("color_tab_hovered", "#E6E6E6");
    SetProperty("color_tab_active", "#DDDDDD");
    SetProperty("color_tab_unfocused", "#F2F2F2");
    SetProperty("color_tab_unfocused_active", "#E6E6E6");
    SetProperty("color_titlebg", "#F0F0F0");
    SetProperty("color_titlebg_active", "#E0E0E0");
    SetProperty("color_titlebg_collapsed", "#FAFAFA");
    SetProperty("color_resizegrip", "#AAAAAA40");
    SetProperty("color_resizegrip_hovered", "#888888AA");
    SetProperty("color_resizegrip_active", "#3D7EFF");
    SetProperty("color_scrollbar_bg", "#EEEEEE");
    SetProperty("color_scrollbar_grab", "#C2C2C2");
    SetProperty("color_scrollbar_grab_hovered", "#A8A8A8");
    SetProperty("color_text", "#252525FF"); // hex
    SetProperty("color_scrollbar_grab_active", "#8F8F8F");
    SetProperty("color_checkmark", "#3D7EFF");
    SetProperty("color_slidergrab", "#BBBBBB");
    SetProperty("color_slidergrab_active", "#3D7EFF");
    SetProperty("color_separator", "#DDDDDD");
    SetProperty("color_separator_active", "#3D7EFF");
    SetProperty("color_separator_hovered", "#CCCCCC");
    SetProperty("color_window_bg", "#FFFFFF");
    SetProperty("color_child_bg", "#F9F9F9");
    SetProperty("color_popup_bg", "#FFFFFF");
    SetProperty("color_border", "#DDDDDD");
    SetProperty("color_table_header_bg", "#EDEDED");
    SetProperty("color_table_border_light", "#F3F3F3");
    SetProperty("color_menubar_bg", "#FAFAFA");

    // Buttons
    SetProperty("button_color_border", "#CCCCCC");
    SetProperty("button_color_border_hovered", "#BBBBBB");
    SetProperty("button_color_border_clicked", "#AAAAAA");
    SetProperty("button_color_border_pressed", "#3D7EFF");
    SetProperty("button_color_bg", "#F0F0F0");
    SetProperty("button_color_bg_hovered", "#E6E6E6");
    SetProperty("button_color_bg_pressed", "#DADADA");
    SetProperty("button_color_bg_clicked", "#DADADA");
    SetProperty("button_color_text", "#333333");
    SetProperty("button_color_text_hovered", "#000000");
    SetProperty("button_color_text_pressed", "#000000");
    SetProperty("button_color_underline", "#CCCCCC");
    SetProperty("button_color_underline_hovered", "#AAAAAA");
    SetProperty("button_color_underline_pressed", "#3D7EFF");
    SetProperty("button_size_x", "0");
    SetProperty("button_size_y", "0");
    SetProperty("button_padding_x", "6");
    SetProperty("button_padding_y", "6");
    SetProperty("button_scale", "0");

    // Checkbox
    SetProperty("checkbox_color_border", "#CCCCCC");
    SetProperty("checkbox_color_border_hovered", "#AAAAAA");
    SetProperty("checkbox_color_border_clicked", "#888888");
    SetProperty("checkbox_color_border_pressed", "#3D7EFF");
    SetProperty("checkbox_color_bg", "#FFFFFF");
    SetProperty("checkbox_color_bg_hovered", "#F5F5F5");
    SetProperty("checkbox_color_bg_pressed", "#E5E5E5");
    SetProperty("checkbox_color_bg_clicked", "#E5E5E5");
    SetProperty("checkbox_size_x", "6");
    SetProperty("checkbox_size_y", "6");

    // Tooltip
    SetProperty("tooltip_color_border", "#CCCCCC");
    SetProperty("tooltip_color_border_hovered", "#AAAAAA");
    SetProperty("tooltip_color_border_clicked", "#888888");
    SetProperty("tooltip_color_bg", "#FAFAFA");
    SetProperty("tooltip_color_bg_hovered", "#F0F0F0");
    SetProperty("tooltip_color_bg_clicked", "#E0E0E0");

    // Text
    SetProperty("text_area_color_text", "#222222");
    SetProperty("text_color_text", "#222222");
    SetProperty("title_color_text", "#000000");

    // Notifications
    SetProperty("notification_color_border", "#AAAAAA");
    SetProperty("notification_color_border_hovered", "#3D7EFF");
    SetProperty("notification_color_bg", "#FFFFFF");
    SetProperty("notification_color_bg_hovered", "#F7F7F7");

    // Blocks
    SetProperty("block_color", "#F5F5F5");
    SetProperty("block_color_hovered", "#E5E5E5");
    SetProperty("block_color_pressed", "#D5D5D5");
    SetProperty("block_border_color", "#CCCCCC");
    SetProperty("block_border_color_hovered", "#BBBBBB");
    SetProperty("block_border_color_pressed", "#3D7EFF");
    SetProperty("block_border_radius", std::to_string(0.0f));
    SetProperty("block_border_size", std::to_string(1.0f));
  }
};
} // namespace Themes
} // namespace Cherry

#endif // CHERRY_KIT_THEME_LIGHT
