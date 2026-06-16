// node_graph.hpp
#ifndef CHERRY_NODES_NODE_GRAPH
#define CHERRY_NODES_NODE_GRAPH

#include <chrono>
#include <fstream>
#include <thread>

#include "./node_context.hpp"
#include "./objects/node_schema.hpp"
using json = nlohmann::json;

namespace Cherry {
  namespace NodeSystem {
    namespace EffectType {
      enum class Connection { Pulsating, Flow };
      enum class Node { Message };
    }  // namespace EffectType

    struct ConnectionEffectParams {
      EffectType::Connection type = EffectType::Connection::Pulsating;

      float pulsatingIntensity = 1.0f;
      float pulsatingRate = 2.0f;
      std::string pulsatingColor = "#FF4444FF";

      float flowIntensity = 1.0f;
      float flowSpeed = 120.0f;
      bool flowReverse = false;
      std::string flowColor = "#44AAFFFF";
    };

    struct ConnectionEffect {
      std::string NodeInstanceIDA;
      std::string PinIDA;
      std::string NodeInstanceIDB;
      std::string PinIDB;
      ConnectionEffectParams params;
    };

    struct NodeEffectParams {
      EffectType::Node type = EffectType::Node::Message;

      std::string messageText = "";
      std::string messageColor = "#FF4444FF";
      std::string textColor = "#FFFFFFFF";
    };

    struct NodeEffect {
      std::string InstanceID;
      NodeEffectParams params;
    };

    struct Vec2 {
      float x = 0.0f;
      float y = 0.0f;
    };

    struct NodeInstance {
      std::string TypeID;
      std::string InstanceID;
      Vec2 Position;
      Vec2 Size;
      nlohmann::json Datas;
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

    struct FocusRequest {
      bool pending = false;
      float x = 0.0f;
      float y = 0.0f;
      float zoom = -1.0f;  // default
    };

    class NodeGraph {
     public:
      NodeGraph() = default;

      void SetGraphFile(const std::string &filePath) {
        m_GraphFile = filePath;
      }

      bool IsNodeGraphEmpty() const {
        return m_InstanciatedNodes.empty();
      }

      void AddNodeInstance(const NodeInstance &instance) {
        m_InstanciatedNodes.push_back(instance);
      }

      void AddConnection(const NodeConnection &connection) {
        m_Connections.push_back(connection);
      }

      void RemoveNodeInstance(const std::string &instanceID) {
        m_InstanciatedNodes.erase(
            std::remove_if(
                m_InstanciatedNodes.begin(),
                m_InstanciatedNodes.end(),
                [&](const NodeInstance &inst) { return inst.InstanceID == instanceID; }),
            m_InstanciatedNodes.end());

        m_Connections.erase(
            std::remove_if(
                m_Connections.begin(),
                m_Connections.end(),
                [&](const NodeConnection &conn) {
                  return conn.NodeInstanceIDA == instanceID || conn.NodeInstanceIDB == instanceID;
                }),
            m_Connections.end());
      }

      void RemoveConnection(
          const std::string &nodeIDA,
          const std::string &pinIDA,
          const std::string &nodeIDB,
          const std::string &pinIDB) {
        m_Connections.erase(
            std::remove_if(
                m_Connections.begin(),
                m_Connections.end(),
                [&](const NodeConnection &conn) {
                  return conn.NodeInstanceIDA == nodeIDA && conn.PinIDA == pinIDA && conn.NodeInstanceIDB == nodeIDB &&
                         conn.PinIDB == pinIDB;
                }),
            m_Connections.end());
      }

      std::optional<std::string> GetNodeLinkedToInputInstanceID(
          const std::string &nodeInstanceID,
          const std::string &inputPinID) const {
        for (const auto &conn : m_Connections) {
          if (conn.NodeInstanceIDB == nodeInstanceID && conn.PinIDB == inputPinID)
            return conn.NodeInstanceIDA;
        }
        return std::nullopt;
      }

      std::optional<std::string> GetNodeLinkedToOutputInstanceID(
          const std::string &nodeInstanceID,
          const std::string &outputPinID) const {
        for (const auto &conn : m_Connections) {
          if (conn.NodeInstanceIDA == nodeInstanceID && conn.PinIDA == outputPinID)
            return conn.NodeInstanceIDB;
        }
        return std::nullopt;
      }

