#include "./docking_appwindow.hpp"

namespace Cherry
{
    DockingAppWindow::DockingAppWindow(const std::string &name)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDockingMode(true);
        m_AppWindow->SetRenderCallback([this]() {

        });

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        auto windodo_one = EmptyAppWindow::Create("sec1", win);
        Cherry::AddAppWindow(windodo_one->m_AppWindow);
        auto window_two = EmptyAppWindow::Create("sec2", win);
        Cherry::AddAppWindow(window_two->m_AppWindow);
        auto window_three = EmptyAppWindow::Create("sec3");
        Cherry::AddAppWindow(window_three->m_AppWindow);
        auto window_four = EmptyAppWindow::Create("sec4");
        Cherry::AddAppWindow(window_four->m_AppWindow);
    }

    std::shared_ptr<DockingAppWindow> DockingAppWindow::Create(const std::string &name)
    {
        auto instance = std::shared_ptr<DockingAppWindow>(new DockingAppWindow(name));
        instance->SetupRenderCallback();
        return instance;
    }
    
    void DockingAppWindow::Render()
    {
        ImGui::Text("Hello, World");
    }

    void DockingAppWindow::SetupRenderCallback()
    {
        auto self = shared_from_this();
        m_AppWindow->SetRenderCallback([self]()
                                       {
            if (self) {
                self->Render();
            } });
    }

    std::shared_ptr<Cherry::AppWindow> &DockingAppWindow::GetAppWindow()
    {
        return m_AppWindow;
    }
}
