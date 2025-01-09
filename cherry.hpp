#ifndef CHERRY_H
#define CHERRY_H

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
#include "./platform/engine/imgui/ImGuiTheme.h"
#include "./platform/engine/image.hpp"
#include "./platform/engine/app.hpp"
#include "./assets/icons.h"

#ifdef CHERRY_V1
// Buttons
#include "./kit/v1/components/buttons/dropdown_buttons.hpp"
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
// MenuItems
#include "./kit/v1/components/menuitems/separator.hpp"
#include "./kit/v1/components/menuitems/menuitem.hpp"

// AppWindows templates
#include "./kit/v1/aio/appwindows/demo/demo.hpp"
#include "./kit/v1/aio/appwindows/basic/empty/empty_appwindow.hpp"
#include "./kit/v1/aio/appwindows/basic/mutli_child_list/multi_child_list.hpp"
#include "./kit/v1/aio/appwindows/basic/mutli_child_areas/mutli_child_areas.hpp"
#include "./kit/v1/aio/appwindows/basic/mutli_child_tabs/multi_child_tabs.hpp"
#include "./kit/v1/aio/appwindows/docking/docking_appwindow.hpp"
#include "./kit/v1/aio/appwindows/data/data_logs_viewer/data_logs_viewer.hpp"
#include "./kit/v1/aio/appwindows/editors/text_editor_simple/text_editor_simple.hpp"
#include "./kit/v1/aio/appwindows/editors/node_editor_simple/node_editor_simple.hpp"
#include "./kit/v1/aio/appwindows/editors/props_editor_simple/props_editor_simple.hpp"
#include "./kit/v1/aio/appwindows/content/content_browser/content_browser.hpp"
#include "./kit/v1/aio/appwindows/content/content_outliner_simple/content_outliner_simple.hpp"
// Trees
#include "./kit/v1/components/lists/custom_lists.hpp"
// Inputs
#include "./kit/v1/components/inputs/keyval/keyval_double.hpp"
#include "./kit/v1/components/inputs/simple/simple_inputs.hpp"
#include "./kit/v1/components/inputs/simple/image_inputs.hpp"
#include "./kit/v1/components/tables/simple_tables.hpp"

#endif

// AIO kits & components includes

#endif // CHERRY_H