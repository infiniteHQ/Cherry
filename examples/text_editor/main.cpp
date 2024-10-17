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

  std::string name = "Text Editor";
  spec.Name = name;
  spec.MinHeight = 500;
  spec.MinWidth = 500;
  spec.CustomTitlebar = false;
  spec.DisableWindowManagerTitleBar = false;
  spec.RenderMode = WindowRenderingMethod::SimpleWindow;
  spec.UniqueAppWindowName = "Test";
  spec.WindowSaves = false;
  spec.IconPath = Cherry::GetPath("ressources/imgs/favicon.png");

  Cherry::Application *app = new Cherry::Application(spec);
  app->SetFavIconPath(Cherry::GetPath("ressources/imgs/favicon.png"));
  app->AddFont("Consola", Cherry::GetPath("ressources/fonts/consola.ttf"), 17.0f);

  app->AddLocale("fr", Cherry::GetPath("ressources/locales/fr.json"));
  app->AddLocale("en", Cherry::GetPath("ressources/locales/en.json"));
  app->SetLocale("fr");

  app->PushLayer(layer);
  app->SetMenubarCallback([app, layer]()
                          {
                            ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);        
                            ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                            ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
                            ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);       

                            ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

                            ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);

                            ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);

                            static bool t;

                            if (ImGui::BeginMenu("Edit"))
                            {
                              Cherry::MenuItemTextSeparator("Main stuff");

                              if (Cherry::MenuItemSimple("Project Settings", "Main configurations of this project", &t))
                              {
                              }

                              Cherry::MenuItemTextSeparator("Main stuff");

                              if (Cherry::MenuItemSimple("Logs Utility", "Overview of all logs", &t))
                              {
                              }

                              Cherry::MenuItemTextSeparator("Main stuff");

                              if (Cherry::MenuItemSimple("Manage plugins", "Add, remove, edit plugins of this project"))
                              {
                              }

                              if (Cherry::MenuItemSimple("Manage modules", "Manage modules loaded/registered", &t))
                              {
                              }

                              if (Cherry::MenuItemSimple("Templates modules", "Create, add template in your project", &t))
                              {
                              }

                              ImGui::EndMenu();
                            }


                            ImGui::PopStyleVar();  
                            ImGui::PopStyleColor(2); });

  auto TextEditor = TextEditorSimple::Create("Test");
  Cherry::AddAppWindow(TextEditor->GetAppWindow());

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