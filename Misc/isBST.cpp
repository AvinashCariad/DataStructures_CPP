#include <iostream>






/*

 Problem definition for the BST

 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return isValid(root, NULL, NULL);
    }
    
    bool isValid(TreeNode* node, TreeNode* lower, TreeNode* upper) {
        if (node == NULL) {
            return true;
        }
        if ((lower != NULL && node->val <= lower->val) || (upper != NULL && node->val >= upper->val)) {
            return false;
        }
        return isValid(node->left, lower, node) && isValid(node->right, node, upper);
    }
};


*/



using namespace std;

//---------------------------------------------
// Definition for a binary tree node
//---------------------------------------------
struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;

    // Define constructors:
    TreeNode()
    {
        val = 0;
        left = nullptr;
        right = nullptr;
    }

    TreeNode(int x)
    {
        val = x;
        left = nullptr;
        right = nullptr;
    }

    TreeNode(int x,
             TreeNode* left,
             TreeNode* right)
    {
        val = x;
        this->left = left;
        this->right = right;
    }
};

//---------------------------------------------
// Solution
//---------------------------------------------
class Solution
{
public:

    bool isValidBST(TreeNode* root)
    {
        return isValid(root, nullptr, nullptr);
    }

private:

    bool isValid(TreeNode* node,
                 TreeNode* lower,
                 TreeNode* upper)
    {
        if (node == nullptr)
            return true;

        if ((lower != nullptr &&
             node->val <= lower->val) ||

            (upper != nullptr &&
             node->val >= upper->val))
        {
            return false;
        }

        return isValid(node->left,
                       lower,
                       node)

            &&

               isValid(node->right,
                       node,
                       upper);
    }
};

//---------------------------------------------
// Main
//---------------------------------------------
int main(int argc, char** argv)
{
    /*
             8
           /   \
          3     10
         / \      \
        1   6      14
           / \     /
          4   7   13

        Valid BST
    */

    TreeNode* root = new TreeNode(8);

    root->left = new TreeNode(3);
    root->right = new TreeNode(10);

    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(6);

    root->left->right->left = new TreeNode(4);
    root->left->right->right = new TreeNode(7);

    root->right->right = new TreeNode(14);
    root->right->right->left = new TreeNode(13);

    Solution sol;

    if (sol.isValidBST(root))
        cout << "Tree is a Valid BST\n";
    else
        cout << "Tree is NOT a Valid BST\n";

    //-------------------------------------------------------
    // Invalid BST Example
    //-------------------------------------------------------

    /*
              5
            /   \
           1     4
                / \
               3   6

        Invalid because 4 is in the
        right subtree of 5 but 4 < 5.
    */

    TreeNode* root2 = new TreeNode(5);

    root2->left = new TreeNode(1);
    root2->right = new TreeNode(4);

    root2->right->left = new TreeNode(3);
    root2->right->right = new TreeNode(6);

    if (sol.isValidBST(root2))
        cout << "Tree is a Valid BST\n";
    else
        cout << "Tree is NOT a Valid BST\n";

    return 0;
}
