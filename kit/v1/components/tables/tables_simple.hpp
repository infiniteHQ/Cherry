#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_TABLES_SIMPLES
#define UIKIT_V1_TABLES_SIMPLES

namespace UIKit
{
    static bool DrawArrowButton(const std::string &id, bool isExpanded)
    {
        ImVec2 oldPadding = ImGui::GetStyle().FramePadding;
        float oldRounding = ImGui::GetStyle().FrameRounding;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.7f, 0.7f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.7f); 

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0)); 
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));       

        bool clicked = ImGui::ArrowButton(id.c_str(), isExpanded ? ImGuiDir_Down : ImGuiDir_Right);

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        return clicked;
    }

    class CustomTree : public Component
    {
    public:
        struct CustomTreeNode
        {
            std::vector<std::function<void()>> ColumnCallbacks;
            std::vector<CustomTreeNode> ChildNodes;
            bool IsExpanded;

            CustomTreeNode(std::vector<std::function<void()>> callbacks, std::vector<CustomTreeNode> children = {}, bool expanded = false)
                : ColumnCallbacks(callbacks), ChildNodes(children), IsExpanded(expanded) {}
        };

        CustomTree(const std::string &id, const std::vector<CustomTreeNode> &nodes, int columnCount, const std::string &label = "Button")
            : Component(id),
              m_Label(label),
              m_Nodes(nodes),
              m_ColumnCount(columnCount)
        {
            m_ID = id;
        }

        void Render()
        {
            static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

            if (ImGui::BeginTable("treeTable", m_ColumnCount, flags))
            {
                for (int i = 0; i < m_ColumnCount; ++i)
                {
                    ImGui::TableSetupColumn(("Column " + std::to_string(i + 1)).c_str(), ImGuiTableColumnFlags_WidthStretch);
                }
                ImGui::TableHeadersRow();

                for (auto &node : m_Nodes)
                {
                    RenderNode(node);
                }

                ImGui::EndTable();
            }
        }

    private:
        std::string m_Label;
        std::vector<CustomTreeNode> m_Nodes;
        int m_ColumnCount;

        void RenderNode(CustomTreeNode &node)
        {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            bool nodeIsExpanded = node.IsExpanded;

            if (!node.ChildNodes.empty())
            {
                if (DrawArrowButton("##arrow" + std::to_string(reinterpret_cast<std::uintptr_t>(&node)), nodeIsExpanded))
                {
                    nodeIsExpanded = !nodeIsExpanded;
                    node.IsExpanded = nodeIsExpanded;
                }

                ImGui::SameLine();
            }

            if (!node.ColumnCallbacks.empty())
            {
                node.ColumnCallbacks[0]();
            }

            for (int i = 1; i < m_ColumnCount; ++i)
            {
                ImGui::TableNextColumn();
                if (i < node.ColumnCallbacks.size())
                {
                    node.ColumnCallbacks[i]();
                }
            }

            if (nodeIsExpanded)
            {
                ImGui::Indent(20.0f);

                for (auto &child : node.ChildNodes)
                {
                    RenderNode(child);
                }

                ImGui::Unindent(20.0f);
            }
        }
    };

}

#endif // UIKIT_V1_TABLES_SIMPLES
