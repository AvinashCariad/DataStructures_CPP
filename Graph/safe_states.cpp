#include <iostream>
#include <vector>


// define the solution class:
class Solution {
private:
    bool hasCycle(int node, const std::vector<std::vector<int>>& graph, std::vector<int>& state) {
        // If state is 1, we hit a node currently in our DFS path -> Cycle detected!
        if (state[node] == 1) return true;
        // If state is 2, it has already been processed and confirmed safe
        if (state[node] == 2) return false;

        // Mark node as "Visiting"
        state[node] = 1;

        // Check all outgoing edges
        for (int neighbor : graph[node]) {
            if (hasCycle(neighbor, graph, state)) {
                return true; // If any neighbor leads to a cycle, this node is unsafe
            }
        }

        // Mark node as "Safe"
        state[node] = 2;
        return false;
    }

public:
    std::vector<int> eventualSafeNodes(std::vector<std::vector<int>>& graph) {
        int n = graph.size();
        std::vector<int> state(n, 0); // 0 = unvisited, 1 = visiting, 2 = safe
        std::vector<int> safeNodes;

        for (int i = 0; i < n; ++i) {
            // If the node does not lead to a cycle, it is safe
            if (!hasCycle(i, graph, state)) {        // if there is no cycle
                safeNodes.push_back(i);
            }
        }

        return safeNodes;
    }
};


// main code harness:
int main(int argc, char** argv) {
    Solution solver;

    // --- TEST CASE 1 ---
    // Graph representation: graph[i] is a list of nodes adjacent to i
    // Node 5 and 6 are terminal nodes. 
    // Cycle exists between 0 -> 1 -> 2 -> 0 and 3 -> 4 -> 3
    std::vector<std::vector<int>> graph = {{1, 2}, {2, 3}, {5}, {0}, {5}, {}, {}};
    
    std::vector<int> result = solver.eventualSafeNodes(graph);
    
    std::cout << "Eventual Safe Nodes: [";
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << (i < result.size() - 1 ? ", " : "");
    }
    std::cout << "]\n"; // Expected Output: [2, 4, 5, 6]

    return 0;
}
