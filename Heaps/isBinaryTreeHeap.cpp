#include <iostream>

// Define the Node structure
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

class Solution {
  public:
    int countNodes(struct Node* root){
        if(root == NULL){
            return 0;
        }
        int ans = 1 + countNodes(root->left) + countNodes(root->right);
        return ans;
    }
    
   
    // check if the heap is Binary Tree:
    bool isCBT(struct Node* root, int index, int totalCount){
        if(root == NULL){
            return true;
        }
        if(index >= totalCount){
            return false;
        }
        else{
            bool left = isCBT(root->left, 2 * index + 1, totalCount);
            bool right = isCBT(root->right, 2 * index + 2, totalCount);
            return left && right;
        }
    }
    


    /*
   isMaxOrder Safety Handle: Added a if (root == NULL) return true; guard statement inside isMaxOrder. 
   Without this, checking an empty tree or certain malformed inputs could trigger a segmentation fault 
   when trying to resolve root->left. 
   */

    bool isMaxOrder(struct Node* root) {
        // Fix for edge case: empty tree should be a valid heap structure
        if (root == NULL) {
            return true;
        }
        if(root->left == NULL && root->right == NULL){
            return true;
        }
        if(root->right == NULL){
            return root->data > root->left->data;
        }
        else{
            bool left = isMaxOrder(root->left);
            bool right = isMaxOrder(root->right);
            
            return left && right && (root->data > root->left->data && root->data > root->right->data);
        }
    }
  
    // Check if the given binary tree is a heap:
    bool isHeap(struct Node* root) {
        int index = 0;
        int totalCount = countNodes(root);
        if(isCBT(root, index, totalCount) && isMaxOrder(root)){
            return true;
        }
        else{
            return false;
        }
    }
};

// Helper function to clean up memory
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete(root);
}



// main code harness:
int main(int argc, char** argv) {
    Solution solver;

    /* Constructing a valid Max-Heap Binary Tree:
             50
           /    \
         30      40
        /  \    /
       10  20  35
    */
    Node* root = new Node(50);
    root->left = new Node(30);
    root->right = new Node(40);
    root->left->left = new Node(10);
    root->left->right = new Node(20);
    root->right->left = new Node(35);

    // Evaluate the tree
    if (solver.isHeap(root)) {
        std::cout << "The binary tree is a valid Max-Heap.\n";
    } else {
        std::cout << "The binary tree is NOT a valid Max-Heap.\n";
    }

    // Clean up dynamically allocated memory
    deleteTree(root);

    return 0;
}

