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
#include "./kit/components/buttons/button_text_dropdown/button_text_dropdown.hpp"
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
#include "./kit/components/combos/combo_string/combo_string.hpp"

namespace CherryKit = Cherry::Kit;

// AIO kits & components includes

#endif // CHERRY_H