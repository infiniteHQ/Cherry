#include "./data_logs_viewer.hpp"

namespace UIKit
{
    DataLogsViewer::DataLogsViewer(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    DataLogsViewer::DataLogsViewer()
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

    DataLogsViewer::DataLogsViewer(const std::string &name, const std::string &path)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_FilePath = path;
        LoadFileContent(m_FilePath);

        m_AppWindow->SetInternalPaddingX(1.0f);
        m_AppWindow->SetInternalPaddingY(0.0f);

        m_AppWindow->SetLeftMenubarCallback([this]() {});

        cp_TextEditor = std::make_shared<TextEditor>(m_FilePath);
        cp_TextEditor->SetShowWhitespaces(false);

        m_AppWindow->SetRightMenubarCallback([win]()
                                             {
                                                 ImGui::SetRenderCallback(ImGuiRenderCallback("ButtonEx::Test", []()
                                                                                              { ImGui::Text("This is working !"); }));
                                                 ImGui::Button("Add");
                                                 ImGui::Button("Settings");
                                                 // Add Folder
                                                 // Settings
                                             });

    }

    void DataLogsViewer::RefreshRender(const std::shared_ptr<DataLogsViewer> &instance)
    {
        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                           ImFont *font = Application::GetFontList()["Consola"];

                                           if (font)
                                           {
                                               ImGui::PushFont(font);
                                           }
                                           instance->cp_TextEditor->Render("Title");

                                           if (font)
                                           {
                                               ImGui::PopFont();
                                           }
                                       });
    }

}
