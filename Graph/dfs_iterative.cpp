#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// DFS traversal starting from a given source node 'startNode'
void dfsIterative(const vector<vector<int>>& adj, int startNode) {
    int n = adj.size();
    vector<bool> visited(n, false);
    stack<int> s;

    // Push the starting node onto the stack
    s.push(startNode);

    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        // Nodes can be pushed onto the stack multiple times, 
        // so we check and process them only on their first extraction (pop)
        if (!visited[curr]) {
            visited[curr] = true;
            cout << curr << " ";
        }

        // Traverse neighbors in reverse order to match the traditional recursive DFS ordering 
        // (so the lower index neighbor gets processed first)
        for (auto it = adj[curr].rbegin(); it != adj[curr].rend(); ++it) {
            int neighbor = *it;
            if (!visited[neighbor]) {
                s.push(neighbor);
            }
        }
    }
    cout << endl;
}


// main code harness:
int main(int argc, char** argv) {
    // Example graph with 5 nodes (0 through 4)
    int vertices = 5;
    vector<vector<int>> adj(vertices);

    // Add edges (Undirected graph example)
    adj[0] = {1, 2};
    adj[1] = {0, 3, 4};
    adj[2] = {0, 4};
    adj[3] = {1};
    adj[4] = {1, 2};

    cout << "Iterative DFS starting from node 0: ";
    dfsIterative(adj, 0);

    return 0;
}

