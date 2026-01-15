#include "../../../cherry.hpp"
#include <iostream>

void Render() {
  // WARNING, to test live scripting, you need to edit
  // "bin/resources/scripts/render.lua", because of Cherry::GetPath, our
  // relative linking feature
  Cherry::Script::RenderLuaFreshScript(
      Cherry::GetPath("resources/scripts/render.lua"));
}
CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetName("Cherry Scripting Demo");
  config.SetMainRenderCallback(Render);

  auto app = new CherryApplication(config);

  Cherry::Kit::RegisterTextSimple();

  return app;
}
int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}