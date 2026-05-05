#include "../../../cherry.hpp"

void RenderNoCustomTitleBar() {
  CherryKit::TextSimple("Hello from no custom titlebar window !");
}
void RenderCustomTitleBar() {
  CherryKit::TextSimple("Hello from custom titlebar window !");
}

void AppWindowRender() {
  if (CherryKit::ButtonText("Spawn window with no custom titlebar").GetDataAs<bool>("isClicked")) {
    auto ap = CherryKit::WindowSimple("No titlebar", RenderNoCustomTitleBar);
    Cherry::ApplicationSpecification spec;
    spec.SetName("No titlebar");
    spec.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);
    spec.SetUniqueAppWindowName("No titlebar");
    spec.SetMinimumHeight(200);
    spec.SetMinimumWidth(100);
    spec.SetDefaultHeight(450);
    spec.SetDefaultWidth(450);
    spec.SetCustomTitlebar(false);
    spec.DisableWindowManagerTitleBar = false;
    ap->AttachOnNewWindow(spec);
    Cherry::AddAppWindow(ap);
  }

  if (CherryKit::ButtonText("Spawn window with custom titlebar").GetDataAs<bool>("isClicked")) {
    auto ap = CherryKit::WindowSimple("Custom titlebar", RenderCustomTitleBar);
    Cherry::ApplicationSpecification spec;
    spec.SetName("Custom titlebar");
    spec.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);
    spec.SetUniqueAppWindowName("Custom titlebar");
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