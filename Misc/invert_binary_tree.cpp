#include <iostream>

using namespace std;

//------------------------------------------------------
// Tree Node
//------------------------------------------------------

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

//------------------------------------------------------
// Solution
//------------------------------------------------------

class Solution
{
public:

    TreeNode* invertTree(TreeNode* root)
    {
        return invert(root);
    }

private:

    TreeNode* invert(TreeNode* node)
    {
        if (node != nullptr)
        {
            // Invert left subtree
            TreeNode* left = invert(node->left);

            // Invert right subtree
            TreeNode* right = invert(node->right);

            // Swap them
            node->left = right;
            node->right = left;
        }

        return node;
    }
};

//------------------------------------------------------
// Print Tree (Inorder)
//------------------------------------------------------

void inorder(TreeNode* root)
{
    if (root == nullptr)
        return;

    inorder(root->left);

    cout << root->val << " ";

    inorder(root->right);
}

//------------------------------------------------------
// Level Order Printing
//------------------------------------------------------

void levelOrder(TreeNode* root)
{
    if (root == nullptr)
        return;

    queue<TreeNode*> q;

    q.push(root);

    while (!q.empty())
    {
        int sz = q.size();

        while (sz--)
        {
            TreeNode* node = q.front();
            q.pop();

            cout << node->val << " ";

            if (node->left)
                q.push(node->left);

            if (node->right)
                q.push(node->right);
        }

        cout << endl;
    }
}

//------------------------------------------------------
// Main
//------------------------------------------------------

int main(int argc, char** argv)
{
    /*
             4
           /   \
          2     7
         / \   / \
        1  3  6   9
    */

    TreeNode* root = new TreeNode(4);

    root->left = new TreeNode(2);
    root->right = new TreeNode(7);

    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(3);

    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(9);

    cout << "Original Tree (Level Order)\n\n";

    levelOrder(root);

    Solution sol;

    root = sol.invertTree(root);

    cout << "\nInverted Tree (Level Order)\n\n";

    levelOrder(root);

    return 0;
}
