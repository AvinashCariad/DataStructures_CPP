#include <iostream>
#include <vector>
#include <queue>

using namespace std;

//---------------------------------------------------
// Definition of TreeNode
//---------------------------------------------------

struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;

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

    TreeNode(int x, TreeNode* l, TreeNode* r)
    {
        val = x;
        left = l;
        right = r;
    }
};

//---------------------------------------------------
// Solution
//---------------------------------------------------

class Solution
{
public:

    vector<vector<int>> levelOrder(TreeNode* root)
    {
        vector<vector<int>> answer;

        if (root == nullptr)
            return answer;

        queue<pair<TreeNode*, int>> q;

        q.push(make_pair(root, 1));                  // note the use of make_pair in the queue

        while (!q.empty())
        {
            pair<TreeNode*, int> p = q.front();
            q.pop();

            TreeNode* currentNode = p.first;
            int currentLevel = p.second;

            // Create a new vector for a new level
            if (answer.size() < currentLevel)        // note the use of expression check in the code
            {
                answer.push_back(vector<int>());
            }

            // Add current node to its level
            answer[currentLevel - 1].push_back(currentNode->val);      // push the current node val to the level

            // left recurse:
            if (currentNode->left)
            {
                q.push(make_pair(
                    currentNode->left,
                    currentLevel + 1));
            }

            // right recurse:
            if (currentNode->right)
            {
                q.push(make_pair(
                    currentNode->right,
                    currentLevel + 1));
            }
        }

        return answer;
    }
};

//---------------------------------------------------
// Main code harness:
//---------------------------------------------------
int main(int argc, char** argv)
{
    /*
              3
            /   \
           9     20
                /  \
               15   7

    Expected Output

    Level 1 : 3
    Level 2 : 9 20
    Level 3 : 15 7
    */

    TreeNode* root = new TreeNode(3);

    root->left = new TreeNode(9);

    root->right = new TreeNode(20);

    root->right->left = new TreeNode(15);

    root->right->right = new TreeNode(7);

    Solution sol;

    vector<vector<int>> result = sol.levelOrder(root);

    cout << "Level Order Traversal\n\n";

    for (int i = 0; i < result.size(); i++)
    {
        cout << "Level " << i + 1 << " : ";

        for (int x : result[i])
        {
            cout << x << " ";
        }

        cout << endl;
    }

    return 0;
}
