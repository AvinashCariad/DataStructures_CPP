#include "bits/stdc++.h"

using namespace std;

// Definition for undirected graph.
struct UndirectedGraphNode {
    int label;
    vector<UndirectedGraphNode *> neighbors;
    UndirectedGraphNode(int x) : label(x) {};
};


// Define class - solution:
class Solution {
private:
    unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> hashtable;
    unordered_set<UndirectedGraphNode*> visited;
public:
    void cloneNode(UndirectedGraphNode* node) {
        if (!hashtable.count(node)) {
            hashtable[node] = new UndirectedGraphNode(node->label);
        }
    }
    UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node) {
        if (node == nullptr) { return nullptr; }
        queue<UndirectedGraphNode*> q;
        q.push(node);
        visited.insert(node);
        while(!q.empty()) {
            auto cur = q.front(); q.pop();
            cloneNode(cur);
            for (auto neighbour : cur->neighbors) {
                cloneNode(neighbour);
                hashtable[cur]->neighbors.emplace_back(hashtable[neighbour]);
                if (visited.count(neighbour)) 
                { continue; }
                visited.insert(neighbour);
                q.push(neighbour);
            }
        }
        return hashtable[node];
    }
};


/////////////////////////////////////////////////////////////
// Print Graph Using BFS
/////////////////////////////////////////////////////////////

void printGraph(UndirectedGraphNode* node)
{
    if (!node)
        return;

    unordered_set<UndirectedGraphNode*> visited;

    queue<UndirectedGraphNode*> q;

    q.push(node);

    visited.insert(node);

    while (!q.empty())
    {
        auto cur = q.front();
        q.pop();

        cout << "Node " << cur->label << " -> ";

        for (auto n : cur->neighbors)
        {
            cout << n->label << " ";

            if (!visited.count(n))
            {
                visited.insert(n);
                q.push(n);
            }
        }

        cout << endl;
    }
}

/////////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////////

int main()
{
    //
    // Create Original Graph
    //
    //          1
    //        /   \
    //       2-----3
    //

    UndirectedGraphNode* node1 = new UndirectedGraphNode(1);
    UndirectedGraphNode* node2 = new UndirectedGraphNode(2);
    UndirectedGraphNode* node3 = new UndirectedGraphNode(3);

    node1->neighbors.push_back(node2);
    node1->neighbors.push_back(node3);

    node2->neighbors.push_back(node1);
    node2->neighbors.push_back(node3);

    node3->neighbors.push_back(node1);
    node3->neighbors.push_back(node2);

    cout << "Original Graph\n";
    printGraph(node1);

    Solution sol;

    UndirectedGraphNode* clone = sol.cloneGraph(node1);

    cout << "\nCloned Graph\n";
    printGraph(clone);

    cout << "\nMemory Addresses\n";

    cout << "Original Node1 : " << node1 << endl;
    cout << "Cloned   Node1 : " << clone << endl;

    return 0;
}


