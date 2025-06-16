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
  if (CherryKit::ButtonText("Red theme").GetData("isClicked") == "true") {
    CherryAppWindow.SetTheme("red");
  }

  if (CherryKit::ButtonText("Blue theme").GetData("isClicked") == "true") {
    CherryAppWindow.SetTheme("blue");
  }

  if (CherryKit::ButtonText("Green theme").GetData("isClicked") == "true") {
    CherryAppWindow.SetTheme("green");
  }

  // We can specify a theme in a small part of the window !
  CherryApp.PushTheme("yellow");
  CherryApp.PopTheme();
}

void MainThemes() {
  CherryKit::SeparatorText("Dark themes");

  if (CherryKit::ButtonText("Dark").GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("dark");
  }

  if (CherryKit::ButtonText("Dark Colorfull").GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("dark_colorfull");
  }

  if (CherryKit::ButtonText("Dark Vortex").GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("dark_vortex");
  }

  if (CherryKit::ButtonText("Dark High contrast")
          .GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("dark_high_contrast");
  }

  CherryKit::SeparatorText("Light themes");

  if (CherryKit::ButtonText("Light").GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("light");
  }

  if (CherryKit::ButtonText("Light Colorfull").GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("light_colorfull");
  }

  if (CherryKit::ButtonText("Light Vortex").GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("light_vortex");
  }

  if (CherryKit::ButtonText("Light High contrast")
          .GetDataAs<bool>("isClicked")) {
    CherryApp.SetTheme("light_high_contrast");
  }

  CherryKit::SeparatorText("Preview");

  CherryKit::TextWrapped(
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
      "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
      "veniam, quis nostrud exercitation.");

  CherryKit::Separator();

  static int integer_test = 42;
  static std::string string_test = "Hello world";

  CherryKit::TableSimple(
      "ExampleTable",
      {
          CherryKit::KeyValInteger("Enter integer", &integer_test),
          CherryKit::KeyValInteger("Enter string", &integer_test),
          CherryKit::KeyValCustom("Project paths",
                                  [=]() { CherryKit::TitleOne("nfg"); }),
      });

  CherryKit::ListCustom("ExampleTablList",
                        {
                            CherryKit::TitleFive("Enter integer"),
                            CherryKit::TitleOne("Entdqer integer"),
                            CherryKit::TitleFour("Entegr integer"),
                        });
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.SetRenderMode(Cherry::WindowRenderingMethod::TabWidows);
  config.AddTheme(CherryThemes::Dark());
  config.AddTheme(CherryThemes::Light());
  config.AddTheme(CherryThemes::DarkColorfull());
  config.AddTheme(CherryThemes::LightColorfull());
  config.AddTheme(CherryThemes::DarkVortex());
  config.AddTheme(CherryThemes::LightVortex());
  config.AddTheme(CherryThemes::DarkHighContrast());
  config.AddTheme(CherryThemes::LightHighContrast());

  config.SetCustomTitlebar(true);

  config.AddTheme(RedThemeCherry());
  config.AddTheme(GreenThemeCherry());
  config.AddTheme(BlueThemeCherry());
  config.AddTheme(YellowThemeCherry());

  Cherry::AddAppWindow(CherryKit::WindowSimple("Main Themes", MainThemes));
  Cherry::AddAppWindow(CherryKit::WindowSimple("First", AppWindowSample));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Second", AppWindowSample));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Third", AppWindowSample));
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}