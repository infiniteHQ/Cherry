#ifndef CHERRY_NODES_NODE_SCHEMA
#define CHERRY_NODES_NODE_SCHEMA

#include "../../../../base.hpp"

namespace Cherry
{
    namespace NodeSystem
    {
        enum class NodeSchemaStatus
        {
            Active,
            Disabled,
            Depreciated,
            Obsolete,
        };

        enum class NodeType
        {
            Blueprint,
            Simple,
            Tree,
            Comment,
            Houdini
        };

        struct NodePin
        {
            std::string Name;
            std::string ColorVariuant; // Hex (#FFFFFF)
            std::string TypeName;
        };

        class NodeSchema
        {
        public:
            NodeSchema() = default;
            NodeSchema(const std::string &id) : m_ID(id) {}

            void AddInputPin(const std::string &name, const std::string &pinTypeID)
            {
                NodePin pin;
                pin.Name = name;
                pin.TypeName = pinTypeID;
                m_InputPins.push_back(pin);
            }

            void AddOutputPin(const std::string &name, const std::string &pinTypeID)
            {
                NodePin pin;
                pin.Name = name;
                pin.TypeName = pinTypeID;
                m_OutputPins.push_back(pin);
            }

            void SetContent(const std::string &key, const std::string &value)
            {
                m_Content[key] = value;
            }

            std::string GetContent(const std::string &key) const
            {
                auto it = m_Content.find(key);
                if (it != m_Content.end())
                    return it->second;
                return "";
            }

            std::string GetHexHeaderColor() const { return m_HexHeaderColor; }
            std::string GetHexBorderColor() const { return m_HexBorderColor; }
            std::string GetHexBackgroundColor() const { return m_HexBackgroundColor; }
            std::string GetLabel() const { return m_Label; }
            std::string GetLabelHexColor() const { return m_LabelHexColor; }
            std::string GetDescriptionHexColor() const { return m_DescriptionHexColor; }
            std::string GetLogoPath() const { return m_LogoPath; }
            NodeType GetType() const { return m_NodeType; }

            void SetHexHeaderColor(const std::string &color) { m_HexHeaderColor = color; }
            void SetHexBorderColor(const std::string &color) { m_HexBorderColor = color; }
            void SetHexBackgroundColor(const std::string &color) { m_HexBackgroundColor = color; }
            void SetLabel(const std::string &label) { m_Label = label; }
            void SetLabelHexColor(const std::string &color) { m_LabelHexColor = color; }
            void SetDescriptionHexColor(const std::string &color) { m_DescriptionHexColor = color; }
            void SetLogoPath(const std::string &path) { m_LogoPath = path; }
            void SetType(NodeType type) { m_NodeType = type; }

            std::vector<NodePin> m_InputPins;
            std::vector<NodePin> m_OutputPins;
            std::unordered_map<std::string, std::string> m_Content;
            std::string m_ID; // nodeid@contextid

            std::string m_HexHeaderColor = "#B1FF31";
            std::string m_HexBorderColor;
            std::string m_HexBackgroundColor;
            std::string m_Label;
            std::string m_LabelHexColor;
            std::string m_DescriptionHexColor;
            std::string m_LogoPath;
            NodeSchemaStatus m_NodeStatus;
            NodeType m_NodeType = NodeType::Simple;
        };
    }
}

#endif // CHERRY_NODES_NODE_SCHEMA