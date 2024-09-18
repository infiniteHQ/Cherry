#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CUSTOM_LISTS
#define UIKIT_V1_CUSTOM_LISTS

namespace UIKit
{
    struct SimpleTitleTreeNode
    {
        std::vector<std::function<void()>> m_ColumnCallbacks;
        std::vector<SimpleTitleTreeNode> m_Children;
        std::string m_Name;

        void AttachChild(std::vector<UIKit::SimpleTitleTreeNode> child)
        {
            m_Children.push_back(child[0]);
        }

        SimpleTitleTreeNode(const std::string &name, std::vector<std::function<void()>> callbacks)
            : m_ColumnCallbacks(callbacks),
              m_Name(name) {}
    };

    class CustomListTree : public Component
    {
    public:
        CustomListTree(const std::string &id, const std::vector<SimpleTitleTreeNode> &nodes, int columnCount, const std::string &label = "Button")
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

            const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
            const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

            if (ImGui::BeginTable("customTable", m_ColumnCount + 1, flags)) // +1 : Add the title column. (n content columns + 1 title column)
            {
                ImGui::TableSetupColumn("Title ", ImGuiTableColumnFlags_WidthStretch);

                for (int i = 0; i < m_ColumnCount; ++i)
                {
                    ImGui::TableSetupColumn(("Column " + std::to_string(i + 1)).c_str(), ImGuiTableColumnFlags_WidthStretch);
                }
                ImGui::TableHeadersRow();

                for (const auto &node : m_Nodes)
                {
                    RenderNode(node, true);
                }

                ImGui::EndTable();
            }
        }

    private:
        std::string m_Label;
        std::vector<SimpleTitleTreeNode> m_Nodes;
        int m_ColumnCount;

        void RenderNode(const SimpleTitleTreeNode &node, bool isFirstColumn, int depth = 0)
        {
            ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow;
            bool isFolder = !node.m_Children.empty();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            depth = depth + 10.0f;

            ImGui::Indent(depth);

            bool isOpen = ImGui::TreeNodeEx(node.m_Name.c_str(), nodeFlags);
            ImGui::Unindent(depth);

            for (int i = 0; i < m_ColumnCount; ++i)
            {
                ImGui::TableNextColumn();
                if (i < node.m_ColumnCallbacks.size())
                {
                    node.m_ColumnCallbacks[i]();
                }
            }

            if (isOpen && isFolder)
            {
                for (const auto &child : node.m_Children)
                {
                    RenderNode(child, false, depth + 1);
                }
                ImGui::TreePop();
            }
        }
    };

}

#endif // UIKIT_V1_CUSTOM_LISTS