      std::vector<std::string> GetAllNodesLinkedToOutputInstanceID(
          const std::string &nodeInstanceID,
          const std::string &outputPinID) const {
        std::vector<std::string> result;
        for (const auto &conn : m_Connections) {
          if (conn.NodeInstanceIDA == nodeInstanceID && conn.PinIDA == outputPinID) {
            ;
            result.push_back(conn.NodeInstanceIDB);
          }
        }
        return result;
      }

      std::vector<std::string> GetAllNodesLinkedToInputInstanceID(
          const std::string &nodeInstanceID,
          const std::string &inputPinID) const {
        std::vector<std::string> result;
        for (const auto &conn : m_Connections) {
          if (conn.NodeInstanceIDB == nodeInstanceID && conn.PinIDB == inputPinID)
            result.push_back(conn.NodeInstanceIDA);
        }
        return result;
      }

      json DumpGraph(const NodeContext *ctx) const {
        json j;
        j["nodes"] = json::array();
        j["connections"] = json::array();

        for (const auto &node : m_InstanciatedNodes) {
          json nodeJson = { { "TypeID", node.TypeID },
                            { "InstanceID", node.InstanceID },
                            { "Position", { node.Position.x, node.Position.y } },
                            { "Size", { node.Size.x, node.Size.y } },
                            { "Datas", json::object() } };

          if (ctx) {
            std::string baseType = node.TypeID;
            if (auto pos = baseType.find('@'); pos != std::string::npos)
              baseType = baseType.substr(0, pos);

            if (auto schema = ctx->GetSchema(baseType)) {
              for (const auto &pin : schema->m_InputPins) {
                auto handlerIt = g_NodeDataHandlers.find(pin.TypeName);
                if (handlerIt != g_NodeDataHandlers.end()) {
                  if (node.Datas.contains(pin.Name))
                    nodeJson["Datas"][pin.Name] = node.Datas[pin.Name];
                  else if (handlerIt->second.serialize)
                    nodeJson["Datas"][pin.Name] = handlerIt->second.serialize(node);
                }
              }

              for (const auto &pin : schema->m_OutputPins) {
                auto handlerIt = g_NodeDataHandlers.find(pin.TypeName);
                if (handlerIt != g_NodeDataHandlers.end()) {
                  if (node.Datas.contains(pin.Name))
                    nodeJson["Datas"][pin.Name] = node.Datas[pin.Name];
                  else if (handlerIt->second.serialize)
                    nodeJson["Datas"][pin.Name] = handlerIt->second.serialize(node);
                }
              }
            }
          }

          if (nodeJson["Datas"].empty() && !node.Datas.empty())
            nodeJson["Datas"] = node.Datas;

          j["nodes"].push_back(nodeJson);
        }

        for (const auto &conn : m_Connections) {
          j["connections"].push_back(
              { { "NodeInstanceIDA", conn.NodeInstanceIDA },
                { "PinIDA", conn.PinIDA },
                { "NodeInstanceIDB", conn.NodeInstanceIDB },
                { "PinIDB", conn.PinIDB } });
        }

        return j;
      }

      std::function<void(const std::string &instanceID, const std::string &pinName)> m_OnNodeChanged;
      bool SetNodeData(const std::string &instanceID, const std::string &pinName, const json &value) {
        for (auto &node : m_InstanciatedNodes) {
          if (node.InstanceID == instanceID) {
            if (!node.Datas.is_object())
              node.Datas = json::object();
            node.Datas[pinName] = value;

            if (m_OnNodeChanged)
              m_OnNodeChanged(instanceID, pinName);

            return true;
          }
        }
        return false;
      }

