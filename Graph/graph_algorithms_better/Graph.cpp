#include "Graph.h"

#include <iostream>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include <stdexcept>

using namespace std;


// ================================================================
// Constructor
// ================================================================

Graph::Graph(int numberOfVertices,
             AdjacencyMatrix adjacencyMatrix,
             bool directed)
    : adjacencyMatrix(adjacencyMatrix),
      numberOfVertices(numberOfVertices),
      directed(directed)
{
    createNodes();
    createEdges();
    createAdjacencyList();
}


// ================================================================
// Destructor
// ================================================================

Graph::~Graph()
{
    // Graph owns the edges
    for (GraphEdge* edge : edges)
    {
        delete edge;
    }

    edges.clear();

    // Graph owns the nodes
    for (GraphNode* node : vertices)
    {
        delete node;
    }

    vertices.clear();

    adjacencyList.clear();
}


// ================================================================
// CREATE NODES
// ================================================================

void Graph::createNodes()
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        GraphNode* node = new GraphNode(i);

        vertices.push_back(node);
    }
}


// ================================================================
// CREATE EDGES FROM ADJACENCY MATRIX
//
// Assumption:
//
// adjacencyMatrix[i][j] == 0
//
// means there is no edge.
//
// For an undirected graph:
//
//      matrix[i][j] == matrix[j][i]
//
// We create the edge only once.
// ================================================================

void Graph::createEdges()
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        for (int j = 0; j < numberOfVertices; ++j)
        {
            int weight = adjacencyMatrix[i][j];

            // No edge
            if (weight == 0)
            {
                continue;
            }

            // ----------------------------------------------------
            // Undirected graph
            //
            // Example:
            //
            // matrix[0][1] = 5
            // matrix[1][0] = 5
            //
            // We want only ONE GraphEdge.
            // ----------------------------------------------------

            if (!directed && j <= i)
            {
                continue;
            }

            GraphEdge* edge =
                new GraphEdge(vertices[i],
                              vertices[j],
                              weight);

            edges.push_back(edge);
        }
    }
}


// ================================================================
// CREATE ADJACENCY LIST
//
// adjacencyList:
//
// node -> vector of all connected edges
//
// Example:
//
//         5
//     A ------- B
//     |
//   10|
//     |
//     C
//
// A -> AB, AC
// B -> AB
// C -> AC
//
// For directed graphs, only outgoing edges are inserted.
// ================================================================

void Graph::createAdjacencyList()
{
    adjacencyList.clear();

    // Add all vertices, including isolated ones.
    for (GraphNode* node : vertices)
    {
        adjacencyList[node] = {};
    }

    for (GraphEdge* edge : edges)
    {
        GraphNode* node1 = edge->getNode1();
        GraphNode* node2 = edge->getNode2();

        // Outgoing edge from node1
        adjacencyList[node1].push_back(edge);

        // For undirected graph, edge belongs to both endpoints.
        if (!directed)
        {
            adjacencyList[node2].push_back(edge);
        }
    }
}


// ================================================================
// ADD NODE
// ================================================================

GraphNode* Graph::addNode(int index)
{
    GraphNode* node = new GraphNode(index);

    vertices.push_back(node);

    adjacencyList[node] = {};

    ++numberOfVertices;

    return node;
}


// ================================================================
// ADD EDGE
// ================================================================

GraphEdge* Graph::addEdge(GraphNode* node1,
                          GraphNode* node2,
                          int weight)
{
    if (node1 == nullptr || node2 == nullptr)
    {
        return nullptr;
    }

    GraphEdge* edge =
        new GraphEdge(node1,
                      node2,
                      weight);

    edges.push_back(edge);

    adjacencyList[node1].push_back(edge);

    if (!directed)
    {
        adjacencyList[node2].push_back(edge);
    }

    return edge;
}


// ================================================================
// GETTERS
// ================================================================

const vector<GraphNode*>&
Graph::getVertices() const
{
    return vertices;
}


const vector<GraphEdge*>&
Graph::getEdges() const
{
    return edges;
}


const AdjacencyList&
Graph::getAdjacencyList() const
{
    return adjacencyList;
}


