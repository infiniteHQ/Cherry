#include "../../cherry.hpp"

void Render()
{
  Cherry::BrowserView("test", 1280, 920, "about:gpu");
}

Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  Cherry::ApplicationSpecification config;
  config.SetName("Hello Cherry");                                    // The name of the window
  config.SetMinimumWidth(100);                                       // The minimum width
  config.SetMinimumHeight(100);                                      // The minimum height
  config.SetDefaultWidth(1500);                                       // The default width
  config.SetDefaultHeight(900);                                      // The default height
  config.SetGlobalScale(0.84f);                                      // The flobal scale of components and windows
  config.SetFontGlobalScale(0.84f);                                  // The flobal scale of fonts
  config.SetFavIconPath(Cherry::GetPath("resources/imgs/icon.png")); // Icon of the window. (for folder in [project]/assets)
  config.SetRenderMode(WindowRenderingMethod::DockingWindows);       // Render mode

  // UseAudio
  // UseCEF
  Cherry::ImGui_ImplSDL2_CefInit(argc, argv);
  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window", Render));

  return new Cherry::Application(config);
}

int main(int argc, char *argv[])
{
  if(Cherry::HandleCEF() == 0)
  {
    return 0;
  }
  
  std::thread uiThread([&]() {
      Cherry::Main(argc, argv);
  });

  uiThread.join(); 
  return 0;
}
