#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
Given an undirected, connected and weighted graph, find Minimum Spanning Tree (MST) of the graph using Kruskal's algorithm.
*/



// Structure to represent an edge in the graph
struct Edge {
    int src, dest, weight;
};

// Comparator function to sort edges by weight in non-decreasing order
bool compareEdges(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

// A standard DFS helper function to see if 'dest' is reachable from 'src'
bool hasPath(int src, int dest, int parent, const vector<vector<int>>& mstAdj) {
    if (src == dest) return true;

    for (int neighbor : mstAdj[src]) {
        if (neighbor != parent) { // Avoid going back directly to the caller
            if (hasPath(neighbor, dest, src, mstAdj)) {
                return true;
            }
        }
    }
    return false;
}



// Kruskal without DSU:
void kruskalWithoutDSU(int numVertices, vector<Edge>& edges) {
    // 1. Sort all edges in non-decreasing order of their weight
    sort(edges.begin(), edges.end(), compareEdges);

    // This adjacency list tracks the structural connections inside our growing MST
    vector<vector<int>> mstAdj(numVertices);
    vector<Edge> mstResult;
    int totalMSTWeight = 0;

    // 2. Iterate through the sorted edges
    for (const auto& edge : edges) {
        // Check if adding this edge creates a cycle. 
        // If a path already exists between src and dest in our current MST, adding this edge forms a cycle.
        if (!hasPath(edge.src, edge.dest, -1, mstAdj)) {
            // No cycle detected! Safe to add to MST
            mstAdj[edge.src].push_back(edge.dest);
            mstAdj[edge.dest].push_back(edge.src);
            mstResult.push_back(edge);
            totalMSTWeight += edge.weight;
        }

        // Optimization: Stop early if we have collected V - 1 edges
        if (mstResult.size() == numVertices - 1) {
            break;
        }
    }

    // Print out results
    cout << "Edges in the constructed MST:\n";
    for (const auto& edge : mstResult) {
        cout << edge.src << " -- " << edge.dest << " == " << edge.weight << "\n";
    }
    cout << "Minimum Spanning Tree Total Weight: " << totalMSTWeight << "\n";
}


// main code harness:
int main(int argc, char** argv) {
    int numVertices = 4;
    
    // Graph representation (Edge list)
    vector<Edge> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    kruskalWithoutDSU(numVertices, edges);

    return 0;
}