// ================================================================
// GET OTHER ENDPOINT OF AN UNDIRECTED EDGE
//
// Example:
//
// edge = A -------- B
//
// getOtherNode(edge, A) -> B
// getOtherNode(edge, B) -> A
// ================================================================

GraphNode* Graph::getOtherNode(GraphEdge* edge,
                               GraphNode* node) const
{
    if (edge == nullptr || node == nullptr)
    {
        return nullptr;
    }

    if (edge->getNode1() == node)
    {
        return edge->getNode2();
    }

    if (edge->getNode2() == node)
    {
        return edge->getNode1();
    }

    return nullptr;
}


// ================================================================
// BFS
//
// Uses:
//
//      queue
//
// Time:
//
//      O(V + E)
//
// Space:
//
//      O(V)
// ================================================================

vector<int> Graph::bfs(GraphNode* start)
{
    vector<int> result;

    if (start == nullptr)
    {
        return result;
    }

    unordered_set<GraphNode*> visited;

    queue<GraphNode*> q;

    // Mark when inserting into queue.
    visited.insert(start);

    q.push(start);

    while (!q.empty())
    {
        GraphNode* current = q.front();

        q.pop();

        result.push_back(
            current->getIndex()
        );

        auto it = adjacencyList.find(current);

        if (it == adjacencyList.end())
        {
            continue;
        }

        for (GraphEdge* edge : it->second)
        {
            GraphNode* neighbor = nullptr;

            if (directed)
            {
                // adjacency list only contains outgoing edges
                neighbor = edge->getNode2();
            }
            else
            {
                neighbor =
                    getOtherNode(edge,
                                 current);
            }

            if (neighbor == nullptr)
            {
                continue;
            }

            if (visited.find(neighbor) ==
                visited.end())
            {
                visited.insert(neighbor);

                q.push(neighbor);
            }
        }
    }

    return result;
}


// ================================================================
// DFS HELPER
// ================================================================

void Graph::dfsHelper(
        GraphNode* node,
        unordered_set<GraphNode*>& visited,
        vector<int>& result)
{
    visited.insert(node);

    result.push_back(
        node->getIndex()
    );

    auto it = adjacencyList.find(node);

    if (it == adjacencyList.end())
    {
        return;
    }

    for (GraphEdge* edge : it->second)
    {
        GraphNode* neighbor = nullptr;

        if (directed)
        {
            neighbor =
                edge->getNode2();
        }
        else
        {
            neighbor =
                getOtherNode(edge,
                             node);
        }

        if (neighbor == nullptr)
        {
            continue;
        }

        if (visited.find(neighbor) ==
            visited.end())
        {
            dfsHelper(neighbor,
                      visited,
                      result);
        }
    }
}


// ================================================================
// DFS
//
// Time:
//
//      O(V + E)
//
// Space:
//
//      O(V)
// ================================================================

vector<int> Graph::dfs(GraphNode* start)
{
    vector<int> result;

    if (start == nullptr)
    {
        return result;
    }

    unordered_set<GraphNode*> visited;

    dfsHelper(start,
              visited,
              result);

    return result;
}


// ================================================================
// PATH EXISTS
//
// Uses BFS.
//
// Time:
//
//      O(V + E)
// ================================================================

bool Graph::pathExists(GraphNode* source,
                       GraphNode* destination)
{
    if (source == nullptr ||
        destination == nullptr)
    {
        return false;
    }

    if (source == destination)
    {
        return true;
    }

    queue<GraphNode*> q;

    unordered_set<GraphNode*> visited;

    q.push(source);

    visited.insert(source);

    while (!q.empty())
    {
        GraphNode* current =
            q.front();

        q.pop();

        for (GraphEdge* edge :
             adjacencyList[current])
        {
            GraphNode* neighbor = nullptr;

            if (directed)
            {
                neighbor =
                    edge->getNode2();
            }
            else
            {
                neighbor =
                    getOtherNode(edge,
                                 current);
            }

            if (neighbor == nullptr)
            {
                continue;
            }

            if (neighbor == destination)
            {
                return true;
            }

            if (!visited.count(neighbor))
            {
                visited.insert(neighbor);

                q.push(neighbor);
            }
        }
    }

    return false;
}


