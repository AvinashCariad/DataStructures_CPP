#ifndef GRAPHALGORITHMS_GRAPHNODE_H
#define GRAPHALGORITHMS_GRAPHNODE_H
#include <vector>


//resolving a circular dependency by calling class GraphEdge instead of include "GraphEdge.h"
class GraphEdge;
using namespace std;
class GraphNode {
    //Each GraphNode holds a vector of edges that it is connect with.
    vector<GraphEdge*> edges;
    //node index that recognizes the node.
    int nodeIndex;
public:
    GraphNode(int nodeIndex);
    vector<GraphEdge *> &getEdges() ;
    void setEdges(const vector<GraphEdge *> &edges);
    int getNodeIndex() ;
};
#endif //GRAPHALGORITHMS_GRAPHNODE_H


#ifndef GRAPHALGORITHMS_GRAPHEDGE_H
#define GRAPHALGORITHMS_GRAPHEDGE_H

#include <array>
//#include "GraphNode.h"

using namespace std;
class GraphEdge {
private:
    //weight of the edge.
    int weight;
    // Node 1 is the output node.
    // Node 2 is the input node.
    GraphNode *Node1,*Node2;
public:
    GraphEdge( int weight,  GraphNode *Node1,  GraphNode *Node2);
    //Simple Getter methods for variables.
    int getWeight() ;
    GraphNode *getNode1();
    GraphNode *getNode2();
    //Operator Over-ride in order to check if 2 edges are equal we check all the possible combinations of nodes.
    bool operator ==(GraphEdge *edge);
};
#endif //GRAPHALGORITHMS_GRAPHEDGE_H


