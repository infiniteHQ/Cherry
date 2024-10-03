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
  spec.MinHeight = 500;
  spec.MinWidth = 500;
  spec.CustomTitlebar = true;
  spec.DisableWindowManagerTitleBar = true;
  spec.EnableDocking = true;
  spec.DisableTitle = true;
  spec.WindowSaves = true;
  spec.IconPath = BINPATH("ressources/imgs/icon.png");

  UIKit::Application *app = new UIKit::Application(spec);
  app->SetWindowSaveDataFile("savedatda.json", true);
  app->SetFavIconPath(Application::CookPath("ressources/imgs/favicon.png"));
  app->AddFont("Consola", Application::CookPath("ressources/fonts/consola.ttf"), 17.0f);

  app->AddLocale("fr", Application::CookPath("ressources/locales/fr.json"));
  app->AddLocale("en", Application::CookPath("ressources/locales/en.json"));
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
                              ImGui::GetFont()->Scale *= 0.8;
                              ImGui::PushFont(ImGui::GetFont());

                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                              ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                              ImGui::Text("Main stuff");
                              ImGui::PopStyleColor();

                              ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                              ImGui::Separator();
                              ImGui::PopStyleColor();

                              ImGui::GetFont()->Scale = 0.84;
                              ImGui::PopFont();
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                              if (ImGui::MenuItem("Project Settings", "Main configurations of this project", &t))
                              {
                              }

                              if(ImGui::Button("Set en"))
                              {
  app->SetLocale("en");
                              }

                              if(ImGui::Button("Set fr"))
                              {
  app->SetLocale("fr");
                              }
                              ImGui::GetFont()->Scale *= 0.8;
                              ImGui::PushFont(ImGui::GetFont());

                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                              ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                              ImGui::Text("Main stuff");
                              ImGui::PopStyleColor();

                              ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                              ImGui::Separator();
                              ImGui::PopStyleColor();

                              ImGui::GetFont()->Scale = 0.84;
                              ImGui::PopFont();
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                              if (ImGui::MenuItem("Logs Utility", "Overview of all logs", &t))
                              {
                              }

                              ImGui::GetFont()->Scale *= 0.8;
                              ImGui::PushFont(ImGui::GetFont());

                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                              ImGui::PushStyleColor(ImGuiCol_Text, grayColor);
                              ImGui::Text("Main stuff");
                              ImGui::PopStyleColor();

                              ImGui::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                              ImGui::Separator();
                              ImGui::PopStyleColor();

                              ImGui::GetFont()->Scale = 0.84;
                              ImGui::PopFont();
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);

                              if (ImGui::MenuItem("Manage plugins", "Add, remove, edit plugins of this project"))
                              {
                              }

                              if (ImGui::MenuItem("Manage modules", "Manage modules loaded/registered", &t))
                              {
                              }

                              if (ImGui::MenuItem("Templates modules", "Create, add template in your project", &t))
                              {
                              }

                              ImGui::EndMenu();
                            }

                            ImGui::PopStyleVar();  
                            ImGui::PopStyleColor(2); });



 /*std::shared_ptr<ContentOutlinerSimple> appContentOutliner = std::make_shared<UIKit::ContentOutlinerSimple>("?loc:loc.window_names.outliner");
  appContentOutliner->RefreshRender(appContentOutliner);

  std::shared_ptr<TextEditorSimple> appTextEditor = std::make_shared<UIKit::TextEditorSimple>("?loc:loc.window_names.text_editor");
  appTextEditor->RefreshRender(appTextEditor);

  std::shared_ptr<PropsEditorSimple> appPropsEditor = std::make_shared<UIKit::PropsEditorSimple>("?loc:loc.window_names.props_editor");
  appPropsEditor->RefreshRender(appPropsEditor);
  
  std::shared_ptr<ContentBrowserAppWindow> appContentBrowser = std::make_shared<UIKit::ContentBrowserAppWindow>("?loc:loc.window_names.content_browser", "/home/diego");
  appContentBrowser->RefreshRender(appContentBrowser);
   
  std::shared_ptr<MultiChildAreas> appAreas = std::make_shared<UIKit::MultiChildAreas>("indow_names.content_browser");
  appAreas->AddChild(Child("One", [](){ImGui::Text("One");}));
  appAreas->AddChild(Child("Two", [](){ImGui::Text("Two");}));
  appAreas->AddChild(Child("Three", [](){ImGui::Text("Three");}));
  appAreas->AddChild(Child("Four", [](){ImGui::Text("Four");}));
  appAreas->RefreshRender(appAreas);

   
  std::shared_ptr<MultiChildTabs> appTabs = std::make_shared<UIKit::MultiChildTabs>("indow_names.qsd");
  appTabs->AddChild("One", [](){ImGui::Text("One");});
  appTabs->AddChild("Two", [](){ImGui::Text("Two");});
  appTabs->AddChild("Three", [](){ImGui::Text("Three");});
  appTabs->RefreshRender(appTabs);

  std::shared_ptr<NodeEditorSimple> appNodeEditor = std::make_shared<UIKit::NodeEditorSimple>("?loc:loc.window_names.node_editor");
  appNodeEditor->RefreshRender(appNodeEditor);*/
  
  /*std::shared_ptr<DemoAppWindow> appDemo = std::make_shared<UIKit::DemoAppWindow>("?loc:loc.window_names.node_editor");
  appDemo->RefreshRender(appDemo);
  
  


  /*std::shared_ptr<DemoAppWindow> appDemo = std::make_shared<UIKit::DemoAppWindow>("Demo");
  appDemo->RefreshRender(appDemo);*/
  /*std::shared_ptr<MultiChildAreas> appContentBrowser = std::make_shared<UIKit::MultiChildAreas>("?loc:loc.window_names.content_browser");
  appContentBrowser->AddChild(Child("One", [](){ImGui::Text("One");}));
  appContentBrowser->AddChild(Child("Two", [](){ImGui::Text("Two");}));
  appContentBrowser->AddChild(Child("Three", [](){ImGui::Text("Three");}));

  appContentBrowser->RefreshRender(appContentBrowser);*/

  // List

  /*std::shared_ptr<PropsEditorSimple> appPropsEditor = std::make_shared<UIKit::PropsEditorSimple>("?loc:loc.window_names.props_editor");
  appPropsEditor->RefreshRender(appPropsEditor);*/
