#include "../../uikit.h"

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
    spec.MinHeight = 500;
    spec.MinWidth = 500;
    spec.CustomTitlebar = true;
    spec.DisableTitle = true;
    spec.IconPath = "icon.png";


  UIKit::Application *app = new UIKit::Application(spec);
    app->SetWindowSaveDataFile("/home/diego/savedata.json");

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
    std::thread Thread([&]() { UIKit::Main(argc, argv); });
    mainthread.swap(Thread);

    while(true){
        /*Your program loop...*/
    }

    return 0;
}