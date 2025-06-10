#include "../../../cherry.hpp"

class RedThemeCherry : public Cherry::Theme {
public:
  RedThemeCherry() {
    SetName("red");
    SetProperty("color_window_bg", "#FF1515");
    SetProperty("color_child_bg", "#FF1515");
    SetProperty("color_framebg", "#FF1515");
  }
};

class GreenThemeCherry : public Cherry::Theme {
public:
  GreenThemeCherry() {
    SetName("green");
    SetProperty("color_window_bg", "#15FF15");
    SetProperty("color_child_bg", "#15FF15");
    SetProperty("color_framebg", "#15FF15");
  }
};

class BleueThemeCherry : public Cherry::Theme {
public:
  BleueThemeCherry() {
    SetName("bleue");
    SetProperty("color_window_bg", "#1515FF");
    SetProperty("color_child_bg", "#1515FF");
    SetProperty("color_framebg", "#1515FF");
  }
};

void AppWindowFirst() {
  CherryAppWindow.SetTheme("red");
  /*if (CherryKit::ButtonText("Light theme")->GetData("isClicked") == "true") {
    // CherryApp.SetTheme("light");
  }

  if (CherryKit::ButtonText("Dark theme")->GetData("isClicked") == "true") {
    // CherryApp.SetTheme("dark");
  }*/
}

void AppWindowSecond() {
  CherryAppWindow.SetTheme("green");
  /*if (CherryKit::ButtonText("Light theme")->GetData("isClicked") == "true") {
    // CherryApp.SetTheme("light");
  }

  if (CherryKit::ButtonText("Dark theme")->GetData("isClicked") == "true") {
    // CherryApp.SetTheme("dark");
  }*/
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.SetCustomTitlebar(true);
  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);

  config.AddTheme(RedThemeCherry());
  config.AddTheme(GreenThemeCherry());
  config.AddTheme(BleueThemeCherry());

  Cherry::AddAppWindow(CherryKit::WindowSimple("First", AppWindowFirst));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Second", AppWindowSecond));
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}