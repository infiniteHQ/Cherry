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

using CherryID = Cherry::Identifier;
namespace CherryKit = Cherry::Kit;

// AIO kits & components includes

#endif // CHERRY_H