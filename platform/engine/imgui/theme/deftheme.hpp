#include "../../themes/themes.hpp"

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
    SetProperty("button_color_border", "#454545FF");            // hex
    SetProperty("button_color_border_hovered", "#555555FF");    // hex
    SetProperty("button_color_border_clicked", "#444444FF");    // hex
    SetProperty("button_color_border_pressed", "#757575FF");    // hex
    SetProperty("button_color_bg", "#242424FF");                // hex
    SetProperty("button_color_bg_hovered", "#343434FF");        // hex
    SetProperty("button_color_bg_pressed", "#444444FF");        // hex
    SetProperty("button_color_bg_clicked", "#444444FF");        // hex
    SetProperty("button_color_text", "#BCBCBCFF");              // hex
    SetProperty("button_color_text_hovered", "#FFFFFFFF");      // hex
    SetProperty("button_color_text_pressed", "#FFFFFFFF");      // hex
    SetProperty("button_color_underline", "#242424FF");         // hex
    SetProperty("button_color_underline_hovered", "#343434FF"); // hex
    SetProperty("button_color_underline_pressed", "#444444FF"); // hex
    SetProperty("button_size_x", "0");                          // int
    SetProperty("button_size_y", "0");                          // int
    SetProperty("button_padding_x", "6");                       // int
    SetProperty("button_padding_y", "6");                       // int
    SetProperty("button_scale", "0");                           // int

    // Checkbox
    SetProperty("checkbox_color_border", "#454545B2");         // hex
    SetProperty("checkbox_color_border_hovered", "#454545B2"); // hex
    SetProperty("checkbox_color_border_clicked", "#454545B2"); // hex
    SetProperty("checkbox_color_border_pressed", "#454545B2"); // hex
    SetProperty("checkbox_color_bg", "#242424FF");             // hex
    SetProperty("checkbox_color_bg_hovered", "#343434FF");     // hex
    SetProperty("checkbox_color_bg_pressed", "#444444FF");     // hex
    SetProperty("checkbox_color_bg_clicked", "#444444FF");     // hex
    SetProperty("checkbox_size_x", "6");                       // int
    SetProperty("checkbox_size_y", "6");                       // int

    // Combo
    SetProperty("combo_color_border", "#454545B2");         // hex
    SetProperty("combo_color_border_hovered", "#454545B2"); // hex
    SetProperty("combo_color_border_clicked", "#454545B2"); // hex
    SetProperty("combo_color_border_pressed", "#454545B2"); // hex
    SetProperty("combo_color_bg", "#242424FF");             // hex
    SetProperty("combo_color_bg_hovered", "#343434FF");     // hex
    SetProperty("combo_color_bg_pressed", "#444444FF");     // hex
    SetProperty("combo_color_bg_clicked", "#444444FF");     // hex
    SetProperty("combo_size_x", "6");                       // int
    SetProperty("combo_size_y", "6");                       // int

    // Header
    SetProperty("header_color_border", "#454545B2");                // hex
    SetProperty("header_color_border_hovered", "#454545B2");        // hex
    SetProperty("header_color_border_clicked", "#454545B2");        // hex
    SetProperty("header_color_border_pressed", "#454545B2");        // hex
    SetProperty("header_color_bg", "#242424FF");                    // hex
    SetProperty("header_color_bg_hovered", "#343434FF");            // hex
    SetProperty("header_color_bg_pressed", "#444444FF");            // hex
    SetProperty("header_color_bg_clicked", "#444444FF");            // hex
    SetProperty("header_size_x", "6");                              // int
    SetProperty("header_size_y", "6");                              // int
    SetProperty("header_button_color_border", "#454545FF");         // hex
    SetProperty("header_button_color_border_hovered", "#555555FF"); // hex
    SetProperty("header_button_color_border_clicked", "#444444FF"); // hex
    SetProperty("header_button_color_border_pressed", "#757575FF"); // hex
    SetProperty("header_button_color_bg", "#242424FF");             // hex
    SetProperty("header_button_color_bg_hovered", "#343434FF");     // hex
    SetProperty("header_button_color_bg_pressed", "#444444FF");     // hex
    SetProperty("header_button_color_bg_clicked", "#444444FF");     // hex
    SetProperty("header_button_color_text", "#BCBCBCFF");           // hex
    SetProperty("header_button_color_text_hovered", "#FFFFFFFF");   // hex
    SetProperty("header_button_color_text_pressed", "#FFFFFFFF");   // hex

    // Image
    SetProperty("image_color_border", "#454545B2");         // hex
    SetProperty("image_color_border_hovered", "#454545B2"); // hex
    SetProperty("image_color_border_clicked", "#454545B2"); // hex
    SetProperty("image_color_border_pressed", "#454545B2"); // hex
    SetProperty("image_color_bg", "#242424FF");             // hex
    SetProperty("image_color_bg_hovered", "#343434FF");     // hex
    SetProperty("image_color_bg_pressed", "#444444FF");     // hex
    SetProperty("image_color_bg_clicked", "#444444FF");     // hex
    SetProperty("image_size_x", "6");                       // int
    SetProperty("image_size_y", "6");                       // int

    // Modal
    SetProperty("modal_padding_x", "6"); // int
    SetProperty("modal_padding_y", "6"); // int

    // Notification
    SetProperty("notification_color_border", "#454545FF");
    SetProperty("notification_color_border_hovered", "#555555FF");
    SetProperty("notification_color_bg", "#242424FF");
    SetProperty("notification_color_bg_hovered", "#343434FF");

    // Separator
    SetProperty("separator_color", "#424242");
    SetProperty("separator_color_text", "#454545B2");

    // Keyval
    SetProperty("keyval_color_text", "#454545B2"); // hex

    // Table
    SetProperty("table_color_border", "#232323");
    SetProperty("table_cell_padding_x_header", "6.0"); // float
    SetProperty("table_cell_padding_y_header", "6.0"); // float
    SetProperty("table_cell_padding_x_row", "6.0");    // float
    SetProperty("table_cell_padding_y_row", "6.0");    // float

    // Text Area
    SetProperty("text_area_color_text", "#FFFFFFFF");

    // Text
    SetProperty("text_color_text", "#454545B2");

    // Titles
    SetProperty("title_color_text", "#FFFFFFFF");

    // Tooltip
    SetProperty("tooltip_color_border", "#454545B2");
    SetProperty("tooltip_color_border_hovered", "#454545B2");
    SetProperty("tooltip_color_border_clicked", "#454545B2");
    SetProperty("tooltip_color_bg", "#242424FF");
    SetProperty("tooltip_color_bg_hovered", "#343434FF");
    SetProperty("tooltip_color_bg_clicked", "#444444FF");

    // Blocks
    SetProperty("block_color", "#252525");
    SetProperty("block_color_hovered", "#454545");
    SetProperty("block_color_pressed", "#555555");
    SetProperty("block_border_color", "#353535");
    SetProperty("block_border_color_hovered", "#353535");
    SetProperty("block_border_color_pressed", "#555555");
    SetProperty("block_border_radius", std::to_string(0.0f));
    SetProperty("block_border_size", std::to_string(1.0f));
  }
};

}; // namespace Cherry
#endif // CHERRY_DEFTHEME_HPP