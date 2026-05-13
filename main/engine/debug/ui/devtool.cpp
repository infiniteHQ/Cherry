//
//  devtools.cpp
//  Main sources for Cherry devtools window
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "devtool.hpp"
#ifdef CHERRY_DEBUG
#include "../../app/app.hpp"
namespace Cherry {
  namespace Tools {

    Devtool::Devtool() { };
    Devtool::~Devtool() { };

    void Devtool::Start() {
      const std::string window_name = "Cherry devtools";
      Cherry::ApplicationSpecification spec;
      spec.SetName(window_name);
      spec.MinHeight = 200;
      spec.MinWidth = 400;
      spec.Height = 600;
      spec.Width = 1100;
      spec.DisableLogo = true;
      spec.DisableResize = true;
      spec.CustomTitlebar = false;
      spec.DisableWindowManagerTitleBar = false;
      spec.RenderMode = Cherry::WindowRenderingMethod::TabWidows;
      spec.UsingCloseCallback = true;
      spec.CloseCallback = []() { CherryApp.ToggleDebugTools(); };
      spec.WindowSaves = false;

      ConsoleAppWindow = ConsoleView::Create("Console");
      ConsoleAppWindow->GetAppWindow()->SetVisibility(true);
      Cherry::AddAppWindow(ConsoleAppWindow->GetAppWindow());
      ConsoleAppWindow->GetAppWindow()->AttachOnNewWindow(spec);

      ComponentsAppWindow = ComponentsView::Create("Components");
      ComponentsAppWindow->GetAppWindow()->SetVisibility(true);
      Cherry::AddAppWindow(ComponentsAppWindow->GetAppWindow());
      ComponentsAppWindow->GetAppWindow()->AttachOnWindow(window_name);

      HooksAppWindow = HooksView::Create("Hooks");
      HooksAppWindow->GetAppWindow()->SetVisibility(true);
      Cherry::AddAppWindow(HooksAppWindow->GetAppWindow());
      HooksAppWindow->GetAppWindow()->AttachOnWindow(window_name);
    };

    void Devtool::Stop() {
      Cherry::DeleteAppWindow(ComponentsAppWindow->GetAppWindow());
      Cherry::DeleteAppWindow(ConsoleAppWindow->GetAppWindow());
      Cherry::DeleteAppWindow(HooksAppWindow->GetAppWindow());
    };

  }  // namespace Tools
}  // namespace Cherry

#endif  // CHERRY_DEBUG
