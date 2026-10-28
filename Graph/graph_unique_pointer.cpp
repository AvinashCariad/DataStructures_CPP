#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <memory>
#include <deque>

using namespace std;

// Weighted Edge of graph
struct WeightedEdge {
    int start, end;
    int weight;
    WeightedEdge(int start, int end, int weight) : start(start), end(end), weight(weight) {}
};

// Graph node
struct Vertex {
    int data;
    vector<WeightedEdge> edges;
    Vertex(int data) : data(data), edges({}) {}
};

// Creates a graph node using smart pointers for automated memory tracking
unique_ptr<Vertex> createNode(int data) {
    return make_unique<Vertex>(data);
}

// Adds an edge between two vertices
void addEdge(vector<unique_ptr<Vertex>>& g, int src, int dst, int weight = 1) {
    g[src]->edges.emplace_back(src, dst, weight);
}

// DFS Helper
void dfsHelper(const vector<unique_ptr<Vertex>>& g, Vertex* curr, unordered_map<Vertex*, bool>& visited) {
    visited[curr] = true;
    cout << curr->data << " ";

    // Traverse adjacent edges
    for (const auto& edge : curr->edges) {
        Vertex* neighbor = g[edge.end].get();
        if (visited.find(neighbor) == visited.end()) {
            dfsHelper(g, neighbor, visited);
        }
    }
}

// Driver for DFS of graph
// TC: O(V + E)
void DFS(const vector<unique_ptr<Vertex>>& g) {
    unordered_map<Vertex*, bool> visited;
    for (size_t i = 0; i < g.size(); i++) {
        if (visited.find(g[i].get()) == visited.end()) {
            dfsHelper(g, g[i].get(), visited);
        }
    }
    cout << endl;
}

// BFS of graph
// TC : O(V + E)
void BFS(const vector<unique_ptr<Vertex>>& g) {
    unordered_map<Vertex*, bool> visited;
    for (size_t i = 0; i < g.size(); i++) {
        if (visited.find(g[i].get()) != visited.end()) continue;

        queue<Vertex*> q;
        q.emplace(g[i].get());
        visited[g[i].get()] = true; // Mark visited IMMEDIATELY when pushed to avoid infinite loop

        while (!q.empty()) {
            Vertex* curr = q.front();
            q.pop();

            cout << curr->data << " ";

            for (const auto& edge : curr->edges) {
                Vertex* neighbor = g[edge.end].get();
                if (visited.find(neighbor) == visited.end()) {
                    visited[neighbor] = true; // Mark visited here!
                    q.emplace(neighbor);
                }
            }
        }
    }
    cout << endl;
}

// Prints the graph
void printGraph(const vector<unique_ptr<Vertex>>& g) {
    for (const auto& v : g) {
        cout << v->data << ": ";
        for (const auto& edge : v->edges)
            cout << g[edge.end]->data << "(" << edge.weight << ") ";
        cout << endl;
    }
}

