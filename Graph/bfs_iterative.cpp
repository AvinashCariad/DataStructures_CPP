#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// BFS traversal starting from a given source node 'startNode'
void bfsIterative(const vector<vector<int>>& adj, int startNode) {
    int n = adj.size();
    vector<bool> visited(n, false);
    queue<int> q;

    // Push the starting node and mark it as visited
    q.push(startNode);
    visited[startNode] = true;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        cout << curr << " ";

        // Traverse all adjacent nodes
        for (int neighbor : adj[curr]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true; // Mark as visited immediately when pushed
                q.push(neighbor);
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

    cout << "BFS starting from node 0: ";
    bfsIterative(adj, 0);

    return 0;
}

