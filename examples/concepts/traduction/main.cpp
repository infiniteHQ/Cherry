#include "../../cherry.hpp"

void Render() {
  if (CherryKit::ButtonText("Change for English").GetDataAs<bool>("isClicked")) {
    CherryApp.SetLocale("en");
  }
  if (CherryKit::ButtonText("Change for Spanish").GetDataAs<bool>("isClicked")) {
    CherryApp.SetLocale("es");
  }
  if (CherryKit::ButtonText("Change for French").GetDataAs<bool>("isClicked")) {
    CherryApp.SetLocale("fr");
  }

  CherryKit::Separator();

  CherryStyle::AddMarginX(20.0f);

  // Get the traduction with topic ID with Cherry::GetLocale
  CherryKit::TitleOne(Cherry::GetLocale("hello_world"));
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(Render);

  auto app = new CherryApplication(config);

  app->AddLocale("en", Cherry::GetPath("resources/locales/en.json"));
  app->AddLocale("fr", Cherry::GetPath("resources/locales/fr.json"));
  app->AddLocale("es", Cherry::GetPath("resources/locales/es.json"));

  app->SetDefaultLocale("en");
  app->SetLocale("en");

  return app;
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}