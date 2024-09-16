#define UIKIT_V1
#include "../../uikit.hpp"

#include <thread>
#include <memory>

class Layer : public UIKit::Layer
{
public:
  Layer() {};

  virtual void OnUIRender() override
  {
    // The render channel (OBSOLETE)
    ImGui::ShowDemoWindow();
  }
};

UIKit::Application *UIKit::CreateApplication(int argc, char **argv)
{
  UIKit::ApplicationSpecification spec;
  std::shared_ptr<Layer> layer = std::make_shared<Layer>();

  std::string name = "UIKit example";
  spec.Name = name;
  spec.Height = 300;
  spec.Width = 500;
  spec.CustomTitlebar = true;
  spec.DisableWindowManagerTitleBar = true;
  spec.DisableTitleBar = true;
  spec.EnableDocking = false;
  spec.DisableTitle = true;
  spec.WindowSaves = true;
  spec.IconPath = "icon.png";

  UIKit::Application *app = new UIKit::Application(spec);
  app->SetWindowSaveDataFile("savedatda.json", true);
  app->SetFavIconPath("/usr/local/include/Vortex/imgs/vortex.png");
  app->PushLayer(layer);
  app->SetMenubarCallback([app, layer]() {});

  app->SetMainRenderCallback([]()
                             { TextButtonUnderline("Test"); });

  return app;
}

int main(int argc, char *argv[])
{
  std::thread mainthread;
  std::thread Thread([&]()
                     { UIKit::Main(argc, argv); });
  mainthread.swap(Thread);

  while (true)
  {
    /*Your program loop...*/
  }

  return 0;
}