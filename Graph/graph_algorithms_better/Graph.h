#ifndef GRAPHALGORITHMS_GRAPH_H
#define GRAPHALGORITHMS_GRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <functional>
#include <iostream>

#include "GraphNode.h"
#include "GraphEdge.h"

using namespace std;

typedef unordered_map<GraphNode*, vector<GraphEdge*>> AdjacencyList;
typedef vector<vector<int>> AdjacencyMatrix;

class Graph {

private:

    AdjacencyMatrix adjacencyMatrix;
    AdjacencyList adjacencyList;

    int numberOfVertices;
    bool directed;

    vector<GraphEdge*> edges;
    vector<GraphNode*> vertices;

public:

    virtual ~Graph();

    Graph(int numberOfVertices,
          AdjacencyMatrix adjacencyMatrix,
          bool directed = false);

    // --------------------------------------------------
    // Graph construction
    // --------------------------------------------------

    void createNodes();
    void createEdges();
    void createAdjacencyList();

    GraphNode* addNode(int index);

    GraphEdge* addEdge(GraphNode* node1,
                       GraphNode* node2,
                       int weight);

    const vector<GraphNode*>& getVertices() const;
    const vector<GraphEdge*>& getEdges() const;
    const AdjacencyList& getAdjacencyList() const;

    // --------------------------------------------------
    // Traversal
    // --------------------------------------------------

    vector<int> bfs(GraphNode* start);
    vector<int> dfs(GraphNode* start);

    // --------------------------------------------------
    // Connectivity
    // --------------------------------------------------

    int connectedComponents();
    bool pathExists(GraphNode* source,
                    GraphNode* destination);

    // --------------------------------------------------
    // Cycle detection
    // --------------------------------------------------

    bool hasCycleUndirected();
    bool hasCycleDirected();

    // --------------------------------------------------
    // Topological sorting
    // --------------------------------------------------

    vector<int> topologicalSortKahn();
    vector<int> topologicalSortDFS();

    // --------------------------------------------------
    // Shortest paths
    // --------------------------------------------------

    vector<int> shortestPathUnweighted(GraphNode* source);

    vector<int> dijkstra(GraphNode* source);

    vector<int> bellmanFord(GraphNode* source);

    vector<vector<int>> floydWarshall();

    // --------------------------------------------------
    // Minimum Spanning Tree
    // --------------------------------------------------

    vector<GraphEdge*> primMST(GraphNode* start);

    vector<GraphEdge*> kruskalMST();

    // --------------------------------------------------
    // Other useful graph algorithms
    // --------------------------------------------------

    bool isBipartite();

    vector<int> articulationPoints();

    vector<pair<int,int>> bridges();

private:

    // --------------------------------------------------
    // Helpers
    // --------------------------------------------------

    GraphNode* getOtherNode(GraphEdge* edge,
                            GraphNode* node) const;

    void dfsHelper(GraphNode* node,
                   unordered_set<GraphNode*>& visited,
                   vector<int>& result);

    bool cycleUndirectedDFS(
            GraphNode* node,
            GraphNode* parent,
            unordered_set<GraphNode*>& visited);

    bool cycleDirectedDFS(
            GraphNode* node,
            unordered_map<GraphNode*, int>& state);

    void topoDFS(
            GraphNode* node,
            unordered_set<GraphNode*>& visited,
            stack<int>& result);

    void articulationDFS(
            GraphNode* node,
            GraphNode* parent,
            int& timer,
            unordered_map<GraphNode*, int>& discovery,
            unordered_map<GraphNode*, int>& low,
            unordered_set<GraphNode*>& visited,
            unordered_set<int>& result);

    void bridgeDFS(
            GraphNode* node,
            GraphNode* parent,
            int& timer,
            unordered_map<GraphNode*, int>& discovery,
            unordered_map<GraphNode*, int>& low,
            unordered_set<GraphNode*>& visited,
            vector<pair<int,int>>& result);
};

#endif
