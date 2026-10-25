#include <iostream>
#include <vector>
#include <climits>

// Structure to represent a weighted edge in the graph
struct Edge {
    int src, dest, weight;
};

/**
 * Implements the Bellman-Ford algorithm.
 * @param V Number of vertices in the graph
 * @param edges Vector containing all edges of the graph
 * @param src The source vertex
 * @return A vector of shortest distances from src to all other vertices.
 *         Returns an empty vector if a negative-weight cycle is detected.
 
Complexity:Time Complexity: \(\mathcal{O}(V \times E)\) because it processes 
all edges \(V-1\) times.Space Complexity: \(\mathcal{O}(V)\)
to store the array tracking shortest distances

The Bellman-Ford algorithm is a single-source shortest path algorithm that can 
handle graphs with negative edge weights, unlike Dijkstra's algorithm. 
It also excels at detecting negative weight cycles

Below is a clean, production-ready C++ implementation designed similarly 
to popular competitive programming and GitHub interview repositories


*/
std::vector<int> bellmanFord(int V, const std::vector<Edge>& edges, int src) {
    // Initialize distances from src to all other vertices as infinite (INT_MAX)
    std::vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    // Step 1: Relax all edges |V| - 1 times.
    // A shortest path from src to any other vertex can have at most |V| - 1 edges.
    for (int i = 1; i <= V - 1; ++i) {
        for (const auto& edge : edges) {
            int u = edge.src;
            int v = edge.dest;
            int weight = edge.weight;
            
            // If the source vertex has a valid distance, relax the edge
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    // Step 2: Check for negative-weight cycles.
    // If we can still relax an edge, then a negative cycle exists.
    for (const auto& edge : edges) {
        int u = edge.src;
        int v = edge.dest;
        int weight = edge.weight;
        
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            // Negative cycle detected!
            return {}; 
        }
    }

    return dist;
}



// main code harness:
int main(int argc, char** argv) {
    int V = 5; // Number of vertices
    int E = 8; // Number of edges

    // Example graph represented as an edge list
    std::vector<Edge> edges = {
        {0, 1, -1}, {0, 2, 4},  {1, 2, 3}, {1, 3, 2},
        {1, 4, 2},  {3, 2, 5},  {3, 1, 1}, {4, 3, -3}
    };

    int source = 0;
    std::vector<int> distances = bellmanFord(V, edges, source);

    if (distances.empty()) {
        std::cout << "Graph contains a negative-weight cycle! Shortest paths cannot be determined.\n";
    } else {
        std::cout << "Vertex Distance from Source " << source << ":\n";
        for (int i = 0; i < V; ++i) {
            std::cout << i << " \t\t " << (distances[i] == INT_MAX ? -1 : distances[i]) << "\n";
        }
    }

    return 0;
}

