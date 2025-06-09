#include "../../../cherry.hpp"

void Render() {
  /*if (CherryKit::ButtonText("Light theme")->GetData("isClicked") == "true") {
    // CherryApp.SetTheme("light");
  }

  if (CherryKit::ButtonText("Dark theme")->GetData("isClicked") == "true") {
    // CherryApp.SetTheme("dark");
  }*/
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.AddTheme(CherryThemes::DarkVortex(), "test");
  config.SetDefaultTheme("test");

  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}