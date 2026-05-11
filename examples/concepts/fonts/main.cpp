#include "../../../cherry.hpp"

void Render() {
  Cherry::PushFont("rocket");
  CherryKit::TitleOne("Hello font !!!");
  Cherry::PopFont();

  Cherry::PushFont("chunky");
  CherryKit::TitleOne("Hello font !!!");
  Cherry::PopFont();

  Cherry::PushFont("jetbrainsmono");
  CherryKit::TitleOne("Hello font !!!");
  Cherry::PopFont();

  Cherry::PushFont("rocket");
  CherryStyle::PushFontSize(0.25f);
  CherryKit::TitleOne("Hello font !!!");
  CherryStyle::PopFontSize();
  Cherry::PopFont();

  Cherry::PushFont("chunky");
  CherryStyle::PushFontSize(0.25f);
  CherryKit::TitleOne("Hello font !!!");
  CherryStyle::PopFontSize();
  Cherry::PopFont();

  Cherry::PushFont("jetbrainsmono");
  CherryStyle::PushFontSize(0.25f);
  CherryKit::TitleOne("Hello font !!!");
  CherryStyle::PopFontSize();
  Cherry::PopFont();
}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.UseAudio();
  config.SetMainRenderCallback(Render);

  auto app = new CherryApplication(config);
  app->AddFont("rocket", CherryPath("rocket.otf"), 80.0f);
  app->AddFont("chunky", CherryPath("chunky.otf"), 80.0f);
  app->AddFont("jetbrainsmono", CherryPath("jetbrainsmono.ttf"), 80.0f);

  return app;
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}