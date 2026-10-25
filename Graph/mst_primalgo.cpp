#include <iostream>
#include <vector>
#include <queue>
#include <climits>

class PrimMST {
public:
    void findMST(int numVertices, const std::vector<std::vector<std::pair<int, int>>>& adj) {
        // Min-heap to store pairs of {edge_weight, destination_node}
        // Elements are ordered by edge weight in ascending order
        std::priority_queue<std::pair<int, int>, 
                            std::vector<std::pair<int, int>>, 
                            std::greater<std::pair<int, int>>> pq;

        // Tracks nodes currently included in the MST
        std::vector<bool> inMST(numVertices, false);

        // Tracks the minimum weight to connect a node to the MST
        std::vector<int> minKey(numVertices, INT_MAX);

        // Tracks the parent of each node to reconstruct the MST paths
        std::vector<int> parent(numVertices, -1);

        // Initialize by pushing the root node (0) with a weight of 0
        int startNode = 0;
        pq.push({0, startNode});
        minKey[startNode] = 0;

        int totalMSTWeight = 0;

        while (!pq.empty()) {
            // Extract the vertex with the minimum edge weight connection
            auto [weight, u] = pq.top();
            pq.pop();

            // Skip processing if the node is already part of the MST
            if (inMST[u]) continue;

            // Include vertex u into the MST
            inMST[u] = true;
            totalMSTWeight += weight;

            // Iterate over all neighbors of vertex u
            for (const auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int edgeWeight = neighbor.second;

                // Relaxation condition: if v is not yet in MST and this edge 
                // is cheaper than the existing recorded connection path to v
                if (!inMST[v] && edgeWeight < minKey[v]) {
                    minKey[v] = edgeWeight;
                    parent[v] = u;
                    pq.push({edgeWeight, v});
                }
            }
        }

        // Output the resulting MST configuration
        std::cout << "Edges in the constructed MST:\n";
        for (int i = 1; i < numVertices; ++i) {
            if (parent[i] != -1) {
                std::cout << parent[i] << " -- " << i << " == " << minKey[i] << "\n";
            }
        }
        std::cout << "Minimum Spanning Tree Total Weight: " << totalMSTWeight << "\n";
    }
};




// main code harness:
int main(int argc, char** argv) {
    int numVertices = 4;
    
    // Adjacency list storing pairs of {neighbor_node, edge_weight}
    std::vector<std::vector<std::pair<int, int>>> adj(numVertices);
    
    // Representing an undirected, connected graph matching the Kruskal example:
    // 0 - 1 (weight 10), 0 - 2 (weight 6), 0 - 3 (weight 5), 1 - 3 (weight 15), 2 - 3 (weight 4)
    adj[0].push_back({1, 10});
    adj[1].push_back({0, 10});

    adj[0].push_back({2, 6});
    adj[2].push_back({0, 6});

    adj[0].push_back({3, 5});
    adj[3].push_back({0, 5});

    adj[1].push_back({3, 15});
    adj[3].push_back({1, 15});

    adj[2].push_back({3, 4});
    adj[3].push_back({2, 4});

    PrimMST solver;
    solver.findMST(numVertices, adj);

    return 0;
}

