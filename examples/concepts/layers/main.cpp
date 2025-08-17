#include "../../../cherry.hpp"

void AppWindowRender() {
  std::string WindowName = CherryWindow.GetName();
  std::string AppWindowName = CherryAppWindow.GetName();

  CherryKit::TextSimple("I'm \"" + AppWindowName +
                        "\" and i'm on the window \"" + WindowName + "\"");
}

CherryApplication CherryMain(int argc, char **argv) {
  // Configuration of the application
  Cherry::ApplicationSpecification config;
  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);

  auto app =  new CherryApplication(config);

  // When created, let's add some components to the runtime
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("App Window 1", AppWindowRender));
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("App Window 2", AppWindowRender));
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("App Window 3", AppWindowRender));
  return app;
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}