// Creates a graph
vector<unique_ptr<Vertex>> createGraph(int n_vertices) {
    vector<unique_ptr<Vertex>> g;
    for (int i = 0; i < n_vertices; i++)
        g.emplace_back(createNode(i));
    return g;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////	UNION-FIND ////////////////////////////////////////////

class UnionFind {
private:
    vector<int> root_;
    vector<int> size_;
public:
    UnionFind(int n_nodes) {
        root_.resize(n_nodes);
        size_.resize(n_nodes, 1);
        for (int i = 0; i < n_nodes; i++) {
            root_[i] = i;
        }
    }

    // Finds the root of component with path compression
    int getRoot(int idx) {
        if (idx == root_[idx])
            return idx;
        return root_[idx] = getRoot(root_[idx]); // Recursive path compression is cleaner
    }

    // Union operation using size-based weight consideration
    void Union(int a, int b) {
        int root_a = getRoot(a);
        int root_b = getRoot(b);

        // Crucial fix: elements are already in the same component
        if (root_a == root_b) return;

        if (size_[root_b] < size_[root_a]) {
            root_[root_b] = root_a;
            size_[root_a] += size_[root_b];
        }
        else {
            root_[root_a] = root_b;
            size_[root_b] += size_[root_a];
        }
    }

    bool Find(int a, int b) {
        return getRoot(a) == getRoot(b);
    }

    void connectedComponents() {
        vector<int> result;
        for (size_t i = 0; i < root_.size(); i++) {
            if (root_[i] == i)
                result.emplace_back(size_[i]);
        }
        sort(result.begin(), result.end());
        for (const int& a : result)
            cout << a << " ";
        cout << endl;
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////   TOPOLOGICAL SORTING   ///////////////////////////////////////

// Modified version of DFS that prevents cycles from causing an infinite stack overflow
bool topologicalSortDFSHelper(int src, const vector<unique_ptr<Vertex>>& g,
    deque<int>& order, vector<int>& visited) {
    
    visited[src] = 1; // Visited status: 1 = Visiting (in recursion stack)

    for (const auto& edge : g[src]->edges) {
        if (visited[edge.end] == 1) return true; // Cycle detected!
        if (visited[edge.end] == 0) {
            if (topologicalSortDFSHelper(edge.end, g, order, visited))
                return true;
        }
    }
    visited[src] = 2; // Visited status: 2 = Fully processed
    order.push_front(src); // Pushing to front automatically sorts correctly
    return false;
}

vector<deque<int>> allTopologicalSortOrders(const vector<unique_ptr<Vertex>>& g) {
    const int n_vertices = g.size();
    vector<deque<int>> all_orders;
    
    for (int i = 0; i < n_vertices; i++) {
        deque<int> order;
        vector<int> visited(n_vertices, 0); // 0 = unvisited
        
        // If no cycles are detected originating from this root index execution path
        if (!topologicalSortDFSHelper(i, g, order, visited)) {
            all_orders.emplace_back(order);
        }
    }
    return all_orders;
}

// DFS tracking approach (Kahn alternative style)
bool topologicalDFS(int curr, const vector<unique_ptr<Vertex>>& g, vector<int>& order,
    vector<bool>& rec_stack, vector<bool>& visited) {
    
    if (visited[curr]) return false;

    visited[curr] = true;
    rec_stack[curr] = true;

    for (const auto& edge : g[curr]->edges) {
        if (rec_stack[edge.end]) return true; // Cycle
        if (topologicalDFS(edge.end, g, order, rec_stack, visited)) return true;
    }

    rec_stack[curr] = false;
    order.emplace_back(curr);
    return false;
}

vector<int> topologicalDFS(const vector<unique_ptr<Vertex>>& g) {
    vector<bool> visited(g.size(), false);
    vector<bool> rec_stack(g.size(), false);
    vector<int> order;

    for (size_t i = 0; i < g.size(); i++) {
        if (!visited[i]) {
            if (topologicalDFS(i, g, order, rec_stack, visited))
                return vector<int>{}; // Empty array indicates cycle error
        }
    }
    reverse(order.begin(), order.end()); // DFS topology needs reversal at end
    return order;
}

// Kahn's Algorithm (BFS-based Topological Sort)
vector<int> topologicalBFS(const vector<unique_ptr<Vertex>>& g) {
    vector<int> order;
    vector<int> indegree(g.size(), 0);

    // Compute in-degrees
    for (size_t i = 0; i < g.size(); i++) {
        for (const auto& edge : g[i]->edges) {
            indegree[edge.end]++;
        }
    }

    // Queue nodes with 0 in-degree
    queue<int> q;
    for (size_t i = 0; i < g.size(); i++) {
        if (indegree[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        order.push_back(curr);

        for (const auto& edge : g[curr]->edges) {
            indegree[edge.end]--;
            if (indegree[edge.end] == 0) {
                q.push(edge.end);
            }
        }
    }

    // If order size doesn't equal node size, a loop/cycle exists!
    if (order.size() != g.size()) return vector<int>{}; 
    
    // Note: DO NOT reverse the output array here. Kahn's naturally returns the proper dependency order.
    return order;
}


// main code harness:
int main(int argc, char** argv) {
    // Demonstration pipeline
    int vertices = 6;
    auto graph = createGraph(vertices);

    // Form a Directed Acyclic Graph (DAG) for testing Topological algorithms
    addEdge(graph, 5, 2);
    addEdge(graph, 5, 0);
    addEdge(graph, 4, 0);
    addEdge(graph, 4, 1);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 1);

    cout << "--- Graph Layout Structure ---" << endl;
    printGraph(graph);

    cout << "\n--- DFS Traversal Output ---" << endl;
    DFS(graph);

    cout << "\n--- BFS Traversal Output ---" << endl;
    BFS(graph);

    cout << "\n--- Topological Sorting (Kahn's BFS Method) ---" << endl;
    vector<int> kahnOrder = topologicalBFS(graph);
    for (int node : kahnOrder) cout << node << " ";
    cout << endl;

    cout << "\n--- Union-Find Validation Example ---" << endl;
    UnionFind uf(6);
    uf.Union(0, 1);
    uf.Union(1, 2);
    uf.Union(3, 4);
    cout << "Is 0 connected to 2? " << (uf.Find(0, 2) ? "Yes" : "No") << endl;
    cout << "Is 0 connected to 3? " << (uf.Find(0, 3) ? "Yes" : "No") << endl;
    cout << "Component sizes list: ";
    uf.connectedComponents();

    return 0;
}

