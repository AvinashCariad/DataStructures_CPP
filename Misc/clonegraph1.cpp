#include <bits/stdc++.h>
using namespace std;

// Definition for undirected graph
struct UndirectedGraphNode {
    int label;                                           // current node label
    vector<UndirectedGraphNode*> neighbors;              // neighbors of the current node
    UndirectedGraphNode(int x) : label(x) {}             // Initialize
};

class Solution {

private:

    unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> hashtable;
    unordered_set<UndirectedGraphNode*> visited;

public:

    void cloneNode(UndirectedGraphNode* node)
    {
        if (!hashtable.count(node))
        {
            hashtable[node] = new UndirectedGraphNode(node->label);
        }
    }

    UndirectedGraphNode* cloneGraph(UndirectedGraphNode* node)
    {
        if (node == nullptr)
            return nullptr;

        queue<UndirectedGraphNode*> q;

        q.push(node);

        visited.insert(node);

        while (!q.empty())
        {
            auto cur = q.front();
            q.pop();

            cloneNode(cur);                      // do whatever you want to do with the current node

            for (auto neighbour : cur->neighbors)
            {
                cloneNode(neighbour);

                hashtable[cur]->neighbors.push_back(hashtable[neighbour]);      // this is imp

                if (visited.count(neighbour))
                    continue;

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




    return 0;
}
