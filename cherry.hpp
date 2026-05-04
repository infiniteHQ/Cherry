//
//  cherry.hpp
//  Root header file, with all important includes.
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include "options.hpp"

#ifndef CHERRY_H
#define CHERRY_H
#define SDL_MAIN_HANDLED

#ifdef CHERRY_USER_CONFIG
#include CHERRY_USER_CONFIG
#endif
#if !defined(CHERRY_DISABLE_INCLUDE_CONFIG_H) || defined(CHERRY_INCLUDE_CONFIG_H)
#include "options.hpp"
#endif

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif  // IMGUI_DEFINE_MATH_OPERATORS

// Backends
#include "./main/core/assert/assert.hpp"
#include "./main/core/color/color.hpp"
#include "./main/core/entry_point.hpp"
#include "./main/core/random/random.hpp"
#include "./main/engine/app/app.hpp"
#include "./main/engine/components/components.hpp"
#include "./main/engine/components/factory.hpp"
#include "./main/engine/embed/assets/icons.h"
#include "./main/engine/image/image.hpp"
#include "./main/engine/imgui/imgui_theme.h"
#include "./main/engine/imgui/markdown/md.hpp"
#include "./main/engine/imgui/wrappers/wrappers.hpp"
#include "./main/engine/scripting/scripting.hpp"
#include "./main/engine/shaders/shaders.hpp"
#include "./main/engine/ui/notifications/notifications.hpp"
#include "./main/engine/ui/ui.hpp"

// Minimal kit
#include "./main/kit/components/texts/text_simple/text_simple.hpp"
#include "./main/kit/themes/dark/dark.hpp"
#include "./main/kit/utils/behavior/margin/margin.hpp"

