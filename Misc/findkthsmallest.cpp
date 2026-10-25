#include <iostream>

using namespace std;

//-------------------------------------------------------
// Tree Node
//-------------------------------------------------------

struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x)
    {
        val = x;
        left = nullptr;
        right = nullptr;
    }
};

//-------------------------------------------------------
// Recursive Solution
//-------------------------------------------------------

class Solution
{
private:

    int count = 0;

public:

    int kthSmallest(TreeNode* root, int k)
    {
        return inorder(root, k);
    }

private:

    int inorder(TreeNode* node, int k)
    {
        if (node == nullptr)
            return -1;

        // Search left subtree
        int left = inorder(node->left, k);

        if (left != -1)
            return left;

        // Visit current node
        count++;

        if (count == k)
            return node->val;

        // Search right subtree
        return inorder(node->right, k);
    }
};

//-------------------------------------------------------
// Main
//-------------------------------------------------------

// main code harness:
int main(int argc, char** argv)
{
    /*
               5
             /   \
            3     7
           / \   / \
          2   4 6   8
         /
        1

    Inorder:
    1 2 3 4 5 6 7 8
    */

    TreeNode* root = new TreeNode(5);

    root->left = new TreeNode(3);
    root->right = new TreeNode(7);

    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(4);

    root->left->left->left = new TreeNode(1);

    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(8);

    Solution sol;

    int k = 5;

    cout << "Kth smallest = "
         << sol.kthSmallest(root, k)
         << endl;

    return 0;
}
