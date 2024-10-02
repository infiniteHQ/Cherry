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
                                       const float splitterSize = 2.5f;
                                       const float margin = 10.0f;

                                       auto &children = instance->m_Childs;
                                       const bool isHorizontal = instance->m_IsHorizontal;

                                       ImGuiWindow *window = ImGui::GetCurrentWindow();
                                       ImVec2 windowSize = window->ContentRegionRect.GetSize();

                                       float totalAvailableSize = isHorizontal ? windowSize.y : windowSize.x;
                                       totalAvailableSize -= (children.size() - 1) * splitterSize;

                                       float usedSize = 0.0f;
                                       int childrenWithoutDefaultSize = 0;

                                       for (auto &child : children)
                                       {
                                           if (child.m_DefaultSize > 0.0f)
                                           {
                                               if (child.m_Size == 0.0f)
                                               {
                                                   child.m_Size = child.m_DefaultSize;
                                               }
                                               usedSize += child.m_Size;
                                               child.m_Initialized = true;
                                           }
                                           else
                                           {
                                               childrenWithoutDefaultSize++;
                                           }
                                       }

                                       float remainingSize = totalAvailableSize - usedSize;

                                       if (remainingSize < 0)
                                       {
                                           float reductionFactor = totalAvailableSize / usedSize;
                                           for (auto &child : children)
                                           {
                                               if (child.m_DefaultSize > 0.0f && !child.m_InitializedSec)
                                               {
                                                   child.m_Size *= reductionFactor;
                                                   child.m_InitializedSec = true;
                                               }
                                           }
                                           remainingSize = 0;
                                       }

                                       float sizePerChild = (childrenWithoutDefaultSize > 0) ? (remainingSize / childrenWithoutDefaultSize) : 0.0f;
                                       for (auto &child : children)
                                       {
                                           if (child.m_DefaultSize == 0.0f && !child.m_InitializedTh)
                                           {
                                               child.m_Size = sizePerChild;
                                               child.m_InitializedTh = true;
                                           }
                                       }

                                       for (size_t i = 0; i < children.size(); ++i)
                                       {
                                           auto &child = children[i];

                                           ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                                           ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

                                           ImGui::BeginChild(child.m_Name.c_str(), 
                                               isHorizontal ? ImVec2(windowSize.x, child.m_Size) : ImVec2(child.m_Size, windowSize.y), 
                                               true);

                                           child.m_Child();

                                           ImGui::EndChild();
                                           ImGui::PopStyleColor(2);

                                           if (i + 1 < children.size())
                                           {
                                               auto &next_child = children[i + 1];
                                               
                                               if (child.m_ResizeDisabled)
                                               {
                                                   continue;
                                               }

                                               if (!isHorizontal) {
                                                   ImGui::SameLine();
                                               } else {
                                                   ImGui::Dummy(ImVec2(0.0f, splitterSize));
                                               }

                                               std::string lab = "##splitter" + child.m_Name;

                                               ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                                               ImGui::Button(lab.c_str(), isHorizontal ? ImVec2(-1, splitterSize) : ImVec2(splitterSize, -1));
                                               ImGui::PopStyleColor();

                                               if (ImGui::IsItemHovered())
                                               {
                                                   ImGui::SetMouseCursor(isHorizontal ? ImGuiMouseCursor_ResizeNS : ImGuiMouseCursor_ResizeEW);
                                               }

                                               if (ImGui::IsItemActive())
                                               {
                                                   float delta = isHorizontal ? ImGui::GetIO().MouseDelta.y : ImGui::GetIO().MouseDelta.x;

                                                   child.m_Size += delta;
                                                   next_child.m_Size -= delta;

                                                   if (child.m_Size < child.m_MinSize + margin)
                                                   {
                                                       float diff = (child.m_MinSize + margin) - child.m_Size;
                                                       child.m_Size = child.m_MinSize + margin;
                                                       next_child.m_Size -= diff;
                                                   }

                                                   if (next_child.m_Size < next_child.m_MinSize + margin)
                                                   {
                                                       float diff = (next_child.m_MinSize + margin) - next_child.m_Size;
                                                       next_child.m_Size = next_child.m_MinSize + margin;
                                                       child.m_Size -= diff;
                                                   }
                                               }

                                               if (isHorizontal)
                                               {
                                                   ImGui::Dummy(ImVec2(0.0f, splitterSize));
                                               }
                                           }
                                       } });
    }

}
