#include <iostream>
#include <vector>
#include <cfloat>       // this can be used for DBL_MAX
#include <algorithm>

using namespace std;

class Graph {
private:
    int V;
    vector<vector<double>> adj;

public:
    Graph(int vertices)
        : V(vertices),
          adj(vertices, vector<double>(vertices, DBL_MAX))
    {
        // Distance from a vertex to itself is 0
        for (int i = 0; i < V; i++)
            adj[i][i] = 0;
    }

    // Add undirected edge
    void addEdge(int u, int v, double weight)
    {
        adj[u][v] = weight;
        adj[v][u] = weight;
    }

    int vertices() const
    {
        return V;
    }

    double getWeight(int u, int v) const
    {
        return adj[u][v];
    }
};



// Define module for Travelling Salesman Problem:
// The best path would define the optimum path, starting city and 
// the end city
double TSP(const Graph& graph, vector<int>& bestPath)
{
    int V = graph.vertices();

    /*
        Fix vertex 0 as the starting point.

        We only permute:
            1, 2, 3, ..., V-1

        This avoids generating equivalent tours that
        merely start at a different vertex.
    */
    vector<int> vertices;

    for (int i = 1; i < V; i++)
        vertices.push_back(i);

    double minPathWeight = DBL_MAX;

    do
    {
        double currentPathWeight = 0;

        int current = 0;

        bool validPath = true;

        vector<int> currentPath;

        currentPath.push_back(0);

        // Visit vertices according to current permutation
        for (int next : vertices)
        {
            double weight =
                graph.getWeight(current, next);

            // No edge exists
            if (weight == DBL_MAX)
            {
                validPath = false;
                break;
            }

            currentPathWeight += weight;

            currentPath.push_back(next);

            current = next;
        }

        // Return to starting vertex - returning weight also needs to be added
        if (validPath)
        {
            double returnWeight =
                graph.getWeight(current, 0);

            if (returnWeight == DBL_MAX)
            {
                validPath = false;
            }
            else
            {
                currentPathWeight += returnWeight;

                currentPath.push_back(0);
            }
        }

        // Update minimum
        if (validPath &&
            currentPathWeight < minPathWeight)
        {
            minPathWeight = currentPathWeight;

            bestPath = currentPath;
        }

    }
    while (next_permutation(
        vertices.begin(),
        vertices.end()
    ));

    return minPathWeight;
}



// main code harness:
int main(int argc, char** argv)
{
    int V = 4;

    Graph graph(V);

    /*
        Graph:

              10
         0 -------- 1
         |          |
       12|          |15
         |          |
         3 -------- 2
              20

        Valid Hamiltonian cycle:

        0 -> 1 -> 2 -> 3 -> 0

        Cost:
        10 + 15 + 20 + 12 = 57
    */

    graph.addEdge(0, 1, 10);
    graph.addEdge(1, 2, 15);
    graph.addEdge(2, 3, 20);
    graph.addEdge(3, 0, 12);

    vector<int> bestPath;

    double minPathWeight =
        TSP(graph, bestPath);

    if (minPathWeight == DBL_MAX)
    {
        cout << "No Hamiltonian cycle exists."
             << endl;
    }
    else
    {
        cout << "Minimum path weight: "
             << minPathWeight
             << endl;

        cout << "Path: ";

        for (int vertex : bestPath)
        {
            cout << vertex << " ";
        }

        cout << endl;
    }

    return 0;
}


/*

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

// Use a sufficiently large value representing infinity for unreachable paths
const double INF = 1e9;

class TSPSolver {
private:
    int num_cities;
    std::vector<std::vector<double>> distance_matrix;
    // memo[mask][pos] stores the minimum cost to visit the remaining unvisited cities
    // represented by the 'mask', starting from city 'pos'.
    std::vector<std::vector<double>> memo;
    // parent[mask][pos] stores the next city to visit, used to reconstruct the optimal tour.
    std::vector<std::vector<int>> parent;

    // Recursive DP function with bitmasking
    double solveTSP(int mask, int pos) {
        // Base Case: If all cities have been visited (mask contains all 1s)
        // Return the distance from the current city back to the starting city (0)
        if (mask == (1 << num_cities) - 1) {
            return distance_matrix[pos][0];
        }

        // Return the result immediately if it has already been computed
        if (memo[mask][pos] != -1.0) {
            return memo[mask][pos];
        }

        double min_cost = INF;
        int best_next_city = -1;

        // Try to visit all next unvisited cities
        for (int next_city = 0; next_city < num_cities; ++next_city) {
            // Check if the next_city has not been visited yet
            if ((mask & (1 << next_city)) == 0) {
                // Calculate the path cost if we travel to next_city
                double current_cost = distance_matrix[pos][next_city] + 
                                      solveTSP(mask | (1 << next_city), next_city);
                
                if (current_cost < min_cost) {
                    min_cost = current_cost;
                    best_next_city = next_city;
                }
            }
        }

        parent[mask][pos] = best_next_city;
        return memo[mask][pos] = min_cost;
    }

public:
    TSPSolver(const std::vector<std::vector<double>>& matrix) {
        distance_matrix = matrix;
        num_cities = matrix.size();
        memo.assign(1 << num_cities, std::vector<double>(num_cities, -1.0));
        parent.assign(1 << num_cities, std::vector<int>(num_cities, -1));
    }

    void computeAndPrintTour() {
        // Start at city 0, with only city 0 marked as visited (mask = 1)
        double total_distance = solveTSP(1, 0);

        std::cout << "Minimum Distance: " << std::fixed << std::setprecision(2) << total_distance << "\n";
        
        // Reconstruct and print the optimal path sequence
        std::cout << "Optimal Tour Path: ";
        int mask = 1;
        int curr_city = 0;
        
        std::cout << curr_city;
        while (true) {
            int next_city = parent[mask][curr_city];
            if (next_city == -1) break;
            std::cout << " -> " << next_city;
            mask |= (1 << next_city);
            curr_city = next_city;
        }
        std::cout << " -> 0\n"; // Return to start
    }
};


// main code harness:
int main(int argc, char** argv) {
    // Example 4x4 Distance Matrix (Symmetric TSP)
    // Rows and columns represent cities 0, 1, 2, and 3
    std::vector<std::vector<double>> graph = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    TSPSolver solver(graph);
    solver.computeAndPrintTour();

    return 0;
}












*/