// ================================================================
// CONNECTED COMPONENTS
//
// Intended for UNDIRECTED graphs.
//
// Time:
//
//      O(V + E)
// ================================================================

int Graph::connectedComponents()
{
    unordered_set<GraphNode*> visited;

    int componentCount = 0;

    for (GraphNode* node : vertices)
    {
        if (visited.count(node))
        {
            continue;
        }

        ++componentCount;

        queue<GraphNode*> q;

        q.push(node);

        visited.insert(node);

        while (!q.empty())
        {
            GraphNode* current =
                q.front();

            q.pop();

            for (GraphEdge* edge :
                 adjacencyList[current])
            {
                GraphNode* neighbor =
                    getOtherNode(edge,
                                 current);

                if (neighbor == nullptr)
                {
                    continue;
                }

                if (!visited.count(neighbor))
                {
                    visited.insert(neighbor);

                    q.push(neighbor);
                }
            }
        }
    }

    return componentCount;
}


// ================================================================
// UNDIRECTED CYCLE DETECTION HELPER
//
// Important:
//
// If we see the parent again, that is NOT a cycle.
//
//       A
//       |
//       B
//
// When DFS(B) sees A, A is simply the parent.
//
// But:
//
//       A
//      / \
//     B---C
//
// If C sees A after coming from B,
// then a cycle exists.
// ================================================================

bool Graph::cycleUndirectedDFS(
        GraphNode* node,
        GraphNode* parent,
        unordered_set<GraphNode*>& visited)
{
    visited.insert(node);

    for (GraphEdge* edge :
         adjacencyList[node])
    {
        GraphNode* neighbor =
            getOtherNode(edge,
                         node);

        if (neighbor == nullptr)
        {
            continue;
        }

        if (!visited.count(neighbor))
        {
            if (cycleUndirectedDFS(
                    neighbor,
                    node,
                    visited))
            {
                return true;
            }
        }
        else if (neighbor != parent)
        {
            return true;
        }
    }

    return false;
}


// ================================================================
// UNDIRECTED CYCLE DETECTION
// ================================================================

bool Graph::hasCycleUndirected()
{
    unordered_set<GraphNode*> visited;

    for (GraphNode* node : vertices)
    {
        if (!visited.count(node))
        {
            if (cycleUndirectedDFS(
                    node,
                    nullptr,
                    visited))
            {
                return true;
            }
        }
    }

    return false;
}


// ================================================================
// DIRECTED CYCLE DETECTION
//
// States:
//
// 0 = unvisited
// 1 = currently in DFS recursion stack
// 2 = completely processed
//
// Back edge:
//
//      state[neighbor] == 1
//
// indicates a cycle.
// ================================================================

bool Graph::cycleDirectedDFS(
        GraphNode* node,
        unordered_map<GraphNode*, int>& state)
{
    state[node] = 1;

    for (GraphEdge* edge :
         adjacencyList[node])
    {
        GraphNode* neighbor =
            edge->getNode2();

        if (neighbor == nullptr)
        {
            continue;
        }

        if (state[neighbor] == 1)
        {
            return true;
        }

        if (state[neighbor] == 0)
        {
            if (cycleDirectedDFS(
                    neighbor,
                    state))
            {
                return true;
            }
        }
    }

    state[node] = 2;

    return false;
}


// ================================================================
// DIRECTED CYCLE DETECTION MAIN
// ================================================================

bool Graph::hasCycleDirected()
{
    unordered_map<GraphNode*, int> state;

    for (GraphNode* node : vertices)
    {
        state[node] = 0;
    }

    for (GraphNode* node : vertices)
    {
        if (state[node] == 0)
        {
            if (cycleDirectedDFS(
                    node,
                    state))
            {
                return true;
            }
        }
    }

    return false;
}


// ================================================================
// TOPOLOGICAL SORT - KAHN'S ALGORITHM
//
// Uses:
//
//      indegree + queue
//
// Valid only for directed acyclic graph.
//
// If result.size() != V,
// the graph contains a cycle.
// ================================================================

