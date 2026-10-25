/*
    Given two tree nodes, find the LCA.

    Solution:
        We start the traversal of tree, when we find any of the required
        nodes, we return the pointer to that node as the LCA, if later on
        again we find the other node then we return the pointer to it. If
        both the nodes are in the same subtree, then this will ensure that the 
        first appearing node wil be the ancestor.
        If we get pointers from both left and right subtree, then LCA will be
        the parent node, we return that as the LCA. 
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
    unique_ptr<Node<T>> left, right;
    // 
    Node(T data) : data(data), left(nullptr), right(nullptr) {}
};

// finds the LCA of two nodes
Node<int>* findLCA(unique_ptr<Node<int>>& root,
    unique_ptr<Node<int>>& node1, unique_ptr<Node<int>>& node2) {

    if (!root)
        return nullptr;

    // if the current node is one of the nodes
    if (root->data == node1->data || root->data == node2->data)
        return root.get();

    // search the left and right subtrees
    Node<int>* left = findLCA(root->left, node1, node2);
    Node<int>* right = findLCA(root->right, node1, node2);

    // if the two nodes are on different subtrees, then this is the 
    // first parent to have them on opposite sides, so it is the LCA
    if (left && right)
        return root.get();
    // return LCA if there
    if (left || right)
        return left ? left : right;

    // return null indicating that LCA is not found yet
    return nullptr;
}

// contains ancestor info
struct AncestorInfo {
    int nodes_found;    // tracks no. of required nodes found
    Node<int>* ancestor; // ptr to the LCA if there else null
};

// EPI solution
AncestorInfo searchLCA(unique_ptr<Node<int>>& root, unique_ptr<Node<int>>& node1,
                        unique_ptr<Node<int>>& node2) {
    if (!root)
        return { 0, nullptr };

    // check left and right subtree
    AncestorInfo left = searchLCA(root->left, node1, node2);
    if (left.ancestor)
        return left;
    AncestorInfo right = searchLCA(root->right, node1, node2);
    if (right.ancestor)
        return right;

    // if the current node is one of the required nodes,
    // then update nodes_found
    int nodes_found = left.nodes_found + right.nodes_found +
                    (root->data == node1->data || root->data == node2->data);
    
    // if both the nodes found then return current ptr else nullptr
    return {nodes_found, nodes_found == 2 ? root.get() : nullptr};
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
    /*
                    1
                  /   \
                2      3
               /  \   /
              4    5  6
                     /
                    7
    */
    unique_ptr<Node<int>> root = make_unique<Node<int>>(1);
    root->left = make_unique<Node<int>>(2);
    root->right = make_unique<Node<int>>(3);
    root->left->left = make_unique<Node<int>>(4);
    root->left->right = make_unique<Node<int>>(5);

    root->right->left = make_unique<Node<int>>(6);
    root->right->left->left = make_unique<Node<int>>(7);

    postOrderTraversal(root);
    cout << endl;
    cout << (findLCA(root, root->right->left, root->right->left->left))->data << endl;
    cout << (searchLCA(root, root->right->left, root->right->left->left)).ancestor->data;
    return 0;
}



