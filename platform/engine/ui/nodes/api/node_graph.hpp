#ifndef CHERRY_NODES_NODE_GRAPH
#define CHERRY_NODES_NODE_GRAPH

#include "./objects/node_schema.hpp"

namespace Cherry
{
    namespace NodeSystem
    {
        struct Vec2
        {
            float x = 0.0f;
            float y = 0.0f;
        };

        struct NodeInstance
        {
            std::string TypeID;
            std::string InstanceID;
            Vec2 Position;
        };

        struct NodeConnection
        {
            std::string NodeInstanceIDA;
            std::string PinIDA;

            std::string NodeInstanceIDB;
            std::string PinIDB;
        };

        class NodeGraph
        {
        public:
            NodeGraph() = default;

            void AddNodeInstance(const NodeInstance &instance)
            {
                m_InstanciatedNodes.push_back(instance);
            }

            void AddConnection(const NodeConnection &connection)
            {
                m_Connections.push_back(connection);
            }

            void RemoveNodeInstance(const std::string &instanceID)
            {
                m_InstanciatedNodes.erase(
                    std::remove_if(m_InstanciatedNodes.begin(), m_InstanciatedNodes.end(),
                                   [&](const NodeInstance &inst)
                                   { return inst.InstanceID == instanceID; }),
                    m_InstanciatedNodes.end());

                m_Connections.erase(
                    std::remove_if(m_Connections.begin(), m_Connections.end(),
                                   [&](const NodeConnection &conn)
                                   {
                                       return conn.NodeInstanceIDA == instanceID || conn.NodeInstanceIDB == instanceID;
                                   }),
                    m_Connections.end());
            }

            void RemoveConnection(const std::string &nodeIDA, const std::string &pinIDA,
                                  const std::string &nodeIDB, const std::string &pinIDB)
            {
                m_Connections.erase(
                    std::remove_if(m_Connections.begin(), m_Connections.end(),
                                   [&](const NodeConnection &conn)
                                   {
                                       return conn.NodeInstanceIDA == nodeIDA &&
                                              conn.PinIDA == pinIDA &&
                                              conn.NodeInstanceIDB == nodeIDB &&
                                              conn.PinIDB == pinIDB;
                                   }),
                    m_Connections.end());
            }

            std::vector<NodeInstance> m_InstanciatedNodes;
            std::vector<NodeConnection> m_Connections;
        };
    }
}

#endif // CHERRY_NODES_NODE_GRAPH