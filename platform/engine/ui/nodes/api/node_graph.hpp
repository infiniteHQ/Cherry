#ifndef CHERRY_NODES_NODE_GRAPH
#define CHERRY_NODES_NODE_GRAPH

#include <fstream>
#include "./objects/node_schema.hpp"
#include <thread> // pour sleep_for
#include <chrono> // pour milliseconds

using json = nlohmann::json;

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
            Vec2 Size;
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

            void SetGraphFile(const std::string &filePath)
            {
                m_GraphFile = filePath;
            }

            bool IsNodeGraphEmpty()
            {
                return m_InstanciatedNodes.empty();
            }

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

            json DumpGraph()
            {

                json j;
                j["nodes"] = json::array();
                j["connections"] = json::array();

                for (const auto &node : m_InstanciatedNodes)
                {
                    j["nodes"].push_back({{"TypeID", node.TypeID},
                                          {"InstanceID", node.InstanceID},
                                          {"Position", {node.Position.x, node.Position.y}},
                                          {"Size", {node.Size.x, node.Size.y}}});
                }

                for (const auto &conn : m_Connections)
                {
                    j["connections"].push_back({{"NodeInstanceIDA", conn.NodeInstanceIDA},
                                                {"PinIDA", conn.PinIDA},
                                                {"NodeInstanceIDB", conn.NodeInstanceIDB},
                                                {"PinIDB", conn.PinIDB}});
                }

                return j;
            }

            void PopulateGraph(const json &j)
            {
                m_InstanciatedNodes.clear();
                m_Connections.clear();

                for (const auto &jn : j["nodes"])
                {
                    if (!jn.contains("TypeID") || !jn.contains("InstanceID") || !jn.contains("Position") || !jn.contains("Size"))
                        continue;

                    NodeInstance node;

                    std::string rawType = jn["TypeID"]; // tid
                    size_t atPos = rawType.find('@');
                    node.TypeID = (atPos != std::string::npos) ? rawType.substr(0, atPos) : rawType;

                    node.InstanceID = jn["InstanceID"]; // iid
                    node.Position = {jn["Position"][0], jn["Position"][1]}; // pos
                    node.Size = {jn["Size"][0], jn["Size"][1]}; // s 

                    m_InstanciatedNodes.push_back(node);
                }

                for (const auto &jc : j["connections"])
                {
                    NodeConnection conn;
                    conn.NodeInstanceIDA = jc["NodeInstanceIDA"]; // niida
                    conn.PinIDA = jc["PinIDA"]; // pida
                    conn.NodeInstanceIDB = jc["NodeInstanceIDB"]; // niidb
                    conn.PinIDB = jc["PinIDB"]; // pidb
                    m_Connections.push_back(conn);
                }
            }

            bool DumpGraphToJsonFile()
            {
                if (m_GraphFile.empty())
                    return false;

                std::ofstream out(m_GraphFile);
                if (!out.is_open())
                    return false;

                out << DumpGraph().dump(4); // Indented for readability
                return true;
            }

            bool PopulateGraphFromJsonFile()
            {
                if (m_GraphFile.empty())
                    return false;

                std::ifstream in(m_GraphFile);
                if (!in.is_open())
                    return false;

                try
                {
                    json j;
                    in >> j;

                    if (!j.contains("nodes") || !j["nodes"].is_array() ||
                        !j.contains("connections") || !j["connections"].is_array())
                    {
                        return false;
                    }

                    PopulateGraph(j);
                }
                catch (const std::exception &e)
                {
                    return false;
                }

                return true;
            }
            std::vector<NodeInstance> m_InstanciatedNodes;
            std::vector<NodeConnection> m_Connections;

        private:
            std::string m_GraphFile;
        };
    }
}

#endif // CHERRY_NODES_NODE_GRAPH