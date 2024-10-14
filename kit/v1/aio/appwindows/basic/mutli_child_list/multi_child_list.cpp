#include "./multi_child_list.hpp"

namespace Cherry
{
    MultiChildList::MultiChildList(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetParent(parent);
        m_AppWindow->SetRenderCallback([this]() {

        });

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]() {});

        m_AppWindow->SetRightMenubarCallback([win]() {});
    }

    MultiChildList::MultiChildList(const std::string &name)
    {
        m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");

        this->AddChild("Color Pickers", [this]() {

        });

        m_SelectedChildName = "TEst1";

        std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
        m_AppWindow->SetLeftMenubarCallback([]() {});
        m_AppWindow->SetRightMenubarCallback([win]() {});
    }

    std::shared_ptr<Cherry::AppWindow> &MultiChildList::GetAppWindow()
    {
        return m_AppWindow;
    }

    std::shared_ptr<MultiChildList> MultiChildList::Create(const std::string &name)
    {
        auto instance = std::shared_ptr<MultiChildList>(new MultiChildList(name));
        instance->SetupRenderCallback();
        return instance;
    }

    void MultiChildList::SetupRenderCallback()
    {
        auto self = shared_from_this();
        m_AppWindow->SetRenderCallback([self]()
                                       {
            if (self) {
                self->Render();
            } });
    }

    void MultiChildList::AddChild(const std::string &child_name, const std::function<void()> &child)
    {
        m_Childs[child_name] = child;
    }

    void MultiChildList::RemoveChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            m_Childs.erase(it);
        }
    }

    std::function<void()> MultiChildList::GetChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void MultiChildList::Render()
    {
        static float leftPaneWidth = 300.0f;
        const float minPaneWidth = 50.0f;
        const float splitterWidth = 1.5f;
        static int selected;

        ImGui::BeginChild("left pane", ImVec2(leftPaneWidth, 0), true, ImGuiWindowFlags_NoBackground);

        TitleThree("Uikit Components");

        for (const auto &child : m_Childs)
        {
            if (child.first == m_SelectedChildName)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            }

            if (TextButtonUnderline(child.first.c_str()))
            {
                m_SelectedChildName = child.first;
            }

            ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA("#44444466"));
        ImGui::Button("splitter", ImVec2(splitterWidth, -1));
        ImGui::PopStyleVar();

        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        }

        if (ImGui::IsItemActive())
        {
            float delta = ImGui::GetIO().MouseDelta.x;
            leftPaneWidth += delta;
            if (leftPaneWidth < minPaneWidth)
                leftPaneWidth = minPaneWidth;
        }

        ImGui::SameLine();
        ImGui::BeginGroup();

        if (!m_SelectedChildName.empty())
        {
            std::function<void()> pannel_render = GetChild(m_SelectedChildName);
            if (pannel_render)
            {
                pannel_render();
            }
        }

        ImGui::EndGroup();
    }

}
