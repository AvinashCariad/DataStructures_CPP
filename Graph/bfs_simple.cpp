#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
private:
    int V;
    vector<vector<int>> adj;   // adjacency list

public:
    // Constructor
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add edge for an undirected graph
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // BFS starting from a given vertex
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        // Mark starting vertex visited and enqueue it
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {

            // Get the vertex at the front
            int current = q.front();
            q.pop();

            cout << current << " ";

            // Visit all neighbors
            for (int neighbor : adj[current]) {

                if (!visited[neighbor]) {

                    // Mark visited BEFORE putting into queue
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        cout << endl;
    }
};

// main code harness:
int main(int argc, char** argv) {

    Graph g(7);

    /*
              0
             / \
            1   2
           / \   \
          3   4   5
                   \
                    6
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);

    g.addEdge(1, 3);
    g.addEdge(1, 4);

    g.addEdge(2, 5);
    g.addEdge(5, 6);

    cout << "BFS traversal starting from vertex 0:" << endl;

    g.BFS(0);

    return 0;
}