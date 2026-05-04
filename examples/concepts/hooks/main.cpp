#include "../../cherry.hpp"

void LoadHook() {
  auto foo = CherryHook({
    static int i = 0;
    i++;
    self->SetData("toto", std::to_string(i));
  });

  auto cond = CherryHookCond({
    if (CherryApp.GetComponent(CherryID("btn")).GetDataAs<bool>("isPressed")) {
      return true;
    }
    return false;
  });

  Cherry::CreateHook("tt", CherryHookSecond, foo, cond);
}

void Render() {
  LoadHook();

  CherryKit::ButtonText(CherryID("btn"), "button");

  std::string val = Cherry::GetHookData(CherryID("tt"), "toto");
  std::cout << val << std::endl;
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