#pragma once

#include "options.hpp"

#ifndef CHERRY_H
#define CHERRY_H
#define SDL_MAIN_HANDLED

#ifdef CHERRY_USER_CONFIG
#include CHERRY_USER_CONFIG
#endif
#if !defined(CHERRY_DISABLE_INCLUDE_CONFIG_H) ||                               \
    defined(CHERRY_INCLUDE_CONFIG_H)
#include "options.hpp"
#endif

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS

///////////////////////////////////////////////////////////////
/// Backends and Render engine                              ///
///////////////////////////////////////////////////////////////
#include "./platform/engine/app/app.hpp"
#include "./platform/engine/components/components.hpp"
#include "./platform/engine/embed/assets/icons.h"
#include "./platform/engine/image/image.hpp"
#include "./platform/engine/imgui/ImGuiTheme.h"
#include "./platform/engine/imgui/wrappers/wrappers.hpp"
#include "./platform/engine/ui/notifications/notifications.hpp"
#include "./platform/engine/ui/ui.hpp"
#include "./src/entry_point.hpp"
#include "./src/layer.hpp"
#include "./src/random.hpp"
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
/// Kit : 80+ Components, All in one windows, theme, utils...   ///
///////////////////////////////////////////////////////////////
// Buttons
#include "./kit/components/buttons/button_image/button_image.hpp"
#include "./kit/components/buttons/button_image_drowpdown/button_image_drowpdown.hpp"
#include "./kit/components/buttons/button_image_text/button_image_text.hpp"
#include "./kit/components/buttons/button_image_text_drowpdown/button_image_text_drowpdown.hpp"
#include "./kit/components/buttons/button_image_text_image/button_image_text_image.hpp"
#include "./kit/components/buttons/button_text/button_text.hpp"
#include "./kit/components/buttons/button_text_dropdown/button_text_dropdown.hpp"
#include "./kit/components/buttons/button_text_image/button_text_image.hpp"
#include "./kit/components/buttons/button_text_image_drowpdown/button_text_image_drowpdown.hpp"
#include "./kit/components/buttons/button_text_underline/button_text_underline.hpp"
// Titles
#include "./kit/components/titles/title_h1/title_h1.hpp"
#include "./kit/components/titles/title_h2/title_h2.hpp"
#include "./kit/components/titles/title_h3/title_h3.hpp"
#include "./kit/components/titles/title_h4/title_h4.hpp"
#include "./kit/components/titles/title_h5/title_h5.hpp"
#include "./kit/components/titles/title_h6/title_h6.hpp"
// Texts
#include "./kit/components/texts/text_center/text_center.hpp"
#include "./kit/components/texts/text_image/text_image.hpp"
#include "./kit/components/texts/text_left/text_left.hpp"
#include "./kit/components/texts/text_right/text_right.hpp"
#include "./kit/components/texts/text_simple/text_simple.hpp"
#include "./kit/components/texts/text_wrapped/text_wrapped.hpp"
// Table
#include "./kit/components/tables/table_custom/table_custom.hpp"
#include "./kit/components/tables/table_simple/table_simple.hpp"
// Keyval
#include "./kit/components/keyval/keyval_bool/keyval_bool.hpp"
#include "./kit/components/keyval/keyval_color_hex/keyval_color_hex.hpp"
#include "./kit/components/keyval/keyval_combo_string/keyval_combo_string.hpp"
#include "./kit/components/keyval/keyval_custom/keyval_custom.hpp"
#include "./kit/components/keyval/keyval_double/keyval_double.hpp"
#include "./kit/components/keyval/keyval_float/keyval_float.hpp"
#include "./kit/components/keyval/keyval_int/keyval_int.hpp"
#include "./kit/components/keyval/keyval_parent/keyval_parent.hpp"
#include "./kit/components/keyval/keyval_string/keyval_string.hpp"
#include "./kit/components/keyval/keyval_vector1/keyval_vector1.hpp"
#include "./kit/components/keyval/keyval_vector2/keyval_vector2.hpp"
#include "./kit/components/keyval/keyval_vector3/keyval_vector3.hpp"
#include "./kit/components/keyval/keyval_vector3_5/keyval_vector3_5.hpp"
#include "./kit/components/keyval/keyval_vector4/keyval_vector4.hpp"
// Headers
#include "./kit/components/headers/header_image_text/header_image_text.hpp"
#include "./kit/components/headers/header_image_text_button/header_image_text_button.hpp"
#include "./kit/components/headers/header_text/header_text.hpp"
#include "./kit/components/headers/header_text_button/header_text_button.hpp"
// Combos
#include "./kit/components/combos/combo_custom/combo_custom.hpp"
#include "./kit/components/combos/combo_image_text/combo_image_text.hpp"
#include "./kit/components/combos/combo_text/combo_text.hpp"
// Separators
#include "./kit/components/separators/separator/separator.hpp"
#include "./kit/components/separators/separator_text/separator_text.hpp"
// Spaces
#include "./kit/components/spaces/space/space.hpp"
// Nodes
#include "./kit/components/nodes/node_areas/node_area_maker/node_area_maker.hpp"
#include "./kit/components/nodes/node_areas/node_area_open/node_area_open.hpp"
// Checkboxes
#include "./kit/components/checkboxes/checkbox_text/checkbox_text.hpp"
// Images
#include "./kit/components/images/image_http/image_http.hpp"
#include "./kit/components/images/image_local/image_local.hpp"
#include "./kit/components/images/image_local_centered/image_local_centered.hpp"
// Inputs
#include "./kit/components/inputs/input_double/input_double.hpp"
#include "./kit/components/inputs/input_float/input_float.hpp"
#include "./kit/components/inputs/input_int/input_int.hpp"
#include "./kit/components/inputs/input_multiline_string/input_multiline_string.hpp"
#include "./kit/components/inputs/input_string/input_string.hpp"
// Bullets
#include "./kit/components/bullets/bullet_button/bullet_button.hpp"
#include "./kit/components/bullets/bullet_multitext/bullet_multitext.hpp"
#include "./kit/components/bullets/bullet_text/bullet_text.hpp"
// Bullets
#include "./kit/components/lists/list_custom/list_custom.hpp"
#include "./kit/components/lists/list_image_string/list_image_string.hpp"
#include "./kit/components/lists/list_string/list_string.hpp"
// Tooltips
#include "./kit/components/tooltips/tooltip_image/tooltip_image.hpp"
#include "./kit/components/tooltips/tooltip_image_custom/tooltip_image_custom.hpp"
#include "./kit/components/tooltips/tooltip_text/tooltip_text.hpp"
#include "./kit/components/tooltips/tooltip_text_custom/tooltip_text_custom.hpp"
// Progresses
#include "./kit/components/progress/progress_bar_percentage/progress_bar_percentage.hpp"
#include "./kit/components/progress/progress_bar_percentage_float/progress_bar_percentage_float.hpp"
#include "./kit/components/progress/progress_bar_percentage_int/progress_bar_percentage_int.hpp"
// Progresses
#include "./kit/components/plots/plot_historigram_float/plot_historigram_float.hpp"
#include "./kit/components/plots/plot_line_float/plot_line_float.hpp"
// Grid
#include "./kit/components/grid/grid_simple/grid_simple.hpp"
// Widgets
#include "./kit/components/widgets/widget_folder/widget_folder.hpp"
#include "./kit/components/widgets/widget_small_folder/widget_small_folder.hpp"
// Blocks
#include "./kit/components/blocks/block_horizontal_custom/block_horizontal_custom.hpp"
#include "./kit/components/blocks/block_vertical_custom/block_vertical_custom.hpp"
// Banner
#include "./kit/components/banners/banner_image_context/banner_image_context.hpp"
// Modals
#include "./kit/components/modals/modal_title/modal_title.hpp"
// Notifications
#include "./kit/components/notifications/notification_button/notification_button.hpp"
#include "./kit/components/notifications/notification_custom/notification_custom.hpp"
#include "./kit/components/notifications/notification_simple/notification_simple.hpp"
// Text
#include "./kit/components/text/text_area/text_area.hpp"

