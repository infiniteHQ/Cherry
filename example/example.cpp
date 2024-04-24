#include "../uikit.h"
#include "../src/EntryPoint.h"

#include <thread>
#include <memory>

class Layer : public UIKit::Layer
{
public:
  Layer(){};

  virtual void OnUIRender() override
  {
    // The render channel
    ImGui::ShowDemoWindow();
  }
};

UIKit::Application *UIKit::CreateApplication(int argc, char **argv)
{
  UIKit::ApplicationSpecification spec;
  std::shared_ptr<Layer> layer = std::make_shared<Layer>();
  
  std::string name = "UIKit example";
  spec.Name = name;
  spec.CustomTitlebar = true;

  UIKit::Application *app = new UIKit::Application(spec);

  app->PushLayer(layer);
  app->SetMenubarCallback([app, layer]()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                app->Close();
            }
            ImGui::EndMenu();
        }
   });

  return app;
}

int main(int argc, char *argv[])
{
    std::thread mainthread;
    std::thread Thread([&]() { VortexMaker::VortexInterface(argc, argv); });
    mainthread.swap(Thread);

    while(true){
        /*Your program loop...*/
    }

    return 0;
}