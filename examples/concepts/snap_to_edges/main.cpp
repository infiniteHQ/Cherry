#include "../../../cherry.hpp"

void RenderNoCustomTitleBar() {
  CherryKit::TextSimple("You can natively snap this window to edges !");
}
void RenderCustomTitleBar() {
  CherryKit::TextSimple("You can't snap this window to edges !");
}
void RenderCustomTitleBarSnap() {
  CherryKit::TextSimple("You can snap this window to edges !");
}

void AppWindowRender() {
  if (CherryKit::ButtonText("Native window with snap to edges capability").GetDataAs<bool>("isClicked")) {
    auto ap = CherryKit::WindowSimple("No titlebar with snap", RenderNoCustomTitleBar);
    Cherry::ApplicationSpecification spec;
    spec.SetName("No titlebar with snap");
    spec.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);
    spec.SetUniqueAppWindowName("No titlebar with snap");
    spec.SetMinimumHeight(200);
    spec.SetMinimumWidth(100);
    spec.SetDefaultHeight(450);
    spec.SetDefaultWidth(450);
    spec.SetCustomTitlebar(false);
    spec.DisableWindowManagerTitleBar = false;
    ap->AttachOnNewWindow(spec);
    Cherry::AddAppWindow(ap);
  }

  if (CherryKit::ButtonText("Custom window with snap to edges capability").GetDataAs<bool>("isClicked")) {
    auto ap = CherryKit::WindowSimple("Custom titlebar with snap", RenderCustomTitleBarSnap);
    Cherry::ApplicationSpecification spec;
    spec.SetName("Custom titlebar with snap");
    spec.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);
    spec.SetUniqueAppWindowName("Custom titlebar with snap");
    spec.SetMinimumHeight(200);
    spec.SetMinimumWidth(100);
    spec.SetDefaultHeight(450);
    spec.SetDefaultWidth(450);
    spec.SetCustomTitlebar(true);
    spec.EnableSnapToEdge();  // Here is the magic !
    spec.DisableWindowManagerTitleBar = true;
    ap->AttachOnNewWindow(spec);
    Cherry::AddAppWindow(ap);
  }

  if (CherryKit::ButtonText("Custom window without snap capability").GetDataAs<bool>("isClicked")) {
    auto ap = CherryKit::WindowSimple("Custom titlebar without snap", RenderCustomTitleBar);
    Cherry::ApplicationSpecification spec;
    spec.SetName("Custom titlebar without snap");
    spec.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);
    spec.SetUniqueAppWindowName("Custom titlebar without snap");
    spec.SetMinimumHeight(200);
    spec.SetMinimumWidth(100);
    spec.SetDefaultHeight(450);
    spec.SetDefaultWidth(450);
    spec.SetCustomTitlebar(true);
    spec.DisableWindowManagerTitleBar = true;
    ap->AttachOnNewWindow(spec);
    Cherry::AddAppWindow(ap);
  }
}

CherryApplication CherryMain(int argc, char **argv) {
  // Configuration of the application
  Cherry::ApplicationSpecification config;
  config.SetUniqueAppWindowName("Main");
  config.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);

  auto app = new CherryApplication(config);

  // When created, let's add some components to the runtime
  Cherry::AddAppWindow(CherryKit::WindowSimple("Main", AppWindowRender));
  return app;
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}