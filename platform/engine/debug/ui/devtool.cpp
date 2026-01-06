
#include "devtool.hpp"
#ifdef CHERRY_DEBUG
#include "../../app/app.hpp"
namespace Cherry {
namespace Tools {

Devtool::Devtool() {};
Devtool::~Devtool() {};

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

  GeneralAppWindow = GeneralView::Create("General");
  GeneralAppWindow->GetAppWindow()->SetVisibility(true);
  Cherry::AddAppWindow(GeneralAppWindow->GetAppWindow());
  GeneralAppWindow->GetAppWindow()->AttachOnNewWindow(spec);

  InspectorAppWindow = InspectorView::Create("Inspector");
  InspectorAppWindow->GetAppWindow()->SetVisibility(true);
  Cherry::AddAppWindow(InspectorAppWindow->GetAppWindow());
  InspectorAppWindow->GetAppWindow()->AttachOnWindow(window_name);

  AboutAppWindow = AboutView::Create("About");
  AboutAppWindow->GetAppWindow()->SetVisibility(true);
  Cherry::AddAppWindow(AboutAppWindow->GetAppWindow());
  AboutAppWindow->GetAppWindow()->AttachOnWindow(window_name);
};

void Devtool::Stop() {
  const std::string window_name = "Cherry devtools";
  Cherry::DeleteAppWindow(InspectorAppWindow->GetAppWindow());
  Cherry::DeleteAppWindow(AboutAppWindow->GetAppWindow());
  Cherry::DeleteAppWindow(GeneralAppWindow->GetAppWindow());
};

} // namespace Tools
} // namespace Cherry

#endif // CHERRY_DEBUG
