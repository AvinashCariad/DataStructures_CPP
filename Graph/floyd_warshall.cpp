// C++ program to implement Floyd-Warshall algorithm

#include <iostream>
#include <limits.h>
#include <vector>

#define INF INT_MAX

/*
The Floyd-Warshall algorithm is a dynamic programming technique used to find the shortest paths between all pairs of vertices in a weighted graph. This algorithm is particularly useful for graphs with dense connections and can handle both positive and negative edge weights, though it cannot handle negative cycles.

In this article, we will learn about the Floyd-Warshall algorithm and how to implement it in C++ language

The Floyd-Warshall algorithm works by considering all pairs of vertices and updating the shortest paths iteratively. It uses a matrix to represent the distances between each pair of vertices, initially setting the distance to infinity for all pairs except for the diagonal (distance from a vertex to itself), which is set to zero

The algorithm repeatedly updates the shortest distance between two vertices by checking if the distance can be minimized by passing through an intermediate vertex.

Initialize the distance matrix with given edge weights.
Set the distance from each vertex to itself as 0.
For each pair of vertices (i, j), update the distance by considering each vertex k as an intermediate vertex.
If the distance from i to j through k is less than the current distance from i to j, update the distance

Initialize the solution matrix same as the input graph matrix as a first step.
Then update the solution matrix by considering all vertices as an intermediate vertex.
The idea is to pick all vertices one by one and updates all shortest paths which include the picked vertex as an intermediate vertex in the shortest path.
When we pick vertex number k as an intermediate vertex, we already have considered vertices {0, 1, 2, .. k-1} as intermediate vertices.
For every pair (i, j) of the source and destination vertices respectively, there are two possible cases.
k is not an intermediate vertex in shortest path from i to j. We keep the value of dist[i][j] as it is.
k is an intermediate vertex in shortest path from i to j. We update the value of dist[i][j] as dist[i][k] + dist[k][j], if dist[i][j] > dist[i][k] + dist[k][j]


https://www.geeksforgeeks.org/cpp/floyd-warshall-algorithm-cpp/

*/




using namespace std;

// Function to print the solution matrix
void printSolution(const vector<vector<int>> &dist)
{
    int V = dist.size();
    cout << "The following matrix shows the shortest distances"
            " between every pair of vertices:\n";
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            if (dist[i][j] == INF)
                cout << "INF"
                     << "\t";
            else
                cout << dist[i][j] << "\t";
        }
        cout << endl;
    }
}

// Function to implement the Floyd-Warshall algorithm
void floydWarshall(vector<vector<int>> &graph)
{
    int V = graph.size();
    vector<vector<int>> dist = graph;

    // Update the solution matrix by considering all vertices
    for (int k = 0; k < V; ++k)         // Intermediate vertex
    {
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < V; ++j)
            {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // Print the shortest distance matrix
    printSolution(dist);
}


// main code harness:
int main(int argc, char** argv)
{
    /* Let us create the following weighted graph
           10
      (0)------->(3)
       |         /|\
     5 |          |
       |          | 1
      \|/         |
      (1)------->(2)
           3           */
    vector<vector<int>> graph = {{0, 5, INF, 10}, {INF, 0, 3, INF}, {INF, INF, 0, 1}, {INF, INF, INF, 0}};

    // Function call to implement Floyd-Warshall algorithm
    floydWarshall(graph);

    return 0;
}



/*

// https://github.com/TheAlgorithms/C-Plus-Plus/blob/master/dynamic_programming/floyd_warshall.cpp

#include <climits>
#include <cstddef>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

// Wrapper class for storing a graph
class Graph {
 public:
    int vertexNum;
    int **edges;

    // Constructs a graph with V vertices and E edges
    Graph(int V) {
        this->vertexNum = V;
        this->edges = new int *[V];
        for (int i = 0; i < V; i++) {
            this->edges[i] = new int[V];
            for (int j = 0; j < V; j++) this->edges[i][j] = INT_MAX;
            this->edges[i][i] = 0;
        }
    }

    ~Graph() {
        for (int i = 0; i < vertexNum; i++) {
            delete[] edges[i];
        }
        delete[] edges;
    }

    // Adds the given edge to the graph
    void addEdge(int src, int dst, int weight) {
        this->edges[src][dst] = weight;
    }
};

// Utility function to print distances
void print(const std::vector<int>& dist, int V) {
    cout << "\nThe Distance matrix for Floyd - Warshall" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i * V + j] != INT_MAX)
                cout << dist[i * V + j] << "\t";
            else
                cout << "INF"
                     << "\t";
        }
        cout << endl;
    }
}

// The main function that finds the shortest path from a vertex
// to all other vertices using Floyd-Warshall Algorithm.
void FloydWarshall(Graph graph) {
    std::size_t V = graph.vertexNum;
    std::vector<std::vector<int> > dist(V, std::vector<int>(V));

    // Initialise distance array
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++) dist[i][j] = graph.edges[i][j];

    // Calculate distances
    for (int k = 0; k < V; k++)
        // Choose an intermediate vertex

        for (int i = 0; i < V; i++)
            // Choose a source vertex for given intermediate

            for (int j = 0; j < V; j++)
                // Choose a destination vertex for above source vertex

                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                    // If the distance through intermediate vertex is less than
                    // direct edge then update value in distance array
                    dist[i][j] = dist[i][k] + dist[k][j];

    // Convert 2d array to 1d array for print
    std::vector<int> dist1d(V * V);
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++) dist1d[i * V + j] = dist[i][j];

    print(dist1d, V);
}

// Driver Function
int main(int argc, char** argv) {
    int V, E;
    int src, dst, weight;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Enter number of edges: ";
    cin >> E;
    Graph G(V);
    for (int i = 0; i < E; i++) {
        cout << "\nEdge " << i + 1 << "\nEnter source: ";
        cin >> src;
        cout << "Enter destination: ";
        cin >> dst;
        cout << "Enter weight: ";
        cin >> weight;
        G.addEdge(src, dst, weight);
    }
    FloydWarshall(G);

    return 0;
}



















*/