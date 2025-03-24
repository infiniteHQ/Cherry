#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS

#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

#include "../../../../lib/imgui/imgui.h"
#include "../../../../lib/imgui/imgui_internal.h"
#include "../../../../lib/imgui/misc/nodes/imgui_node_editor.h"
#include "../../../../lib/imgui/misc/nodes/imgui_node_editor_internal.h"
#include "../../../../platform/engine/ui/nodes/utils/widgets.h"
#include "../../../../platform/engine/ui/nodes/utils/builders.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

//
// NodeArea
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_NODE_AREA
#define CHERRY_KIT_NODE_AREA

// Move into the vortex render engine ?
namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

static inline ImRect ImGui_GetItemRect()
{
    return ImRect(CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax());
}

static inline ImRect ImRect_Expanded(const ImRect &rect, float x, float y)
{
    auto result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
}

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

// static ed::EditorContext* m_Editor = nullptr;

// extern "C" __declspec(dllimport) short __stdcall GetAsyncKeyState(int vkey);
// extern "C" bool Debug_KeyPress(int vkey)
//{
//     static std::map<int, bool> state;
//     auto lastState = state[vkey];
//     state[vkey] = (GetAsyncKeyState(vkey) & 0x8000) != 0;
//     if (state[vkey] && !lastState)
//         return true;
//     else
//         return false;
// }

enum class PinType
{
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
};

enum class PinKind
{
    Output,
    Input
};

enum class NodeType
{
    Blueprint,
    Simple,
    Tree,
    Comment,
    Houdini
};

struct Node;

struct Pin
{
    ed::PinId ID;
    ::Node *Node;
    std::string Name;
    PinType Type;
    PinKind Kind;

    Pin(int id, const char *name, PinType type) : ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
    {
    }
};

struct Node
{
    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;
    NodeType Type;
    ImVec2 Size;

    std::string State;
    std::string SavedState;

    Node(int id, const char *name, ImColor color = ImColor(255, 255, 255)) : ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
    {
    }
};

struct Link
{
    ed::LinkId ID;

    ed::PinId StartPinID;
    ed::PinId EndPinID;

    ImColor Color;

    Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) : ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
    {
    }
};

