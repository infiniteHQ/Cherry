#include "./text_editor_simple.hpp"

namespace Cherry
{
    TextEditorSimple::TextEditorSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

    TextEditorSimple::TextEditorSimple()
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

    TextEditorSimple::TextEditorSimple(const std::string &name)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        m_AppWindow->SetSaveMode(true);
        // m_AppWindow->SetDisableContextMenu(true);
        // m_AppWindow->SetDisableDragging(true);
        cp_TextEditor = std::make_shared<TextEditor>("/home/diego/Bureau/main.cpp");

        v_StringOne = std::make_shared<std::string>("");


        cp_SaveButton = Application::Get().CreateComponent<ImageTextButtonSimple>("save_button", Application::Get().GetLocale("loc.content_browser.save_all") + "####content_browser.save_all", Cherry::Application::CookPath("ressources/imgs/icons/misc/icon_save.png"));
        cp_SaveButton->SetScale(0.85f);
        cp_SaveButton->SetLogoSize(15, 15);
        cp_SaveButton->SetInternalMarginX(10.0f);
        


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
                                                
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));
            
        if(cp_SaveButton->Render())
        {

        }
		ImGui::PopStyleColor();


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
