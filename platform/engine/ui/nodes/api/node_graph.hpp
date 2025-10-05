#ifndef CHERRY_NODES_NODE_GRAPH
#define CHERRY_NODES_NODE_GRAPH

#include "./objects/node_schema.hpp"
#include <chrono> // pour milliseconds
#include <fstream>
#include <thread> // pour sleep_for

using json = nlohmann::json;

namespace Cherry {
namespace NodeSystem {
struct Vec2 {
  float x = 0.0f;
  float y = 0.0f;
};

struct NodeInstance {
  std::string TypeID;
  std::string InstanceID;
  Vec2 Position;
  Vec2 Size;
};

struct NodeConnection {
  std::string NodeInstanceIDA;
  std::string PinIDA;
  std::string NodeInstanceIDB;
  std::string PinIDB;
};

struct NodeSpawnPossibility {
  std::string proper_name;
  std::string proper_description;
  std::string proper_logo;
  std::string category;
  std::string schema_id;
};

class NodeGraph {
public:
  NodeGraph() = default;

  void SetGraphFile(const std::string &filePath) { m_GraphFile = filePath; }

  bool IsNodeGraphEmpty() const { return m_InstanciatedNodes.empty(); }

  void AddNodeInstance(const NodeInstance &instance) {
    m_InstanciatedNodes.push_back(instance);
  }

  void AddConnection(const NodeConnection &connection) {
    m_Connections.push_back(connection);
  }

  void RemoveNodeInstance(const std::string &instanceID) {
    m_InstanciatedNodes.erase(
        std::remove_if(m_InstanciatedNodes.begin(), m_InstanciatedNodes.end(),
                       [&](const NodeInstance &inst) {
                         return inst.InstanceID == instanceID;
                       }),
        m_InstanciatedNodes.end());

    m_Connections.erase(
        std::remove_if(m_Connections.begin(), m_Connections.end(),
                       [&](const NodeConnection &conn) {
                         return conn.NodeInstanceIDA == instanceID ||
                                conn.NodeInstanceIDB == instanceID;
                       }),
        m_Connections.end());
  }

  void RemoveConnection(const std::string &nodeIDA, const std::string &pinIDA,
                        const std::string &nodeIDB, const std::string &pinIDB) {
    m_Connections.erase(
        std::remove_if(m_Connections.begin(), m_Connections.end(),
                       [&](const NodeConnection &conn) {
                         return conn.NodeInstanceIDA == nodeIDA &&
                                conn.PinIDA == pinIDA &&
                                conn.NodeInstanceIDB == nodeIDB &&
                                conn.PinIDB == pinIDB;
                       }),
        m_Connections.end());
  }

  // --- Navigation avec strings ---
  std::optional<std::string>
  GetNodeLinkedToInputInstanceID(const std::string &nodeInstanceID,
                                 const std::string &inputPinID) const {
    for (const auto &conn : m_Connections) {
      if (conn.NodeInstanceIDB == nodeInstanceID && conn.PinIDB == inputPinID)
        return conn.NodeInstanceIDA;
    }
    return std::nullopt;
  }

  std::optional<std::string>
  GetNodeLinkedToOutputInstanceID(const std::string &nodeInstanceID,
                                  const std::string &outputPinID) const {
    for (const auto &conn : m_Connections) {
      if (conn.NodeInstanceIDA == nodeInstanceID && conn.PinIDA == outputPinID)
        return conn.NodeInstanceIDB;
    }
    return std::nullopt;
  }

  std::vector<std::string>
  GetAllNodesLinkedToOutputInstanceID(const std::string &nodeInstanceID,
                                      const std::string &outputPinID) const {
    std::vector<std::string> result;
    for (const auto &conn : m_Connections) {
      if (conn.NodeInstanceIDA == nodeInstanceID &&
          conn.PinIDA == outputPinID) {
        ;
        result.push_back(conn.NodeInstanceIDB);
      }
    }
    return result;
  }

  std::vector<std::string>
  GetAllNodesLinkedToInputInstanceID(const std::string &nodeInstanceID,
                                     const std::string &inputPinID) const {
    std::vector<std::string> result;
    for (const auto &conn : m_Connections) {
      if (conn.NodeInstanceIDB == nodeInstanceID && conn.PinIDB == inputPinID)
        result.push_back(conn.NodeInstanceIDA);
    }
    return result;
  }

