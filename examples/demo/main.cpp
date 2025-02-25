#include "../../cherry.hpp"

void Render()
{
  CherryKit::ImageLocal(Cherry::GetPath("resources/imgs/banner.png"), ImGui::GetWindowSize().x, ImGui::GetWindowSize().x / 4.0f);
  Cherry::SetNextComponentProperty("color_text", "#454545B2");
  CherryKit::TextWrapped("Cherry is a complete & modern UI/UX framework for low level native apps. Minimal, but complete & advanced. Including backends, a render engine & ui components(ImGui/Vulkan/SDL) ");
  
  CherryKit::TitleThree("Features");

  CherryKit::HeaderText("Components & Kits", CherryLambda(
    CherryKit::BulletText("Many single components ready to use with data managment");
    CherryKit::BulletText("Many all in one windows, ready to use and manage");
    CherryKit::BulletText("ImGui abstractions for simplicity");
    CherryKit::BulletText("SDL abstractions for inputs & user managment");
    CherryKit::BulletText("Short inlines components");
  ));

  CherryKit::HeaderText("Render engine", CherryLambda(
    CherryKit::BulletText("Data handler for windows & components");
    CherryKit::BulletText("Localization & Traduction managment");
    CherryKit::BulletText("Themes manager, Dark & Ligth themes");
    CherryKit::BulletText("Fonts manager");
    CherryKit::BulletText("Images & textures manager");
    CherryKit::BulletText("Menubar & Bottombar simple callbacks");
    CherryKit::BulletText("Custom window close behaviors");
    CherryKit::BulletText("Advanced Docking (WIP)");
    CherryKit::BulletText("Toasts, Notifications & pop-ups");
    CherryKit::BulletText("Behaviros callbacks (closing, moving, etc...)");
    CherryKit::BulletText("Windows customisations (size, colors, properties & styles)");
  ));

  CherryKit::HeaderText("Backends", CherryLambda(
    CherryKit::BulletText("Multi context with contexts cohesion & communication (ImGui/SDL/Vulkan)");
    CherryKit::BulletText("Complete image load & show, cache & optimization");
    CherryKit::BulletText("Window builder");
  ));
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

  return new Cherry::Application(config);;
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