#ifndef CHERRY_NODES_NODE_CONTEXT
#define CHERRY_NODES_NODE_CONTEXT

#include "./objects/node_schema.hpp"

namespace Cherry
{

    namespace NodeSystem
    {
        class NodeContext
        {
        public:
            NodeContext() = default;
            NodeSchema *CreateSchema(const std::string &typeID)
            {
                std::string id = typeID + "@" + m_ContextID;
                m_NodeSchemas.emplace_back(id);
                NodeSchema *schema = &m_NodeSchemas.back();
                m_SchemaMap[id] = schema;
                return schema;
            }

            NodeSchema *GetSchema(const std::string &typeID)
            {
                std::string id = typeID + "@" + m_ContextID;
                auto it = m_SchemaMap.find(id);
                if (it != m_SchemaMap.end())
                    return it->second;
                return nullptr;
            }

            std::vector<NodeSchema> m_NodeSchemas;
            std::unordered_map<std::string, NodeSchema *> m_SchemaMap;
            bool m_AllowComments;
            bool m_AllowSequences;
            std::string m_ContextID;
        };
    }
}

#endif // CHERRY_NODES_NODE_CONTEXT