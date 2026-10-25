/*
    Given two tree nodes with parent pointers, find the LCA.

    Solution:
        Take the two pointers to the same depth. Then start traversing 
        up the parent till the two have the same node pointers, that is tha LCA.
        TC: O(n)
        Sc: O(h), for recursive stack
*/

#include <iostream>
#include <algorithm>
#include <memory>
using namespace std;

template<typename T>
struct Node {
    T data;
    Node<T>* parent; // Correct: Non-owning raw pointer prevents memory cycles
    unique_ptr<Node<T>> left, right;
    
    Node(T data) : data(data), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Finds the depth of a node by climbing parents
int getDepth(const Node<int>* node) {
    int depth = 0;
    while (node && node->parent) {
        ++depth;
        node = node->parent;
    }
    return depth;
}

// Finds the LCA of two nodes using raw pointers
// Note: We removed the unused 'root' parameter to make the function more modular
Node<int>* findLCA(Node<int>* node1, Node<int>* node2) {
    if (!node1 || !node2)
        return nullptr;

    int depth1 = getDepth(node1);
    int depth2 = getDepth(node2);

    // Scenario A: node2 is deeper, so climb node2 up to match node1's depth
    // The following can be combined in a single block of code:
    //
    if (depth2 > depth1) {
        while (depth2 != depth1) {
            node2 = node2->parent;
            --depth2;
        }
    } 
    // Scenario B: node1 is deeper, so climb node1 up to match node2's depth
    else if (depth1 > depth2) {
        while (depth1 != depth2) {
            node1 = node1->parent;
            --depth1;
        }
    }
   
    // Now both nodes are guaranteed to be at the exact same depth level.
    // Move both up together until they meet at their lowest common ancestor.
    while (node1 != node2) {
        node1 = node1->parent;
        node2 = node2->parent;
    }
    
    return node1;
}


// Post-order traversal safely using const references
template<typename T>
void postOrderTraversal(const unique_ptr<Node<T>>& root) {
    if (root) {
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        cout << root->data << " ";
    }
}

// main code harness:
int main(int argc, char** argv) {
    /*
                     1
                  /    \
                 2      3
                / \    /
               4   5  6
                     /
                    7
    */
    auto root = make_unique<Node<int>>(1);
    
    root->left = make_unique<Node<int>>(2);
    root->left->parent = root.get();

    root->right = make_unique<Node<int>>(3);
    root->right->parent = root.get();
    
    root->left->left = make_unique<Node<int>>(4);
    root->left->left->parent = root->left.get();
    
    root->left->right = make_unique<Node<int>>(5);
    root->left->right->parent = root->left.get();

    root->right->left = make_unique<Node<int>>(6);
    root->right->left->parent = root->right.get();
    
    root->right->left->left = make_unique<Node<int>>(7);
    root->right->left->left->parent = root->right->left.get();

    postOrderTraversal(root);
    cout << endl;
    
    // Pass raw pointers cleanly using .get()
    Node<int>* lca = findLCA(root->right->left.get(), root->left->left.get());
    
    if (lca) {
        cout << "LCA of 6 and 4 is: " << lca->data << endl; // Expected output: 1
    }
    
    return 0;
}



/*
#include <iostream>
#include <algorithm>
#include <memory>
using namespace std;

template<typename T>
struct Node {
    T data;
    Node<T>* parent; // I am not writing unique ptr for this, only because there is too much code 
                    // to write in the main body for that. But this should also be unique_ptr
    unique_ptr<Node<T>> left, right;
    
    Node(T data) : data(data), left(nullptr), right(nullptr), parent(nullptr) {}
};

// finds the depth of a node
int getDepth(Node<int>* root) {
    int depth = 0;
    // Iterate:
    while(root->parent) {
        ++depth;
        root = root->parent;
    }
    return depth;
}

// finds the LCA of two nodes
Node<int>* findLCA(unique_ptr<Node<int>>& root,
    unique_ptr<Node<int>>& node1, unique_ptr<Node<int>>& node2) {
    if (!root)
        return nullptr;

    auto iter1 = node1.get(), iter2 = node2.get();     // get the raw pointers
    // first we find the depths of the two nodes
    int depth1 = getDepth(iter1);
    int depth2 = getDepth(iter2);

    // now take the nodes to same depth
    if (depth2 > depth1) {
        swap(iter1, iter2);
        swap(depth1, depth2);
    }
    while (depth1 != depth2) {
        iter1 = iter1->parent;
        --depth1;
    }
   
    // now that both the nodes are at same depth, we
    // move up till both the ptrs are same
    while (iter1 != iter2) {
        iter1 = iter1->parent;
        iter2 = iter2->parent;
    }
    return iter1;
}


// postorder traversal
template<typename T>
void postOrderTraversal(unique_ptr<Node<T>>& root) {
    if (root) {
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        cout << root->data << " ";
    }
}


// main code harness:
int main(int argc, char** argv) {
    
    //                 1
    //              /    \
    //             2      3
    //            / \    /
    //           4   5  6
    //                 /
    //                7
    
    unique_ptr<Node<int>> root = make_unique<Node<int>>(1);
    root->parent = nullptr;
    
    root->left = make_unique<Node<int>>(2);
    root->left->parent = root.get();

    root->right = make_unique<Node<int>>(3);
    root->right->parent = root.get();
    
    root->left->left = make_unique<Node<int>>(4);
    root->left->left->parent = root->left.get();
    
    root->left->right = make_unique<Node<int>>(5);
    root->left->right->parent = root->left.get();

    root->right->left = make_unique<Node<int>>(6);
    root->right->left->parent = root->right.get();
    
    root->right->left->left = make_unique<Node<int>>(7);
    root->right->left->left->parent = root->right->left.get();


    postOrderTraversal(root);
    cout << endl;
    cout << (findLCA(root, root->right->left, root->left->left))->data << endl;
    return 0;
}

*/