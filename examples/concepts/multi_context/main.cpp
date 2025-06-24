#include "../../cherry.hpp"

void Render() {
  std::string AppName = CherryApp.GetName();
  std::string WindowName = CherryWindow.GetName();
  std::string AppWindowName = CherryAppWindow.GetName();

  CherryKit::TitleOne("Try to undock me !!");

  CherryKit::TextSimple("App name (level 1): " + AppName);
  CherryKit::TextSimple("Window name (level 2): " + WindowName);
  CherryKit::TextSimple("App Window name (level 3: " + AppWindowName);
}

CherryApplication CherryMain(int argc, char **argv) {
  CherryAppConfig config;
  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);
  config.AddTheme(CherryThemes::DarkVortex());
  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window", Render));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window 2", Render));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window 3", Render));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window 4", Render));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window 5", Render));
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}