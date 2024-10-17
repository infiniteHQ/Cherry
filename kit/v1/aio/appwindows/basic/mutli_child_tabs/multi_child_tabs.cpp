#include "./multi_child_tabs.hpp"

namespace Cherry
{
    MultiChildTabs::MultiChildTabs(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetParent(parent);
        m_AppWindow->SetRenderCallback([this]() {

        });

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]() {});

        m_AppWindow->SetRightMenubarCallback([win]() {});
    }

    MultiChildTabs::MultiChildTabs(const std::string &name)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
        m_AppWindow->SetInternalPaddingX(8.0f);
        m_AppWindow->SetInternalPaddingY(12.0f);
        m_AppWindow->SetLeftMenubarCallback([]() {});
        m_AppWindow->SetRightMenubarCallback([win]() {});
    }

    std::shared_ptr<Cherry::AppWindow> &MultiChildTabs::GetAppWindow()
    {
        return m_AppWindow;
    }

    std::shared_ptr<MultiChildTabs> MultiChildTabs::Create(const std::string &name)
    {
        auto instance = std::shared_ptr<MultiChildTabs>(new MultiChildTabs(name));
        instance->SetupRenderCallback();
        return instance;
    }

    void MultiChildTabs::SetupRenderCallback()
    {
        auto self = shared_from_this();
        m_AppWindow->SetRenderCallback([self]()
                                       {
            if (self) {
                self->Render();
            } });
    }

    void MultiChildTabs::AddChild(const std::string &child_name, const std::function<void()> &child)
    {
        m_Childs[child_name] = child;
    }

    void MultiChildTabs::RemoveChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            m_Childs.erase(it);
        }
    }

    std::function<void()> MultiChildTabs::GetChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void MultiChildTabs::Render()
    {
        static float leftPaneWidth = 300.0f;
        const float minPaneWidth = 50.0f;
        const float splitterWidth = 1.5f;
        static int selected;

        TitleThree("Uikit Components");

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("TabBar", tab_bar_flags))
        {
            for (const auto &child : m_Childs)
            {
                if (ImGui::BeginTabItem(child.first.c_str()))
                {
                    if (child.second)
                    {
                        child.second();
                    }
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }
}