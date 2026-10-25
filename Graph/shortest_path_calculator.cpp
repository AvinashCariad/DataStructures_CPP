#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <limits>


// define Graph class:
class Graph {
private:
    // Adjacency list storing pairs of {neighbor_node, edge_cost}
    std::vector<std::vector<std::pair<int, int>>> adjList;
    int numNodes;

public:
    // Initializes the object with n nodes and the given edges
    Graph(int n, std::vector<std::vector<int>>& edges) {
        this->numNodes = n;
        adjList.resize(n);
        for (const auto& edge : edges) {
            int from = edge[0];
            int to = edge[1];
            int cost = edge[2];
            adjList[from].push_back({to, cost});        // note this is the defn of directed graph
        }
    }
    
    // Adds a directed edge to the graph - note how the adj list is expanded when additional edge is provided
    void addEdge(std::vector<int> edge) {
        int from = edge[0];
        int to = edge[1];
        int cost = edge[2];
        adjList[from].push_back({to, cost});
    }
    
    // Finds the minimum path cost using Dijkstra's Algorithm - between node1 and node2
    int shortestPath(int node1, int node2) {
        // Track the absolute minimum cost to reach each node from node1
        std::vector<int> dist(numNodes, INT_MAX);
        dist[node1] = 0;

        // Min-heap storing pairs of {current_accumulated_cost, node} - note the pair being stored
        std::priority_queue<std::pair<int, int>, 
                            std::vector<std::pair<int, int>>, 
                            std::greater<std::pair<int, int>>> pq;           // note how the pair is being defined
        
        pq.push({0, node1});

        while (!pq.empty()) {
            auto [currentCost, u] = pq.top();
            pq.pop();

            // Optimization: If we reached our target destination, return immediately
            if (u == node2) return currentCost;

            // Skip processing if a cheaper path to node 'u' has already been explored
            if (currentCost > dist[u]) continue;

            // Relax neighboring edges
            for (const auto& neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (currentCost + weight < dist[v]) {
                    dist[v] = currentCost + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        // Return -1 if node2 is entirely unreachable from node1
        return dist[node2] == INT_MAX ? -1 : dist[node2];
    }
};


// main code harness:
int main(int argc, char** argv) {
    // Define initial setup configuration: 4 nodes
    int n = 4;
    std::vector<std::vector<int>> edges = {{0, 2, 5}, {0, 1, 2}, {1, 2, 1}, {2, 3, 3}};
    
    // 1. Initialize calculator utility on [LeetCode 2642] criteria
    Graph* g = new Graph(n, edges);
    
    // 2. Query initial shortest path from 0 to 3
    // Path: 0 -> 1 -> 2 -> 3 (Cost: 2 + 1 + 3 = 6)
    std::cout << "Shortest path (0 to 3): " << g->shortestPath(0, 3) << " (Expected: 6)\n";
    
    // 3. Query an impossible path configuration
    std::cout << "Shortest path (3 to 0): " << g->shortestPath(3, 0) << " (Expected: -1)\n";
    
    // 4. Dynamically insert a shortcut edge directly linking 1 to 3
    std::cout << "Adding direct shortcut edge from 1 to 3 with cost 1...\n";
    g->addEdge({1, 3, 1});
    
    // 5. Query path from 0 to 3 again with the updated topology
    // New Path: 0 -> 1 -> 3 (Cost: 2 + 1 = 3)
    std::cout << "New shortest path (0 to 3): " << g->shortestPath(0, 3) << " (Expected: 3)\n";

    delete g;
    return 0;
}