      void PopulateGraph(const json &j, const NodeContext *ctx) {
        if (!ctx)
          return;

        m_InstanciatedNodes.clear();
        m_Connections.clear();

        for (const auto &jn : j["nodes"]) {
          if (!jn.contains("TypeID") || !jn.contains("InstanceID") || !jn.contains("Position") || !jn.contains("Size"))
            continue;

          NodeInstance node;
          node.TypeID = jn["TypeID"];
          node.InstanceID = jn["InstanceID"];
          node.Position = { jn["Position"][0], jn["Position"][1] };
          node.Size = { jn["Size"][0], jn["Size"][1] };

          if (jn.contains("Datas") && jn["Datas"].is_object()) {
            if (auto schema = ctx->GetSchema(node.TypeID)) {
              for (const auto &pin : schema->m_InputPins) {
                auto it = g_NodeDataHandlers.find(pin.TypeName);
                if (it != g_NodeDataHandlers.end() && it->second.deserialize) {
                  if (jn["Datas"].contains(pin.Name))
                    it->second.deserialize(node, jn["Datas"][pin.Name]);
                }
              }
              for (const auto &pin : schema->m_OutputPins) {
                auto it = g_NodeDataHandlers.find(pin.TypeName);
                if (it != g_NodeDataHandlers.end() && it->second.deserialize) {
                  if (jn["Datas"].contains(pin.Name))
                    it->second.deserialize(node, jn["Datas"][pin.Name]);
                }
              }
            } else {
              node.Datas = jn["Datas"];
            }
          }

          m_InstanciatedNodes.push_back(std::move(node));
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

      bool DumpGraphToJsonFile(const NodeContext *ctx) const {
        if (m_GraphFile.empty())
          return false;
        std::ofstream out(m_GraphFile);
        if (!out.is_open())
          return false;
        out << DumpGraph(ctx).dump(4);
        return true;
      }

      bool PopulateGraphFromJsonFile(const NodeContext *ctx) {
        if (m_GraphFile.empty())
          return false;
        std::ifstream in(m_GraphFile);
        if (!in.is_open())
          return false;

        try {
          json j;
          in >> j;
          if (!j.contains("nodes") || !j["nodes"].is_array() || !j.contains("connections") || !j["connections"].is_array()) {
            return false;
          }
          PopulateGraph(j, ctx);
        } catch (...) {
          return false;
        }
        return true;
      }

      void AddPossibility(const NodeSpawnPossibility &possibility) {
        auto it = std::find_if(
            m_NodeSpawnPossibilities.begin(), m_NodeSpawnPossibilities.end(), [&](const NodeSpawnPossibility &p) {
              return p.schema_id == possibility.schema_id;
            });
        if (it == m_NodeSpawnPossibilities.end())
          m_NodeSpawnPossibilities.push_back(possibility);
      }

      std::optional<NodeSpawnPossibility> GetPossibility(const std::string &schema_id) const {
        auto it = std::find_if(
            m_NodeSpawnPossibilities.begin(), m_NodeSpawnPossibilities.end(), [&](const NodeSpawnPossibility &p) {
              return p.schema_id == schema_id;
            });
        if (it != m_NodeSpawnPossibilities.end())
          return *it;
        return std::nullopt;
      }

      NodeInstance *GetNodeInstance(const std::string &instanceID) {
        auto it = std::find_if(
            m_InstanciatedNodes.begin(), m_InstanciatedNodes.end(), [&](auto &n) { return n.InstanceID == instanceID; });
        return (it != m_InstanciatedNodes.end()) ? &(*it) : nullptr;
      }

      const NodeInstance *GetNodeInstance(const std::string &instanceID) const {
        auto it = std::find_if(m_InstanciatedNodes.begin(), m_InstanciatedNodes.end(), [&](const auto &n) {
          return n.InstanceID == instanceID;
        });
        return (it != m_InstanciatedNodes.end()) ? &(*it) : nullptr;
      }

      const std::vector<NodeSpawnPossibility> &GetPossibilities() const {
        return m_NodeSpawnPossibilities;
      }

      std::vector<NodeInstance> m_InstanciatedNodes;
      std::vector<NodeConnection> m_Connections;

      std::function<void(const std::string &, float, float, const std::string &)> m_NodeSpawnCallback;

      struct NodeDataHandler {
        using SerializeFn = std::function<json(const NodeInstance &)>;
        using DeserializeFn = std::function<void(NodeInstance &, const json &)>;
        using RenderFn = std::function<void(NodeInstance &)>;

        SerializeFn serialize;
        DeserializeFn deserialize;
        RenderFn render;
      };

      std::map<std::string, NodeDataHandler> g_NodeDataHandlers;

      void AddNodeDataType(
          const std::string &topic,
          NodeDataHandler::SerializeFn serializer,
          NodeDataHandler::DeserializeFn deserializer) {
        g_NodeDataHandlers[topic].serialize = std::move(serializer);
        g_NodeDataHandlers[topic].deserialize = std::move(deserializer);
      }

      void SetRenderCallbackForNodeData(const std::string &topic, NodeDataHandler::RenderFn renderFn) {
        g_NodeDataHandlers[topic].render = std::move(renderFn);
      }
      void AttachRenderCallbackEditorForNodeDataType(const std::string &topic, NodeDataHandler::RenderFn fn) {
        g_NodeDataHandlers[topic].render = fn;
      }

      void AddConnectionEffect(const ConnectionEffect &effect) {
        for (auto &e : m_ConnectionEffects) {
          if (e.NodeInstanceIDA == effect.NodeInstanceIDA && e.PinIDA == effect.PinIDA &&
              e.NodeInstanceIDB == effect.NodeInstanceIDB && e.PinIDB == effect.PinIDB &&
              e.params.type == effect.params.type) {
            e = effect;
            return;
          }
        }
        m_ConnectionEffects.push_back(effect);
      }

      void RemoveConnectionEffect(
          const std::string &nodeIDA,
          const std::string &pinIDA,
          const std::string &nodeIDB,
          const std::string &pinIDB) {
        m_ConnectionEffects.erase(
            std::remove_if(
                m_ConnectionEffects.begin(),
                m_ConnectionEffects.end(),
                [&](const ConnectionEffect &e) {
                  return e.NodeInstanceIDA == nodeIDA && e.PinIDA == pinIDA && e.NodeInstanceIDB == nodeIDB &&
                         e.PinIDB == pinIDB;
                }),
            m_ConnectionEffects.end());
      }

      void AddNodeEffect(const NodeEffect &effect) {
        for (auto &e : m_NodeEffects) {
          if (e.InstanceID == effect.InstanceID && e.params.type == effect.params.type) {
            e = effect;
            return;
          }
        }
        m_NodeEffects.push_back(effect);
      }

      void RemoveNodeEffect(const std::string &instanceID) {
        m_NodeEffects.erase(
            std::remove_if(
                m_NodeEffects.begin(), m_NodeEffects.end(), [&](const NodeEffect &e) { return e.InstanceID == instanceID; }),
            m_NodeEffects.end());
      }

      void ClearAllEffects() {
        m_ConnectionEffects.clear();
        m_NodeEffects.clear();
      }

      void RemoveConnectionEffect(
          const std::string &nodeIDA,
          const std::string &pinIDA,
          const std::string &nodeIDB,
          const std::string &pinIDB,
          EffectType::Connection type) {
        m_ConnectionEffects.erase(
            std::remove_if(
                m_ConnectionEffects.begin(),
                m_ConnectionEffects.end(),
                [&](const ConnectionEffect &e) {
                  return e.NodeInstanceIDA == nodeIDA && e.PinIDA == pinIDA && e.NodeInstanceIDB == nodeIDB &&
                         e.PinIDB == pinIDB && e.params.type == type;
                }),
            m_ConnectionEffects.end());
      }

      void ClearNodeEffects() {
        m_NodeEffects.clear();
      }

      void ClearConnectionEffects() {
        m_ConnectionEffects.clear();
      }

      void SetGraphTitle(const std::string &v) {
        m_GraphTitle = v;
      }

      void FocusAt(float x, float y, float zoom = -1.0f) {
        m_FocusRequest.pending = true;
        m_FocusRequest.x = x;
        m_FocusRequest.y = y;
        m_FocusRequest.zoom = zoom;
      }

      std::vector<ConnectionEffect> m_ConnectionEffects;
      std::vector<NodeEffect> m_NodeEffects;
      std::string m_GraphTitle;
      FocusRequest m_FocusRequest;

     private:
      std::string m_GraphFile;
      std::vector<NodeSpawnPossibility> m_NodeSpawnPossibilities;
    };

  }  // namespace NodeSystem
}  // namespace Cherry

#endif  // CHERRY_NODES_NODE_GRAPH