struct NodeIdLess
{
    bool operator()(const ed::NodeId &lhs, const ed::NodeId &rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};

static bool Splitter(bool split_vertically, float thickness, float *size1, float *size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
    using namespace ImGui;
    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = g.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

struct Example
{
    ed::EditorContext *m_Editor = nullptr;

    int GetNextId()
    {
        return m_NextId++;
    }

    // ed::NodeId GetNextNodeId()
    //{
    //     return ed::NodeId(GetNextId());
    // }

    ed::LinkId GetNextLinkId()
    {
        return ed::LinkId(GetNextId());
    }

    void TouchNode(ed::NodeId id)
    {
        m_NodeTouchTime[id] = m_TouchTime;
    }

    float GetTouchProgress(ed::NodeId id)
    {
        auto it = m_NodeTouchTime.find(id);
        if (it != m_NodeTouchTime.end() && it->second > 0.0f)
            return (m_TouchTime - it->second) / m_TouchTime;
        else
            return 0.0f;
    }

    void UpdateTouch()
    {
        const auto deltaTime = CherryGUI::GetIO().DeltaTime;
        for (auto &entry : m_NodeTouchTime)
        {
            if (entry.second > 0.0f)
                entry.second -= deltaTime;
        }
    }

    Node *FindNode(ed::NodeId id)
    {
        for (auto &node : m_Nodes)
            if (node.ID == id)
                return &node;

        return nullptr;
    }

    Link *FindLink(ed::LinkId id)
    {
        for (auto &link : m_Links)
            if (link.ID == id)
                return &link;

        return nullptr;
    }

    Pin *FindPin(ed::PinId id)
    {
        if (!id)
            return nullptr;

        for (auto &node : m_Nodes)
        {
            for (auto &pin : node.Inputs)
                if (pin.ID == id)
                    return &pin;

            for (auto &pin : node.Outputs)
                if (pin.ID == id)
                    return &pin;
        }

        return nullptr;
    }

    bool IsPinLinked(ed::PinId id)
    {
        if (!id)
            return false;

        for (auto &link : m_Links)
            if (link.StartPinID == id || link.EndPinID == id)
                return true;

        return false;
    }

    bool CanCreateLink(Pin *a, Pin *b)
    {
        if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
            return false;

        return true;
    }

    // void DrawItemRect(ImColor color, float expand = 0.0f)
    //{
    //     CherryGUI::GetWindowDrawList()->AddRect(
    //         CherryGUI::GetItemRectMin() - ImVec2(expand, expand),
    //         CherryGUI::GetItemRectMax() + ImVec2(expand, expand),
    //         color);
    // };

    // void FillItemRect(ImColor color, float expand = 0.0f, float rounding = 0.0f)
    //{
    //     CherryGUI::GetWindowDrawList()->AddRectFilled(
    //         CherryGUI::GetItemRectMin() - ImVec2(expand, expand),
    //         CherryGUI::GetItemRectMax() + ImVec2(expand, expand),
    //         color, rounding);
    // };

    void BuildNode(Node *node)
    {
        for (auto &input : node->Inputs)
        {
            input.Node = node;
            input.Kind = PinKind::Input;
        }

        for (auto &output : node->Outputs)
        {
            output.Node = node;
            output.Kind = PinKind::Output;
        }
    }

    Node *SpawnInputActionNode()
    {
        m_Nodes.emplace_back(GetNextId(), "InputAction Fire", ImColor(255, 128, 128));
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Delegate);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Pressed", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Released", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnBranchNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Branch");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "True", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "False", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnDoNNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Do N");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Enter", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "N", PinType::Int);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Reset", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Exit", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Counter", PinType::Int);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnOutputActionNode()
    {
        m_Nodes.emplace_back(GetNextId(), "OutputAction");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Sample", PinType::Float);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Event", PinType::Delegate);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnPrintStringNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Print String");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "In String", PinType::String);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnMessageNode()
    {
        m_Nodes.emplace_back(GetNextId(), "", ImColor(128, 195, 248));
        m_Nodes.back().Type = NodeType::Simple;
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Message", PinType::String);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnSetTimerNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Set Timer", ImColor(128, 195, 248));
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Object", PinType::Object);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Function Name", PinType::Function);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Time", PinType::Float);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Looping", PinType::Bool);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnLessNode()
    {
        m_Nodes.emplace_back(GetNextId(), "<", ImColor(128, 195, 248));
        m_Nodes.back().Type = NodeType::Simple;
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Float);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Float);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Float);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnWeirdNode()
    {
        m_Nodes.emplace_back(GetNextId(), "o.O", ImColor(128, 195, 248));
        m_Nodes.back().Type = NodeType::Simple;
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Float);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Float);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Float);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnTraceByChannelNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Single Line Trace by Channel", ImColor(255, 128, 64));
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Start", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "End", PinType::Int);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Trace Channel", PinType::Float);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Trace Complex", PinType::Bool);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Actors to Ignore", PinType::Int);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Draw Debug Type", PinType::Bool);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Ignore Self", PinType::Bool);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Out Hit", PinType::Float);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Return Value", PinType::Bool);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnTreeSequenceNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Sequence");
        m_Nodes.back().Type = NodeType::Tree;
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnTreeTaskNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Move To");
        m_Nodes.back().Type = NodeType::Tree;
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnTreeTask2Node()
    {
        m_Nodes.emplace_back(GetNextId(), "Random Wait");
        m_Nodes.back().Type = NodeType::Tree;
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnComment()
    {
        m_Nodes.emplace_back(GetNextId(), "Test Comment");
        m_Nodes.back().Type = NodeType::Comment;
        m_Nodes.back().Size = ImVec2(300, 200);

        return &m_Nodes.back();
    }

    Node *SpawnHoudiniTransformNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Transform");
        m_Nodes.back().Type = NodeType::Houdini;
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node *SpawnHoudiniGroupNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Group");
        m_Nodes.back().Type = NodeType::Houdini;
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    void BuildNodes()
    {
        for (auto &node : m_Nodes)
            BuildNode(&node);
    }

    /* void OnStart() override
     {
         ed::Config config;

         config.SettingsFile = "Blueprints.json";

         config.UserPointer = this;

         config.LoadNodeSettings = [](ed::NodeId nodeId, char* data, void* userPointer) -> size_t
         {
             auto self = static_cast<Example*>(userPointer);

             auto node = self->FindNode(nodeId);
             if (!node)
                 return 0;

             if (data != nullptr)
                 memcpy(data, node->State.data(), node->State.size());
             return node->State.size();
         };

         config.SaveNodeSettings = [](ed::NodeId nodeId, const char* data, size_t size, ed::SaveReasonFlags reason, void* userPointer) -> bool
         {
             auto self = static_cast<Example*>(userPointer);

             auto node = self->FindNode(nodeId);
             if (!node)
                 return false;

             node->State.assign(data, size);

             self->TouchNode(nodeId);

             return true;
         };

         m_Editor = ed::CreateEditor(&config);
         ed::SetCurrentEditor(m_Editor);

         Node* node;
         node = SpawnInputActionNode();      ed::SetNodePosition(node->ID, ImVec2(-252, 220));
         node = SpawnBranchNode();           ed::SetNodePosition(node->ID, ImVec2(-300, 351));
         node = SpawnDoNNode();              ed::SetNodePosition(node->ID, ImVec2(-238, 504));
         node = SpawnOutputActionNode();     ed::SetNodePosition(node->ID, ImVec2(71, 80));
         node = SpawnSetTimerNode();         ed::SetNodePosition(node->ID, ImVec2(168, 316));

         node = SpawnTreeSequenceNode();     ed::SetNodePosition(node->ID, ImVec2(1028, 329));
         node = SpawnTreeTaskNode();         ed::SetNodePosition(node->ID, ImVec2(1204, 458));
         node = SpawnTreeTask2Node();        ed::SetNodePosition(node->ID, ImVec2(868, 538));

         node = SpawnComment();              ed::SetNodePosition(node->ID, ImVec2(112, 576)); ed::SetGroupSize(node->ID, ImVec2(384, 154));
         node = SpawnComment();              ed::SetNodePosition(node->ID, ImVec2(800, 224)); ed::SetGroupSize(node->ID, ImVec2(640, 400));

         node = SpawnLessNode();             ed::SetNodePosition(node->ID, ImVec2(366, 652));
         node = SpawnWeirdNode();            ed::SetNodePosition(node->ID, ImVec2(144, 652));
         node = SpawnMessageNode();          ed::SetNodePosition(node->ID, ImVec2(-348, 698));
         node = SpawnPrintStringNode();      ed::SetNodePosition(node->ID, ImVec2(-69, 652));

         node = SpawnHoudiniTransformNode(); ed::SetNodePosition(node->ID, ImVec2(500, -70));
         node = SpawnHoudiniGroupNode();     ed::SetNodePosition(node->ID, ImVec2(500, 42));

         ed::NavigateToContent();

         BuildNodes();

         m_Links.push_back(Link(GetNextLinkId(), m_Nodes[5].Outputs[0].ID, m_Nodes[6].Inputs[0].ID));
         m_Links.push_back(Link(GetNextLinkId(), m_Nodes[5].Outputs[0].ID, m_Nodes[7].Inputs[0].ID));

         m_Links.push_back(Link(GetNextLinkId(), m_Nodes[14].Outputs[0].ID, m_Nodes[15].Inputs[0].ID));

         m_HeaderBackground = LoadTexture("data/BlueprintBackground.png");
         m_SaveIcon         = LoadTexture("data/ic_save_white_24dp.png");
         m_RestoreIcon      = LoadTexture("data/ic_restore_white_24dp.png");


         //auto& io = CherryGUI::GetIO();
     }
 */
    /*void OnStop() override
    {
        auto releaseTexture = [this](ImTextureID& id)
        {
            if (id)
            {
                DestroyTexture(id);
                id = nullptr;
            }
        };

        releaseTexture(m_RestoreIcon);
        releaseTexture(m_SaveIcon);
        releaseTexture(m_HeaderBackground);

        if (m_Editor)
        {
            ed::DestroyEditor(m_Editor);
            m_Editor = nullptr;
        }
    }
*/
    ImColor GetIconColor(PinType type)
    {
        switch (type)
        {
        default:
        case PinType::Flow:
            return ImColor(255, 255, 255);
        case PinType::Bool:
            return ImColor(220, 48, 48);
        case PinType::Int:
            return ImColor(68, 201, 156);
        case PinType::Float:
            return ImColor(147, 226, 74);
        case PinType::String:
            return ImColor(124, 21, 153);
        case PinType::Object:
            return ImColor(51, 150, 215);
        case PinType::Function:
            return ImColor(218, 0, 183);
        case PinType::Delegate:
            return ImColor(255, 48, 48);
        }
    };

    void DrawPinIcon(const Pin &pin, bool connected, int alpha)
    {
        IconType iconType;
        ImColor color = GetIconColor(pin.Type);
        color.Value.w = alpha / 255.0f;
        switch (pin.Type)
        {
        case PinType::Flow:
            iconType = IconType::Flow;
            break;
        case PinType::Bool:
            iconType = IconType::Circle;
            break;
        case PinType::Int:
            iconType = IconType::Circle;
            break;
        case PinType::Float:
            iconType = IconType::Circle;
            break;
        case PinType::String:
            iconType = IconType::Circle;
            break;
        case PinType::Object:
            iconType = IconType::Circle;
            break;
        case PinType::Function:
            iconType = IconType::Circle;
            break;
        case PinType::Delegate:
            iconType = IconType::Square;
            break;
        default:
            return;
        }

        ax::Widgets::Icon(ImVec2(static_cast<float>(m_PinIconSize), static_cast<float>(m_PinIconSize)), iconType, connected, color, ImColor(32, 32, 32, alpha));
    };

    void ShowStyleEditor(bool *show = nullptr)
    {
        if (!CherryGUI::Begin("Style", show))
        {
            CherryGUI::End();
            return;
        }

        auto paneWidth = CherryGUI::GetContentRegionAvail().x;

        auto &editorStyle = ed::GetStyle();
        CherryGUI::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
        CherryGUI::TextUnformatted("Values");
        CherryGUI::Spring();
        if (CherryGUI::Button("Reset to defaults"))
            editorStyle = ed::Style();
        CherryGUI::EndHorizontal();
        CherryGUI::Spacing();
        CherryGUI::DragFloat4("Node Padding", &editorStyle.NodePadding.x, 0.1f, 0.0f, 40.0f);
        CherryGUI::DragFloat("Node Rounding", &editorStyle.NodeRounding, 0.1f, 0.0f, 40.0f);
        CherryGUI::DragFloat("Node Border Width", &editorStyle.NodeBorderWidth, 0.1f, 0.0f, 15.0f);
        CherryGUI::DragFloat("Hovered Node Border Width", &editorStyle.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
        CherryGUI::DragFloat("Hovered Node Border Offset", &editorStyle.HoverNodeBorderOffset, 0.1f, -40.0f, 40.0f);
        CherryGUI::DragFloat("Selected Node Border Width", &editorStyle.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
        CherryGUI::DragFloat("Selected Node Border Offset", &editorStyle.SelectedNodeBorderOffset, 0.1f, -40.0f, 40.0f);
        CherryGUI::DragFloat("Pin Rounding", &editorStyle.PinRounding, 0.1f, 0.0f, 40.0f);
        CherryGUI::DragFloat("Pin Border Width", &editorStyle.PinBorderWidth, 0.1f, 0.0f, 15.0f);
        CherryGUI::DragFloat("Link Strength", &editorStyle.LinkStrength, 1.0f, 0.0f, 500.0f);
        // ImVec2  SourceDirection;
        // ImVec2  TargetDirection;
        CherryGUI::DragFloat("Scroll Duration", &editorStyle.ScrollDuration, 0.001f, 0.0f, 2.0f);
        CherryGUI::DragFloat("Flow Marker Distance", &editorStyle.FlowMarkerDistance, 1.0f, 1.0f, 200.0f);
        CherryGUI::DragFloat("Flow Speed", &editorStyle.FlowSpeed, 1.0f, 1.0f, 2000.0f);
        CherryGUI::DragFloat("Flow Duration", &editorStyle.FlowDuration, 0.001f, 0.0f, 5.0f);
        // ImVec2  PivotAlignment;
        // ImVec2  PivotSize;
        // ImVec2  PivotScale;
        // float   PinCorners;
        // float   PinRadius;
        // float   PinArrowSize;
        // float   PinArrowWidth;
        CherryGUI::DragFloat("Group Rounding", &editorStyle.GroupRounding, 0.1f, 0.0f, 40.0f);
        CherryGUI::DragFloat("Group Border Width", &editorStyle.GroupBorderWidth, 0.1f, 0.0f, 15.0f);

        CherryGUI::Separator();

        static ImGuiColorEditFlags edit_mode = ImGuiColorEditFlags_DisplayRGB;
        CherryGUI::BeginHorizontal("Color Mode", ImVec2(paneWidth, 0), 1.0f);
        CherryGUI::TextUnformatted("Filter Colors");
        CherryGUI::Spring();
        CherryGUI::RadioButton("RGB", &edit_mode, ImGuiColorEditFlags_DisplayRGB);
        CherryGUI::Spring(0);
        CherryGUI::RadioButton("HSV", &edit_mode, ImGuiColorEditFlags_DisplayHSV);
        CherryGUI::Spring(0);
        CherryGUI::RadioButton("HEX", &edit_mode, ImGuiColorEditFlags_DisplayHex);
        CherryGUI::EndHorizontal();

        static ImGuiTextFilter filter;
        filter.Draw("##filter", paneWidth);

        CherryGUI::Spacing();

        CherryGUI::PushItemWidth(-160);
        for (int i = 0; i < ed::StyleColor_Count; ++i)
        {
            auto name = ed::GetStyleColorName((ed::StyleColor)i);
            if (!filter.PassFilter(name))
                continue;

            CherryGUI::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
        }
        CherryGUI::PopItemWidth();

        CherryGUI::End();
    }

    void ShowLeftPane(float paneWidth)
    {
        auto &io = CherryGUI::GetIO();

        CherryGUI::BeginChild("Selection", ImVec2(paneWidth, 0));

        paneWidth = CherryGUI::GetContentRegionAvail().x;

        static bool showStyleEditor = false;
        CherryGUI::BeginHorizontal("Style Editor", ImVec2(paneWidth, 0));
        CherryGUI::Spring(0.0f, 0.0f);
        if (CherryGUI::Button("Zoom to Content"))
            ed::NavigateToContent();
        CherryGUI::Spring(0.0f);
        if (CherryGUI::Button("Show Flow"))
        {
            for (auto &link : m_Links)
                ed::Flow(link.ID);
        }
        CherryGUI::Spring();
        if (CherryGUI::Button("Edit Style"))
            showStyleEditor = true;
        CherryGUI::EndHorizontal();
        CherryGUI::Checkbox("Show Ordinals", &m_ShowOrdinals);

        if (showStyleEditor)
            ShowStyleEditor(&showStyleEditor);

        std::vector<ed::NodeId> selectedNodes;
        std::vector<ed::LinkId> selectedLinks;
        selectedNodes.resize(ed::GetSelectedObjectCount());
        selectedLinks.resize(ed::GetSelectedObjectCount());

        int nodeCount = ed::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
        int linkCount = ed::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

        selectedNodes.resize(nodeCount);
        selectedLinks.resize(linkCount);

        int saveIconWidth = 50;     // GetTextureWidth(m_SaveIcon);
        int saveIconHeight = 50;    // GetTextureWidth(m_SaveIcon);
        int restoreIconWidth = 50;  // GetTextureWidth(m_RestoreIcon);
        int restoreIconHeight = 50; // GetTextureWidth(m_RestoreIcon);

        CherryGUI::GetWindowDrawList()->AddRectFilled(
            CherryGUI::GetCursorScreenPos(),
            CherryGUI::GetCursorScreenPos() + ImVec2(paneWidth, CherryGUI::GetTextLineHeight()),
            ImColor(CherryGUI::GetStyle().Colors[ImGuiCol_HeaderActive]), CherryGUI::GetTextLineHeight() * 0.25f);
        CherryGUI::Spacing();
        CherryGUI::SameLine();
        CherryGUI::TextUnformatted("Nodes");
        CherryGUI::Indent();
        for (auto &node : m_Nodes)
        {
            CherryGUI::PushID(node.ID.AsPointer());
            auto start = CherryGUI::GetCursorScreenPos();

            if (const auto progress = GetTouchProgress(node.ID))
            {
                CherryGUI::GetWindowDrawList()->AddLine(
                    start + ImVec2(-8, 0),
                    start + ImVec2(-8, CherryGUI::GetTextLineHeight()),
                    IM_COL32(255, 0, 0, 255 - (int)(255 * progress)), 4.0f);
            }

            bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node.ID) != selectedNodes.end();
#if IMGUI_VERSION_NUM >= 18967
            CherryGUI::SetNextItemAllowOverlap();
#endif
            if (CherryGUI::Selectable((node.Name + "##" + std::to_string(reinterpret_cast<uintptr_t>(node.ID.AsPointer()))).c_str(), &isSelected))
            {
                if (io.KeyCtrl)
                {
                    if (isSelected)
                        ed::SelectNode(node.ID, true);
                    else
                        ed::DeselectNode(node.ID);
                }
                else
                    ed::SelectNode(node.ID, false);

                ed::NavigateToSelection();
            }
            if (CherryGUI::IsItemHovered() && !node.State.empty())
                CherryGUI::SetTooltip("State: %s", node.State.c_str());

            auto id = std::string("(") + std::to_string(reinterpret_cast<uintptr_t>(node.ID.AsPointer())) + ")";
            auto textSize = CherryGUI::CalcTextSize(id.c_str(), nullptr);
            auto iconPanelPos = start + ImVec2(
                                            paneWidth - CherryGUI::GetStyle().FramePadding.x - CherryGUI::GetStyle().IndentSpacing - saveIconWidth - restoreIconWidth - CherryGUI::GetStyle().ItemInnerSpacing.x * 1,
                                            (CherryGUI::GetTextLineHeight() - saveIconHeight) / 2);
            CherryGUI::GetWindowDrawList()->AddText(
                ImVec2(iconPanelPos.x - textSize.x - CherryGUI::GetStyle().ItemInnerSpacing.x, start.y),
                IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);

            auto drawList = CherryGUI::GetWindowDrawList();
            CherryGUI::SetCursorScreenPos(iconPanelPos);
#if IMGUI_VERSION_NUM < 18967
            CherryGUI::SetItemAllowOverlap();
#else
            CherryGUI::SetNextItemAllowOverlap();
#endif
            if (node.SavedState.empty())
            {
                if (CherryGUI::InvisibleButton("save", ImVec2((float)saveIconWidth, (float)saveIconHeight)))
                    node.SavedState = node.State;

                if (CherryGUI::IsItemActive())
                    drawList->AddImage(m_SaveIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
                else if (CherryGUI::IsItemHovered())
                    drawList->AddImage(m_SaveIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
                else
                    drawList->AddImage(m_SaveIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
            }
            else
            {
                CherryGUI::Dummy(ImVec2((float)saveIconWidth, (float)saveIconHeight));
                drawList->AddImage(m_SaveIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
            }

            CherryGUI::SameLine(0, CherryGUI::GetStyle().ItemInnerSpacing.x);
#if IMGUI_VERSION_NUM < 18967
            CherryGUI::SetItemAllowOverlap();
#else
            CherryGUI::SetNextItemAllowOverlap();
#endif
            if (!node.SavedState.empty())
            {
                if (CherryGUI::InvisibleButton("restore", ImVec2((float)restoreIconWidth, (float)restoreIconHeight)))
                {
                    node.State = node.SavedState;
                    ed::RestoreNodeState(node.ID);
                    node.SavedState.clear();
                }

                if (CherryGUI::IsItemActive())
                    drawList->AddImage(m_RestoreIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
                else if (CherryGUI::IsItemHovered())
                    drawList->AddImage(m_RestoreIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
                else
                    drawList->AddImage(m_RestoreIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
            }
            else
            {
                CherryGUI::Dummy(ImVec2((float)restoreIconWidth, (float)restoreIconHeight));
                drawList->AddImage(m_RestoreIcon, CherryGUI::GetItemRectMin(), CherryGUI::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
            }

            CherryGUI::SameLine(0, 0);
#if IMGUI_VERSION_NUM < 18967
            CherryGUI::SetItemAllowOverlap();
#endif
            CherryGUI::Dummy(ImVec2(0, (float)restoreIconHeight));

            CherryGUI::PopID();
        }
        CherryGUI::Unindent();

        static int changeCount = 0;

        CherryGUI::GetWindowDrawList()->AddRectFilled(
            CherryGUI::GetCursorScreenPos(),
            CherryGUI::GetCursorScreenPos() + ImVec2(paneWidth, CherryGUI::GetTextLineHeight()),
            ImColor(CherryGUI::GetStyle().Colors[ImGuiCol_HeaderActive]), CherryGUI::GetTextLineHeight() * 0.25f);
        CherryGUI::Spacing();
        CherryGUI::SameLine();
        CherryGUI::TextUnformatted("Selection");

        CherryGUI::BeginHorizontal("Selection Stats", ImVec2(paneWidth, 0));
        CherryGUI::Text("Changed %d time%s", changeCount, changeCount > 1 ? "s" : "");
        CherryGUI::Spring();
        if (CherryGUI::Button("Deselect All"))
            ed::ClearSelection();
        CherryGUI::EndHorizontal();
        CherryGUI::Indent();
        for (int i = 0; i < nodeCount; ++i)
            CherryGUI::Text("Node (%p)", selectedNodes[i].AsPointer());
        for (int i = 0; i < linkCount; ++i)
            CherryGUI::Text("Link (%p)", selectedLinks[i].AsPointer());
        CherryGUI::Unindent();

        if (CherryGUI::IsKeyPressed(CherryGUI::GetKeyIndex(ImGuiKey_Z)))
            for (auto &link : m_Links)
                ed::Flow(link.ID);

        if (ed::HasSelectionChanged())
            ++changeCount;

        CherryGUI::EndChild();
    }
    int m_NextId = 1;
    const int m_PinIconSize = 24;
    std::vector<Node> m_Nodes;
    std::vector<Link> m_Links;
    ImTextureID m_HeaderBackground = nullptr;
    ImTextureID m_SaveIcon = nullptr;
    ImTextureID m_RestoreIcon = nullptr;
    const float m_TouchTime = 1.0f;
    std::map<ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
    bool m_ShowOrdinals = false;
};

namespace Cherry
{
    namespace Components
    {
        class NodeArea : public Component
        {
        public:
            NodeArea(const Cherry::Identifier &id, const std::string &label, int width, int heigh)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_border", "#454545B2");
                SetProperty("color_border_hovered", "#454545B2");
                SetProperty("color_border_clicked", "#454545B2");
                SetProperty("color_bg", "#242424FF");
                SetProperty("color_bg_hovered", "#343434FF");
                SetProperty("color_bg_clicked", "#444444FF");

                // Images
                SetProperty("width", std::to_string(width));
                SetProperty("heigh", std::to_string(heigh));

                // Informations
                SetProperty("label", label);

                // Data & User-level informations
                SetData("lastClicked", "never");
                SetData("isClicked", "false");

                // Init
                m_NodeEngine = std::make_shared<Example>();

                ed::Config config;

                config.SettingsFile = "Blueprints.json";

                config.UserPointer = m_NodeEngine.get();

                config.LoadNodeSettings = [](ed::NodeId nodeId, char *data, void *userPointer) -> size_t
                {
                    auto self = static_cast<Example *>(userPointer);

                    auto node = self->FindNode(nodeId);
                    if (!node)
                        return 0;

                    if (data != nullptr)
                        memcpy(data, node->State.data(), node->State.size());
                    return node->State.size();
                };

                config.SaveNodeSettings = [](ed::NodeId nodeId, const char *data, size_t size, ed::SaveReasonFlags reason, void *userPointer) -> bool
                {
                    auto self = static_cast<Example *>(userPointer);

                    auto node = self->FindNode(nodeId);
                    if (!node)
                        return false;

                    node->State.assign(data, size);

                    self->TouchNode(nodeId);

                    return true;
                };

                m_NodeEngine->m_Editor = ed::CreateEditor(&config);
                ed::SetCurrentEditor(m_NodeEngine->m_Editor);

                this->BuildNodes();
            }

            void BuildNodes()
            {
                Node *node;
                node = m_NodeEngine->SpawnInputActionNode();
                ed::SetNodePosition(node->ID, ImVec2(-252, 220));
                node = m_NodeEngine->SpawnBranchNode();
                ed::SetNodePosition(node->ID, ImVec2(-300, 351));
                node = m_NodeEngine->SpawnDoNNode();
                ed::SetNodePosition(node->ID, ImVec2(-238, 504));
                node = m_NodeEngine->SpawnOutputActionNode();
                ed::SetNodePosition(node->ID, ImVec2(71, 80));
                node = m_NodeEngine->SpawnSetTimerNode();
                ed::SetNodePosition(node->ID, ImVec2(168, 316));

                node = m_NodeEngine->SpawnTreeSequenceNode();
                ed::SetNodePosition(node->ID, ImVec2(1028, 329));
                node = m_NodeEngine->SpawnTreeTaskNode();
                ed::SetNodePosition(node->ID, ImVec2(1204, 458));
                node = m_NodeEngine->SpawnTreeTask2Node();
                ed::SetNodePosition(node->ID, ImVec2(868, 538));

                node = m_NodeEngine->SpawnComment();
                ed::SetNodePosition(node->ID, ImVec2(112, 576));
                ed::SetGroupSize(node->ID, ImVec2(384, 154));
                node = m_NodeEngine->SpawnComment();
                ed::SetNodePosition(node->ID, ImVec2(800, 224));
                ed::SetGroupSize(node->ID, ImVec2(640, 400));

                node = m_NodeEngine->SpawnLessNode();
                ed::SetNodePosition(node->ID, ImVec2(366, 652));
                node = m_NodeEngine->SpawnWeirdNode();
                ed::SetNodePosition(node->ID, ImVec2(144, 652));
                node = m_NodeEngine->SpawnMessageNode();
                ed::SetNodePosition(node->ID, ImVec2(-348, 698));
                node = m_NodeEngine->SpawnPrintStringNode();
                ed::SetNodePosition(node->ID, ImVec2(-69, 652));

                node = m_NodeEngine->SpawnHoudiniTransformNode();
                ed::SetNodePosition(node->ID, ImVec2(500, -70));
                node = m_NodeEngine->SpawnHoudiniGroupNode();
                ed::SetNodePosition(node->ID, ImVec2(500, 42));

                m_NodeEngine->BuildNodes();

                m_NodeEngine->m_Links.push_back(Link(m_NodeEngine->GetNextLinkId(), m_NodeEngine->m_Nodes[5].Outputs[0].ID, m_NodeEngine->m_Nodes[6].Inputs[0].ID));
                m_NodeEngine->m_Links.push_back(Link(m_NodeEngine->GetNextLinkId(), m_NodeEngine->m_Nodes[5].Outputs[0].ID, m_NodeEngine->m_Nodes[7].Inputs[0].ID));

                m_NodeEngine->m_Links.push_back(Link(m_NodeEngine->GetNextLinkId(), m_NodeEngine->m_Nodes[14].Outputs[0].ID, m_NodeEngine->m_Nodes[15].Inputs[0].ID));
            }

            void Render() override
            {
                m_NodeEngine->m_HeaderBackground = Cherry::GetTexture(Cherry::GetPath("resources/base/blueprintbackground.png"));
                m_NodeEngine->m_SaveIcon = Cherry::GetTexture(Cherry::GetPath("resources/base/x.png"));
                m_NodeEngine->m_RestoreIcon = Cherry::GetTexture(Cherry::GetPath("resources/base/x.png"));

                m_NodeEngine->UpdateTouch();

                auto &io = CherryGUI::GetIO();

                CherryGUI::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

                ed::SetCurrentEditor(m_NodeEngine->m_Editor);

                // auto& style = CherryGUI::GetStyle();

#if 0
        {
            for (auto x = -io.DisplaySize.y; x < io.DisplaySize.x; x += 10.0f)
            {
                CherryGUI::GetWindowDrawList()->AddLine(ImVec2(x, 0), ImVec2(x + io.DisplaySize.y, io.DisplaySize.y),
                    IM_COL32(255, 255, 0, 255));
            }
        }
#endif

                static ed::NodeId contextNodeId = 0;
                static ed::LinkId contextLinkId = 0;
                static ed::PinId contextPinId = 0;
                static bool createNewNode = false;
                static Pin *newNodeLinkPin = nullptr;
                static Pin *newLinkPin = nullptr;

                static float leftPaneWidth = 400.0f;
                static float rightPaneWidth = 800.0f;
                static bool navigated = false;

                if (!navigated)
                {
                    ed::NavigateToContent();
                    navigated = true;
                }

                ed::Begin("Node qsd");
                {
                    auto cursorTopLeft = CherryGUI::GetCursorScreenPos();

                    util::BlueprintNodeBuilder builder(
                        m_NodeEngine->m_HeaderBackground,
                        Application::Get().GetCurrentRenderedWindow()->get("/home/diego/data/BlueprintBackground.png")->GetWidth(),
                        Application::Get().GetCurrentRenderedWindow()->get("/home/diego/data/BlueprintBackground.png")->GetHeight());

                    for (auto &node : m_NodeEngine->m_Nodes)
                    {
                        if (node.Type != NodeType::Blueprint && node.Type != NodeType::Simple)
                            continue;

                        const auto isSimple = node.Type == NodeType::Simple;

                        bool hasOutputDelegates = false;
                        for (auto &output : node.Outputs)
                            if (output.Type == PinType::Delegate)
                                hasOutputDelegates = true;

                        builder.Begin(node.ID);

                        if (!isSimple)
                        {
                            builder.Header(node.Color);
                            CherryGUI::Spring(0);
                            CherryGUI::TextUnformatted(node.Name.c_str());
                            CherryGUI::Spring(1);
                            CherryGUI::Dummy(ImVec2(0, 28));
                            if (hasOutputDelegates)
                            {
                                CherryGUI::BeginVertical("delegates", ImVec2(0, 28));
                                CherryGUI::Spring(1, 0);
                                for (auto &output : node.Outputs)
                                {
                                    if (output.Type != PinType::Delegate)
                                        continue;

                                    auto alpha = CherryGUI::GetStyle().Alpha;

                                    if (newLinkPin && !m_NodeEngine->CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                                        alpha = alpha * (48.0f / 255.0f);

                                    ed::BeginPin(output.ID, ed::PinKind::Output);
                                    ed::PinPivotAlignment(ImVec2(1.0f, 0.5f));
                                    ed::PinPivotSize(ImVec2(0, 0));
                                    CherryGUI::BeginHorizontal(output.ID.AsPointer());
                                    CherryGUI::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                                    if (!output.Name.empty())
                                    {
                                        CherryGUI::TextUnformatted(output.Name.c_str());
                                        CherryGUI::Spring(0);
                                    }
                                    m_NodeEngine->DrawPinIcon(output, m_NodeEngine->IsPinLinked(output.ID), (int)(alpha * 255));
                                    CherryGUI::Spring(0, CherryGUI::GetStyle().ItemSpacing.x / 2);
                                    CherryGUI::EndHorizontal();
                                    CherryGUI::PopStyleVar();
                                    ed::EndPin();

                                    // DrawItemRect(ImColor(255, 0, 0));
                                }
                                CherryGUI::Spring(1, 0);
                                CherryGUI::EndVertical();
                                CherryGUI::Spring(0, CherryGUI::GetStyle().ItemSpacing.x / 2);
                            }
                            else
                                CherryGUI::Spring(0);
                            builder.EndHeader();
                        }

                        for (auto &input : node.Inputs)
                        {
                            auto alpha = CherryGUI::GetStyle().Alpha;
                            if (newLinkPin && !m_NodeEngine->CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
                                alpha = alpha * (48.0f / 255.0f);

                            builder.Input(input.ID);
                            CherryGUI::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                            m_NodeEngine->DrawPinIcon(input, m_NodeEngine->IsPinLinked(input.ID), (int)(alpha * 255));
                            CherryGUI::Spring(0);
                            if (!input.Name.empty())
                            {
                                CherryGUI::TextUnformatted(input.Name.c_str());
                                CherryGUI::Spring(0);
                            }
                            if (input.Type == PinType::Bool)
                            {
                                CherryGUI::Button("Hello");
                                CherryGUI::Spring(0);
                            }
                            CherryGUI::PopStyleVar();
                            builder.EndInput();
                        }

                        if (isSimple)
                        {
                            builder.Middle();

                            CherryGUI::Spring(1, 0);
                            CherryGUI::TextUnformatted(node.Name.c_str());
                            CherryGUI::Spring(1, 0);
                        }

                        for (auto &output : node.Outputs)
                        {
                            if (!isSimple && output.Type == PinType::Delegate)
                                continue;

                            auto alpha = CherryGUI::GetStyle().Alpha;
                            if (newLinkPin && !m_NodeEngine->CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                                alpha = alpha * (48.0f / 255.0f);

                            CherryGUI::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                            builder.Output(output.ID);
                            if (output.Type == PinType::String)
                            {
                                static char buffer[128] = "Edit Me\nMultiline!";
                                static bool wasActive = false;

                                CherryGUI::PushItemWidth(100.0f);
                                CherryGUI::InputText("##edit", buffer, 127);
                                CherryGUI::PopItemWidth();
                                if (CherryGUI::IsItemActive() && !wasActive)
                                {
                                    ed::EnableShortcuts(false);
                                    wasActive = true;
                                }
                                else if (!CherryGUI::IsItemActive() && wasActive)
                                {
                                    ed::EnableShortcuts(true);
                                    wasActive = false;
                                }
                                CherryGUI::Spring(0);
                            }
                            if (!output.Name.empty())
                            {
                                CherryGUI::Spring(0);
                                CherryGUI::TextUnformatted(output.Name.c_str());
                            }
                            CherryGUI::Spring(0);
                            m_NodeEngine->DrawPinIcon(output, m_NodeEngine->IsPinLinked(output.ID), (int)(alpha * 255));
                            CherryGUI::PopStyleVar();
                            builder.EndOutput();
                        }

                        builder.End();
                    }

                    for (auto &node : m_NodeEngine->m_Nodes)
                    {
                        if (node.Type != NodeType::Tree)
                            continue;

                        const float rounding = 5.0f;
                        const float padding = 12.0f;

                        const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

                        ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
                        ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
                        ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 180, 255, 150));
                        ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

                        ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
                        ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
                        ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
                        ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
                        ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
                        ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
                        ed::PushStyleVar(ed::StyleVar_PinRadius, 5.0f);
                        ed::BeginNode(node.ID);

                        CherryGUI::BeginVertical(node.ID.AsPointer());
                        CherryGUI::BeginHorizontal("inputs");
                        CherryGUI::Spring(0, padding * 2);

                        ImRect inputsRect;
                        int inputAlpha = 200;
                        if (!node.Inputs.empty())
                        {
                            auto &pin = node.Inputs[0];
                            CherryGUI::Dummy(ImVec2(0, padding));
                            CherryGUI::Spring(1, 0);
                            inputsRect = ImGui_GetItemRect();

                            ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
                            ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);
#if IMGUI_VERSION_NUM > 18101
                            ed::PushStyleVar(ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersBottom);
#else
                            ed::PushStyleVar(ed::StyleVar_PinCorners, 12);
#endif
                            ed::BeginPin(pin.ID, ed::PinKind::Input);
                            ed::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
                            ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
                            ed::EndPin();
                            ed::PopStyleVar(3);

                            if (newLinkPin && !m_NodeEngine->CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
                                inputAlpha = (int)(255 * CherryGUI::GetStyle().Alpha * (48.0f / 255.0f));
                        }
                        else
                            CherryGUI::Dummy(ImVec2(0, padding));

                        CherryGUI::Spring(0, padding * 2);
                        CherryGUI::EndHorizontal();

                        CherryGUI::BeginHorizontal("content_frame");
                        CherryGUI::Spring(1, padding);

                        CherryGUI::BeginVertical("content", ImVec2(0.0f, 0.0f));
                        CherryGUI::Dummy(ImVec2(160, 0));
                        CherryGUI::Spring(1);
                        CherryGUI::TextUnformatted(node.Name.c_str());
                        CherryGUI::Spring(1);
                        CherryGUI::EndVertical();
                        auto contentRect = ImGui_GetItemRect();

                        CherryGUI::Spring(1, padding);
                        CherryGUI::EndHorizontal();

                        CherryGUI::BeginHorizontal("outputs");
                        CherryGUI::Spring(0, padding * 2);

                        ImRect outputsRect;
                        int outputAlpha = 200;
                        if (!node.Outputs.empty())
                        {
                            auto &pin = node.Outputs[0];
                            CherryGUI::Dummy(ImVec2(0, padding));
                            CherryGUI::Spring(1, 0);
                            outputsRect = ImGui_GetItemRect();

#if IMGUI_VERSION_NUM > 18101
                            ed::PushStyleVar(ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersTop);
#else
                            ed::PushStyleVar(ed::StyleVar_PinCorners, 3);
#endif
                            ed::BeginPin(pin.ID, ed::PinKind::Output);
                            ed::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
                            ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
                            ed::EndPin();
                            ed::PopStyleVar();

                            if (newLinkPin && !m_NodeEngine->CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
                                outputAlpha = (int)(255 * CherryGUI::GetStyle().Alpha * (48.0f / 255.0f));
                        }
                        else
                            CherryGUI::Dummy(ImVec2(0, padding));

                        CherryGUI::Spring(0, padding * 2);
                        CherryGUI::EndHorizontal();

                        CherryGUI::EndVertical();

                        ed::EndNode();
                        ed::PopStyleVar(7);
                        ed::PopStyleColor(4);

                        auto drawList = ed::GetNodeBackgroundDrawList(node.ID);

                        // const auto fringeScale = CherryGUI::GetStyle().AntiAliasFringeScale;
                        // const auto unitSize    = 1.0f / fringeScale;

                        // const auto ImDrawList_AddRect = [](ImDrawList* drawList, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
                        //{
                        //     if ((col >> 24) == 0)
                        //         return;
                        //     drawList->PathRect(a, b, rounding, rounding_corners);
                        //     drawList->PathStroke(col, true, thickness);
                        // };

#if IMGUI_VERSION_NUM > 18101
                        const auto topRoundCornersFlags = ImDrawFlags_RoundCornersTop;
                        const auto bottomRoundCornersFlags = ImDrawFlags_RoundCornersBottom;
#else
                        const auto topRoundCornersFlags = 1 | 2;
                        const auto bottomRoundCornersFlags = 4 | 8;
#endif

                        drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
                                                IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
                        // CherryGUI::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                        drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
                                          IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
                        // CherryGUI::PopStyleVar();
                        drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
                                                IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
                        // CherryGUI::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                        drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
                                          IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
                        // CherryGUI::PopStyleVar();
                        drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
                        // CherryGUI::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                        drawList->AddRect(
                            contentRect.GetTL(),
                            contentRect.GetBR(),
                            IM_COL32(48, 128, 255, 100), 0.0f);
                        // CherryGUI::PopStyleVar();
                    }

                    for (auto &node : m_NodeEngine->m_Nodes)
                    {
                        if (node.Type != NodeType::Houdini)
                            continue;

                        const float rounding = 10.0f;
                        const float padding = 12.0f;

                        ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(229, 229, 229, 200));
                        ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(125, 125, 125, 200));
                        ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(229, 229, 229, 60));
                        ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(125, 125, 125, 60));

                        const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

                        ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
                        ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
                        ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
                        ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
                        ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
                        ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
                        ed::PushStyleVar(ed::StyleVar_PinRadius, 6.0f);
                        ed::BeginNode(node.ID);

                        CherryGUI::BeginVertical(node.ID.AsPointer());
                        if (!node.Inputs.empty())
                        {
                            CherryGUI::BeginHorizontal("inputs");
                            CherryGUI::Spring(1, 0);

                            ImRect inputsRect;
                            int inputAlpha = 200;
                            for (auto &pin : node.Inputs)
                            {
                                CherryGUI::Dummy(ImVec2(padding, padding));
                                inputsRect = ImGui_GetItemRect();
                                CherryGUI::Spring(1, 0);
                                inputsRect.Min.y -= padding;
                                inputsRect.Max.y -= padding;

#if IMGUI_VERSION_NUM > 18101
                                const auto allRoundCornersFlags = ImDrawFlags_RoundCornersAll;
#else
                                const auto allRoundCornersFlags = 15;
#endif
                                // ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
                                // ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);
                                ed::PushStyleVar(ed::StyleVar_PinCorners, allRoundCornersFlags);

                                ed::BeginPin(pin.ID, ed::PinKind::Input);
                                ed::PinPivotRect(inputsRect.GetCenter(), inputsRect.GetCenter());
                                ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
                                ed::EndPin();
                                // ed::PopStyleVar(3);
                                ed::PopStyleVar(1);

                                auto drawList = CherryGUI::GetWindowDrawList();
                                drawList->AddRectFilled(inputsRect.GetTL(), inputsRect.GetBR(),
                                                        IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, allRoundCornersFlags);
                                drawList->AddRect(inputsRect.GetTL(), inputsRect.GetBR(),
                                                  IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, allRoundCornersFlags);

                                if (newLinkPin && !m_NodeEngine->CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
                                    inputAlpha = (int)(255 * CherryGUI::GetStyle().Alpha * (48.0f / 255.0f));
                            }

                            // CherryGUI::Spring(1, 0);
                            CherryGUI::EndHorizontal();
                        }

                        CherryGUI::BeginHorizontal("content_frame");
                        CherryGUI::Spring(1, padding);

                        CherryGUI::BeginVertical("content", ImVec2(0.0f, 0.0f));
                        CherryGUI::Dummy(ImVec2(160, 0));
                        CherryGUI::Spring(1);
                        CherryGUI::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                        CherryGUI::TextUnformatted(node.Name.c_str());
                        CherryGUI::PopStyleColor();
                        CherryGUI::Spring(1);
                        CherryGUI::EndVertical();
                        auto contentRect = ImGui_GetItemRect();

                        CherryGUI::Spring(1, padding);
                        CherryGUI::EndHorizontal();

                        if (!node.Outputs.empty())
                        {
                            CherryGUI::BeginHorizontal("outputs");
                            CherryGUI::Spring(1, 0);

                            ImRect outputsRect;
                            int outputAlpha = 200;
                            for (auto &pin : node.Outputs)
                            {
                                CherryGUI::Dummy(ImVec2(padding, padding));
                                outputsRect = ImGui_GetItemRect();
                                CherryGUI::Spring(1, 0);
                                outputsRect.Min.y += padding;
                                outputsRect.Max.y += padding;

#if IMGUI_VERSION_NUM > 18101
                                const auto allRoundCornersFlags = ImDrawFlags_RoundCornersAll;
                                const auto topRoundCornersFlags = ImDrawFlags_RoundCornersTop;
#else
                                const auto allRoundCornersFlags = 15;
                                const auto topRoundCornersFlags = 3;
#endif

                                ed::PushStyleVar(ed::StyleVar_PinCorners, topRoundCornersFlags);
                                ed::BeginPin(pin.ID, ed::PinKind::Output);
                                ed::PinPivotRect(outputsRect.GetCenter(), outputsRect.GetCenter());
                                ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
                                ed::EndPin();
                                ed::PopStyleVar();

                                auto drawList = CherryGUI::GetWindowDrawList();
                                drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR(),
                                                        IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, allRoundCornersFlags);
                                drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR(),
                                                  IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, allRoundCornersFlags);

                                if (newLinkPin && !m_NodeEngine->CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
                                    outputAlpha = (int)(255 * CherryGUI::GetStyle().Alpha * (48.0f / 255.0f));
                            }

                            CherryGUI::EndHorizontal();
                        }

                        CherryGUI::EndVertical();

                        ed::EndNode();
                        ed::PopStyleVar(7);
                        ed::PopStyleColor(4);

                        // auto drawList = ed::GetNodeBackgroundDrawList(node.ID);

                        // const auto fringeScale = CherryGUI::GetStyle().AntiAliasFringeScale;
                        // const auto unitSize    = 1.0f / fringeScale;

                        // const auto ImDrawList_AddRect = [](ImDrawList* drawList, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
                        //{
                        //     if ((col >> 24) == 0)
                        //         return;
                        //     drawList->PathRect(a, b, rounding, rounding_corners);
                        //     drawList->PathStroke(col, true, thickness);
                        // };

                        // drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
                        //     IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);
                        // CherryGUI::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                        // drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
                        //     IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);
                        // CherryGUI::PopStyleVar();
                        // drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
                        //     IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);
                        ////CherryGUI::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                        // drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
                        //     IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);
                        ////CherryGUI::PopStyleVar();
                        // drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
                        // CherryGUI::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                        // drawList->AddRect(
                        //     contentRect.GetTL(),
                        //     contentRect.GetBR(),
                        //     IM_COL32(48, 128, 255, 100), 0.0f);
                        // CherryGUI::PopStyleVar();
                    }

                    for (auto &node : m_NodeEngine->m_Nodes)
                    {
                        if (node.Type != NodeType::Comment)
                            continue;

                        const float commentAlpha = 0.75f;

                        CherryGUI::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
                        ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
                        ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
                        ed::BeginNode(node.ID);
                        CherryGUI::PushID(node.ID.AsPointer());
                        CherryGUI::BeginVertical("content");
                        CherryGUI::BeginHorizontal("horizontal");
                        CherryGUI::Spring(1);
                        CherryGUI::TextUnformatted(node.Name.c_str());
                        CherryGUI::Spring(1);
                        CherryGUI::EndHorizontal();
                        ed::Group(node.Size);
                        CherryGUI::EndVertical();
                        CherryGUI::PopID();
                        ed::EndNode();
                        ed::PopStyleColor(2);
                        CherryGUI::PopStyleVar();

                        if (ed::BeginGroupHint(node.ID))
                        {
                            // auto alpha   = static_cast<int>(commentAlpha * CherryGUI::GetStyle().Alpha * 255);
                            auto bgAlpha = static_cast<int>(CherryGUI::GetStyle().Alpha * 255);

                            // CherryGUI::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * CherryGUI::GetStyle().Alpha);

                            auto min = ed::GetGroupMin();
                            // auto max = ed::GetGroupMax();

                            CherryGUI::SetCursorScreenPos(min - ImVec2(-8, CherryGUI::GetTextLineHeightWithSpacing() + 4));
                            CherryGUI::BeginGroup();
                            CherryGUI::TextUnformatted(node.Name.c_str());
                            CherryGUI::EndGroup();

                            auto drawList = ed::GetHintBackgroundDrawList();

                            auto hintBounds = ImGui_GetItemRect();
                            auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

                            drawList->AddRectFilled(
                                hintFrameBounds.GetTL(),
                                hintFrameBounds.GetBR(),
                                IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f);

                            drawList->AddRect(
                                hintFrameBounds.GetTL(),
                                hintFrameBounds.GetBR(),
                                IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f);

                            // CherryGUI::PopStyleVar();
                        }
                        ed::EndGroupHint();
                    }

                    for (auto &link : m_NodeEngine->m_Links)
                        ed::Link(link.ID, link.StartPinID, link.EndPinID, link.Color, 2.0f);

                    if (!createNewNode)
                    {
                        if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
                        {
                            auto showLabel = [](const char *label, ImColor color)
                            {
                                CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() - CherryGUI::GetTextLineHeight());
                                auto size = CherryGUI::CalcTextSize(label);

                                auto padding = CherryGUI::GetStyle().FramePadding;
                                auto spacing = CherryGUI::GetStyle().ItemSpacing;

                                CherryGUI::SetCursorPos(CherryGUI::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

                                auto rectMin = CherryGUI::GetCursorScreenPos() - padding;
                                auto rectMax = CherryGUI::GetCursorScreenPos() + size + padding;

                                auto drawList = CherryGUI::GetWindowDrawList();
                                drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
                                CherryGUI::TextUnformatted(label);
                            };

                            ed::PinId startPinId = 0, endPinId = 0;
                            if (ed::QueryNewLink(&startPinId, &endPinId))
                            {
                                auto startPin = m_NodeEngine->FindPin(startPinId);
                                auto endPin = m_NodeEngine->FindPin(endPinId);

                                newLinkPin = startPin ? startPin : endPin;

                                if (startPin->Kind == PinKind::Input)
                                {
                                    std::swap(startPin, endPin);
                                    std::swap(startPinId, endPinId);
                                }

                                if (startPin && endPin)
                                {
                                    if (endPin == startPin)
                                    {
                                        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                                    }
                                    else if (endPin->Kind == startPin->Kind)
                                    {
                                        showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                                        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                                    }
                                    // else if (endPin->Node == startPin->Node)
                                    //{
                                    //     showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                                    //     ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
                                    // }
                                    else if (endPin->Type != startPin->Type)
                                    {
                                        showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                                        ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                                    }
                                    else
                                    {
                                        showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                                        if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                                        {
                                            m_NodeEngine->m_Links.emplace_back(Link(m_NodeEngine->GetNextId(), startPinId, endPinId));
                                            m_NodeEngine->m_Links.back().Color = m_NodeEngine->GetIconColor(startPin->Type);
                                        }
                                    }
                                }
                            }

                            ed::PinId pinId = 0;
                            if (ed::QueryNewNode(&pinId))
                            {
                                newLinkPin = m_NodeEngine->FindPin(pinId);
                                if (newLinkPin)
                                    showLabel("+ Create Node", ImColor(32, 45, 32, 180));

                                if (ed::AcceptNewItem())
                                {
                                    createNewNode = true;
                                    newNodeLinkPin = m_NodeEngine->FindPin(pinId);
                                    newLinkPin = nullptr;
                                    ed::Suspend();
                                    CherryGUI::OpenPopup("Create New Node");
                                    ed::Resume();
                                }
                            }
                        }
                        else
                            newLinkPin = nullptr;

                        ed::EndCreate();

                        if (ed::BeginDelete())
                        {
                            ed::NodeId nodeId = 0;
                            while (ed::QueryDeletedNode(&nodeId))
                            {
                                if (ed::AcceptDeletedItem())
                                {
                                    auto id = std::find_if(m_NodeEngine->m_Nodes.begin(), m_NodeEngine->m_Nodes.end(), [nodeId](auto &node)
                                                           { return node.ID == nodeId; });
                                    if (id != m_NodeEngine->m_Nodes.end())
                                        m_NodeEngine->m_Nodes.erase(id);
                                }
                            }

                            ed::LinkId linkId = 0;
                            while (ed::QueryDeletedLink(&linkId))
                            {
                                if (ed::AcceptDeletedItem())
                                {
                                    auto id = std::find_if(m_NodeEngine->m_Links.begin(), m_NodeEngine->m_Links.end(), [linkId](auto &link)
                                                           { return link.ID == linkId; });
                                    if (id != m_NodeEngine->m_Links.end())
                                        m_NodeEngine->m_Links.erase(id);
                                }
                            }
                        }
                        ed::EndDelete();
                    }

                    CherryGUI::SetCursorScreenPos(cursorTopLeft);
                }

#if 1
                auto openPopupPosition = CherryGUI::GetMousePos();
                ed::Suspend();
                if (ed::ShowNodeContextMenu(&contextNodeId))
                    CherryGUI::OpenPopup("Node Context Menu");
                else if (ed::ShowPinContextMenu(&contextPinId))
                    CherryGUI::OpenPopup("Pin Context Menu");
                else if (ed::ShowLinkContextMenu(&contextLinkId))
                    CherryGUI::OpenPopup("Link Context Menu");
                else if (ed::ShowBackgroundContextMenu())
                {
                    CherryGUI::OpenPopup("Create New Node");
                    newNodeLinkPin = nullptr;
                }
                ed::Resume();

                ed::Suspend();
                CherryGUI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
                if (CherryGUI::BeginPopup("Node Context Menu"))
                {
                    auto node = m_NodeEngine->FindNode(contextNodeId);

                    CherryGUI::TextUnformatted("Node Context Menu");
                    CherryGUI::Separator();
                    if (node)
                    {
                        CherryGUI::Text("ID: %p", node->ID.AsPointer());
                        CherryGUI::Text("Type: %s", node->Type == NodeType::Blueprint ? "Blueprint" : (node->Type == NodeType::Tree ? "Tree" : "Comment"));
                        CherryGUI::Text("Inputs: %d", (int)node->Inputs.size());
                        CherryGUI::Text("Outputs: %d", (int)node->Outputs.size());
                    }
                    else
                        CherryGUI::Text("Unknown node: %p", contextNodeId.AsPointer());
                    CherryGUI::Separator();
                    if (CherryGUI::MenuItem("Delete"))
                        ed::DeleteNode(contextNodeId);
                    CherryGUI::EndPopup();
                }

                if (CherryGUI::BeginPopup("Pin Context Menu"))
                {
                    auto pin = m_NodeEngine->FindPin(contextPinId);

                    CherryGUI::TextUnformatted("Pin Context Menu");
                    CherryGUI::Separator();
                    if (pin)
                    {
                        CherryGUI::Text("ID: %p", pin->ID.AsPointer());
                        if (pin->Node)
                            CherryGUI::Text("Node: %p", pin->Node->ID.AsPointer());
                        else
                            CherryGUI::Text("Node: %s", "<none>");
                    }
                    else
                        CherryGUI::Text("Unknown pin: %p", contextPinId.AsPointer());

                    CherryGUI::EndPopup();
                }

                if (CherryGUI::BeginPopup("Link Context Menu"))
                {
                    auto link = m_NodeEngine->FindLink(contextLinkId);

                    CherryGUI::TextUnformatted("Link Context Menu");
                    CherryGUI::Separator();
                    if (link)
                    {
                        CherryGUI::Text("ID: %p", link->ID.AsPointer());
                        CherryGUI::Text("From: %p", link->StartPinID.AsPointer());
                        CherryGUI::Text("To: %p", link->EndPinID.AsPointer());
                    }
                    else
                        CherryGUI::Text("Unknown link: %p", contextLinkId.AsPointer());
                    CherryGUI::Separator();
                    if (CherryGUI::MenuItem("Delete"))
                        ed::DeleteLink(contextLinkId);
                    CherryGUI::EndPopup();
                }

                if (CherryGUI::BeginPopup("Create New Node"))
                {
                    auto newNodePostion = openPopupPosition;
                    // CherryGUI::SetCursorScreenPos(CherryGUI::GetMousePosOnOpeningCurrentPopup());

                    // auto drawList = CherryGUI::GetWindowDrawList();
                    // drawList->AddCircleFilled(CherryGUI::GetMousePosOnOpeningCurrentPopup(), 10.0f, 0xFFFF00FF);

                    Node *node = nullptr;
                    if (CherryGUI::MenuItem("Input Action"))
                        node = m_NodeEngine->SpawnInputActionNode();
                    if (CherryGUI::MenuItem("Output Action"))
                        node = m_NodeEngine->SpawnOutputActionNode();
                    if (CherryGUI::MenuItem("Branch"))
                        node = m_NodeEngine->SpawnBranchNode();
                    if (CherryGUI::MenuItem("Do N"))
                        node = m_NodeEngine->SpawnDoNNode();
                    if (CherryGUI::MenuItem("Set Timer"))
                        node = m_NodeEngine->SpawnSetTimerNode();
                    if (CherryGUI::MenuItem("Less"))
                        node = m_NodeEngine->SpawnLessNode();
                    if (CherryGUI::MenuItem("Weird"))
                        node = m_NodeEngine->SpawnWeirdNode();
                    if (CherryGUI::MenuItem("Trace by Channel"))
                        node = m_NodeEngine->SpawnTraceByChannelNode();
                    if (CherryGUI::MenuItem("Print String"))
                        node = m_NodeEngine->SpawnPrintStringNode();
                    CherryGUI::Separator();
                    if (CherryGUI::MenuItem("Comment"))
                        node = m_NodeEngine->SpawnComment();
                    CherryGUI::Separator();
                    if (CherryGUI::MenuItem("Sequence"))
                        node = m_NodeEngine->SpawnTreeSequenceNode();
                    if (CherryGUI::MenuItem("Move To"))
                        node = m_NodeEngine->SpawnTreeTaskNode();
                    if (CherryGUI::MenuItem("Random Wait"))
                        node = m_NodeEngine->SpawnTreeTask2Node();
                    CherryGUI::Separator();
                    if (CherryGUI::MenuItem("Message"))
                        node = m_NodeEngine->SpawnMessageNode();
                    CherryGUI::Separator();
                    if (CherryGUI::MenuItem("Transform"))
                        node = m_NodeEngine->SpawnHoudiniTransformNode();
                    if (CherryGUI::MenuItem("Group"))
                        node = m_NodeEngine->SpawnHoudiniGroupNode();

                    if (node)
                    {
                        m_NodeEngine->BuildNodes();

                        createNewNode = false;

                        ed::SetNodePosition(node->ID, newNodePostion);

                        if (auto startPin = newNodeLinkPin)
                        {
                            auto &pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

                            for (auto &pin : pins)
                            {
                                if (m_NodeEngine->CanCreateLink(startPin, &pin))
                                {
                                    auto endPin = &pin;
                                    if (startPin->Kind == PinKind::Input)
                                        std::swap(startPin, endPin);

                                    m_NodeEngine->m_Links.emplace_back(Link(m_NodeEngine->GetNextId(), startPin->ID, endPin->ID));
                                    m_NodeEngine->m_Links.back().Color = m_NodeEngine->GetIconColor(startPin->Type);

                                    break;
                                }
                            }
                        }
                    }

                    CherryGUI::EndPopup();
                }
                else
                    createNewNode = false;
                CherryGUI::PopStyleVar();
                ed::Resume();
#endif

                ed::End();
                auto editorMin = CherryGUI::GetItemRectMin();
                auto editorMax = CherryGUI::GetItemRectMax();

                if (m_NodeEngine->m_ShowOrdinals)
                {
                    int nodeCount = ed::GetNodeCount();
                    std::vector<ed::NodeId> orderedNodeIds;
                    orderedNodeIds.resize(static_cast<size_t>(nodeCount));
                    ed::GetOrderedNodeIds(orderedNodeIds.data(), nodeCount);

                    auto drawList = CherryGUI::GetWindowDrawList();
                    drawList->PushClipRect(editorMin, editorMax);

                    int ordinal = 0;
                    for (auto &nodeId : orderedNodeIds)
                    {
                        auto p0 = ed::GetNodePosition(nodeId);
                        auto p1 = p0 + ed::GetNodeSize(nodeId);
                        p0 = ed::CanvasToScreen(p0);
                        p1 = ed::CanvasToScreen(p1);

                        ImGuiTextBuffer builder;
                        builder.appendf("#%d", ordinal++);

                        auto textSize = CherryGUI::CalcTextSize(builder.c_str());
                        auto padding = ImVec2(2.0f, 2.0f);
                        auto widgetSize = textSize + padding * 2;

                        auto widgetPosition = ImVec2(p1.x, p0.y) + ImVec2(0.0f, -widgetSize.y);

                        drawList->AddRectFilled(widgetPosition, widgetPosition + widgetSize, IM_COL32(100, 80, 80, 190), 3.0f, ImDrawFlags_RoundCornersAll);
                        drawList->AddRect(widgetPosition, widgetPosition + widgetSize, IM_COL32(200, 160, 160, 190), 3.0f, ImDrawFlags_RoundCornersAll);
                        drawList->AddText(widgetPosition + padding, IM_COL32(255, 255, 255, 255), builder.c_str());
                    }

                    drawList->PopClipRect();
                }
            }

        private:
            std::shared_ptr<Example> m_NodeEngine;
        };
    }

    // End-User API
    namespace Kit
    {
        inline bool NodeArea(const std::string &label, int width, int heigh = 0)
        {
            // Inline component
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing->GetData("isClicked") == "true" ? true : false;
            }

            else
            {
                auto button = Application::CreateAnonymousComponent<Components::NodeArea>(Components::NodeArea(anonymous_id, label, width, heigh));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
        }

        inline bool NodeArea(const Cherry::Identifier &identifier, const std::string &label, int width, int heigh = 0)
        {
            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
                return existing_button->GetData("isClicked") == "true" ? true : false;
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::NodeArea>(Components::NodeArea(identifier, label, width, heigh));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_NODE_AREA
