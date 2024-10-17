#define CHERRY_V1
#include "../../cherry.hpp"

#include <thread>
#include <memory>

class Layer : public Cherry::Layer
{
public:
  Layer() {};
};

void FirstPart()
{
  ImVec2 imageSize = Cherry::Application::GetCurrentRenderedWindow()->get_texture_size(
      Cherry::GetPath("ressources/imgs/back.jpg"));

  ImVec2 availableSize = ImGui::GetContentRegionAvail();

  float scaleX = availableSize.x / imageSize.x;
  float scaleY = availableSize.y / imageSize.y;

  float scale = (scaleX > scaleY) ? scaleX : scaleY;

  ImVec2 displaySize = ImVec2(imageSize.x * scale, imageSize.y * scale);

  ImVec2 offset = ImVec2(
      (availableSize.x - displaySize.x) * 0.5f,
      (availableSize.y - displaySize.y) * 0.5f);

  if (offset.x < 0)
    offset.x = 0;
  if (offset.y < 0)
    offset.y = 0;

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset.x);
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset.y);

  ImGui::Image(
      Cherry::GetTexture(Cherry::GetPath("ressources/imgs/back.jpg")),
      ImVec2(availableSize.x, availableSize.y),
      ImVec2(-offset.x / displaySize.x, -offset.y / displaySize.y),
      ImVec2((availableSize.x - offset.x) / displaySize.x, (availableSize.y - offset.y) / displaySize.y));
}

void SecondPart()
{
  ImGui::Separator();
  Cherry::TitleTwo("Welcome to Cherry !!!");
  Cherry::TitleSixColored("by Infinite");
  ImGui::TextWrapped("Cherry is a powerfull low-level framework to help developpers to create simple or advanced application, with no advanced knowledges required. Cherry provide components, tools, a render engine & all backends you need to make the next revolutionnary app !");
}

Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  Cherry::ApplicationSpecification spec;
  std::shared_ptr<Layer> layer = std::make_shared<Layer>();

  // Properties
  std::string name = "Tiny window";
  spec.Name = name;
  spec.MinHeight = 200;
  spec.MinWidth = 200;
  spec.Height = 300;
  spec.Width = 700;
  spec.CustomTitlebar = false;
  spec.WindowResizeable = false;
  spec.DisableWindowManagerTitleBar = true;
  spec.RenderMode = WindowRenderingMethod::SimpleWindow;
  spec.UniqueAppWindowName = "main_window";
  spec.DisableTitle = true;
  spec.WindowSaves = true;
  spec.IconPath = Application::CookPath("ressources/imgs/icon.png");

  // Main app & font setup
  Cherry::Application *app = new Cherry::Application(spec);
  app->SetWindowSaveDataFile("savedatda.json", true);
  app->SetFavIconPath(Application::CookPath("ressources/imgs/favicon.png"));
  app->AddFont("Consola", Application::CookPath("ressources/fonts/consola.ttf"), 17.0f);

  // Localizations
  app->AddLocale("fr", Application::CookPath("ressources/locales/fr.json"));
  app->AddLocale("en", Application::CookPath("ressources/locales/en.json"));
  app->AddLocale("es", Application::CookPath("ressources/locales/es.json")); 
  app->SetDefaultLocale("en");                                             
  app->SetLocale("fr");

  app->PushLayer(layer);

  // Main render
  auto Areas = Cherry::MultiChildAreas::Create("main_window");
  Areas->m_IsHorizontal = true;
  Areas->AddChild(Child("One", []() { FirstPart(); }, 60.0f, true));
  Areas->AddChild(Child("Two", [](){ SecondPart(); }));
  Cherry::AddAppWindow(Areas->GetAppWindow());

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