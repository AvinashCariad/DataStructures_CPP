#include <iostream>
#include <vector>
#include <queue>

/*

To solve the Reorder Routes to Make All Paths Lead to the City Zero problem (LeetCode 1466) in C++, 
you treat the network as a tree structure rooted at city 0.Since the original graph is a directed tree,
changing the directions of paths to make everything flow toward city 0 is equivalent to running a 
traversal (DFS or BFS) starting from 0. As we move outward from 0 to its neighbors, any original
edge pointing away from 0 (i.e., matching the direction of our traversal) represents a road that
must be flipped.To traverse the graph in both directions, we build an undirected adjacency list but
tag each edge with a boolean flag (true if the edge exists in the original graph, and false if it is
a simulated reverse edge).

Time Complexity: \(\mathcal{O}(V + E)\) where \(V = n\) is the number of cities and \(E = n - 1\) 
is the number of connections. We look at every node and its incident edges exactly once during the
level-order traversal.Space Complexity: \(\mathcal{O}(V + E)\) to maintain the bidirectional adjacency list
and the visited array mapping states across all cities.

*/

// define the class solutions:
class Solution {
public:
    int minReorder(int n, std::vector<std::vector<int>>& connections) {
        // Adjacency list storing: {neighbor, is_original_direction}
        std::vector<std::vector<std::pair<int, bool>>> adj(n);
        
        for (const auto& conn : connections) {
            int u = conn[0];
            int v = conn[1];
            // Original direction u -> v (needs reordering if we go u -> v from root 0)
            adj[u].push_back({v, true});
            // Virtual reverse direction v -> u (already correctly leading towards root 0)
            adj[v].push_back({u, false});
        }
        
        int changeCount = 0;
        std::vector<bool> visited(n, false);
        std::queue<int> q;
        
        // Start BFS traversal from city 0
        q.push(0);
        visited[0] = true;
        
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            
            for (const auto& edge : adj[curr]) {
                int neighbor = edge.first;
                bool isOriginal = edge.second;
                
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    // If the original edge points AWAY from 0 (curr -> neighbor),
                    // we must flip it so that it points TOWARD 0 (neighbor -> curr).
                    if (isOriginal) {
                        changeCount++;
                    }
                    q.push(neighbor);
                }
            }
        }
        
        return changeCount;
    }
};


// main code harness:
int main(int argc, char** argv) {
    Solution solver;

    // --- TEST CASE 1 ---
    // Connections: 0->1, 1->3, 2->3, 4->0, 4->5
    // To make all paths lead to 0:
    // 0->1 must be 1->0 (Flip)
    // 1->3 must be 3->1 (Flip)
    // 4->5 must be 5->4 (Flip)
    // Total flips = 3
    int n1 = 6;
    std::vector<std::vector<int>> connections1 = {{0, 1}, {1, 3}, {2, 3}, {4, 0}, {4, 5}};
    std::cout << "Test Case 1 Shifts: " << solver.minReorder(n1, connections1) << " (Expected: 3)\n";

    // --- TEST CASE 2 ---
    // Connections: 1->0, 1->2, 3->2, 3->4
    // Total flips = 2
    int n2 = 5;
    std::vector<std::vector<int>> connections2 = {{1, 0}, {1, 2}, {3, 2}, {3, 4}};
    std::cout << "Test Case 2 Shifts: " << solver.minReorder(n2, connections2) << " (Expected: 2)\n";

    return 0;
}
