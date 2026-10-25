#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

/*
Both solutions have identical time and space complexities:
Time Complexity: \(\mathcal{O}(V + E)\) where \(V\) is the 
number of vertices (nodes) and \(E\) is the number of edges.
We visit every node and every edge exactly once.
Space Complexity: \(\mathcal{O}(V)\) to store the mapping of
 original nodes to cloned nodes in the hash map, plus the overhead
  of the recursion stack (DFS) or queue (BFS).
*/

// Definition for a Node.
class Node {
public:
    int val;
    std::vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = std::vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = std::vector<Node*>();
    }
    Node(int _val, std::vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};



// Method 1: DFS recommended:
class Solution {
private:
    // Hash map to store the mapping from original node to cloned node
    std::unordered_map<Node*, Node*> clonedMap;

public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        // If the node has already been cloned, return its clone
        if (clonedMap.find(node) != clonedMap.end()) {
            return clonedMap[node];
        }

        // Create a copy of the node (without its neighbors for now)
        Node* clone = new Node(node->val);
        clonedMap[node] = clone;

        // Recursively clone all neighbors and add them to the cloned node's neighbors list
        for (Node* neighbor : node->neighbors) {
            clone->neighbors.push_back(cloneGraph(neighbor));
        }

        return clone;
    }
};



// Method 2: BFS, iterative - alternative approach:
class SolutionBFS {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        std::unordered_map<Node*, Node*> clonedMap;
        std::queue<Node*> q;

        // Clone the root node and put it in the map
        clonedMap[node] = new Node(node->val);
        q.push(node);

        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();

            // Iterate through all neighbors of the current node
            for (Node* neighbor : curr->neighbors) {
                // If neighbor hasn't been cloned yet
                if (clonedMap.find(neighbor) == clonedMap.end()) {
                    // Clone it and add to map
                    clonedMap[neighbor] = new Node(neighbor->val);
                    // Push to queue to process its neighbors later
                    q.push(neighbor);
                }
                // Connect the clone of the current node to the clone of the neighbor
                clonedMap[curr]->neighbors.push_back(clonedMap[neighbor]);
            }
        }

        return clonedMap[node];
    }
};



void printGraph(Node* startNode) {
    if (!startNode) return;
    
    std::queue<Node*> q;
    std::unordered_set<Node*> visited;
    
    q.push(startNode);
    visited.insert(startNode);
    
    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();
        
        std::cout << "Node [" << curr->val << "] Address: " << curr << " -> Neighbors: [";
        for (size_t i = 0; i < curr->neighbors.size(); ++i) {
            std::cout << curr->neighbors[i]->val << " (Addr: " << curr->neighbors[i] << ")";
            if (i < curr->neighbors.size() - 1) std::cout << ", ";
            
            if (visited.find(curr->neighbors[i]) == visited.end()) {
                visited.insert(curr->neighbors[i]);
                q.push(curr->neighbors[i]);
            }
        }
        std::cout << "]\n";
    }
}



// main code harness:
int main(int argc, char** argv) {
    // 1. Manually build a cyclic graph (Square shape: 1-2-3-4-1)
    Node* node1 = new Node(1);
    Node* node2 = new Node(2);
    Node* node3 = new Node(3);
    Node* node4 = new Node(4);
    
    node1->neighbors = {node2, node4};
    node2->neighbors = {node1, node3};
    node3->neighbors = {node2, node4};
    node4->neighbors = {node1, node3};
    
    std::cout << "--- ORIGINAL GRAPH ---\n";
    printGraph(node1);
    
    // 2. Clone the graph using the solution instance
    Solution solver;
    Node* clonedGraphHead = solver.cloneGraph(node1);
    
    std::cout << "\n--- CLONED GRAPH ---\n";
    printGraph(clonedGraphHead);


    // 3. Verify that the memory addresses are strictly different deep copies
    std::cout << "\n--- VERIFICATION ---\n";
    if (node1 != clonedGraphHead && node1->val == clonedGraphHead->val) {
        std::cout << "SUCCESS: The graph values are identical, but structural memory addresses are fully independent!\n";
    } else {
        std::cout << "FAILURE: Graph was not deep copied correctly.\n";
    }
    
    // Clean up allocated memory (optional but ideal practice)
    delete node1; delete node2; delete node3; delete node4;
    // Note: A thorough production cleanup would use a tracking set to free the clone nodes too.
    
    return 0;
}



