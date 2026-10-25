/*
	Given a binary tree. Print the nodes at same height, starting from top and left to right.
	Input: root of binary tree
	Output: 

	Solution:
		We can use a queue for doing BFS and keep track when a level is ending, then push the elements of that
		level to a vector.
		TC: O(n), n: no. of nodes
		SC: O(m), for queue, m: max nodes in any level 
		 
*/


#include <iostream>
#include <vector>
#include <list>
#include <queue>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Fixed: Ensured a return path on failure, though standard raw 'new' throws bad_alloc directly
Node* createNode(const int& data) {
    try {
        Node* node = new Node;
        node->data = data;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    } catch(const bad_alloc& e) {
        cerr << "Memory allocation failed: " << e.what() << endl;
        return nullptr; // Return nullptr safely instead of falling off the function end
    }
}

// Returns a 2D layout of nodes grouped by level
vector<list<int>> printNodesAtSameLevel(Node* root) {
    if (!root)
        return vector<list<int>>{};
        
    vector<list<int>> tree_nodes;
    queue<Node*> q;
    
    q.emplace(root);

    while (!q.empty()) {
        int level_size = q.size(); // Clean, standard way to track level size
        list<int> current_level;       // note how the current level is defined

        for (int i = 0; i < level_size; ++i) {
            Node* curr = q.front();
            q.pop();
            current_level.emplace_back(curr->data);

            if (curr->left) q.emplace(curr->left);
            if (curr->right) q.emplace(curr->right);
        }
        tree_nodes.emplace_back(move(current_level));        // note the use of move here
    }

    return tree_nodes;
} 

// Fixed: Added the required return statement for chaining ostream operators
ostream& operator<< (ostream& out, const vector<list<int>>& arr) {
    for (const auto& v : arr) {
        for (const auto& el : v) {
            out << el << " ";
        }
        out << "\n";
    }
    return out; // Critical: Must return out
}

// Optional helper to avoid memory leaks
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}


// main code harness:
int main(int argc, char** argv) {
    Node* root = createNode(1);
    if (!root) return 1;
    
    root->left = createNode(2);
    root->right = createNode(3);
    
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    
    root->right->left = createNode(6);
    root->right->right = createNode(7);
    
    root->right->right->right = createNode(8);
    
    cout << printNodesAtSameLevel(root);
    
    freeTree(root); // Safe cleanup
    return 0;
}