///////////////////////////////////////////////////////////////
/// Style : +5 Style effects , All in one windows, theme, utils...   ///
///////////////////////////////////////////////////////////////
// Margin style
#include "./kit/utils/behavior/margin/margin.hpp"
// Font style
#include "./kit/utils/behavior/font/font.hpp"

///////////////////////////////////////////////////////////////
/// AIO : Windows, Ready-builtin windows                    ///
///////////////////////////////////////////////////////////////
#include "./kit/aio/bases/app_windows/appwindow_simple/appwindow_simple.hpp"

///////////////////////////////////////////////////////////////
/// Themes                                                  ///
///////////////////////////////////////////////////////////////
#include "./kit/themes/dark/dark.hpp"
#include "./kit/themes/dark_colorfull/dark_colorfull.hpp"
#include "./kit/themes/dark_high_contrast/dark_high_contrast.hpp"
#include "./kit/themes/dark_vortex/dark_vortex.hpp"
#include "./kit/themes/light/light.hpp"
#include "./kit/themes/light_colorfull/light_colorfull.hpp"
#include "./kit/themes/light_high_contrast/light_high_contrast.hpp"
#include "./kit/themes/light_vortex/light_vortex.hpp"

namespace CherryKit = Cherry::Kit;
namespace CherryStyle = Cherry::Style;
namespace CherryThemes = Cherry::Themes;
using CherryApplication = Cherry::Application;

// namespace CherryLogic = Cherry::Logic // CherryLogic::PlaySound,
// IsKeyPressed, etc...

// AIO kits & components includes

#endif // CHERRY_H