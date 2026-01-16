#include "../../../cherry.hpp"
#include <iostream>

#include "./assets/resources/components/button/button.hpp"
#include "./assets/resources/components/screen/screen.hpp"

void Render() {
  Cherry::Script::RenderLuaFreshScript(
      Cherry::GetPath("resources/scripts/render.lua"));
}
CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetName("Cherry Calculator");
  config.SetMainRenderCallback(Render);
  config.SetDefaultHeight(500);
  config.SetDefaultWidth(445);
  config.SetMinimumHeight(500);
  config.SetMinimumWidth(445);
  config.UseAudio();
  config.WindowResizeable = false;
  auto app = new CherryApplication(config);

  Cherry::Kit::RegisterCalculatorButton();
  Cherry::Kit::RegisterCalculatorScreen();

  return app;
}
int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}