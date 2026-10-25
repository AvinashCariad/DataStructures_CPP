#include <iostream>
#include <vector>
#include <queue>

using namespace std;

//---------------------------------------------------------
// Tree Node Definition
//---------------------------------------------------------

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

//---------------------------------------------------------
// Solution
//---------------------------------------------------------

class Solution
{
public:

    vector<int> rightSideView(TreeNode* root)
    {
        vector<int> result;

        if (root == nullptr)
            return result;

        queue<TreeNode*> q;

        q.push(root);

        while (!q.empty())
        {
            int size = q.size();
            
            // Iterate:
            for (int i = 0; i < size; i++)
            {
                TreeNode* node = q.front();
                q.pop();

                // Last node in this level
                if (i == size - 1)
                {
                    result.push_back(node->val);
                }

                if (node->left)
                    q.push(node->left);

                if (node->right)
                    q.push(node->right);
            }
        }

        return result;
    }
};

//---------------------------------------------------------
// Main
//---------------------------------------------------------

int main(int argc, char** argv)
{
    /*
                1
              /   \
             2     3
              \     \
               5     4

        Right Side View

        1
        3
        4
    */

    TreeNode* root = new TreeNode(1);

    root->left = new TreeNode(2);
    root->right = new TreeNode(3);

    root->left->right = new TreeNode(5);

    root->right->right = new TreeNode(4);

    Solution sol;

    vector<int> ans = sol.rightSideView(root);

    cout << "Right Side View:\n";

    for (int x : ans)
        cout << x << " ";

    cout << endl;

    return 0;
}
