#include "./content_outliner_simple.hpp"

namespace UIKit
{

    ContentOutlinerSimple::ContentOutlinerSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "full");

        m_AppWindow->SetParent(parent);
        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetRenderCallback([this, name]()
                                       {
        ImGui::Text(name.c_str());
        ImGui::Button("qsd"); });

        cp_ButtonOne = Application::Get().CreateComponent<CustomButtonSimple>("button_1");

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    ContentOutlinerSimple::ContentOutlinerSimple()
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>("Sec", "Sec");
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetRenderCallback([this]()
                                       {
        ImGui::Text("name");
        ImGui::Button("qsd"); });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    ContentOutlinerSimple::ContentOutlinerSimple(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        m_AppWindow->SetSaveMode(true);
        //m_AppWindow->SetDisableContextMenu(true);
        //m_AppWindow->SetDisableDragging(true);

        cp_ButtonOne = Application::Get().CreateComponent<CustomButtonSimple>("button_1");
        m_AppWindow->SetRenderCallback([this, name]()
                                       {
        ImGui::Text(name.c_str());
        ImGui::Button("qsd"); });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { 
                                                ImGui::Button("Upper Left");
                                                // Collapse Filter
                                                // Collapse search bar
                                             });

        m_AppWindow->SetRightMenubarCallback([win]() {
                                                ImGui::Button("Upper Right");
                                                // Add Folder
                                                // Settings
                                                });


        m_AppWindow->SetRightBottombarCallback([win]() {
                                                ImGui::Button("Down Right");
                                                });


        m_AppWindow->SetLeftBottombarCallback([win]() {
                                                ImGui::Button("Down Left");
                                                });


        Application::Get().PutWindow(m_AppWindow);
    }

    void ContentOutlinerSimple::RefreshRender(const std::shared_ptr<ContentOutlinerSimple> &instance)
    {

        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                        // Outliner
                                        
                                        });
    }

}
