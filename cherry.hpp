#ifndef CHERRY_H
#define CHERRY_H
#define SDL_MAIN_HANDLED

#include "./options.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS

///////////////////////////////////////////////////////////////
/// Backends and Render engine                              ///
///////////////////////////////////////////////////////////////
#include "./platform/engine/ui/notifications/notifications.hpp"
#include "./platform/engine/ui/ui.hpp"
#include "./src/layer.hpp"
#include "./src/random.hpp"
#include "./src/entry_point.hpp"
#include "./platform/engine/imgui/wrappers.hpp"
#include "./platform/engine/imgui/ImGuiTheme.h"
#include "./platform/engine/image.hpp"
#include "./platform/engine/app.hpp"
#include "./platform/engine/cef.hpp"
#include "./platform/engine/embed/assets/icons.h"
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
/// Kit : Components, All in one windows, theme, utils...   ///
///////////////////////////////////////////////////////////////
// Buttons
#include "./kit/components/buttons/button_text/button_text.hpp"
#include "./kit/components/buttons/button_image/button_image.hpp"
#include "./kit/components/buttons/button_image_text/button_image_text.hpp"
#include "./kit/components/buttons/button_text_image/button_text_image.hpp"
#include "./kit/components/buttons/button_image_drowpdown/button_image_drowpdown.hpp"
#include "./kit/components/buttons/button_image_text_drowpdown/button_image_text_drowpdown.hpp"
#include "./kit/components/buttons/button_text_dropdown/button_text_dropdown.hpp"
#include "./kit/components/buttons/button_image_text_image/button_image_text_image.hpp"
#include "./kit/components/buttons/button_text_image_drowpdown/button_text_image_drowpdown.hpp"
// Titles
#include "./kit/components/titles/title_h1/title_h1.hpp"
#include "./kit/components/titles/title_h2/title_h2.hpp"
#include "./kit/components/titles/title_h3/title_h3.hpp"
#include "./kit/components/titles/title_h4/title_h4.hpp"
#include "./kit/components/titles/title_h5/title_h5.hpp"
#include "./kit/components/titles/title_h6/title_h6.hpp"
// Texts
#include "./kit/components/texts/text_simple/text_simple.hpp"
#include "./kit/components/texts/text_left/text_left.hpp"
#include "./kit/components/texts/text_right/text_right.hpp"
#include "./kit/components/texts/text_wrapped/text_wrapped.hpp"
#include "./kit/components/texts/text_image/text_image.hpp"
// Table
#include "./kit/components/tables/table_simple/table_simple.hpp"
// Keyval
#include "./kit/components/keyval/keyval_double/keyval_double.hpp"
#include "./kit/components/keyval/keyval_float/keyval_float.hpp"
#include "./kit/components/keyval/keyval_int/keyval_int.hpp"
#include "./kit/components/keyval/keyval_string/keyval_string.hpp"
#include "./kit/components/keyval/keyval_combo_string/keyval_combo_string.hpp"
#include "./kit/components/keyval/keyval_vector1/keyval_vector1.hpp"
#include "./kit/components/keyval/keyval_vector2/keyval_vector2.hpp"
#include "./kit/components/keyval/keyval_vector3/keyval_vector3.hpp"
#include "./kit/components/keyval/keyval_vector4/keyval_vector4.hpp"
#include "./kit/components/keyval/keyval_vector3_5/keyval_vector3_5.hpp"
// Headers
#include "./kit/components/headers/header_text/header_text.hpp"
#include "./kit/components/headers/header_image_text/header_image_text.hpp"
#include "./kit/components/headers/header_image_text_button/header_image_text_button.hpp"
#include "./kit/components/headers/header_text_button/header_text_button.hpp"
// Combos
#include "./kit/components/combos/combo_text/combo_text.hpp"
#include "./kit/components/combos/combo_image_text/combo_image_text.hpp"
#include "./kit/components/combos/combo_custom/combo_custom.hpp"
// Separators
#include "./kit/components/separators/separator_text/separator_text.hpp"
#include "./kit/components/separators/separator/separator.hpp"
// Spaces
#include "./kit/components/spaces/space/space.hpp"
// Nodes
#include "./kit/components/nodes/node_area/node_area.hpp"
// Checkboxes
#include "./kit/components/checkboxes/checkbox_text/checkbox_text.hpp"
// Images
#include "./kit/components/images/image_local/image_local.hpp"
#include "./kit/components/images/image_http/image_http.hpp"
// Inputs
#include "./kit/components/inputs/input_string/input_string.hpp"
#include "./kit/components/inputs/input_int/input_int.hpp"
#include "./kit/components/inputs/input_float/input_float.hpp"
#include "./kit/components/inputs/input_double/input_double.hpp"
// Bullets
#include "./kit/components/bullets/bullet_text/bullet_text.hpp"
#include "./kit/components/bullets/bullet_button/bullet_button.hpp"
#include "./kit/components/bullets/bullet_multitext/bullet_multitext.hpp"

namespace CherryKit = Cherry::Kit;

// AIO kits & components includes

#endif // CHERRY_H