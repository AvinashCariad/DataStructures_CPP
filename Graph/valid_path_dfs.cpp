#include <iostream>
#include <vector>

/*
Time Complexity: \(\mathcal{O}(V + E)\) where \(V = n\) is the number of vertices
and \(E\) is the number of edges. In the worst-case scenario, the algorithm inspects
every node and traverses all available edges to check connectivity.Space Complexity:
\(\mathcal{O}(V + E)\). The allocation of the graph adjacency list requires \(\mathcal{O}(V + E)\) memory space.
The explicit tracker array and the implicit recursive system call stack space scale linearly up to \(\mathcal{O}(V)\)
 deep if the graph layout resembles a single line.

*/


class Solution {
private:
    // Helper function to perform recursive DFS traversal
    bool dfs(int curr, int destination, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited) {
        // Base case: If we reached the destination, a path exists
        if (curr == destination) return true;
        
        // Mark the current node as visited
        visited[curr] = true;
        
        // Recursively check all unvisited neighbors
        for (int neighbor : adj[curr]) {
            
            if (!visited[neighbor]) {
                if (dfs(neighbor, destination, adj, visited)) {
                    return true;
                }
            }
        }
        
        return false;
    }

public:
    bool validPath(int n, std::vector<std::vector<int>>& edges, int source, int destination) {
        // Optimization: If source and destination are identical, path trivially exists
        if (source == destination) return true;

        // 1. Build the bidirectional adjacency list - note how the adjacency list is created using the edges
        std::vector<std::vector<int>> adj(n);
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // 2. Track visited nodes to handle cyclic dependencies
        std::vector<bool> visited(n, false);

        // 3. Initiate the DFS traversal from the source node
        return dfs(source, destination, adj, visited);
    }
};


// main code harness:
int main(int argc, char** argv) {
    Solution solver;

    // --- TEST CASE 1: Path Exists ---
    int n1 = 3;
    std::vector<std::vector<int>> edges1 = {{0, 1}, {1, 2}, {2, 0}};
    int source1 = 0, destination1 = 2;
    std::cout << "Test Case 1 (Valid): " 
              << (solver.validPath(n1, edges1, source1, destination1) ? "True" : "False") 
              << " (Expected: True)\n";

    // --- TEST CASE 2: Path Does Not Exist ---
    int n2 = 6;
    std::vector<std::vector<int>> edges2 = {{0, 1}, {0, 2}, {3, 5}, {5, 4}};
    int source2 = 0, destination2 = 5;
    std::cout << "Test Case 2 (Invalid): " 
              << (solver.validPath(n2, edges2, source2, destination2) ? "True" : "False") 
              << " (Expected: False)\n";

    return 0;
}
