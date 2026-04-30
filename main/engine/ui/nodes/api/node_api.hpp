#ifndef CHERRY_NODES_NODE_API
#define CHERRY_NODES_NODE_API

#include "./node_context.hpp"
#include "./node_graph.hpp"

namespace Cherry
{
    namespace NodeSystem
    {
        // SetNodeProperty(ctx, graph, prop) -> Set internal property of a node (like a value, text, etc)
        // AddLink(ctx, graph) -> Add a link betwen 2 nodes on a graph
        // RemoveLink(ctx, graph) -> Add a link betwen 2 nodes on a graph
        // SpawnNode(ctx, graph, vec2) -> Spawn a node on a graph with 2D pos
        // PopulateGraphFromJson(ctx, graph, json) -> contruct/reconstruct a graph from a json formatted payload
        // DumpGraphToJson(ctx, graph) -> Dump actual graph to json payload 
        
    }
}

#endif // CHERRY_NODES_NODE_API