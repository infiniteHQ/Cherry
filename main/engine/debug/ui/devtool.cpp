
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
    };

    void Devtool::Stop() {
      const std::string window_name = "Cherry devtools";
      Cherry::DeleteAppWindow(ComponentsAppWindow->GetAppWindow());
      Cherry::DeleteAppWindow(ConsoleAppWindow->GetAppWindow());
    };

  }  // namespace Tools
}  // namespace Cherry

#endif  // CHERRY_DEBUG
