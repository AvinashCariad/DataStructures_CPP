#ifndef GRAPHALGORITHMS_GRAPH_H
#define GRAPHALGORITHMS_GRAPH_H
#include <vector>
#include <unordered_map>
#include "GraphNode.h"
#include "GraphEdge.h"
using namespace std;
//Un-implemented yet AdjacencyList Representation. Need to build conversion to the underlying Edge representation.
typedef unordered_map<GraphNode*, vector<GraphEdge*>> AdjacencyList;


/*
// Following would be a typical implementation for the AdjacencyList 
void Graph::createAdjacencyList()
{
    // Start with an empty adjacency list
    adjacencyList.clear();        // clear the Map

    // Add every vertex, even vertices having no edges.
    for (GraphNode* node : vertices)
    {
        adjacencyList[node] = {};
    }

    // Add all edges.
    for (GraphEdge* edge : edges)
    {
        GraphNode* node1 = edge->getNode1();
        GraphNode* node2 = edge->getNode2();

        // For the directed we need to keep the ordering in mind
        adjacencyList[node1].push_back(edge);

        // For an UNDIRECTED graph, add the edge to both vertices.
        adjacencyList[node2].push_back(edge);
    }
}

*/

//Implemented Conversion from AdjacencyMatrix required representation to the underlying Edge Representation.
typedef vector<vector<int>> AdjacencyMatrix;

// Define the Graph Class:
class Graph {
    AdjacencyMatrix adjacencyMatrix;
    int numberOfVertices;
    vector<GraphEdge*> edges;
    vector<GraphNode*> vertices;
public:

    virtual ~Graph();
    Graph(int numberOfVertices, AdjacencyMatrix adjacencyMatrix);
    void createNodes();
    void createEdges();
    const vector<GraphNode *> &getVertices() const;
    GraphNode* addNode(int index);
    GraphEdge addEdge(GraphNode *node1, GraphNode *node2, int weight);
    
    
    // Define the algorithms that needs to be evaluated:
    vector<GraphEdge>* primAlgorithm(GraphNode* node);
    vector<GraphEdge>* dijkstraAlgorithm(GraphNode* node);
private:
    void printParents(int index,int sourceIndex,int *parentVertices);
    // To compare two edges
    class myComparator
    {
    public:
        int operator() (GraphEdge& edge1, GraphEdge& edge2);

    };
};
#endif //GRAPHALGORITHMS_GRAPH_H
