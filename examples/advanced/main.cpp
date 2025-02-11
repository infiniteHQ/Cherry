#include "../../cherry.hpp"

Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  Cherry::ApplicationSpecification config;
  config.SetName("Advanced Example");
  config.SetDefaultWindowName("Advanced Example");
  config.SetMinimumWidth(500);
  config.SetMinimumHeight(500);
  config.SetDefaultWidth(1920);
  config.SetDefaultHeight(1080);
  config.SetGlobalScale(0.84f);
  config.SetFontGlobalScale(0.84f);
  config.SetCustomTitlebar(true);
  config.DisableDesktopTitleBar();
  config.SetRenderMode(WindowRenderingMethod::DockingWindows);
  config.SetUniqueAppWindowName("?loc:loc.window_names.demo");
  config.DisableWindowTitle();
  config.UseWindowSaves();
  config.SetIconPath(Cherry::GetPath("ressources/imgs/icon.png"));
  config.SetFavIconPath(Cherry::GetPath("ressources/imgs/icon.png"));

  Cherry::Application *app = new Cherry::Application(config);
  app->SetWindowSaveDataFile("savedatda.json", true);
  app->AddFont("Consola", Cherry::GetPath("ressources/fonts/consola.ttf"), 17.0f);
  app->AddLocale("fr", Cherry::GetPath("ressources/locales/fr.json"));
  app->AddLocale("fr", Cherry::GetPath("ressources/locales/fr.json"));
  app->AddLocale("en", Cherry::GetPath("ressources/locales/en.json"));
  app->AddLocale("es", Cherry::GetPath("ressources/locales/es.json"));
  app->SetDefaultLocale("en");
  app->SetLocale("fr");
  app->SetMenubarCallback([app, config]() {});

  return app;
}

int main(int argc, char *argv[])
{
  // Starting the UI render
    std::cout << "Running" << std::endl;
  std::thread([&]()
              { Cherry::Main(argc, argv); })
      .detach();
    std::cout << "Running" << std::endl;

  while (g_ApplicationRunning)
  {
    std::cout << "Running" << std::endl;
    /* Your program loop... */
  }

  return 0;
}