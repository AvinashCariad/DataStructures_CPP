/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool isBalanced(TreeNode* root) {
        int rootHeight = height(root);
        return (rootHeight != -1 ? true : false);
    }
    
    int height(TreeNode* node) {
        if (node == NULL) {
            return 0;
        }
        else {
            int leftHeight = height(node->left);
            int rightHeight = height(node->right);
            
            if (leftHeight == -1 || rightHeight == -1 || abs(leftHeight - rightHeight) > 1) {
                return -1;
            }
            
            return max(leftHeight, rightHeight) + 1;
        }
    }
};
