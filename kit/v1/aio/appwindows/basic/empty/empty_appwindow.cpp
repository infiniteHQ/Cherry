#include "./empty_appwindow.hpp"

namespace Cherry
{

    EmptyAppWindow::EmptyAppWindow(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "full");

        m_AppWindow->SetParent(parent);
        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetRenderCallback([this, name]()
                                       {
        ImGui::Text(name.c_str());
        ImGui::Button("qsd"); });

        cp_ButtonOne = CustomButtonSimple::Create("button_one", "Test");

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    EmptyAppWindow::EmptyAppWindow()
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>("Sec", "Sec");
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetRenderCallback([this]()
                                       {
        ImGui::Text("name");
        ImGui::Button("qsd"); });

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    EmptyAppWindow::EmptyAppWindow(const std::string &name)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        m_AppWindow->SetSaveMode(true);
        // m_AppWindow->SetDisableContextMenu(true);
        // m_AppWindow->SetDisableDragging(true);

        cp_ButtonOne = CustomButtonSimple::Create("button_one", "Test");
        m_AppWindow->SetRenderCallback([this, name]()
                                       {
        ImGui::Text(name.c_str());
        ImGui::Button("qsd"); });

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});

        Application::Get().PutWindow(m_AppWindow);

        /* ApplicationSpecification spec;
         spec.Name = name;
         spec.MinHeight = 50;
         spec.MinWidth = 50;
         spec.Height = 300;
         spec.Width = 500;
         spec.CustomTitlebar = true;
         spec.DisableWindowManagerTitleBar = true;
         spec.EnableDocking = true;
         spec.DisableTitle = true;
         spec.WindowSaves = true;
         spec.IconPath = "icon.png";

        // m_AppWindow->AttachOnNewWindow(spec);*/
    }

    std::shared_ptr<Cherry::AppWindow> &EmptyAppWindow::GetAppWindow()
    {
        return m_AppWindow;
    }

    std::shared_ptr<EmptyAppWindow> EmptyAppWindow::Create(const std::string &name)
    {
        auto instance = std::shared_ptr<EmptyAppWindow>(new EmptyAppWindow(name));
        instance->SetupRenderCallback();
        return instance;
    }
    std::shared_ptr<EmptyAppWindow> EmptyAppWindow::Create(const std::string &name, std::shared_ptr<Cherry::AppWindow> parent)
    {
        auto instance = std::shared_ptr<EmptyAppWindow>(new EmptyAppWindow(name, parent));
        instance->SetupRenderCallback();
        return instance;
    }

    void EmptyAppWindow::SetupRenderCallback()
    {
        auto self = shared_from_this();
        m_AppWindow->SetRenderCallback([self]()
                                       {
            if (self) {
                self->Render();
            } });
    }

    void EmptyAppWindow::Render()
    {
        if (cp_ButtonOne->Render("Save"))
        {
            m_AppWindow->SetSaved(true);
        }

        if (cp_ButtonOne->Render("Unsave"))
        {
            m_AppWindow->SetSaved(false);
        }
    }

}
