//
//  hooks.hpp
//  Headers for devtools hooks inspector
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "../../../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_HOOKS
#define CHERRY_DEBUG_VIEW_HOOKS

#include <main/engine/hooks/hooks.hpp>

#include "../../../../app_window/app_window.hpp"

class HooksView : public std::enable_shared_from_this<HooksView> {
 public:
  HooksView(const std::string &name);
  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<HooksView> Create(const std::string &name);

  void SetupRenderCallback();
  void RenderHookDetails(std::shared_ptr<Cherry::Hook> &comp);
  void RenderSimpleTable(const char *label, const std::unordered_map<std::string, std::string> &map, ImVec4 key_color);
  void Render();
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif  // CHERRY_DEBUG_VIEW_HOOKS
#endif  // CHERRY_DEBUG