vector<int> Graph::topologicalSortKahn()
{
    vector<int> result;

    if (!directed)
    {
        return result;
    }

    unordered_map<GraphNode*, int> indegree;

    for (GraphNode* node : vertices)
    {
        indegree[node] = 0;
    }

    for (GraphEdge* edge : edges)
    {
        GraphNode* destination =
            edge->getNode2();

        ++indegree[destination];
    }

    queue<GraphNode*> q;

    for (GraphNode* node : vertices)
    {
        if (indegree[node] == 0)
        {
            q.push(node);
        }
    }

    while (!q.empty())
    {
        GraphNode* current =
            q.front();

        q.pop();

        result.push_back(
            current->getIndex()
        );

        for (GraphEdge* edge :
             adjacencyList[current])
        {
            GraphNode* neighbor =
                edge->getNode2();

            --indegree[neighbor];

            if (indegree[neighbor] == 0)
            {
                q.push(neighbor);
            }
        }
    }

    // Cycle detected
    if (result.size() != vertices.size())
    {
        result.clear();
    }

    return result;
}


// ================================================================
// TOPOLOGICAL SORT DFS HELPER
//
// Vertex is pushed AFTER all its descendants.
// ================================================================

void Graph::topoDFS(
        GraphNode* node,
        unordered_set<GraphNode*>& visited,
        stack<int>& result)
{
    visited.insert(node);

    for (GraphEdge* edge :
         adjacencyList[node])
    {
        GraphNode* neighbor =
            edge->getNode2();

        if (!visited.count(neighbor))
        {
            topoDFS(neighbor,
                    visited,
                    result);
        }
    }

    result.push(
        node->getIndex()
    );
}


// ================================================================
// TOPOLOGICAL SORT USING DFS
// ================================================================

vector<int> Graph::topologicalSortDFS()
{
    vector<int> result;

    if (!directed)
    {
        return result;
    }

    if (hasCycleDirected())
    {
        return result;
    }

    unordered_set<GraphNode*> visited;

    stack<int> st;

    for (GraphNode* node : vertices)
    {
        if (!visited.count(node))
        {
            topoDFS(node,
                    visited,
                    st);
        }
    }

    while (!st.empty())
    {
        result.push_back(
            st.top()
        );

        st.pop();
    }

    return result;
}


// ================================================================
// SHORTEST PATH IN AN UNWEIGHTED GRAPH
//
// BFS guarantees shortest path when every edge has equal cost.
//
// distance[source] = 0
//
// unreachable vertex = INF
// ================================================================

vector<int>
Graph::shortestPathUnweighted(
        GraphNode* source)
{
    const int INF =
        numeric_limits<int>::max();

    vector<int> distance(
        numberOfVertices,
        INF);

    if (source == nullptr)
    {
        return distance;
    }

    int sourceIndex =
        source->getIndex();

    if (sourceIndex < 0 ||
        sourceIndex >= numberOfVertices)
    {
        return distance;
    }

    queue<GraphNode*> q;

    distance[sourceIndex] = 0;

    q.push(source);

    while (!q.empty())
    {
        GraphNode* current =
            q.front();

        q.pop();

        int currentIndex =
            current->getIndex();

        for (GraphEdge* edge :
             adjacencyList[current])
        {
            GraphNode* neighbor = nullptr;

            if (directed)
            {
                neighbor =
                    edge->getNode2();
            }
            else
            {
                neighbor =
                    getOtherNode(
                        edge,
                        current);
            }

            if (neighbor == nullptr)
            {
                continue;
            }

            int neighborIndex =
                neighbor->getIndex();

            if (distance[neighborIndex] == INF)
            {
                distance[neighborIndex] =
                    distance[currentIndex] + 1;

                q.push(neighbor);
            }
        }
    }

    return distance;
}


// ================================================================
// DIJKSTRA
//
// Requirements:
//
//      Edge weights >= 0
//
// Data structure:
//
//      min priority queue
//
// Time:
//
//      O((V + E) log V)
// ================================================================