/*

// [Naive Approach] Checking if each node is LCA - O(N^2) Time and O(h) space
// The idea is to traverse the tree from root node, and for every node check if it is the LCA.
// A node is the LCA if its left subtree has one target node and right subtree has the other target node.

#include <iostream>
using namespace std;

// Node Structure
class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        left = right = nullptr;
    }
};

// returns true if val is present 
// in the subtree of root
bool hasNode(Node* root, Node* node) {
    if (root == nullptr) return false;
    return (root == node) || 
            hasNode(root->left, node) || 
            hasNode(root->right, node);
}

Node* lca(Node* root, Node* n1, Node* n2) {
    if (root == nullptr) return nullptr;

    if (hasNode(root->left, n1) && hasNode(root->right, n2))
        return root;

    if (hasNode(root->left, n2) && hasNode(root->right, n1))
        return root;

    Node* lcaLeft = lca(root->left, n1, n2);
    Node* lcaRight = lca(root->right, n1, n2);

    return lcaLeft != nullptr ? lcaLeft : lcaRight;
}


// main code harness:
int main(int argc, char** argv) {
    // Create binary tree:
    //			   1
    //           /   \
    //          2     3
    //               / \
    //              6   7 
    //             /
    //            8

    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    root->right->left->left = new Node(8);

    // Node 7
    Node* n1 = root->right->right;

    // Node 8
    Node* n2 = root->right->left->left;

    Node* ans = lca(root, n1, n2);
    cout << ans->data << endl;
}

////////////////////////////////////BETTER APPROACH///////////////////////////////////////////
// The idea is to store the paths to the target nodes from the root in two separate arrays. 
// Then start traversing from the 0th index and look simultaneously into the values stored in 
// the arrays, the LCA is the last matching element in both the arrays.

#include <iostream>
#include <vector>
using namespace std;

// Node Structure
class Node {
public:
    int data;
    Node* left;
    Node* right;
    Node(int value) {
        data = value;
        left = right = nullptr;
    }
};

// Finds the path from root to given node
bool findPath(Node* root, vector<Node*>& path, Node* n) {
    if (root == nullptr) return false;

    // Store current node
    path.push_back(root);

    if (root == n || findPath(root->left, path, n) || findPath(root->right, path, n)) {
        return true;
    }

    // remove root from path and return false
    path.pop_back();
    return false;
}

Node* lca(Node* root, Node* n1, Node* n2) {
    vector<Node*> path1, path2;

    // Find paths from root to n1 
    // and root to n2
    if (!findPath(root, path1, n1) || !findPath(root, path2, n2)) return nullptr;

    // Compare the paths to get 
    // the first different value
    int i = 0;
    for (i = 0; i < path1.size() && i < path2.size(); i++) {
        if (path1[i] != path2[i]) return path1[i - 1];        // get where the paths diverge
    }
    return path1[i - 1];
}


// main code harness:
int main(int argc, char** argv) {
    // Create binary tree:
    //			   1
    //           /   \
    //          2     3
    //               / \
    //              6   7
    //             /
    //            8

    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    root->right->left->left = new Node(8);

    Node* n1 = root->right->right;
    Node* n2 = root->right->left->left;

    Node* ans = lca(root, n1, n2);
    if (ans) cout << ans->data << endl;
}


////////////////////////////////////MOST OPTIMUM: EXPECTED//////////////////////
// In this approach, we start with the root node and recursively searches for the 
// given nodes n1 and n2. If the current root is null, it returns null. If the root
// matches either of the keys, it returns the root as a potential LCA. Then, it 
// recursively checks the left and right subtrees. If both the left and right recursive 
// calls return non-null values, it means one key is in the left subtree and the other is
// in the right subtree, so the current root is the LCA. If only one subtree contains the keys,
// the recursive call returns the non-null subtree result as the LCA.

#include <iostream>
using namespace std;

// Node Structure
class Node {
public:
    Node *left, *right;
    int data;
    Node(int k) {
        data = k;
        left = nullptr;
        right = nullptr;
    }
};

Node* lca(Node* root, Node* n1, Node* n2) {

    if (!root)
        return nullptr;

    // If either key matches with root data, return root
    if (root == n1 || root == n2)
        return root;

    Node* leftLca = lca(root->left, n1, n2);
    Node* rightLca = lca(root->right, n1, n2);

    // If both of the above calls return Non-NULL, then one
    // data is present in one subtree and the other is present
    // in the other, so this node is the LCA
    if (leftLca && rightLca)
        return root;

    // Otherwise check if left subtree or 
    // right subtree is LCA
    return leftLca ? leftLca : rightLca;
}


// main code harness:
int main(int argc, char** argv) {

    // Create binary tree:
    //			   1
    //           /   \
    //          2     3
    //               / \
    //              6   7 
    //             /
    //            8

    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    root->right->left->left = new Node(8);

    Node* n1 = root->right->right;
    Node* n2 = root->right->left->left;

    Node* ans = lca(root, n1, n2);
    cout<<ans->data;

    return 0;
}





















*/