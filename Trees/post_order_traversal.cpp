#include <iostream>
#include <vector>
#include <stack>

using namespace std;
/*
Postorder traversal is a depth-first tree traversal method where each node
is processed after its left and right children. While recursive postorder
traversal is common, an iterative approach using a single stack offers a
more efficient alternative. This article explains how to implement iterative
postorder traversal, eliminating recursion while maintaining the correct node
visit order, and improving both space and time efficiency.


In this iterative version, we push each node onto the stack twice: the first
push marks the node, and the second one indicates we need to process it after
its children. We keep moving to the left child until we reach the leaf nodes,
then start processing nodes by popping them from the stack. While popping if
we find stack top() is same as root then go for root->right else print root.


*/

//recursion
//Runtime: 4 ms, faster than 50.22% of C++ online submissions for Binary Tree Postorder Traversal.
//Memory Usage: 9 MB, less than 8.39% of C++ online submissions for Binary Tree Postorder Traversal.
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

 // Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};



class Solution {
public:
    vector<int> ans;
    
    void postorder(TreeNode* node){
        if(!node) return;
        postorder(node->left);
        postorder(node->right);
        ans.push_back(node->val);
    }
    
    vector<int> postorderTraversal(TreeNode* root) {
        postorder(root);
        return ans;
    }
};

//iterative
//Runtime: 4 ms, faster than 50.22% of C++ online submissions for Binary Tree Postorder Traversal.
//Memory Usage: 8.8 MB, less than 8.39% of C++ online submissions for Binary Tree Postorder Traversal.
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> ans;
        if(!root) return ans;
        
        stack<TreeNode*> stk;
        stack<int> stk_val;
        
        stk.push(root);
        TreeNode* cur;
        
        while(!stk.empty()){
            cur = stk.top(); stk.pop();
            ans.insert(ans.begin(), cur->val);
            
            if(cur->left) stk.push(cur->left);
            if(cur->right) stk.push(cur->right);
        }
        
        return ans;
    }
};















/*
// Node:
struct Node {
    int data;
    Node *left, *right;

    Node(int x) {
        data = x;
        left = nullptr;
        right = nullptr;
    }
};


// postorder:
vector<int> postOrder(Node* root) {

    vector<int> res;
    stack<Node*> st;

    while (true) {
        while (root) {
            st.push(root);
            st.push(root);
            root = root->left;
        }
        if (st.empty())
            return res;
        root = st.top();
        st.pop();
        if (!st.empty() && st.top() == root)
            root = root->right;
        else {
            res.push_back(root->data);
            root = nullptr;
        }
    }
    return res;
}

// main code harness:
int main(int argc, char** argv) {
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);

    vector<int> postOrderList = postOrder(root);
    for (auto it : postOrderList)
        cout << it << " ";
    
    return 0;
}

*/