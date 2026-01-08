#include "../../../cherry.hpp"

void Render() {}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.UseAudio();
  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}