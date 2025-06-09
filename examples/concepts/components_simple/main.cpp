#include "../../cherry.hpp"

void FirstAppWindow() {
  if (CherryKit::ButtonText("Light theme")->GetData("isClicked") == "true") {
    // CherryAppWindow.SetTheme("light");
  }

  if (CherryKit::ButtonText("Dark theme")->GetData("isClicked") == "true") {
    // CherryAppWindow.SetTheme("dark");
  }
}

void FirsSecondAppWindow() {
  if (CherryKit::ButtonText("Light theme")->GetData("isClicked") == "true") {
    // CherryAppWindow.SetTheme("light");
  }

  if (CherryKit::ButtonText("Dark theme")->GetData("isClicked") == "true") {
    // CherryAppWindow.SetTheme("dark");
  }
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.AddTheme(CherryThemes::DarkVortex());

  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}