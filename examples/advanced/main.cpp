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
  spec.RenderMode = WindowRenderingMethod::DockingWindows;

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
  app->SetMenubarCallback([app, layer, spec]()
                          {
                            ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);        
                            ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                            ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
                            ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);       

                            ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);
                            ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);
                            ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);

                            static bool t;

                            if (ImGui::BeginMenu("Options"))
                            {
                              Cherry::MenuItemTextSeparator("Test Stuff");

                              if (ImGui::MenuItem("Project Settings", "Main configurations of this project", &t))
                              {
  auto TextEditor = TextEditorSimple::Create("?loc:loc.window_names.text_editor");
  Cherry::AddAppWindow(TextEditor->GetAppWindow());

  Cherry::ApplicationSpecification newspec = spec;
  newspec.Width = 850;
  newspec.Height = 600;
  newspec.WindowOnlyClosable = true;
  newspec.RenderMode = Cherry::WindowRenderingMethod::SimpleWindow;
  newspec.UsingCloseCallback = true;
  newspec.WindowSaves = false;
  newspec.UniqueAppWindowName = TextEditor->GetAppWindow()->m_Name;
  newspec.IconPath = Cherry::GetPath("ressources/imgs/icons/misc/icon_add.png");
  newspec.FavIconPath = Cherry::GetPath("ressources/imgs/icons/misc/icon_add.png");
  newspec.ColorTheme.SetColor(ImGuiCol_Border, "#B1FF31FF");

  newspec.MenubarCallback = [TextEditor]()
  {
    if (ImGui::BeginMenu("Edit"))
    {
      ImGui::Text("Main stuff");
      ImGui::EndMenu();
    }
  };
  newspec.CloseCallback = [TextEditor]()
  {
    if (TextEditor->GetAppWindow())
    {
      Cherry::DeleteAppWindow(TextEditor->GetAppWindow());
    }
  };

  TextEditor->GetAppWindow()->AttachOnNewWindow(newspec);
  TextEditor->GetAppWindow()->SetVisibility(true);

                              }


                              if(ImGui::Button("Set en"))
                              {
                                app->SetLocale("en");
                              }

                              if(ImGui::Button("Set fr"))
                              {
                                app->SetLocale("fr");
                              }

                              if(ImGui::Button("Set es"))
                              {
                                app->SetLocale("es");
                              }

                              Cherry::MenuItemTextSeparator("Main stuff");

                              if (ImGui::MenuItem("Logs Utility", "Overview of all logs", &t))
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
                            
     /*auto ContentBrowser = ContentBrowserAppWindow::Create("?loc:loc.window_names.content_browser", "/home/diego");
    Cherry::AddAppWindow(ContentBrowser->GetAppWindow());

   auto ContentOutliner = ContentOutlinerSimple::Create("?loc:loc.window_names.outliner");
    Cherry::AddAppWindow(ContentOutliner->GetAppWindow());
    auto PropsEditor = PropsEditorSimple::Create("?loc:loc.window_names.props_editor");
    Cherry::AddAppWindow(PropsEditor->GetAppWindow());

    //auto NodalEditor = NodeEditorSimple::Create("?loc:loc.window_names.node_editor");
    //Cherry::AddAppWindow(NodalEditor->GetAppWindow());


    auto TabsWindow = MultiChildTabs::Create("Vertical Tabs");
    TabsWindow->AddChild("One", [](){ImGui::Text("One");});
    TabsWindow->AddChild("Two", [](){ImGui::Text("Two");});
    TabsWindow->AddChild("Three", [](){ImGui::Text("Three");});
    Cherry::AddAppWindow(TabsWindow->GetAppWindow());*/

  /*

  auto WindowWithDockspace = DockingAppWindow::Create("fqdow_names.texqsd");
  Application::Get().PutWindow(WindowWithDockspace->GetAppWindow());
  auto ListWindow = MultiChildList::Create("List Childs");
  ListWindow->AddChild("One", []()
                       { ImGui::Text("One"); });
  ListWindow->AddChild("Two", []()
                       { ImGui::Text("Two"); });
  ListWindow->AddChild("Three", []()
                       { ImGui::Text("Three"); });
  ListWindow->AddChild("Four", []()
                       { ImGui::Text("Four"); });
  Cherry::AddAppWindow(ListWindow->GetAppWindow());

  auto WindowWithDockspace = DockingAppWindow::Create("fqdow_names.texqsd");
  // Application::Get().PutWindow(WindowWithDockspace->GetAppWindow());
  auto ContentOutliner = ContentOutlinerSimple::Create("?loc:loc.window_names.outliner");
  Cherry::AddAppWindow(ContentOutliner->GetAppWindow());
  auto PropsEditor = PropsEditorSimple::Create("?loc:loc.window_names.props_editor");
  Cherry::AddAppWindow(PropsEditor->GetAppWindow());

  auto NodalEditor = NodeEditorSimple::Create("?loc:loc.window_names.node_editor");
  Cherry::AddAppWindow(NodalEditor->GetAppWindow());
  auto ContentBrowser = ContentBrowserAppWindow::Create("?loc:loc.window_names.content_browser", "/home/diego");
  Cherry::AddAppWindow(ContentBrowser->GetAppWindow());

  auto Demo = DemoAppWindow::Create("?loc:loc.window_names.demo");
  Cherry::AddAppWindow(Demo->GetAppWindow());

  auto TabsWindow = MultiChildTabs::Create("Vertical Tabs");
  TabsWindow->AddChild("One", [](){ImGui::Text("One");});
  TabsWindow->AddChild("Two", [](){ImGui::Text("Two");});
  TabsWindow->AddChild("Three", [](){ImGui::Text("Three");});
  Cherry::AddAppWindow(TabsWindow->GetAppWindow());


  auto ListWindow = MultiChildList::Create("List Childs");
  ListWindow->AddChild("One", [](){ImGui::Text("One");});
  ListWindow->AddChild("Two", [](){ImGui::Text("Two");});
  ListWindow->AddChild("Three", [](){ImGui::Text("Three");});
  ListWindow->AddChild("Four", [](){ImGui::Text("Four");});
  Cherry::AddAppWindow(ListWindow->GetAppWindow());

  // Multi childs
  auto VerticalAreas = MultiChildAreas::Create("indow_names.content_browser");
  VerticalAreas->m_IsHorizontal = false;
  VerticalAreas->AddChild(Child("One", [](){ ImGui::Text("One"); }, 5.0f));
  VerticalAreas->AddChild(Child("Two", [](){ ImGui::Text("Two"); }, 20.0f));
  VerticalAreas->AddChild(Child("Three", []() { ImGui::Text("Three"); }));
  VerticalAreas->AddChild(Child("Four", [](){ ImGui::Text("Four"); }));
  Cherry::AddAppWindow(VerticalAreas->GetAppWindow());


  auto HorizontalAreas = MultiChildAreas::Create("indow_names.content_browser2");
  HorizontalAreas->m_IsHorizontal = true;
  HorizontalAreas->AddChild(Child("One2", [](){ ImGui::Text("One"); }, 50.0f));
  HorizontalAreas->AddChild(Child("Two2", [](){ ImGui::Text("Two"); }, 30.0f));
  HorizontalAreas->AddChild(Child("Three2", [](){ ImGui::Text("Three"); }, 20.0f));
  HorizontalAreas->AddChild(Child("Four2", [](){ ImGui::Text("Four"); }));
  Cherry::AddAppWindow(HorizontalAreas->GetAppWindow());*/

    auto Demo = DemoAppWindow::Create("?loc:loc.window_names.demo");
    Cherry::AddAppWindow(Demo->GetAppWindow());
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