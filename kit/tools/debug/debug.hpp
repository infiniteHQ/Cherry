#include "views/about/about.hpp"
#include "views/general/general.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_TOOL
#define CHERRY_DEBUG_TOOL

namespace Cherry {
namespace Tools {
class DebugTool {
public:
  DebugTool() {};
  ~DebugTool() {};

  void StartDebugTool() {
    GeneralAppWindow = GeneralView::Create("General");
    GeneralAppWindow->GetAppWindow()->SetVisibility(true);
    Cherry::AddAppWindow(GeneralAppWindow->GetAppWindow());

    Cherry::ApplicationSpecification spec;
    const std::string window_name = "Cherry devtools";
    spec.SetName(window_name);
    spec.SetUniqueAppWindowName(window_name);

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

    spec.MenubarCallback = []() {};
    spec.WindowSaves = false;

    GeneralAppWindow->GetAppWindow()->AttachOnNewWindow(spec);

    AboutAppWindow = AboutView::Create("About");
    AboutAppWindow->GetAppWindow()->SetVisibility(true);
    Cherry::AddAppWindow(AboutAppWindow->GetAppWindow());
    AboutAppWindow->GetAppWindow()->AttachOnWindow(window_name);
  };

  std::shared_ptr<GeneralView> GeneralAppWindow;
  std::shared_ptr<AboutView> AboutAppWindow;

private:
};

void StartDebugTool() {
  static DebugTool debug;
  debug.StartDebugTool();
}
} // namespace Tools
} // namespace Cherry

#endif // CHERRY_DEBUG_TOOL
#endif // CHERRY_DEBUG
