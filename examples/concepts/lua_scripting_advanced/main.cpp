#include <iostream>

#include "../../../cherry.hpp"

void Render() {
  // WARNING, to test live scripting, you need to edit
  // "bin/resources/scripts/render.lua", because of Cherry::GetPath, our
  // relative linking feature

  static bool initialized = false;
  if (!initialized) {
    Cherry::Script::RenderLuaFreshScript(Cherry::GetPath("static.lua"));
    initialized = true;
  }
  Cherry::Script::RenderLuaFreshScript(Cherry::GetPath("render.lua"));
}
CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetName("Cherry Scripting Demo");
  config.SetMainRenderCallback(Render);
  config.SetDefaultWidth(500);
  config.SetDefaultHeight(500);
  auto app = new CherryApplication(config);

  Cherry::Kit::RegisterTextSimple();

  return app;
}
int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}