vector<int>
Graph::dijkstra(GraphNode* source)
{
    const int INF =
        numeric_limits<int>::max();

    vector<int> distance(
        numberOfVertices,
        INF);

    if (source == nullptr)
    {
        return distance;
    }

    int sourceIndex =
        source->getIndex();

    distance[sourceIndex] = 0;

    using QueueEntry =
        pair<int, GraphNode*>;

    priority_queue<
        QueueEntry,
        vector<QueueEntry>,
        greater<QueueEntry>
    > pq;

    pq.push({
        0,
        source
    });

    while (!pq.empty())
    {
        int currentDistance =
            pq.top().first;

        GraphNode* current =
            pq.top().second;

        pq.pop();

        int currentIndex =
            current->getIndex();

        // ------------------------------------------------
        // Ignore stale priority queue entry.
        //
        // Example:
        //
        // Earlier:
        // distance[A] = 10
        //
        // Later:
        // distance[A] = 5
        //
        // queue can still contain {10, A}.
        // ------------------------------------------------

        if (currentDistance >
            distance[currentIndex])
        {
            continue;
        }

        for (GraphEdge* edge :
             adjacencyList[current])
        {
            int weight =
                edge->getWeight();

            // Dijkstra cannot handle negative edges.
            if (weight < 0)
            {
                throw invalid_argument(
                    "Dijkstra cannot handle negative weights");
            }

            GraphNode* neighbor = nullptr;

            if (directed)
            {
                neighbor =
                    edge->getNode2();
            }
            else
            {
                neighbor =
                    getOtherNode(
                        edge,
                        current);
            }

            if (neighbor == nullptr)
            {
                continue;
            }

            int neighborIndex =
                neighbor->getIndex();

            if (distance[currentIndex] != INF &&
                distance[currentIndex] <=
                    INF - weight)
            {
                int newDistance =
                    distance[currentIndex]
                    + weight;

                if (newDistance <
                    distance[neighborIndex])
                {
                    distance[neighborIndex] =
                        newDistance;

                    pq.push({
                        newDistance,
                        neighbor
                    });
                }
            }
        }
    }

    return distance;
}


// ================================================================
// BELLMAN-FORD
//
// Supports negative-weight edges.
//
// Detects negative cycles.
//
// Time:
//
//      O(VE)
// ================================================================

vector<int>
Graph::bellmanFord(GraphNode* source)
{
    const int INF =
        numeric_limits<int>::max();

    vector<int> distance(
        numberOfVertices,
        INF);

    if (source == nullptr)
    {
        return distance;
    }

    distance[source->getIndex()] = 0;

    // ------------------------------------------------------------
    // Relax all edges V - 1 times.
    // ------------------------------------------------------------

    for (int iteration = 1;
         iteration < numberOfVertices;
         ++iteration)
    {
        bool changed = false;

        for (GraphEdge* edge : edges)
        {
            int u =
                edge->getNode1()
                    ->getIndex();

            int v =
                edge->getNode2()
                    ->getIndex();

            int weight =
                edge->getWeight();

            if (distance[u] != INF)
            {
                long long candidate =
                    static_cast<long long>(
                        distance[u])
                    + weight;

                if (candidate < distance[v])
                {
                    distance[v] =
                        static_cast<int>(
                            candidate);

                    changed = true;
                }
            }

            // --------------------------------------------
            // An undirected edge can be relaxed
            // in both directions.
            // --------------------------------------------

            if (!directed &&
                distance[v] != INF)
            {
                long long candidate =
                    static_cast<long long>(
                        distance[v])
                    + weight;

                if (candidate < distance[u])
                {
                    distance[u] =
                        static_cast<int>(
                            candidate);

                    changed = true;
                }
            }
        }

        // Optimization:
        //
        // If nothing changed, shortest distances
        // are already finalized.

        if (!changed)
        {
            break;
        }
    }

    // ------------------------------------------------------------
    // Negative cycle detection
    // ------------------------------------------------------------

    for (GraphEdge* edge : edges)
    {
        int u =
            edge->getNode1()
                ->getIndex();

        int v =
            edge->getNode2()
                ->getIndex();

        int weight =
            edge->getWeight();

        if (distance[u] != INF)
        {
            long long candidate =
                static_cast<long long>(
                    distance[u])
                + weight;

            if (candidate < distance[v])
            {
                throw runtime_error(
                    "Negative-weight cycle detected");
            }
        }

        if (!directed &&
            distance[v] != INF)
        {
            long long candidate =
                static_cast<long long>(
                    distance[v])
                + weight;

            if (candidate < distance[u])
            {
                throw runtime_error(
                    "Negative-weight cycle detected");
            }
        }
    }

    return distance;
}


