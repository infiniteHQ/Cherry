#include "./empty_appwindow.hpp"

namespace UIKit
{

    EmptyAppWindow::EmptyAppWindow(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetParent(parent);
        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetRenderCallback([this]()
                                       {
        ImGui::Text("AA.BB.CC.DD.EE.FF 1");
        ImGui::Button("qsd"); });

        cp_ButtonOne = Application::Get().CreateComponent<CustomButtonSimple>("button_1");

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    EmptyAppWindow::EmptyAppWindow()
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>("Sec", "Sec");
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetRenderCallback([this]()
                                       {
        ImGui::Text("AA.BB.CC.DD.EE.FF 1");
        ImGui::Button("qsd"); });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    EmptyAppWindow::EmptyAppWindow(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetDisableContextMenu(true);
        m_AppWindow->SetDisableDragging(true);

        cp_ButtonOne = Application::Get().CreateComponent<CustomButtonSimple>("button_1");
        m_AppWindow->SetRenderCallback([this]()
                                       {
        ImGui::Text("AA.BB.CC.DD.EE.FF 1");
        ImGui::Button("qsd"); });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});

        Application::Get().PutWindow(m_AppWindow);

        ApplicationSpecification spec;
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

        m_AppWindow->AttachOnNewWindow(spec);
    }

    void EmptyAppWindow::RefreshRender(const std::shared_ptr<EmptyAppWindow> &instance)
    {

        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                        if(instance->cp_ButtonOne->Render("Save"))
                                        {
                                            instance->m_AppWindow->SetSaved(true);
                                        }

                                        if(instance->cp_ButtonOne->Render("Unsave"))
                                        {
                                            instance->m_AppWindow->SetSaved(false);
                                        } });
    }

}
