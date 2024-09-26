#ifndef UIKIT_H
#define UIKIT_H

// Backend includes
#include "./platform/engine/ui/spinner.hpp"
#include "./platform/engine/ui/notification.hpp"
#include "./platform/engine/ui/ui.hpp"
#include "./src/layer.hpp"
#include "./src/random.hpp"
#include "./src/entry_point.hpp"
#include "./platform/engine/imgui/ImGuiTheme.h"
#include "./platform/engine/image.hpp"
#include "./platform/engine/app.hpp"
#include "./assets/icons.h"

#ifdef UIKIT_V1
// Buttons
#include "./kit/v1/components/buttons/custom_buttons.hpp"
#include "./kit/v1/components/buttons/image_text_buttons.hpp"
#include "./kit/v1/components/buttons/image_buttons.hpp"
#include "./kit/v1/components/buttons/text_buttons.hpp"
// Headers
#include "./kit/v1/components/headers/simple_collapsing.hpp"
#include "./kit/v1/components/headers/custom_collapsing.hpp"
// Texts
#include "./kit/v1/components/texts/simple_text.hpp"
// Combos
#include "./kit/v1/components/combos/simple_combo.hpp"
#include "./kit/v1/components/combos/custom_combo.hpp"
// Titles
#include "./kit/v1/components/titles/h1.hpp"
#include "./kit/v1/components/titles/h2.hpp"
#include "./kit/v1/components/titles/h3.hpp"
#include "./kit/v1/components/titles/h4.hpp"
#include "./kit/v1/components/titles/h5.hpp"
#include "./kit/v1/components/titles/h6.hpp"
// AppWindows templates
#include "./kit/v1/aio/appwindows/demo/demo.hpp"
#include "./kit/v1/aio/appwindows/basic/empty_appwindow.hpp"
#include "./kit/v1/aio/appwindows/basic/multi_child.hpp"
#include "./kit/v1/aio/appwindows/docking/docking_appwindow.hpp"
#include "./kit/v1/aio/appwindows/content/content_browser.hpp"
#include "./kit/v1/aio/appwindows/content/content_outliner_simple/content_outliner_simple.hpp"
// Trees
#include "./kit/v1/components/lists/custom_lists.hpp"
// Inputs
#include "./kit/v1/components/inputs/keyval/keyval_double.hpp"
#include "./kit/v1/components/inputs/simple/simple_inputs.hpp"
//Tables
#include "./kit/v1/components/tables/simple_tables.hpp"

#endif

// AIO kits & components includes

#endif // UIKIT_H