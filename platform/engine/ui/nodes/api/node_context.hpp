#ifndef CHERRY_NODES_NODE_CONTEXT
#define CHERRY_NODES_NODE_CONTEXT

#include "./objects/node_schema.hpp"
#include "./objects/node_pin_format.hpp"

namespace Cherry
{
    namespace NodeSystem
    {

        class NodeContext
        {
        public:
            explicit NodeContext(const std::string &contextID = "")
                : m_ContextID(contextID), m_AllowComments(false), m_AllowSequences(false) {}

            NodeSchema *CreateSchema(const std::string &typeID)
            {
                std::string id = GenerateFullID(typeID);
                auto schema = std::make_unique<NodeSchema>(id);
                NodeSchema *schemaPtr = schema.get();
                m_SchemaMap[id] = std::move(schema);
                return schemaPtr;
            }

            void SetupPinFormat(const PinFormat &pin_format)
            {
                m_PinFormats[pin_format.m_TypeID] = pin_format;
            }

            NodeSchema *GetSchema(const std::string &typeID) const
            {
                std::string id = GenerateFullID(typeID);
                auto it = m_SchemaMap.find(id);
                if (it != m_SchemaMap.end())
                    return it->second.get();
                return nullptr;
            }

            std::optional<PinFormat> GetPinFormat(const std::string &typeID) const
            {
                auto it = m_PinFormats.find(typeID);
                if (it != m_PinFormats.end())
                    return it->second;
                return std::nullopt;
            }

        private:
            std::string GenerateFullID(const std::string &typeID) const
            {
                return typeID + "@" + m_ContextID;
            }

        public:
            std::unordered_map<std::string, std::unique_ptr<NodeSchema>> m_SchemaMap;
            std::unordered_map<std::string, PinFormat> m_PinFormats;
            bool m_AllowComments = false;
            bool m_AllowSequences = false;
            std::string m_ContextID;
        };

    }
}

#endif // CHERRY_NODES_NODE_CONTEXT