//
//  components.hpp
//  Headers for devtools components inspector
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "../../../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_COMPONENTS
#define CHERRY_DEBUG_VIEW_COMPONENTS

#include "../../../../app_window/app_window.hpp"

class ComponentsView : public std::enable_shared_from_this<ComponentsView> {
 public:
  ComponentsView(const std::string &name);
  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<ComponentsView> Create(const std::string &name);

  void SetupRenderCallback();
  void RenderComponentDetails(std::shared_ptr<Cherry::Component> &comp);
  void RenderSimpleTable(const char *label, const std::unordered_map<std::string, std::string> &map, ImVec4 key_color);
  void Render();
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif  // CHERRY_DEBUG_VIEW_COMPONENTS
#endif  // CHERRY_DEBUG