// ================================================================
// FLOYD-WARSHALL
//
// Computes shortest path between EVERY pair.
//
// Time:
//
//      O(V^3)
//
// Space:
//
//      O(V^2)
// ================================================================

vector<vector<int>>
Graph::floydWarshall()
{
    const int INF =
        numeric_limits<int>::max() / 4;

    vector<vector<int>> distance(
        numberOfVertices,
        vector<int>(
            numberOfVertices,
            INF));

    // Distance from vertex to itself
    for (int i = 0;
         i < numberOfVertices;
         ++i)
    {
        distance[i][i] = 0;
    }

    // Add graph edges
    for (GraphEdge* edge : edges)
    {
        int u =
            edge->getNode1()
                ->getIndex();

        int v =
            edge->getNode2()
                ->getIndex();

        int weight =
            edge->getWeight();

        distance[u][v] =
            min(distance[u][v],
                weight);

        if (!directed)
        {
            distance[v][u] =
                min(distance[v][u],
                    weight);
        }
    }

    // ------------------------------------------------------------
    // Dynamic programming
    //
    // k = intermediate vertex
    //
    // dist[i][j] =
    //
    // min(
    //      old path i -> j,
    //      i -> k -> j
    // )
    // ------------------------------------------------------------

    for (int k = 0;
         k < numberOfVertices;
         ++k)
    {
        for (int i = 0;
             i < numberOfVertices;
             ++i)
        {
            for (int j = 0;
                 j < numberOfVertices;
                 ++j)
            {
                if (distance[i][k] == INF ||
                    distance[k][j] == INF)
                {
                    continue;
                }

                distance[i][j] =
                    min(
                        distance[i][j],
                        distance[i][k]
                        +
                        distance[k][j]
                    );
            }
        }
    }

    return distance;
}


// ================================================================
// PRIM MST
//
// Valid for:
//
//      weighted undirected graph
//
// Starts from one vertex.
//
// Time:
//
//      O(E log E)
//      approximately O(E log V)
// ================================================================

vector<GraphEdge*>
Graph::primMST(GraphNode* start)
{
    vector<GraphEdge*> mst;

    if (directed || start == nullptr)
    {
        return mst;
    }

    unordered_set<GraphNode*> visited;

    auto comparator =
        [](GraphEdge* edge1,
           GraphEdge* edge2)
        {
            return edge1->getWeight()
                >
                edge2->getWeight();
        };

    priority_queue<
        GraphEdge*,
        vector<GraphEdge*>,
        decltype(comparator)
    > pq(comparator);

    visited.insert(start);

    for (GraphEdge* edge :
         adjacencyList[start])
    {
        pq.push(edge);
    }

    while (!pq.empty() &&
           mst.size() <
               vertices.size() - 1)
    {
        GraphEdge* edge =
            pq.top();

        pq.pop();

        GraphNode* node1 =
            edge->getNode1();

        GraphNode* node2 =
            edge->getNode2();

        bool node1Visited =
            visited.count(node1);

        bool node2Visited =
            visited.count(node2);

        // Both endpoints already inside MST.
        //
        // Adding this edge would create a cycle.

        if (node1Visited &&
            node2Visited)
        {
            continue;
        }

        GraphNode* nextNode =
            node1Visited
            ? node2
            : node1;

        mst.push_back(edge);

        visited.insert(nextNode);

        for (GraphEdge* nextEdge :
             adjacencyList[nextNode])
        {
            GraphNode* other =
                getOtherNode(
                    nextEdge,
                    nextNode);

            if (other != nullptr &&
                !visited.count(other))
            {
                pq.push(nextEdge);
            }
        }
    }

    return mst;
}


