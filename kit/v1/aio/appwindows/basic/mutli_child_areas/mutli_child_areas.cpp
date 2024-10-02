#include "./mutli_child_areas.hpp"

namespace UIKit
{
    MultiChildAreas::MultiChildAreas(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetParent(parent);
        m_AppWindow->SetRenderCallback([this]() {

        });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]() {});

        m_AppWindow->SetRightMenubarCallback([win]() {});
    }

    MultiChildAreas::MultiChildAreas(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");

        m_SelectedChildName = "TEst1";

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;
        m_AppWindow->SetLeftMenubarCallback([]() {});
        m_AppWindow->SetRightMenubarCallback([win]() {});

        Application::Get().PutWindow(m_AppWindow);
    }

    void MultiChildAreas::AddChild(const Child &child)
    {
        m_Childs.push_back(child);
    }

    void MultiChildAreas::RemoveChild(const std::string &child_name)
    {
    }

    std::function<void()> MultiChildAreas::GetChild(const std::string &child_name)
    {
    }






void MultiChildAreas::RefreshRender(const std::shared_ptr<MultiChildAreas> &instance)
{
    m_AppWindow->SetRenderCallback([instance]()
                                   {
                                       const float splitterWidth = 2.5f;
                                       const float margin = 10.0f; // Marge avant les bords

                                       auto &children = instance->m_Childs;
                                       const bool isHorizontal = instance->m_IsHorizontal;

                                       ImGuiWindow *window = ImGui::GetCurrentWindow();
                                       ImVec2 windowSize = window->ContentRegionRect.GetSize();

                                       // Calculer la taille totale disponible pour les enfants
                                       float totalAvailableSize = windowSize.x - (children.size() - 1) * splitterWidth;

                                       // Initialiser les variables pour le calcul de l'espace restant
                                       float usedSize = 0.0f;
                                       int childrenWithoutDefaultSize = 0;

                                       // Parcourir tous les enfants et appliquer `m_DefaultSize` là où c'est applicable
                                       for (auto &child : children)
                                       {
                                           if (child.m_DefaultSize > 0.0f)
                                           {
                                               // Appliquer `m_DefaultSize` si elle est définie
                                               if (child.m_Size == 0.0f) // Ne pas écraser la taille si elle a déjà été définie
                                               {
                                                   child.m_Size = child.m_DefaultSize;
                                               }
                                               usedSize += child.m_Size;
                                               child.m_Initialized = true;
                                           }
                                           else
                                           {
                                               // Compter les enfants sans taille par défaut pour répartir l'espace restant
                                               childrenWithoutDefaultSize++;
                                           }
                                       }

                                       // Calculer l'espace restant après application des `m_DefaultSize`
                                       float remainingSize = totalAvailableSize - usedSize;

                                       // Si l'espace restant est négatif, réduire la taille des enfants proportionnellement
                                       if (remainingSize < 0)
                                       {
                                           float reductionFactor = totalAvailableSize / usedSize;
                                           for (auto &child : children)
                                           {
                                        if(child.m_InitializedSec)
                                        continue;

                                               if (child.m_DefaultSize > 0.0f)
                                               {
                                                   child.m_Size *= reductionFactor;
                                               child.m_InitializedSec = true;
                                               }
                                           }
                                           remainingSize = 0; // Pas d'espace restant à distribuer
                                       }

                                       // Répartir l'espace restant entre les enfants sans `m_DefaultSize`
                                       float sizePerChild = (childrenWithoutDefaultSize > 0) ? (remainingSize / childrenWithoutDefaultSize) : 0.0f;
                                       for (auto &child : children)
                                       {
                                        if(child.m_InitializedTh)
                                        continue;

                                           if (child.m_DefaultSize == 0.0f)
                                           {
                                               child.m_Size = sizePerChild;
                                               child.m_InitializedTh = true;
                                           }
                                       }

                                       // Gérer la redéfinition des tailles via les splitters
                                       for (size_t i = 0; i < children.size(); ++i)
                                       {
                                           auto &child = children[i];

                                           ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                                           ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                                           ImGui::BeginChild(child.m_Name.c_str(), ImVec2(child.m_Size, windowSize.y), true);

                                           child.m_Child();

                                           ImGui::EndChild();
                                           ImGui::PopStyleColor(2);

                                           // Ajouter un splitter entre les enfants
                                           if (i + 1 < children.size())
                                           {
                                               auto &next_child = children[i + 1];
                                               ImGui::SameLine();

                                               std::string lab = "##splitter" + child.m_Name;

                                               ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                                               ImGui::Button(lab.c_str(), ImVec2(splitterWidth, -1));
                                               ImGui::PopStyleColor();

                                               if (ImGui::IsItemHovered())
                                               {
                                                   ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                                               }

                                               if (ImGui::IsItemActive())
                                               {
                                                   float delta = ImGui::GetIO().MouseDelta.x;

                                                   // Ne modifier que l'enfant à gauche et celui à droite du splitter
                                                   child.m_Size += delta;
                                                   next_child.m_Size -= delta;

                                                   // Limiter la taille de l'enfant à gauche
                                                   if (child.m_Size < child.m_MinSize + margin)
                                                   {
                                                       float diff = (child.m_MinSize + margin) - child.m_Size;
                                                       child.m_Size = child.m_MinSize + margin;
                                                       next_child.m_Size -= diff; // Ajuste la taille de l'enfant de droite
                                                   }

                                                   // Limiter la taille de l'enfant à droite
                                                   if (next_child.m_Size < next_child.m_MinSize + margin)
                                                   {
                                                       float diff = (next_child.m_MinSize + margin) - next_child.m_Size;
                                                       next_child.m_Size = next_child.m_MinSize + margin;
                                                       child.m_Size -= diff; // Ajuste la taille de l'enfant de gauche
                                                   }
                                               }

                                               ImGui::SameLine();
                                           }
                                       }
                                   });
}



}
