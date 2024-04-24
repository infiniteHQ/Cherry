#include "../uikit.h"

class ExampleLayer : public UIKit::Layer
{
public:
  ExampleLayer(){};

  virtual void OnUIRender() override
  {
    ImGui::ShowDemoWindow();
  }
};

UIKit::Application *UIKit::CreateApplication(int argc, char **argv)
{
  UIKit::ApplicationSpecification spec;
  std::shared_ptr<ExampleLayer> exampleLayer = std::make_shared<ExampleLayer>();
  
  std::string name = "UIKit example";
  spec.Name = name;
  spec.CustomTitlebar = true;

  UIKit::Application *app = new UIKit::Application(spec);

  app->PushLayer(exampleLayer);
  app->SetMenubarCallback([app, exampleLayer]()
                          {
                            static int number = 1;
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
