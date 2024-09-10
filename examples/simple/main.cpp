#include "../../uikit.hpp"

#include <thread>
#include <memory>

void render()
{
}

void menubar_left()
{
}

class FirstWindow
{
public:
  FirstWindow(const std::string &name)
  {
    m_AppWindow = std::make_shared<UIKit::AppWindow>("first_window", "FirstWindow");
    m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
    m_AppWindow->SetRenderCallback([this]()
                                   {
        ImGui::Text("AA.BB.CC.DD.EE.FF 1");
        ImGui::Button("qsd"); });

    std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

    m_AppWindow->SetLeftMenubarCallback([]()
                                        { ImGui::Text("ll"); });
    m_AppWindow->SetRightMenubarCallback([win]()
                                         { ImGui::Text("123456789123456789"); 
                                         
                                         static ImTextureID texture = win->GetImage("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Ajuster le style des boutons pour éviter le rognage
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6)); // Augmenter le padding

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
    if (ImGui::UIKit_ImageButtonWithText(texture, "Add", ImVec2(20, 20))) // Augmenter la taille du bouton si nécessaire
    {
        // Action du bouton
    }
    if (ImGui::UIKit_ImageButtonWithText(texture, "CasserDuTerrio", ImVec2(20, 20))) // Augmenter la taille du bouton si nécessaire
    {
        // Action du bouton
    }
    ImGui::PopStyleColor();

    // Styles pour les autres boutons
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(62, 62, 62, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(62, 62, 62, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(62, 62, 62, 0));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(62, 62, 62, 0));

    
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(); // Pop du style modifié
                                         
                                         });
  }

  std::shared_ptr<UIKit::AppWindow> m_AppWindow;
};

class sFirstWindow
{
public:
  sFirstWindow(const std::string &name)
  {
    m_AppWindow = std::make_shared<UIKit::AppWindow>("first_window", "sFirstWindow");
    m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
    m_AppWindow->SetRenderCallback([this]()
                                   {
        ImGui::Text("AA.BB.CC.DD.EE.FF 1");
        ImGui::Button("qsd"); });

    std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

    
m_AppWindow->SetLeftMenubarCallback([win]()
{
    static ImTextureID texture = win->GetImage("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Ajuster le style des boutons pour éviter le rognage
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6)); // Augmenter le padding

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
    if (ImGui::UIKit_ImageButtonWithText(texture, "Add", ImVec2(20, 20))) // Augmenter la taille du bouton si nécessaire
    {
        // Action du bouton
    }
    ImGui::PopStyleColor();

    // Styles pour les autres boutons
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(62, 62, 62, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(62, 62, 62, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(62, 62, 62, 0));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(62, 62, 62, 0));

    if (ImGui::UIKit_ImageButtonWithText(texture, "Save all", ImVec2(20, 20))) // Vérifie la taille du bouton
    {
        // Action du bouton
    }
    if (ImGui::UIKit_ImageButtonWithText(texture, "Import", ImVec2(20, 20))) // Ajuster la taille si nécessaire
    {
        // Action du bouton
    }
    
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(); // Pop du style modifié
});
  }

  std::shared_ptr<UIKit::AppWindow> m_AppWindow;
};

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
  spec.DisableTitle = true;
  spec.IconPath = "icon.png";

  UIKit::Application *app = new UIKit::Application(spec);
  app->SetWindowSaveDataFile("/home/diego/savedatda.json");

  app->PushLayer(layer);
  app->SetMenubarCallback([app, layer]()
                          {
                            ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);              // Gris (50% blanc)
                            ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);     // Gris (50% blanc)
                            ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f); // Fond plus foncé
                            ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);       // Bordure plus claire

                            // Pousser le style pour le fond plus foncé
                            ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

                            // Pousser le style pour la bordure plus claire
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

                            ImGui::PopStyleVar();    // Pour les bords arrondis
                            ImGui::PopStyleColor(2); // Pour le fond et la bordure
                          });

  /*app->SetCloseCallback([app, layer]()
                          {

    std::cout << "QDS" << std::endl;
                          });*/

  std::shared_ptr<FirstWindow> windodo = std::make_shared<FirstWindow>("FirstWindow");
  std::shared_ptr<sFirstWindow> windodos = std::make_shared<sFirstWindow>("sFirstWindow");
  app->PutWindow(windodo->m_AppWindow);
  app->PutWindow(windodos->m_AppWindow);

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