// ================================================================
// KRUSKAL MST
//
// Uses:
//
//      Disjoint Set Union
//      Union-Find
//
// Time:
//
//      sorting = O(E log E)
//
// DSU operations are almost constant time.
// ================================================================

vector<GraphEdge*>
Graph::kruskalMST()
{
    vector<GraphEdge*> mst;

    if (directed)
    {
        return mst;
    }

    vector<GraphEdge*> sortedEdges =
        edges;

    sort(
        sortedEdges.begin(),
        sortedEdges.end(),
        [](GraphEdge* edge1,
           GraphEdge* edge2)
        {
            return edge1->getWeight()
                <
                edge2->getWeight();
        });

    vector<int> parent(
        numberOfVertices);

    vector<int> rank(
        numberOfVertices,
        0);

    for (int i = 0;
         i < numberOfVertices;
         ++i)
    {
        parent[i] = i;
    }

    // ------------------------------------------------------------
    // FIND with path compression
    // ------------------------------------------------------------

    function<int(int)> findSet =
        [&](int node)
        {
            if (parent[node] != node)
            {
                parent[node] =
                    findSet(
                        parent[node]);
            }

            return parent[node];
        };

    // ------------------------------------------------------------
    // UNION by rank
    // ------------------------------------------------------------

    auto unionSets =
        [&](int a,
            int b)
        {
            int rootA =
                findSet(a);

            int rootB =
                findSet(b);

            if (rootA == rootB)
            {
                return false;
            }

            if (rank[rootA] <
                rank[rootB])
            {
                swap(rootA,
                     rootB);
            }

            parent[rootB] =
                rootA;

            if (rank[rootA] ==
                rank[rootB])
            {
                ++rank[rootA];
            }

            return true;
        };

    for (GraphEdge* edge :
         sortedEdges)
    {
        int u =
            edge->getNode1()
                ->getIndex();

        int v =
            edge->getNode2()
                ->getIndex();

        if (unionSets(u, v))
        {
            mst.push_back(edge);

            if (mst.size() ==
                vertices.size() - 1)
            {
                break;
            }
        }
    }

    return mst;
}


// ================================================================
// BIPARTITE GRAPH
//
// Uses 2-color BFS.
//
// color:
//
// -1 = not colored
//  0 = first color
//  1 = second color
//
// Adjacent vertices must have different colors.
//
// Time:
//
//      O(V + E)
// ================================================================

bool Graph::isBipartite()
{
    if (directed)
    {
        // This implementation treats bipartiteness
        // as an undirected graph property.
        return false;
    }

    vector<int> color(
        numberOfVertices,
        -1);

    for (GraphNode* start :
         vertices)
    {
        int startIndex =
            start->getIndex();

        if (color[startIndex] != -1)
        {
            continue;
        }

        queue<GraphNode*> q;

        color[startIndex] = 0;

        q.push(start);

        while (!q.empty())
        {
            GraphNode* current =
                q.front();

            q.pop();

            int currentIndex =
                current->getIndex();

            for (GraphEdge* edge :
                 adjacencyList[current])
            {
                GraphNode* neighbor =
                    getOtherNode(
                        edge,
                        current);

                if (neighbor == nullptr)
                {
                    continue;
                }

                int neighborIndex =
                    neighbor->getIndex();

                if (color[neighborIndex] == -1)
                {
                    color[neighborIndex] =
                        1 - color[currentIndex];

                    q.push(neighbor);
                }
                else if (
                    color[neighborIndex]
                    ==
                    color[currentIndex])
                {
                    return false;
                }
            }
        }
    }

    return true;
}


// ================================================================
// ARTICULATION POINT DFS
//
// discovery[node]:
//
//      DFS discovery time
//
// low[node]:
//
//      earliest discovery time reachable
//      through subtree/back edge.
//
// A non-root vertex u is articulation if:
//
//      low[v] >= discovery[u]
//
// for any DFS child v.
//
// Root is articulation if it has > 1 DFS children.
// ================================================================

