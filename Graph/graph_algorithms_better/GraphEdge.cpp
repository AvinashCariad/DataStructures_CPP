#include "GraphEdge.h"


// define graph edge:
GraphEdge::GraphEdge(int weight, GraphNode *Node1, GraphNode *Node2) {
    this->Node1 = Node1;
    this->Node2 = Node2;
    this->weight = weight;
}



//Operator Over-ride in order to check if 2 edges are equal we check all the possible combinations of nodes.
bool GraphEdge::operator==(GraphEdge *edge) {
    if (edge->Node1 == Node1) {
        if (edge->Node2 == Node2)
            return true;
    } else if (edge->Node1 == Node2) {
        if (edge->Node2 == Node1)
            return true;
    } else if (edge->Node2 == Node2) {
        if (edge->Node1 == Node1)
            return true;
    } else if (edge->Node2 == Node1) {
        if (edge->Node1 == Node2)
            return true;
    }


    return false;
}


// get weight:
int GraphEdge::getWeight()  {
    return weight;
}


// get node:
GraphNode *GraphEdge::getNode1()  {
    return Node1;
}


// get another node:
GraphNode *GraphEdge::getNode2()  {
    return Node2;
}
