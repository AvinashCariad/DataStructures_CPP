#include <iostream>

using namespace std;

//-----------------------------------------------------
// Tree Node
//-----------------------------------------------------

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

//-----------------------------------------------------
// Solution
//-----------------------------------------------------

class Solution
{
public:

    bool isSymmetric(TreeNode* root)
    {
        if (root == nullptr)
            return true;

        return isSym(root->left, root->right);
    }

private:

    bool isSym(TreeNode* leftTree,
               TreeNode* rightTree)
    {
        // Both empty
        if (leftTree == nullptr &&
            rightTree == nullptr)
            return true;

        // One empty
        if (leftTree == nullptr ||
            rightTree == nullptr)
            return false;

        // Values differ
        if (leftTree->val != rightTree->val)
            return false;

        // Mirror comparison
        return isSym(leftTree->left,
                     rightTree->right)
            &&
               isSym(leftTree->right,
                     rightTree->left);
    }
};

//-----------------------------------------------------
// Main
//-----------------------------------------------------
int main(int argc, char** argv)
{
    /*
                1
              /   \
             2     2
            / \   / \
           3   4 4   3
    */

    TreeNode* root = new TreeNode(1);

    root->left = new TreeNode(2);
    root->right = new TreeNode(2);

    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);

    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(3);

    Solution sol;

    if (sol.isSymmetric(root))
        cout << "Tree is Symmetric\n";
    else
        cout << "Tree is NOT Symmetric\n";

    //-------------------------------------------------
    // Make the tree asymmetric
    //-------------------------------------------------

    root->right->right = nullptr;

    if (sol.isSymmetric(root))
        cout << "Tree is Symmetric\n";
    else
        cout << "Tree is NOT Symmetric\n";

    return 0;
}
