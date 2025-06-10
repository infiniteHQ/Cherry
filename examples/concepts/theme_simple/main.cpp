#include "../../../cherry.hpp"

class RedThemeCherry : public Cherry::Theme {
public:
  RedThemeCherry() {
    SetName("red");
    SetProperty("color_window_bg", "#881515");
    SetProperty("color_child_bg", "#881515");
    SetProperty("color_framebg", "#881515");
  }
};

class GreenThemeCherry : public Cherry::Theme {
public:
  GreenThemeCherry() {
    SetName("green");
    SetProperty("color_window_bg", "#158815");
    SetProperty("color_child_bg", "#158815");
    SetProperty("color_framebg", "#158815");
  }
};

class BlueThemeCherry : public Cherry::Theme {
public:
  BlueThemeCherry() {
    SetName("blue");
    SetProperty("color_window_bg", "#151588");
    SetProperty("color_child_bg", "#151588");
    SetProperty("color_framebg", "#151588");
  }
};

class YellowThemeCherry : public Cherry::Theme {
public:
  YellowThemeCherry() {
    SetName("yellow");
    SetProperty("color_window_bg", "#888815");
    SetProperty("color_child_bg", "#888815");
    SetProperty("color_framebg", "#888815");
  }
};

void AppWindowSample() {
  if (CherryKit::ButtonText("Red theme")->GetData("isClicked") == "true") {
    CherryAppWindow.SetTheme("red");
  }

  if (CherryKit::ButtonText("Blue theme")->GetData("isClicked") == "true") {
    CherryAppWindow.SetTheme("blue");
  }

  if (CherryKit::ButtonText("Green theme")->GetData("isClicked") == "true") {
    CherryAppWindow.SetTheme("green");
  }

  // We can specify a theme in a small part of the window !
  CherryApp.PushTheme("yellow");
  CherryApp.PopTheme();
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.SetCustomTitlebar(true);
  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);

  config.AddTheme(RedThemeCherry());
  config.AddTheme(GreenThemeCherry());
  config.AddTheme(BlueThemeCherry());
  config.AddTheme(YellowThemeCherry());

  Cherry::AddAppWindow(CherryKit::WindowSimple("First", AppWindowSample));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Second", AppWindowSample));
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}