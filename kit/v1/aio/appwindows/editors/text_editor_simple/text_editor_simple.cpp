#include "./text_editor_simple.hpp"

namespace UIKit
{
    TextEditorSimple::TextEditorSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

    TextEditorSimple::TextEditorSimple()
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

    TextEditorSimple::TextEditorSimple(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        m_AppWindow->SetSaveMode(true);
        // m_AppWindow->SetDisableContextMenu(true);
        // m_AppWindow->SetDisableDragging(true);
        cp_TextEditor = std::make_shared<TextEditor>("/home/diego/Bureau/main.cpp");

        v_StringOne = std::make_shared<std::string>("");

        // Components init
        cp_ButtonOne = Application::Get().CreateComponent<CustomButtonSimple>("button_1");
        cp_SearchBar = Application::Get().CreateComponent<SimpleStringInput>("qs", v_StringOne, "Simple string value");

        std::vector<SimpleTree::SimpleTreeNode> node2 = {
            SimpleTree::SimpleTreeNode("Node 2", {[this]()
                                                  { ImGui::Text("Content 1 --"); },
                                                  []()
                                                  { ImGui::Text("Content 2 --"); }})};

        std::vector<SimpleTree::SimpleTreeNode> node3 = {
            SimpleTree::SimpleTreeNode("Node 3", {[this]()
                                                  { ImGui::Text("Content 1 --"); },
                                                  []()
                                                  { ImGui::Text("Content 2 --"); }})};

        std::vector<SimpleTree::SimpleTreeNode> nodes = {
            SimpleTree::SimpleTreeNode("Node 1", {[this]()
                                                  { ImGui::Text("Content 1 --"); },
                                                  []()
                                                  { ImGui::Text("Content 2 --"); }})};

        node2[0].m_Children.push_back(node3[0]);
        nodes[0].m_Children.push_back(node2[0]);

        cp_Tree = Application::Get().CreateComponent<SimpleTree>("keyvaldouble_2", nodes, 2, "SuperKeyval");

        m_AppWindow->SetRenderCallback([this, name]()
                                       {
        ImGui::Text(name.c_str());
        ImGui::Button("qsd"); });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([this]()
                                            {
                                                ImGui::Button("F");
                                                cp_SearchBar->Render("Normal");
                                                // Collapse Filter
                                                // Collapse search bar
                                            });

        m_AppWindow->SetRightMenubarCallback([win]()
                                             {
                                                 ImGui::Button("Add");
                                                 ImGui::Button("Settings");
                                                 // Add Folder
                                                 // Settings
                                             });

        m_AppWindow->SetRightBottombarCallback([win]()
                                               { ImGui::Button("7 element(s) selected"); });

        Application::Get().PutWindow(m_AppWindow);
    }

    void TextEditorSimple::RefreshRender(const std::shared_ptr<TextEditorSimple> &instance)
    {

        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                           static ImFont* font = Application::GetFontList()["Consola"];

                                            ImFont* old = ImGui::GetFont();
                                           if(font)
                                           {
                                            ImGui::PushFont(font);
                                           }

                                           instance->cp_TextEditor->Render("Title");

                                           if(font)
                                           {
                                            ImGui::PopFont();
                                           }
                                       });
    }

}
