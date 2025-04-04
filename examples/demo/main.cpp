#include "../../cherry.hpp"

void Render()
{
  CherryKit::TitleThree("Welcome to cherry !");
}


Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  Cherry::ApplicationSpecification config;
  config.SetName("Hello Cherry");                                    // The name of the window
  config.SetMinimumWidth(100);                                       // The minimum width
  config.SetMinimumHeight(100);                                      // The minimum height
  config.SetDefaultWidth(800);                                       // The default width
  config.SetDefaultHeight(500);                                      // The default height
  config.SetGlobalScale(0.84f);                                      // The flobal scale of components and windows
  config.SetFontGlobalScale(0.84f);                                  // The flobal scale of fonts
  config.SetFavIconPath(Cherry::GetPath("resources/imgs/icon.png")); // Icon of the window. (for folder in [project]/assets)
  config.SetRenderMode(WindowRenderingMethod::DockingWindows);       // Render mode

  Cherry::AddAppWindow(CherryKit::WindowSimple("Test window", Render));

  return new Cherry::Application(config);
}

int main(int argc, char *argv[]) // Entry point of the executable
{
  std::thread([&](){ Cherry::Main(argc, argv); }).detach(); // Independant UI thread

  while (true) // Optionnal, but this is an exemple of cherry signals implementation
  {
    if (!Cherry::IsRunning()) // The ui is not running and will stop? 
    return 0;

    if (!Cherry::IsReady())  // The ui is ready? (all criticals backend and components are loaded?)
    continue;
    
  }
}