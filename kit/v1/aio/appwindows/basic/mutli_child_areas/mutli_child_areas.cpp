#include "./mutli_child_areas.hpp"

namespace UIKit
{
    MultiChildAreas::MultiChildAreas(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetParent(parent);
        m_AppWindow->SetRenderCallback([this]() {

        });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]() {});

        m_AppWindow->SetRightMenubarCallback([win]() {});
    }

    MultiChildAreas::MultiChildAreas(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking, "right");

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;
    }

    void MultiChildAreas::AddChild(const Child &child)
    {
        m_Childs.push_back(child);
    }

    void MultiChildAreas::RemoveChild(const std::string &child_name)
    {
    }

    std::function<void()> MultiChildAreas::GetChild(const std::string &child_name)
    {
    }

    void MultiChildAreas::RefreshRender(const std::shared_ptr<MultiChildAreas> &instance)
    {
        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                           if (instance->m_IsHorizontal)
                                           {
const float splitterHeight = 2.5f;
const float margin = 10.0f;

auto &children = instance->m_Childs;
static float lastTotalHeight = 0.0f; 

ImVec2 availableSize = ImGui::GetContentRegionAvail();
float totalAvailableSize = availableSize.y - (children.size() - 1) * splitterHeight;

if (totalAvailableSize != lastTotalHeight)
{
    float totalSizeAssigned = 0.0f; 

    for (auto &child : children)
    {
        if (!child.m_Initialized || totalAvailableSize != lastTotalHeight)
        {
            if (child.m_DefaultSize > 0.0f)
            {
                child.m_Size = child.m_DefaultSize;
            }
            else
            {
                child.m_Size = totalAvailableSize / children.size();
            }
            child.m_Initialized = true;
        }
        totalSizeAssigned += child.m_Size;
    }

    if (totalSizeAssigned < totalAvailableSize)
    {
        float remainingSize = totalAvailableSize - totalSizeAssigned;
        for (auto &child : children)
        {
            child.m_Size += remainingSize / children.size();
        }
    }
    
    lastTotalHeight = totalAvailableSize;
}

for (size_t i = 0; i < children.size(); ++i)
{
    auto &child = children[i];

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    std::string childname = child.m_Name + "##childvh" + instance->m_AppWindow->m_Name;
    ImGui::BeginChild(childname.c_str(), ImVec2(availableSize.x, child.m_Size), true);

    child.m_Child();

    ImGui::EndChild();
    ImGui::PopStyleColor(2);

    if (i + 1 < children.size() && !child.m_ResizeDisabled)
    {
        auto &next_child = children[i + 1];
        ImGui::Dummy(ImVec2(0.0f, splitterHeight));

        std::string lab = "##splitter" + child.m_Name + instance->m_AppWindow->m_Name;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::Button(lab.c_str(), ImVec2(-1, splitterHeight));
        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
        }

        if (ImGui::IsItemActive())
        {
            float delta = ImGui::GetIO().MouseDelta.y;

            if (child.m_Size + delta < child.m_MinSize + margin)
            {
                delta = child.m_MinSize + margin - child.m_Size;
            }
            if (next_child.m_Size - delta < next_child.m_MinSize + margin)
            {
                delta = next_child.m_Size - next_child.m_MinSize - margin;
            }

            if (child.m_Size + delta >= child.m_MinSize + margin && next_child.m_Size - delta >= next_child.m_MinSize + margin)
            {
                child.m_Size += delta;
                next_child.m_Size -= delta;
                lastTotalHeight = totalAvailableSize;
            }
        }

        ImGui::Dummy(ImVec2(0.0f, splitterHeight));
    }
}
                                           }
                                           else
                                           {
const float splitterWidth = 2.5f;

const float margin = 10.0f;

auto &children = instance->m_Childs;
static float lastTotalWidth = 0.0f;

ImVec2 availableSize = ImGui::GetContentRegionAvail();
float totalAvailableSize = availableSize.x - (children.size() - 1) * splitterWidth - 40.0f;

float usedSize = 0.0f;
int childrenWithoutDefaultSize = 0;

if (totalAvailableSize != lastTotalWidth && lastTotalWidth > 0.0f)
{
    float scale = totalAvailableSize / lastTotalWidth;

    for (auto &child : children)
    {
        child.m_Size *= scale;
    }

    lastTotalWidth = totalAvailableSize;
}

if (lastTotalWidth == 0.0f)
{
    float totalSizeAssigned = 0.0f;

    for (auto &child : children)
    {
        if (!child.m_Initialized || totalAvailableSize != lastTotalWidth)
        {
            if (child.m_DefaultSize > 0.0f)
            {
                child.m_Size = child.m_DefaultSize;
            }
            else
            {
                child.m_Size = totalAvailableSize / children.size();
            }
            child.m_Initialized = true;
        }
        totalSizeAssigned += child.m_Size;
    }

    if (totalSizeAssigned < totalAvailableSize)
    {
        float remainingSize = totalAvailableSize - totalSizeAssigned;
        for (auto &child : children)
        {
            child.m_Size += remainingSize / children.size();
        }
    }

    lastTotalWidth = totalAvailableSize;
}

for (size_t i = 0; i < children.size(); ++i)
{
    auto &child = children[i];

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));  
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    std::string childname = child.m_Name + "##childnh" + instance->m_AppWindow->m_Name;
    ImGui::BeginChild(childname.c_str(), ImVec2(child.m_Size, availableSize.y), true);

    child.m_Child();

    ImGui::EndChild();
    ImGui::PopStyleColor(2);

    if (i + 1 < children.size())
    {
        auto &next_child = children[i + 1];
        ImGui::SameLine();

        std::string lab = "##splitter" + child.m_Name + instance->m_AppWindow->m_Name;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::Button(lab.c_str(), ImVec2(splitterWidth, -1));
        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        }

        if (ImGui::IsItemActive())
        {
            float delta = ImGui::GetIO().MouseDelta.x;

            if (child.m_Size + delta < child.m_MinSize + margin)
            {
                delta = child.m_MinSize + margin - child.m_Size;
            }
            if (next_child.m_Size - delta < next_child.m_MinSize + margin)
            {
                delta = next_child.m_Size - next_child.m_MinSize - margin;
            }

            if (child.m_Size + delta >= child.m_MinSize + margin && next_child.m_Size - delta >= next_child.m_MinSize + margin)
            {
                child.m_Size += delta;
                next_child.m_Size -= delta;
                lastTotalWidth = totalAvailableSize;
            }
        }

        ImGui::SameLine();
    }
}







                                       } });
    }

}
