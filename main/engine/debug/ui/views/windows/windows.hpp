//
//  windows.hpp
//  Headers for devtools windows inspector
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "../../../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_WINDOWS
#define CHERRY_DEBUG_VIEW_WINDOWS

#include <main/engine/hooks/hooks.hpp>

#include "../../../../app_window/app_window.hpp"

class WindowsView : public std::enable_shared_from_this<WindowsView> {
 public:
  WindowsView(const std::string &name);
  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<WindowsView> Create(const std::string &name);

  void SetupRenderCallback();
  void Render();
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif  // CHERRY_DEBUG_VIEW_WINDOWS
#endif  // CHERRY_DEBUG