// Optionnal
#ifdef CHERRY_ENABLE_KIT
#include "./main/kit/components/banners/banner_image_context/banner_image_context.hpp"
#include "./main/kit/components/blocks/block_horizontal_custom/block_horizontal_custom.hpp"
#include "./main/kit/components/blocks/block_vertical_custom/block_vertical_custom.hpp"
#include "./main/kit/components/bullets/bullet_button/bullet_button.hpp"
#include "./main/kit/components/bullets/bullet_multitext/bullet_multitext.hpp"
#include "./main/kit/components/bullets/bullet_text/bullet_text.hpp"
#include "./main/kit/components/buttons/button_image/button_image.hpp"
#include "./main/kit/components/buttons/button_image_drowpdown/button_image_drowpdown.hpp"
#include "./main/kit/components/buttons/button_image_text/button_image_text.hpp"
#include "./main/kit/components/buttons/button_image_text_drowpdown/button_image_text_drowpdown.hpp"
#include "./main/kit/components/buttons/button_image_text_image/button_image_text_image.hpp"
#include "./main/kit/components/buttons/button_text/button_text.hpp"
#include "./main/kit/components/buttons/button_text_dropdown/button_text_dropdown.hpp"
#include "./main/kit/components/buttons/button_text_image/button_text_image.hpp"
#include "./main/kit/components/buttons/button_text_image_drowpdown/button_text_image_drowpdown.hpp"
#include "./main/kit/components/buttons/button_text_underline/button_text_underline.hpp"
#include "./main/kit/components/checkboxes/checkbox_text/checkbox_text.hpp"
#include "./main/kit/components/combos/combo_custom/combo_custom.hpp"
#include "./main/kit/components/combos/combo_image_text/combo_image_text.hpp"
#include "./main/kit/components/combos/combo_text/combo_text.hpp"
#include "./main/kit/components/grid/grid_simple/grid_simple.hpp"
#include "./main/kit/components/headers/header_image_text/header_image_text.hpp"
#include "./main/kit/components/headers/header_image_text_button/header_image_text_button.hpp"
#include "./main/kit/components/headers/header_text/header_text.hpp"
#include "./main/kit/components/headers/header_text_button/header_text_button.hpp"
#include "./main/kit/components/images/image_http/image_http.hpp"
#include "./main/kit/components/images/image_local/image_local.hpp"
#include "./main/kit/components/images/image_local_centered/image_local_centered.hpp"
#include "./main/kit/components/inputs/input_double/input_double.hpp"
#include "./main/kit/components/inputs/input_float/input_float.hpp"
#include "./main/kit/components/inputs/input_int/input_int.hpp"
#include "./main/kit/components/inputs/input_multiline_string/input_multiline_string.hpp"
#include "./main/kit/components/inputs/input_string/input_string.hpp"
#include "./main/kit/components/keyval/keyval_bool/keyval_bool.hpp"
#include "./main/kit/components/keyval/keyval_color_hex/keyval_color_hex.hpp"
#include "./main/kit/components/keyval/keyval_combo_string/keyval_combo_string.hpp"
#include "./main/kit/components/keyval/keyval_custom/keyval_custom.hpp"
#include "./main/kit/components/keyval/keyval_double/keyval_double.hpp"
#include "./main/kit/components/keyval/keyval_float/keyval_float.hpp"
#include "./main/kit/components/keyval/keyval_int/keyval_int.hpp"
#include "./main/kit/components/keyval/keyval_parent/keyval_parent.hpp"
#include "./main/kit/components/keyval/keyval_string/keyval_string.hpp"
#include "./main/kit/components/keyval/keyval_vector1/keyval_vector1.hpp"
#include "./main/kit/components/keyval/keyval_vector2/keyval_vector2.hpp"
#include "./main/kit/components/keyval/keyval_vector3/keyval_vector3.hpp"
#include "./main/kit/components/keyval/keyval_vector3_5/keyval_vector3_5.hpp"
#include "./main/kit/components/keyval/keyval_vector4/keyval_vector4.hpp"
#include "./main/kit/components/lists/list_custom/list_custom.hpp"
#include "./main/kit/components/lists/list_image_string/list_image_string.hpp"
#include "./main/kit/components/lists/list_string/list_string.hpp"
#include "./main/kit/components/modals/modal_title/modal_title.hpp"
#include "./main/kit/components/nodes/node_areas/node_area_maker/node_area_maker.hpp"
#include "./main/kit/components/nodes/node_areas/node_area_open/node_area_open.hpp"
#include "./main/kit/components/notifications/notification_button/notification_button.hpp"
#include "./main/kit/components/notifications/notification_custom/notification_custom.hpp"
#include "./main/kit/components/notifications/notification_simple/notification_simple.hpp"
#include "./main/kit/components/plots/plot_historigram_float/plot_historigram_float.hpp"
#include "./main/kit/components/plots/plot_line_float/plot_line_float.hpp"
#include "./main/kit/components/progress/progress_bar_percentage/progress_bar_percentage.hpp"
#include "./main/kit/components/progress/progress_bar_percentage_float/progress_bar_percentage_float.hpp"
#include "./main/kit/components/progress/progress_bar_percentage_int/progress_bar_percentage_int.hpp"
#include "./main/kit/components/separators/separator/separator.hpp"
#include "./main/kit/components/separators/separator_text/separator_text.hpp"
#include "./main/kit/components/spaces/space/space.hpp"
#include "./main/kit/components/tables/table_custom/table_custom.hpp"
#include "./main/kit/components/tables/table_simple/table_simple.hpp"
#include "./main/kit/components/text/text_area/text_area.hpp"
#include "./main/kit/components/texts/text_center/text_center.hpp"
#include "./main/kit/components/texts/text_image/text_image.hpp"
#include "./main/kit/components/texts/text_left/text_left.hpp"
#include "./main/kit/components/texts/text_right/text_right.hpp"
#include "./main/kit/components/texts/text_wrapped/text_wrapped.hpp"
#include "./main/kit/components/titles/title_h1/title_h1.hpp"
#include "./main/kit/components/titles/title_h2/title_h2.hpp"
#include "./main/kit/components/titles/title_h3/title_h3.hpp"
#include "./main/kit/components/titles/title_h4/title_h4.hpp"
#include "./main/kit/components/titles/title_h5/title_h5.hpp"
#include "./main/kit/components/titles/title_h6/title_h6.hpp"
#include "./main/kit/components/tooltips/tooltip_image/tooltip_image.hpp"
#include "./main/kit/components/tooltips/tooltip_image_custom/tooltip_image_custom.hpp"
#include "./main/kit/components/tooltips/tooltip_text/tooltip_text.hpp"
#include "./main/kit/components/tooltips/tooltip_text_custom/tooltip_text_custom.hpp"
#include "./main/kit/components/widgets/widget_folder/widget_folder.hpp"
#include "./main/kit/components/widgets/widget_small_folder/widget_small_folder.hpp"

// Style
#include "./main/kit/utils/behavior/font/font.hpp"

// Ready to use app windows
#include "./main/kit/app_windows/appwindow_simple/appwindow_simple.hpp"

// Themes
#include "./main/kit/themes/dark_colorfull/dark_colorfull.hpp"
#include "./main/kit/themes/dark_high_contrast/dark_high_contrast.hpp"
#include "./main/kit/themes/dark_vortex/dark_vortex.hpp"
#include "./main/kit/themes/light/light.hpp"
#include "./main/kit/themes/light_colorfull/light_colorfull.hpp"
#include "./main/kit/themes/light_high_contrast/light_high_contrast.hpp"
#include "./main/kit/themes/light_vortex/light_vortex.hpp"
#endif  // CHERRY_ENABLE_KIT

namespace CherryKit = Cherry::Kit;
namespace CherryDrawing = Cherry::Drawing;
namespace CherryScript = Cherry::Script;
namespace CherryStyle = Cherry::Style;
namespace CherryThemes = Cherry::Themes;
using CherryApplication = Cherry::Application;

// namespace CherryLogic = Cherry::Logic // CherryLogic::PlaySound,
// IsKeyPressed, etc...

#endif  // CHERRY_H