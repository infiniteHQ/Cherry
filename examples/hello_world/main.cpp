#define SDL_MAIN_HANDLED
#define CHERRY_V1
#include "../../cherry.hpp"

#include <thread>
#include <memory>

class Layer : public Cherry::Layer
{
public:
  Layer() {};
};

Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  Cherry::ApplicationSpecification spec;
  std::shared_ptr<Layer> layer = std::make_shared<Layer>();

  std::string name = "UIKit example";
  spec.Name = name;
  spec.DefaultWindowName = name;
  spec.MinHeight = 500;
  spec.GlobalScale = 0.84f;
  spec.FontGlobalScale = 0.84f;
  spec.MinWidth = 500;
  spec.CustomTitlebar = true;
  spec.DisableWindowManagerTitleBar = true;
  spec.RenderMode = WindowRenderingMethod::SimpleRender;

  spec.UniqueAppWindowName = "?loc:loc.window_names.demo";
  spec.DisableTitle = true;
  spec.WindowSaves = true;
  spec.IconPath = Cherry::GetPath("ressources/imgs/icon.png");
  spec.FavIconPath = Cherry::GetPath("ressources/imgs/icon.png");

  Cherry::Application *app = new Cherry::Application(spec);
  app->SetWindowSaveDataFile("savedatda.json", true);
  // app->SetFavIconPath(Cherry::GetPath("ressources/imgs/favicon.png"));
  app->AddFont("Consola", Cherry::GetPath("ressources/fonts/consola.ttf"), 17.0f);

  app->AddLocale("fr", Cherry::GetPath("ressources/locales/fr.json"));
  app->AddLocale("fr", Cherry::GetPath("ressources/locales/fr.json"));
  app->AddLocale("en", Cherry::GetPath("ressources/locales/en.json"));
  app->AddLocale("es", Cherry::GetPath("ressources/locales/es.json")); // With not suffisent locales to show the "default" behavior
  app->SetDefaultLocale("en");                                         // The "default" behavior
  app->SetLocale("fr");

  app->PushLayer(layer);

  app->SetMainRenderCallback([](){
    ImGui::Text("gs");
  });
  
  if (ImGui_ImplSDL2_CefInit(argc, argv) < 0)
  {
    std::cerr << "Failed to initialize ImGui SDL2 CEF integration." << std::endl;
    return nullptr;
  }
std::cout << "Inited" << std::endl;
  InitCEF(500, 500);
std::cout << "Cef" << std::endl;


  return app;
}

int main(int argc, char *argv[])
{
  std::thread mainthread;
  std::thread Thread([&]()
                     { Cherry::Main(argc, argv); });
  mainthread.swap(Thread);

  while (g_ApplicationRunning)
  {
    /* Your program loop... */
  }

  mainthread.join();

  return 0;
}