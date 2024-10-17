#include "./props_editor_simple.hpp"

namespace Cherry
{
    PropsEditorSimple::PropsEditorSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]()
                                            { ImGui::Text("ll"); });

        m_AppWindow->SetRightMenubarCallback([win]() {});
        Application::Get().PutWindow(m_AppWindow);
    }

    PropsEditorSimple::PropsEditorSimple()
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

    PropsEditorSimple::PropsEditorSimple(const std::string &name)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        m_AppWindow->SetSaveMode(true);
        // m_AppWindow->SetDisableContextMenu(true);
        // m_AppWindow->SetDisableDragging(true);

        v_Name = std::make_shared<std::string>("Mon super nom");
        cp_NameInput = Application::Get().CreateComponent<DoubleKeyValString>("keyvaldouble_1", v_Name, "Simple string value");

        v_Description = std::make_shared<std::string>("Ceci est une super description.");
        cp_DescriptionInput = Application::Get().CreateComponent<DoubleKeyValString>("keyvaldouble_1", v_Description, "Simple string value");

        v_SearchValue = std::make_shared<std::string>("");
        cp_SearchInput = Application::Get().CreateComponent<ImageStringInput>("keyvaldouble_2", v_SearchValue, "Simple string value");
        cp_SearchInput->SetImagePath(Application::CookPath("ressources/imgs/icons/misc/icon_magnifying_glass.png"));

        cp_SimpleTable = Application::Get().CreateComponent<SimpleTable>("simpletable_2", "KeyvA", std::vector<std::string>{"Key", "Value"});
        cp_SimpleTable->SetHeaderCellPaddingY(12.0f);
        cp_SimpleTable->SetHeaderCellPaddingX(10.0f);
        cp_SimpleTable->SetRowsCellPaddingY(100.0f);

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetInternalPaddingX(1.0f);
        m_AppWindow->SetInternalPaddingY(0.0f);

        m_AppWindow->SetLeftMenubarCallback([this]()
                                            { cp_SearchInput->Render("Normal"); });

        m_AppWindow->SetRightMenubarCallback([win]()
                                             {
                                                 ImGui::Button("Add");
                                                 ImGui::Button("Settings");
                                                 // Add Folder
                                                 // Settings
                                             });
    }

    std::shared_ptr<PropsEditorSimple> PropsEditorSimple::Create(const std::string &name)
    {
        auto instance = std::shared_ptr<PropsEditorSimple>(new PropsEditorSimple(name));
        instance->SetupRenderCallback();
        return instance;
    }

    void PropsEditorSimple::SetupRenderCallback()
    {
        auto self = shared_from_this();
        m_AppWindow->SetRenderCallback([self]()
                                       {
            if (self) {
                self->Render();
            } });
    }

    std::shared_ptr<Cherry::AppWindow> &PropsEditorSimple::GetAppWindow()
    {
        return m_AppWindow;
    }

    void PropsEditorSimple::Render()
    {

        std::vector<SimpleTable::SimpleTableRow> keyvals;

        keyvals.push_back(SimpleTable::SimpleTableRow({[this]()
                                                       { cp_NameInput->Render(0); },
                                                       [this]()
                                                       { cp_NameInput->Render(1); }}));

        keyvals.push_back(SimpleTable::SimpleTableRow({[this]()
                                                       { cp_DescriptionInput->Render(0); },
                                                       [this]()
                                                       { cp_DescriptionInput->Render(1); }}));

        cp_SimpleTable->Render(keyvals);
    }

}
