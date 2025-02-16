#ifndef CHERRY_H
#define CHERRY_H
#define SDL_MAIN_HANDLED

#include "./options.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS

// Backend includes
#include "./platform/engine/ui/notifications/notifications.hpp"
#include "./platform/engine/ui/ui.hpp"
#include "./src/layer.hpp"
#include "./src/random.hpp"
#include "./src/entry_point.hpp"
#include "./platform/engine/imgui/wrappers.hpp"
#include "./platform/engine/imgui/ImGuiTheme.h"
#include "./platform/engine/image.hpp"
#include "./platform/engine/app.hpp"
#include "./platform/engine/embed/assets/icons.h"

#ifdef CHERRY_ALL
#endif // CHERRY_ALL

#include "./kit/components/buttons/button_text/button_text.hpp"

using CherryID = Cherry::Identifier;
namespace CherryKit = Cherry;

// AIO kits & components includes

#endif // CHERRY_H