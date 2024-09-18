#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CUSTOM_LISTS
#define UIKIT_V1_CUSTOM_LISTS

namespace UIKit
{

    struct TreeNode
    {
        std::string Name;
        std::string Type;
        int Size;
        std::vector<TreeNode> Children; // Dynamic list of child nodes

        TreeNode(const std::string& name, const std::string& type, int size)
            : Name(name), Type(type), Size(size) {}
    };


class CustomListTree : public Component
{
public:
    CustomListTree(const std::string &id, const std::vector<TreeNode>& nodes, const std::string &label = "Button")
        : Component(id),
          m_Label(label),
          m_Nodes(nodes)
    {
        m_ID = id;
    }

    void Render()
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

        if (ImGui::BeginTable("3ways", 3, flags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            ImGui::TableHeadersRow();

            // Start rendering from the root nodes
            for (const auto& node : m_Nodes)
            {
                RenderNode(node);
            }

            ImGui::EndTable();
        }
    }

private:
    std::string m_Label;
    std::vector<TreeNode> m_Nodes;

    // Recursive function to render the tree nodes
    void RenderNode(const TreeNode& node)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        const bool is_folder = !node.Children.empty();
        
        if (is_folder)
        {
            bool open = ImGui::TreeNodeEx(node.Name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::TableNextColumn();
            ImGui::TextDisabled("--"); // Size placeholder for folders
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(node.Type.c_str());

            if (open)
            {
                // Recursively render child nodes
                for (const auto& child : node.Children)
                {
                    RenderNode(child);
                }
                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::TreeNodeEx(node.Name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::TableNextColumn();
            ImGui::Text("%d", node.Size); // Display size for files
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(node.Type.c_str());
        }
    }
};


}

#endif // UIKIT_V1_CUSTOM_LISTS
