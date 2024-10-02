#include "./multi_child_tabs.hpp"

namespace UIKit
{
    MultiChildTabs::MultiChildTabs(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

    MultiChildTabs::MultiChildTabs(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");


        m_SelectedChildName = "TEst1";

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;
        m_AppWindow->SetLeftMenubarCallback([]() {});
        m_AppWindow->SetRightMenubarCallback([win]() {});

        Application::Get().PutWindow(m_AppWindow);
    }

    void MultiChildTabs::AddChild(const Child &child)
    {
        m_Childs.push_back(child);
    }

    void MultiChildTabs::RemoveChild(const std::string &child_name)
    {
    }

    std::function<void()> MultiChildTabs::GetChild(const std::string &child_name)
    {

    }

void MultiChildTabs::RefreshRender(const std::shared_ptr<MultiChildTabs> &instance)
{
    m_AppWindow->SetRenderCallback([instance]()
    {
        auto& children = instance->m_Childs;

        // Obtenir les dimensions de la fenêtre actuelle
        ImVec2 window_size = ImGui::GetContentRegionAvail();
        
        // Calculer l'espace total disponible pour les enfants
        float total_size = instance->m_IsHorizontal ? window_size.x : window_size.y;

        // Calculer la taille des enfants avec l'espace disponible
        float remaining_size = total_size;
        float resizable_child_count = 0;
        
        for (const auto& child : children)
        {
            // Les enfants non redimensionnables doivent occuper leur taille par défaut
            if (!child.m_Resizable)
            {
                remaining_size -= child.m_DefaultSize;
            }
            else
            {
                resizable_child_count += 1;
            }
        }

        // Taille moyenne pour chaque enfant redimensionnable
        float resizable_child_size = (resizable_child_count > 0) ? remaining_size / resizable_child_count : 0;

        // Commencer la disposition
        for (const auto& child : children)
        {
            // Si l'enfant est redimensionnable, ajuster sa taille
            float child_size = child.m_Resizable ? resizable_child_size : child.m_DefaultSize;

            // Appliquer les tailles minimales et maximales
            child_size = std::max(child.m_MinSize, std::min(child_size, child.m_MaxSize));

            if (instance->m_IsHorizontal)
            {
                ImGui::BeginChild(child.m_Name.c_str(), ImVec2(child_size, window_size.y), true);
            }
            else
            {
                ImGui::BeginChild(child.m_Name.c_str(), ImVec2(window_size.x, child_size), true);
            }

            // Exécuter le contenu de l'enfant
            if (child.m_Child)
            {
                child.m_Child();
            }

            ImGui::EndChild();
            
            // Si l'enfant n'est pas redimensionnable, ignorer l'espacement
            if (!child.m_Resizable)
            {
                ImGui::SameLine();
            }
        }
    });
}


}