std::shared_ptr<DataLogsViewer> appContentOutliner = std::make_shared<UIKit::DataLogsViewer>("qsd", Application::CookPath("ressources/locales/new.json"));
  appContentOutliner->RefreshRender(appContentOutliner);

  //std::shared_ptr<DockingAppWindow> appTextEdditor = std::make_shared<UIKit::DockingAppWindow>("fqdow_names.texqsd");
  
  // Multi childs
  /*std::shared_ptr<MultiChildAreas> appAreas = std::make_shared<UIKit::MultiChildAreas>("indow_names.content_browser");
  appAreas->m_IsHorizontal = false;
  appAreas->AddChild(Child("One", [](){ImGui::Text("One");}));
  appAreas->AddChild(Child("Two", [](){ImGui::Text("Two");}, 20.0f));
  appAreas->AddChild(Child("Three", [](){ImGui::Text("Three");}));
  appAreas->AddChild(Child("Four", [](){ImGui::Text("Four");}));
  appAreas->RefreshRender(appAreas);

  std::shared_ptr<MultiChildList> appLists = std::make_shared<UIKit::MultiChildList>("indow_names.list");
  appLists->AddChild("One", [](){ImGui::Text("One");});
  appLists->AddChild("Two", [](){ImGui::Text("Two");});
  appLists->AddChild("Three", [](){ImGui::Text("Three");});
  appLists->AddChild("Four", [](){ImGui::Text("Four");});
  appLists->RefreshRender(appLists);
  

  std::shared_ptr<MultiChildTabs> appTabs = std::make_shared<UIKit::MultiChildTabs>("indow_names.tabs");
  appTabs->AddChild("One", [](){ImGui::Text("One");});
  appTabs->AddChild("Two", [](){ImGui::Text("Two");});
  appTabs->AddChild("Three", [](){ImGui::Text("Three");});
  appTabs->AddChild("Four", [](){ImGui::Text("Four");});
  appTabs->RefreshRender(appTabs);*/
  /*std::shared_ptr<MultiChildAreas> appAreasd = std::make_shared<UIKit::MultiChildAreas>("QSd");
  appAreasd->m_IsHorizontal = true;
  appAreasd->AddChild(Child("O2ne", [](){ImGui::Text("One");}));
  appAreasd->AddChild(Child("T2wo", [](){ImGui::Text("Two");}));
  appAreasd->AddChild(Child("T.hree", [](){ImGui::Text("Three");}));
  appAreasd->AddChild(Child("Fo.ur", [](){ImGui::Text("Four");}));
  appAreasd->RefreshRender(appAreasd);*/
  
  
  return app;
}

int main(int argc, char *argv[])
{
  std::thread mainthread;
  std::thread Thread([&]()
                     { UIKit::Main(argc, argv); });
  mainthread.swap(Thread);

  while (g_ApplicationRunning)
  {
    /* Your program loop... */
  }

  mainthread.join();

  return 0;
}