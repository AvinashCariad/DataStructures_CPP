#include <iostream>

// Definition of BST Node
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

// Function to insert a value into the BST
Node* insert(Node* root, int value) {
    // If the tree/subtree is empty, return a new node
    if (root == nullptr) {
        return new Node(value);
    }

    // Otherwise, recur down the tree
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }

    // Return the unchanged node pointer
    return root;
}

// Helper function to find the minimum value node in a subtree (Inorder Successor)
Node* findMin(Node* root) {
    Node* current = root;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Function to delete a node from the BST
Node* deleteNode(Node* root, int key) {
    // Base Case: key is not in the tree
    if (root == nullptr) {
        return root;
    }

    // If the key to be deleted is smaller than the root's key, it's in the left subtree
    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    }
    // If the key to be deleted is greater than the root's key, it's in the right subtree
    else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    }
    // If key is same as root's key, then this is the node to be deleted
    else {
        // Case 1: Node has no child (Leaf node) OR Case 2: Node has only one child
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // Case 3: Node has two children
        // Get the inorder successor (smallest in the right subtree)
        Node* temp = findMin(root->right);

        // Copy the inorder successor's content to this node
        root->data = temp->data;

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

// Utility function to print the tree (Inorder Traversal to see sorted elements)
void inorder(Node* root) {
    if (root != nullptr) {
        inorder(root->left);
        std::cout << root->data << " ";
        inorder(root->right);
    }
}

// Driver code to test the implementation
int main(int argc, char *argv[]) {
    Node* root = nullptr;

    // Test Insertion
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 20);
    insert(root, 40);
    insert(root, 70);
    insert(root, 60);
    insert(root, 80);

    std::cout << "Inorder traversal of the BST: ";
    inorder(root);
    std::cout << "\n";

    // Test Deletion
    std::cout << "\nDelete 20 (Leaf Node):\n";
    root = deleteNode(root, 20);
    std::cout << "Inorder traversal: ";
    inorder(root);
    std::cout << "\n";

    std::cout << "\nDelete 30 (Node with single child):\n";
    root = deleteNode(root, 30);
    std::cout << "Inorder traversal: ";
    inorder(root);
    std::cout << "\n";

    std::cout << "\nDelete 50 (Node with two children):\n";
    root = deleteNode(root, 50);
    std::cout << "Inorder traversal: ";
    inorder(root);
    std::cout << "\n";

    return 0;
}