  // --- JSON ---
  json DumpGraph() const {
    json j;
    j["nodes"] = json::array();
    j["connections"] = json::array();

    for (const auto &node : m_InstanciatedNodes) {
      j["nodes"].push_back({{"TypeID", node.TypeID},
                            {"InstanceID", node.InstanceID},
                            {"Position", {node.Position.x, node.Position.y}},
                            {"Size", {node.Size.x, node.Size.y}}});
    }

    for (const auto &conn : m_Connections) {
      j["connections"].push_back({{"NodeInstanceIDA", conn.NodeInstanceIDA},
                                  {"PinIDA", conn.PinIDA},
                                  {"NodeInstanceIDB", conn.NodeInstanceIDB},
                                  {"PinIDB", conn.PinIDB}});
    }

    return j;
  }

  void PopulateGraph(const json &j) {
    m_InstanciatedNodes.clear();
    m_Connections.clear();

    for (const auto &jn : j["nodes"]) {
      if (!jn.contains("TypeID") || !jn.contains("InstanceID") ||
          !jn.contains("Position") || !jn.contains("Size"))
        continue;

      NodeInstance node;
      std::string rawType = jn["TypeID"];
      size_t atPos = rawType.find('@');
      node.TypeID =
          (atPos != std::string::npos) ? rawType.substr(0, atPos) : rawType;

      node.InstanceID = jn["InstanceID"];
      node.Position = {jn["Position"][0], jn["Position"][1]};
      node.Size = {jn["Size"][0], jn["Size"][1]};

      m_InstanciatedNodes.push_back(node);
    }

    for (const auto &jc : j["connections"]) {
      NodeConnection conn;
      conn.NodeInstanceIDA = jc["NodeInstanceIDA"];
      conn.PinIDA = jc["PinIDA"];
      conn.NodeInstanceIDB = jc["NodeInstanceIDB"];
      conn.PinIDB = jc["PinIDB"];
      m_Connections.push_back(conn);
    }
  }

  bool DumpGraphToJsonFile() const {
    if (m_GraphFile.empty())
      return false;
    std::ofstream out(m_GraphFile);
    if (!out.is_open())
      return false;
    out << DumpGraph().dump(4);
    return true;
  }

  bool PopulateGraphFromJsonFile() {
    if (m_GraphFile.empty())
      return false;
    std::ifstream in(m_GraphFile);
    if (!in.is_open())
      return false;

    try {
      json j;
      in >> j;
      if (!j.contains("nodes") || !j["nodes"].is_array() ||
          !j.contains("connections") || !j["connections"].is_array()) {
        return false;
      }
      PopulateGraph(j);
    } catch (...) {
      return false;
    }
    return true;
  }

  // --- Spawn Possibilities ---
  void AddPossibility(const NodeSpawnPossibility &possibility) {
    auto it = std::find_if(m_NodeSpawnPossibilities.begin(),
                           m_NodeSpawnPossibilities.end(),
                           [&](const NodeSpawnPossibility &p) {
                             return p.schema_id == possibility.schema_id;
                           });
    if (it == m_NodeSpawnPossibilities.end())
      m_NodeSpawnPossibilities.push_back(possibility);
  }

  std::optional<NodeSpawnPossibility>
  GetPossibility(const std::string &schema_id) const {
    auto it = std::find_if(m_NodeSpawnPossibilities.begin(),
                           m_NodeSpawnPossibilities.end(),
                           [&](const NodeSpawnPossibility &p) {
                             return p.schema_id == schema_id;
                           });
    if (it != m_NodeSpawnPossibilities.end())
      return *it;
    return std::nullopt;
  }

  const std::vector<NodeSpawnPossibility> &GetPossibilities() const {
    return m_NodeSpawnPossibilities;
  }

  std::vector<NodeInstance> m_InstanciatedNodes;
  std::vector<NodeConnection> m_Connections;

  std::function<void(const std::string &, float, float, const std::string &)>
      m_NodeSpawnCallback;

private:
  std::string m_GraphFile;
  std::vector<NodeSpawnPossibility> m_NodeSpawnPossibilities;
};

} // namespace NodeSystem
} // namespace Cherry

#endif // CHERRY_NODES_NODE_GRAPH