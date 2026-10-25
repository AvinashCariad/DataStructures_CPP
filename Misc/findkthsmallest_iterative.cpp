#include <iostream>
#include <stack>

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
// Iterative Solution
//-------------------------------------------------------

class Solution
{
public:

    int kthSmallest(TreeNode* root, int k)
    {
        stack<TreeNode*> st;

        TreeNode* curr = root;

        while (curr != nullptr || !st.empty())
        {
            // Go to leftmost node
            while (curr != nullptr)
            {
                st.push(curr);
                curr = curr->left;
            }

            curr = st.top();
            st.pop();

            k--;

            if (k == 0)
                return curr->val;

            curr = curr->right;
        }

        return -1;
    }
};

//-------------------------------------------------------
// Main: main code harness:
//-------------------------------------------------------

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
