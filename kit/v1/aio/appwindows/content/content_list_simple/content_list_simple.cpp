#include "./content_list_simple.hpp"

namespace Cherry
{
    ContentListSimple::ContentListSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

        cp_ButtonOne = CustomButtonSimple::Create("button_1", "Button");

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    ContentListSimple::ContentListSimple()
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

    ContentListSimple::ContentListSimple(const std::string &name)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        m_AppWindow->SetSaveMode(true);
        m_AppWindow->SetInternalPaddingX(1.0f);
        m_AppWindow->SetInternalPaddingY(1.0f);
        // m_AppWindow->SetDisableContextMenu(true);
        // m_AppWindow->SetDisableDragging(true);

        v_StringOne = std::make_shared<std::string>("");

        // Components init
        cp_ButtonOne = CustomButtonSimple::Create("button_1","Button");
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

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

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

    void ContentListSimple::RefreshRender(const std::shared_ptr<ContentListSimple> &instance)
    {

        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                           instance->cp_Tree->Render();
                                       });
    }

}
