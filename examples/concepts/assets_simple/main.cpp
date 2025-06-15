#include "../../../cherry.hpp"

void Render() {
  if (CherryKit::ButtonText("Click me !").GetDataAs<bool>("isClicked")) {
    CherryApp.PlaySound(CherryPath("resources/audio/tick.wav"));
  }

  if (CherryKit::ButtonText("And me !").GetDataAs<bool>("isClicked")) {
    CherryApp.PlaySound(CherryPath("resources/audio/tick2.wav"));
  }

  if (CherryKit::ButtonTextImage("Infinityyy",
                                 CherryPath("resources/images/settings.png"))
          .GetDataAs<bool>("isClicked")) {
    CherryApp.PlaySound(CherryPath("resources/audio/tick.wav"));
  }
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.UseAudio();

  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}