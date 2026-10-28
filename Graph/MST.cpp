#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>

using namespace std;

/*
 
Recommended Implementation Choice

• Use Kruskal's Algorithm if your graph is sparse (fewer edges). It sorts all edges globally and uses Union-Find to avoid cycles.
• Use Prim's Algorithm if your graph is dense (lots of edges). It builds outward from a starting node using a priority queue.
*/


// --- Graph Infrastructure (From your framework) ---
struct WeightedEdge {
    int start, end;
    int weight;
    WeightedEdge(int start, int end, int weight) : start(start), end(end), weight(weight) {}
};

struct Vertex {
    int data;
    vector<WeightedEdge> edges;
    Vertex(int data) : data(data), edges({}) {}
};

// --- Comparator for Prim's Priority Queue ---
// Solves compilation issues by using a functor instead of std::function templates
struct CompareEdge {
    bool operator()(const WeightedEdge& e1, const WeightedEdge& e2) {
        return e1.weight > e2.weight; // Min-heap behavior
    }
};

// --- Union-Find Infrastructure (For Kruskal's) ---
class UnionFind {
private:
    vector<int> root_;
    vector<int> size_;
public:
    UnionFind(int n_nodes) {
        root_.resize(n_nodes);
        size_.resize(n_nodes, 1);
        for (int i = 0; i < n_nodes; i++) root_[i] = i;
    }

    int getRoot(int idx) {
        if (idx == root_[idx]) return idx;
        return root_[idx] = getRoot(root_[idx]); // Path compression
    }

    void Union(int a, int b) {
        int root_a = getRoot(a);
        int root_b = getRoot(b);
        if (root_a == root_b) return;

        if (size_[root_b] < size_[root_a]) {
            root_[root_b] = root_a;
            size_[root_a] += size_[root_b];
        } else {
            root_[root_a] = root_b;
            size_[root_b] += size_[root_a];
        }
    }
};

// --- 1. Kruskal's Algorithm ---
// Time Complexity: O(E log E) due to edge sorting
// Space Complexity: O(V + E)
vector<WeightedEdge> kruskalMST(const vector<unique_ptr<Vertex>>& g) {
    int n_vertices = g.size();
    vector<WeightedEdge> all_edges;
    vector<WeightedEdge> mst;

    // 1. Gather all unique edges from the graph
    for (size_t i = 0; i < g.size(); i++) {
        for (const auto& edge : g[i]->edges) {
            // In an undirected graph, avoid duplicate processing of edges (src -> dest and dest -> src)
            if (edge.start < edge.end) {
                all_edges.push_back(edge);
            }
        }
    }

    // 2. Sort all edges in non-decreasing order of their weights
    sort(all_edges.begin(), all_edges.end(), [](const WeightedEdge& a, const WeightedEdge& b) {
        return a.weight < b.weight;
    });

    UnionFind uf(n_vertices);

    // 3. Iterate through sorted edges and pick the smallest non-cycling edges
    for (const auto& edge : all_edges) {
        if (uf.getRoot(edge.start) != uf.getRoot(edge.end)) {
            uf.Union(edge.start, edge.end);
            mst.push_back(edge);
        }
        
        // Optimisation: An MST always has exactly V - 1 edges
        if (mst.size() == static_cast<size_t>(n_vertices - 1)) {
            break;
        }
    }

    return mst;
}

// --- 2. Prim's Algorithm ---
// Time Complexity: O(E log V) using a binary heap
// Space Complexity: O(V + E)
vector<WeightedEdge> primMST(const vector<unique_ptr<Vertex>>& g, int start_node = 0) {
    int n_vertices = g.size();
    vector<WeightedEdge> mst;
    vector<bool> in_mst(n_vertices, false);
    
    // Min-priority queue storing edges sorted by weight
    priority_queue<WeightedEdge, vector<WeightedEdge>, CompareEdge> pq;

    // Mark the starting node as visited and push its initial edges into the pool
    in_mst[start_node] = true;
    for (const auto& edge : g[start_node]->edges) {
        pq.push(edge);
    }

    while (!pq.empty() && mst.size() < static_cast<size_t>(n_vertices - 1)) {
        WeightedEdge curr_edge = pq.top();
        pq.pop();

        int next_vertex = curr_edge.end;

        // If the destination node is already part of our MST tree mesh, skip it to prevent cycles
        if (in_mst[next_vertex]) continue;

        // Include this edge in the MST
        in_mst[next_vertex] = true;
        mst.push_back(curr_edge);

        // Feed all unvisited outward edges from this new vertex into the queue pool
        for (const auto& edge : g[next_vertex]->edges) {
            if (!in_mst[edge.end]) {
                pq.push(edge);
            }
        }
    }

    return mst;
}

// Helper to print the generated MST edges and sum total weights
void printMSTResult(const string& algo_name, const vector<WeightedEdge>& mst) {
    cout << "--- " << algo_name << " MST Output ---" << endl;
    int total_weight = 0;
    for (const auto& edge : mst) {
        cout << edge.start << " - " << edge.end << " (Weight: " << edge.weight << ")" << endl;
        total_weight += edge.weight;
    }
    cout << "Total MST Weight Cost: " << total_weight << "\n" << endl;
}

int main() {
    int vertices = 6;
    vector<unique_ptr<Vertex>> graph;
    for (int i = 0; i < vertices; i++) {
        graph.push_back(make_unique<Vertex>(i));
    }

    // Creating a standard connected undirected graph layout
    // Edge additions must be bidirectional for proper MST evaluation
    auto addUndirectedEdge = [&](int src, int dst, int weight) {
        graph[src]->edges.emplace_back(src, dst, weight);
        graph[dst]->edges.emplace_back(dst, src, weight);
    };

    addUndirectedEdge(0, 1, 4);
    addUndirectedEdge(0, 2, 4);
    addUndirectedEdge(1, 2, 2);
    addUndirectedEdge(1, 3, 3);
    addUndirectedEdge(2, 3, 1);
    addUndirectedEdge(2, 4, 4);
    addUndirectedEdge(3, 4, 2);
    addUndirectedEdge(3, 5, 4);
    addUndirectedEdge(4, 5, 3);

    // Compute via Kruskal's
    vector<WeightedEdge> kruskalRes = kruskalMST(graph);
    printMSTResult("Kruskal's Algorithm", kruskalRes);

    // Compute via Prim's
    vector<WeightedEdge> primRes = primMST(graph, 0);
    printMSTResult("Prim's Algorithm", primRes);

    return 0;
}

