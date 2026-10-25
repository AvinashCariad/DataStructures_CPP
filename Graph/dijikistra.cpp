#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Define a shortcut for a pair of integers: {weight, neighbor_node}
typedef pair<int, int> iPair;

// Graph:
class Graph {
    int V; // Number of vertices
    vector<vector<iPair>> adj; // Adjacency list: adj[u] stores pairs of {v, weight}

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Function to add a directed or undirected edge to the graph
    void addEdge(int u, int v, int w) {
        adj[u].push_back({w, v}); // Store {weight, neighbor} for sorting convenience
        adj[v].push_back({w, u}); // Comment out this line if the graph is directed
    }

    // Dijkstra's algorithm logic
    void dijkstra(int src) {
        // Step 1: Create a min-heap priority queue
        // Stores pairs of {distance, vertex}. Sorted by the first element (distance).
        priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

        // Step 2: Initialize distance array with infinity (INT_MAX)
        vector<int> dist(V, INT_MAX);

        // Step 3: Insert source vertex into priority queue and set its distance to 0
        pq.push({0, src});
        dist[src] = 0;

        // Step 4: Process the queue until it's empty
        while (!pq.empty()) {
            // Extract the vertex with the minimum distance
            int u = pq.top().second;
            int current_d = pq.top().first;    // current cost
            pq.pop();

            // Skip processing if we already found a shorter path to u
            if (current_d > dist[u]) continue;

            // Traverse all adjacent neighbors of u
            for (auto const& neighbor : adj[u]) {
                int weight = neighbor.first;
                int v = neighbor.second;

                // Step 5: Edge Relaxation
                if (dist[v] > dist[u] + weight) {
                    // Update shortest path to v
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        // Print the calculated shortest distances
        printSolution(src, dist);
    }

private:
    void printSolution(int src, const vector<int>& dist) {
        cout << "Shortest distances from source node " << src << ":\n";
        cout << "Vertex\tDistance from Source\n";
        for (int i = 0; i < V; ++i) {
            cout << i << "\t\t";
            if (dist[i] == INT_MAX) cout << "INF\n";
            else cout << dist[i] << "\n";
        }
    }
};


// main code harness:
int main(int argc, char** argv) {
    // Create a graph with 5 vertices (numbered 0 to 4)
    Graph g(5);

    // Add edges: addEdge(u, v, weight): 
    g.addEdge(0, 1, 9);
    g.addEdge(0, 2, 6);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 3);
    g.addEdge(2, 3, 1);
    g.addEdge(3, 4, 7);

    // Run Dijkstra's algorithm starting from node 0
    g.dijkstra(0);

    return 0;
}