void Graph::articulationDFS(
        GraphNode* node,
        GraphNode* parent,
        int& timer,
        unordered_map<GraphNode*, int>& discovery,
        unordered_map<GraphNode*, int>& low,
        unordered_set<GraphNode*>& visited,
        unordered_set<int>& result)
{
    visited.insert(node);

    discovery[node] = timer;

    low[node] = timer;

    ++timer;

    int children = 0;

    for (GraphEdge* edge :
         adjacencyList[node])
    {
        GraphNode* neighbor =
            getOtherNode(
                edge,
                node);

        if (neighbor == nullptr)
        {
            continue;
        }

        if (neighbor == parent)
        {
            continue;
        }

        if (!visited.count(neighbor))
        {
            ++children;

            articulationDFS(
                neighbor,
                node,
                timer,
                discovery,
                low,
                visited,
                result);

            low[node] =
                min(
                    low[node],
                    low[neighbor]);

            // --------------------------------------------
            // Non-root articulation point
            // --------------------------------------------

            if (parent != nullptr &&
                low[neighbor] >=
                    discovery[node])
            {
                result.insert(
                    node->getIndex());
            }
        }
        else
        {
            // Back edge

            low[node] =
                min(
                    low[node],
                    discovery[neighbor]);
        }
    }

    // Root articulation point
    if (parent == nullptr &&
        children > 1)
    {
        result.insert(
            node->getIndex());
    }
}


// ================================================================
// ARTICULATION POINTS MAIN
// ================================================================

vector<int>
Graph::articulationPoints()
{
    vector<int> output;

    if (directed)
    {
        return output;
    }

    unordered_map<
        GraphNode*,
        int
    > discovery;

    unordered_map<
        GraphNode*,
        int
    > low;

    unordered_set<
        GraphNode*
    > visited;

    unordered_set<int> points;

    int timer = 0;

    for (GraphNode* node :
         vertices)
    {
        if (!visited.count(node))
        {
            articulationDFS(
                node,
                nullptr,
                timer,
                discovery,
                low,
                visited,
                points);
        }
    }

    output.assign(
        points.begin(),
        points.end());

    sort(
        output.begin(),
        output.end());

    return output;
}


// ================================================================
// BRIDGE DFS
//
// Edge u-v is a bridge if:
//
//      low[v] > discovery[u]
//
// Meaning:
//
// v's subtree cannot reach u or any ancestor of u
// without using edge u-v.
// ================================================================

void Graph::bridgeDFS(
        GraphNode* node,
        GraphNode* parent,
        int& timer,
        unordered_map<GraphNode*, int>& discovery,
        unordered_map<GraphNode*, int>& low,
        unordered_set<GraphNode*>& visited,
        vector<pair<int,int>>& result)
{
    visited.insert(node);

    discovery[node] = timer;

    low[node] = timer;

    ++timer;

    for (GraphEdge* edge :
         adjacencyList[node])
    {
        GraphNode* neighbor =
            getOtherNode(
                edge,
                node);

        if (neighbor == nullptr)
        {
            continue;
        }

        if (neighbor == parent)
        {
            continue;
        }

        if (!visited.count(neighbor))
        {
            bridgeDFS(
                neighbor,
                node,
                timer,
                discovery,
                low,
                visited,
                result);

            low[node] =
                min(
                    low[node],
                    low[neighbor]);

            if (low[neighbor] >
                discovery[node])
            {
                result.push_back(
                    {
                        node->getIndex(),
                        neighbor->getIndex()
                    });
            }
        }
        else
        {
            low[node] =
                min(
                    low[node],
                    discovery[neighbor]);
        }
    }
}


// ================================================================
// BRIDGES MAIN
// ================================================================

vector<pair<int,int>>
Graph::bridges()
{
    vector<pair<int,int>> result;

    if (directed)
    {
        return result;
    }

    unordered_map<
        GraphNode*,
        int
    > discovery;

    unordered_map<
        GraphNode*,
        int
    > low;

    unordered_set<
        GraphNode*
    > visited;

    int timer = 0;

    for (GraphNode* node :
         vertices)
    {
        if (!visited.count(node))
        {
            bridgeDFS(
                node,
                nullptr,
                timer,
                discovery,
                low,
                visited,
                result);
        }
    }

    return result;
}
