#include "../../../cherry.hpp"

void AppWindowRender() {
  std::string WindowName = CherryWindow.GetName();
  std::string AppWindowName = CherryAppWindow.GetName();

  CherryKit::TextSimple("I'm \"" + AppWindowName +
                        "\" and i'm on the window \"" + WindowName + "\"");
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);

  Cherry::AddAppWindow(
      CherryKit::WindowSimple("App Window 1", AppWindowRender));
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("App Window 2", AppWindowRender));
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("App Window 3", AppWindowRender));
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}