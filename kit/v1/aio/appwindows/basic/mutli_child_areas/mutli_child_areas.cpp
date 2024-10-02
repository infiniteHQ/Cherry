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

        m_SelectedChildName = "TEst1";

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;
        m_AppWindow->SetLeftMenubarCallback([]() {});
        m_AppWindow->SetRightMenubarCallback([win]() {});

        Application::Get().PutWindow(m_AppWindow);
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
        auto& children = instance->m_Childs;
        const bool isHorizontal = instance->m_IsHorizontal;

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImVec2 windowSize = window->ContentRegionRect.GetSize();
        float totalSize = isHorizontal ? windowSize.x : windowSize.y;

        float currentPos = 0.0f;

        for (size_t i = 0; i < children.size(); ++i)
        {
            auto& child = children[i];
            float& childSize = child.m_DefaultSize; 
            
            if (childSize == 0.0f || child.m_Resizable)
            {
                childSize = totalSize / children.size();
            }

            if (child.m_MinSize > 0.0f && childSize < child.m_MinSize)
            {
                childSize = child.m_MinSize;
            }
            if (child.m_MaxSize > 0.0f && childSize > child.m_MaxSize)
            {
                childSize = child.m_MaxSize;
            }

            ImVec2 itemSize = isHorizontal ? ImVec2(childSize, windowSize.y) : ImVec2(windowSize.x, childSize);
            ImGui::SetCursorPos(isHorizontal ? ImVec2(currentPos, 0) : ImVec2(0, currentPos));

            ImGui::BeginChild(child.m_Name.c_str(), itemSize, true);
            child.m_Child();
            ImGui::EndChild();

            currentPos += isHorizontal ? itemSize.x : itemSize.y;

            if (i + 1 < children.size())
            {
                auto& nextChild = children[i + 1];
                ImVec2 splitterSize = isHorizontal ? ImVec2(5.0f, windowSize.y) : ImVec2(windowSize.x, 5.0f);
                ImVec2 splitterPos = isHorizontal ? ImVec2(currentPos, 0) : ImVec2(0, currentPos);

                ImGui::SetCursorPos(splitterPos);

                bool canResize = child.m_Resizable && nextChild.m_Resizable;
                ImGui::PushStyleColor(ImGuiCol_Button, canResize ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 165, 0, 255));
                ImGui::Button("##splitter", splitterSize);
                ImGui::PopStyleColor();

                if (ImGui::IsItemHovered() && canResize)
                {
                    ImGui::SetMouseCursor(isHorizontal ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS);
                }

                if (ImGui::IsItemActive() && canResize)
                {
                    float delta = isHorizontal ? ImGui::GetIO().MouseDelta.x : ImGui::GetIO().MouseDelta.y;

                    childSize += delta;
                    nextChild.m_DefaultSize -= delta;

                    if (child.m_MinSize > 0.0f && childSize < child.m_MinSize)
                    {
                        childSize = child.m_MinSize;
                    }
                    if (child.m_MaxSize > 0.0f && childSize > child.m_MaxSize)
                    {
                        childSize = child.m_MaxSize;
                    }

                    if (nextChild.m_MinSize > 0.0f && nextChild.m_DefaultSize < nextChild.m_MinSize)
                    {
                        nextChild.m_DefaultSize = nextChild.m_MinSize;
                    }
                    if (nextChild.m_MaxSize > 0.0f && nextChild.m_DefaultSize > nextChild.m_MaxSize)
                    {
                        nextChild.m_DefaultSize = nextChild.m_MaxSize;
                    }
                }

                currentPos += isHorizontal ? splitterSize.x : splitterSize.y;
            }
        } });
    }

}
