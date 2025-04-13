#ifndef CHERRY_NODES_NODE_SCHEMA
#define CHERRY_NODES_NODE_SCHEMA

#include "../../../../base.hpp"

namespace Cherry
{
    namespace NodeSystem
    {

        enum class PinFormat
        {
            SquarePin,
            RoundPin,
            TrianglePin,
        };

        struct NodePin
        {
            PinFormat Format;
            std::string Name;
            std::string Color; // Hex (#FFFFFF)
            std::string TypeName;
            std::string TypeID; // pinid@nodeid@contextid
        };

        class NodeSchema
        {
        public:
            NodeSchema() = default;
            NodeSchema(const std::string &id) : m_ID(id) {}

            void AddInputPin(const std::string &name, const std::string &typeName, const std::string &color = "#FFFFFF", PinFormat format = PinFormat::SquarePin)
            {
                NodePin pin;
                pin.Name = name;
                pin.TypeName = typeName;
                pin.Color = color;
                pin.Format = format;
                // TypeID is assigned later by context using node id
                m_InputPins.push_back(pin);
            }

            void AddOutputPin(const std::string &name, const std::string &typeName, const std::string &color = "#FFFFFF", PinFormat format = PinFormat::SquarePin)
            {
                NodePin pin;
                pin.Name = name;
                pin.TypeName = typeName;
                pin.Color = color;
                pin.Format = format;
                // TypeID is assigned later by context using node id
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

            std::string GetLabel()
            {
                return m_Label;
            }

            std::string GetHexHeaderColor()
            {
                return m_HexHeaderColor;
            }

            std::vector<NodePin> m_InputPins;
            std::vector<NodePin> m_OutputPins;
            std::unordered_map<std::string, std::string> m_Content;
            std::string m_Label;
            std::string m_HexHeaderColor;
            std::string m_ID; // nodeid@contextid
        };
    }
}

#endif // CHERRY_NODES_NODE_SCHEMA