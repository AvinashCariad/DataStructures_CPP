#include <iostream>
#include <vector>

/*
Core Mechanics & Optimizations\(\mathcal{O}(1)\) Time Complexity: 
Instead of constructing an adjacency list or calculating the degree
of all \(V\) vertices (which takes \(\mathcal{O}(V)\) time), this
trick solves the entire problem by examining just 4 integer values.
\(\mathcal{O}(1)\) Space Complexity: No extra memory is allocated
since we don't build out a physical graph representation in memory.

To solve the Find Center of Star Graph problem (LeetCode 1791) in C++, 
you can exploit the unique structural properties of a star graph.
A star graph consists of 1 central node connected to exactly n - 1 outer nodes.
Because the center node is connected to every single other node, it must appear
in every single edge of the graph. Therefore, you do not need to traverse the entire graph;
you only need to compare the first two edges to see which node is repeated

*/

class Solution {
public:
    int findCenter(std::vector<std::vector<int>>& edges) {
        // A star graph's center must appear in every single edge.
        // Therefore, we only need to look at the first two edges.
        int u1 = edges[0][0], v1 = edges[0][1];
        int u2 = edges[1][0], v2 = edges[1][1];
        
        // Find which node from the first edge is repeated in the second edge
        if (u1 == u2 || u1 == v2) {
            return u1;
        }
        return v1;
    }
};


// main code harness:
int main(int argc, char** argv) {
    Solution solver;

    // --- TEST CASE 1 ---
    // Center is 2. Edges: 1-2, 2-3, 4-2
    std::vector<std::vector<int>> edges1 = {{1, 2}, {2, 3}, {4, 2}};
    std::cout << "Test Case 1 Center: " << solver.findCenter(edges1) << " (Expected: 2)\n";

    // --- TEST CASE 2 ---
    // Center is 1. Edges: 1-2, 5-1, 1-3, 1-4
    std::vector<std::vector<int>> edges2 = {{1, 2}, {5, 1}, {1, 3}, {1, 4}};
    std::cout << "Test Case 2 Center: " << solver.findCenter(edges2) << " (Expected: 1)\n";

    return 0;
}
