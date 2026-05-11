#include "../../cherry.hpp"

// Obviously, you need a internet connexion to test this example

void Render() {  // Or fetch as Dear ImGui texture directly
  Cherry::GetHttpPath(
      "https://static.infinite.si/user_contents/uploads/94eb98d1-d1db-4a45-8a4a-73756391a8f7/"
      "70fa7a77-5f93-4de9-8004-758f8babdbe4.png");
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