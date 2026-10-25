#include <iostream>
#include <vector>
#include <queue>

/*
 
Time Complexity: \(\mathcal{O}(V + E)\) where \(V = n\) is the number of vertices
and \(E\) is the number of edges. We visit each vertex and evaluate each edge at most
once during the traversal.Space Complexity: \(\mathcal{O}(V + E)\) to allocate the adjacency
list (adj) matrix structure, along with the overhead required by the tracking vector and the
BFS queue tracking elements at any level limit.
*/

class Solution {
public:
    bool validPath(int n, std::vector<std::vector<int>>& edges, int source, int destination) {
        // Optimization: If source and destination are the same, path trivially exists
        if (source == destination) return true;

        // 1. Build the bidirectional adjacency list
        std::vector<std::vector<int>> adj(n);
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            // Bidirectional graph:
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // 2. Setup BFS structures
        std::queue<int> q;
        std::vector<bool> visited(n, false);

        q.push(source);
        visited[source] = true;

        // 3. Process traversal level-by-level
        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            // If we've reached the destination, a valid path exists
            if (curr == destination) return true;

            // Inspect all connected neighboring vertices
            for (int neighbor : adj[curr]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        // Destination was not reached during the traversal
        return false;
    }
};


//main code harness:
int main(int argc, char** argv) {
    Solution solver;

    // --- TEST CASE 1: Path Exists ---
    // Graph: 0-1, 1-2, 2-0. Source = 0, Destination = 2
    int n1 = 3;
    std::vector<std::vector<int>> edges1 = {{0, 1}, {1, 2}, {2, 0}};
    int source1 = 0, destination1 = 2;
    std::cout << "Test Case 1 (Valid): " 
              << (solver.validPath(n1, edges1, source1, destination1) ? "True" : "False") 
              << " (Expected: True)\n";

    // --- TEST CASE 2: Path Does Not Exist ---
    // Graph: 0-1, 0-2, 3-5, 5-4. Source = 0, Destination = 5
    int n2 = 6;
    std::vector<std::vector<int>> edges2 = {{0, 1}, {0, 2}, {3, 5}, {5, 4}};
    int source2 = 0, destination2 = 5;
    std::cout << "Test Case 2 (Invalid): " 
              << (solver.validPath(n2, edges2, source2, destination2) ? "True" : "False") 
              << " (Expected: False)\n";

    return 0;
}

