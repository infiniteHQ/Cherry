#include "../../cherry.hpp"

// Obviously, you need a internet connexion to test this example

void Render() {
  // Net components from the Cherry Kit
  CherryKit::ImageHttp("https://infinite.si/assets/in_projects.c64028e7.png");

  CherryKit::Separator();

  // Or fetch as Dear ImGui texture directly
  auto texture = Cherry::GetTexture(Cherry::GetHttpPath("https://infinite.si/assets/in_projects.c64028e7.png"));

  if (texture) {
    ImGui::Image(texture, ImVec2(240.0f, 150.0f));
  }
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}