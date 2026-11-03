#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

// Definition of a Binary Tree Node
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

// 1. Pre-order Traversal (Root -> Left -> Right)
void preOrder(Node* root) {
    if (root == nullptr) return;
    
    cout << root->data << " ";  // Visit root
    preOrder(root->left);       // Traverse left subtree
    preOrder(root->right);      // Traverse right subtree
}

// 2. In-order Traversal (Left -> Root -> Right)
void inOrder(Node* root) {
    if (root == nullptr) return;
    
    inOrder(root->left);        // Traverse left subtree
    cout << root->data << " ";  // Visit root
    inOrder(root->right);       // Traverse right subtree
}

// 3. Post-order Traversal (Left -> Right -> Root)
void postOrder(Node* root) {
    if (root == nullptr) return;
    
    postOrder(root->left);       // Traverse left subtree
    postOrder(root->right);      // Traverse right subtree
    cout << root->data << " ";   // Visit root
}

// 4. Level-order Traversal (Breadth-First Search)
void levelOrder(Node* root) {
    if (root == nullptr) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->data << " ";

        // Push left child to queue if it exists
        if (current->left != nullptr) {
            q.push(current->left);
        }
        // Push right child to queue if it exists
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
}

//=====================================ITERATIVE TRAVERSALS=====================================
// 1. Iterative Pre-order Traversal (Root -> Left -> Right)
void iterativePreOrder(Node* root) {
    if (root == nullptr) return;

    stack<Node*> st;
    st.push(root);

    while (!st.empty()) {
        Node* current = st.top();
        st.pop();

        cout << current->data << " ";

        // Push RIGHT first so LEFT is processed first (LIFO stack behavior)
        if (current->right != nullptr) {
            st.push(current->right);
        }
        if (current->left != nullptr) {
            st.push(current->left);
        }
    }
}


// 2. Iterative In-order Traversal (Left -> Root -> Right)
void iterativeInOrder(Node* root) {
    stack<Node*> st;
    Node* current = root;
    // Traverse the tree:
    while (current != nullptr || !st.empty()) {
        // Reach the leftmost Node of the current Node
        while (current != nullptr) {
            st.push(current);
            current = current->left;
        }

        // Current must be NULL at this point
        current = st.top();
        st.pop();

        cout << current->data << " ";

        // We have visited the node and its left subtree. Now, it's the right subtree's turn
        current = current->right;
    }
}


// 3. Iterative Post-order Traversal (Left -> Right -> Root) using Two Stacks
void iterativePostOrder(Node* root) {
    if (root == nullptr) return;
    // Define two stacks: one for processing nodes
    // and another for storing the post-order sequence
    stack<Node*> st1;
    stack<Node*> st2;

    st1.push(root);

    while (!st1.empty()) {
        Node* current = st1.top();
        
        st1.pop();
        st2.push(current);      // just one push defined for this stack, 
                                //so we can use it to reverse the order of processing

        // Push left first so right is processed first out of st1
        if (current->left != nullptr) {
            st1.push(current->left);
        }
        if (current->right != nullptr) {
            st1.push(current->right);
        }
    }

    // Print all elements from the second stack
    while (!st2.empty()) {
        cout << st2.top()->data << " ";
        st2.pop();
    }
}

// 4. Iterative Level-order Traversal (Breadth-First Search)
void iterativeLevelOrder(Node* root) {
    if (root == nullptr) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->data << " ";

        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }
}


// Main function to demonstrate tree traversals
int main(int argc, char *argv[]) {
    /* Constructed Binary Tree:
             1
           /   \
          2     3
         / \   /
        4   5 6
    */
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);

    cout << "Pre-order Traversal:  ";
    preOrder(root);
    cout << "\n";

    cout << "In-order Traversal:   ";
    inOrder(root);
    cout << "\n";

    cout << "Post-order Traversal: ";
    postOrder(root);
    cout << "\n";

    cout << "Level-order Traversal: ";
    levelOrder(root);
    cout << "\n";

    // Testing iterative traversals:
    cout << "Iterative Pre-order Traversal:  ";
    iterativePreOrder(root);
    cout << "\n";
    cout << "Iterative In-order Traversal:   ";
    iterativeInOrder(root);
    cout << "\n";
    cout << "Iterative Post-order Traversal: ";
    iterativePostOrder(root);
    cout << "\n";
    cout << "Iterative Level-order Traversal: ";
    iterativeLevelOrder(root);
    cout << "\n";


    // Clean up memory (Optional but good practice)
    delete root->left->left;
    delete root->left->right;
    delete root->right->left;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}
