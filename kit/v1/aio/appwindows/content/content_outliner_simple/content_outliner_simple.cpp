#include "./content_outliner_simple.hpp"

namespace Cherry
{
    ContentOutlinerSimple::ContentOutlinerSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

        cp_ButtonOne = Application::Get().CreateComponent<CustomButtonSimple>("button_1");

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    ContentOutlinerSimple::ContentOutlinerSimple()
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

    ContentOutlinerSimple::ContentOutlinerSimple(const std::string &name)
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
    }

    std::shared_ptr<Cherry::AppWindow> &ContentOutlinerSimple::GetAppWindow()
    {
        return m_AppWindow;
    }

    std::shared_ptr<ContentOutlinerSimple> ContentOutlinerSimple::Create(const std::string &name)
    {
        auto instance = std::shared_ptr<ContentOutlinerSimple>(new ContentOutlinerSimple(name));
        instance->SetupRenderCallback();
        return instance;
    }

    void ContentOutlinerSimple::SetupRenderCallback()
    {
        auto self = shared_from_this();
        m_AppWindow->SetRenderCallback([self]()
                                       {
            if (self) {
                self->Render();
            } });
    }

    void ContentOutlinerSimple::Render()
    {
        cp_Tree->Render();
    }

}
