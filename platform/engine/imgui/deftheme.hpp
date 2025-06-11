#include "../themes.hpp"

#ifndef CHERRY_DEFTHEME_HPP
#define CHERRY_DEFTHEME_HPP

namespace Cherry {
class DefaultCherryTheme : public Theme {
public:
  DefaultCherryTheme() {
    SetName("default");

    // General theme, windows
    SetProperty("color_header", "#2F2F2FFF");                 // hex
    SetProperty("color_header_hovered", "#2F2F2FFF");         // hex
    SetProperty("color_header_active", "#2F2F2FFF");          // hex
    SetProperty("color", "#383838C8");                        // hex
    SetProperty("color_hovered", "#464646FF");                // hex
    SetProperty("color_active", "#38383896");                 // hex
    SetProperty("color_framebg", "#0F0F0FFF");                // hex
    SetProperty("color_framebg_hovered", "#0F0F0FFF");        // hex
    SetProperty("color_framebg_active", "#0F0F0FFF");         // hex
    SetProperty("color_tab", "#151515FF");                    // hex
    SetProperty("color_tab_hovered", "#24242464");            // hex
    SetProperty("color_tab_active", "#242424FF");             // hex
    SetProperty("color_tab_unfocused", "#151515FF");          // hex
    SetProperty("color_tab_unfocused_active", "#24242464");   // hex
    SetProperty("color_titlebg", "#151515FF");                // hex
    SetProperty("color_titlebg_active", "#151515FF");         // hex
    SetProperty("color_titlebg_collapsed", "#262626FF");      // hex
    SetProperty("color_resizegrip", "#E8E8E840");             // hex
    SetProperty("color_resizegrip_hovered", "#CFCFCFAA");     // hex
    SetProperty("color_resizegrip_active", "#757575F2");      // hex
    SetProperty("color_scrollbar_bg", "#05050587");           // hex
    SetProperty("color_scrollbar_grab", "#4F4F4FFF");         // hex
    SetProperty("color_scrollbar_grab_hovered", "#696969FF"); // hex
    SetProperty("color_scrollbar_grab_active", "#828282FF");  // hex
    SetProperty("color_checkmark", "#C8C8C8FF");              // hex
    SetProperty("color_slidergrab", "#828282B3");             // hex
    SetProperty("color_slidergrab_active", "#A8A8A8FF");      // hex
    SetProperty("color_separator", "#151515FF");              // hex
    SetProperty("color_separator_active", "#27B9F2FF");       // hex
    SetProperty("color_separator_hovered", "#151515FF");      // hex
    SetProperty("color_window_bg", "#151515FF");              // hex
    SetProperty("color_child_bg", "#242424FF");               // hex
    SetProperty("color_popup_bg", "#383838FF");               // hex
    SetProperty("color_border", "#242424FF");                 // hex
    SetProperty("color_table_header_bg", "#2F2F2FFF");        // hex
    SetProperty("color_table_border_light", "#1A1A1AFF");     // hex
    SetProperty("color_menubar_bg", "#242424FF");             // hex

    // Buttons
    SetProperty("button_color_border", "#454545FF");         // hex
    SetProperty("button_color_border_hovered", "#555555FF"); // hex
    SetProperty("button_color_border_pressed", "#757575FF"); // hex
    SetProperty("button_color_bg", "#242424FF");             // hex
    SetProperty("button_color_bg_hovered", "#343434FF");     // hex
    SetProperty("button_color_bg_pressed", "#444444FF");     // hex
    SetProperty("button_color_text", "#BCBCBCFF");           // hex
    SetProperty("button_color_text_hovered", "#FFFFFFFF");   // hex
    SetProperty("button_color_text_pressed", "#FFFFFFFF");   // hex
    SetProperty("button_size_x", "0");                       // int
    SetProperty("button_size_y", "0");                       // int
    SetProperty("button_padding_x", "6");                    // int
    SetProperty("button_padding_y", "6");                    // int
    SetProperty("button_scale", "0");                        // int

    // Titles
  }
};

}; // namespace Cherry
#endif // CHERRY_DEFTHEME_HPP