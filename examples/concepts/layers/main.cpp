#include "../../cherry.hpp"

void Render() {
  std::string AppName = CherryApp.GetName();
  std::string WindowName = CherryWindow.GetName();
  std::string AppWindowName = CherryAppWindow.GetName();

  CherryKit::TextSimple("App name (level 1): " + AppName);
  CherryKit::TextSimple("Window name (level 2): " + WindowName);
  CherryKit::TextSimple("App Window name (level 3: " + AppWindowName);
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.AddTheme(CherryThemes::DarkVortex());
  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}

/*

#include "../../cherry.hpp"

void Render() {
  std::string AppName = CherryApp.GetName();
  std::string WindowName = CherryWindow.GetName();
  std::string AppWindowName = CherryAppWindow.GetName();

  CherryKit::TextSimple("App name (level 1): " + AppName);
  CherryKit::TextSimple("Window name (level 2): " + WindowName);
  CherryKit::TextSimple("App Window name (level 3: " + AppWindowName);
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);
  config.AddTheme(CherryThemes::DarkVortex());

  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window", Render));
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}
*/