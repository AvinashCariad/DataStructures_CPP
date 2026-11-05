#include <iostream>
#include <vector>
#include <algorithm>

/*
Problem: Traveling Salesman Problem (TSP) using Memoization 
(Top-Down Dynamic Programming) with Bitmasking
This implementation solves the TSP problem using a recursive approach with memoization.
The TSP problem is defined as finding the minimum cost to visit all cities exactly
once and return

Given a list of cities and the distances between each pair, the goal is to find the 
shortest possible route that visits every city exactly once and returns to the origin city.


We can model TSP using graph theory notation:
• The Input: A complete, weighted graph G = (V, E), where:
	• \(V = \{v_1, v_2, \dots, v_n\}\) is a set of vertices representing the cities.
	• \(E = \{(v_i, v_j) : v_i, v_j \in V\}\) is a set of edges representing the paths between cities.
	• \(d(v_i, v_j)\) is a weight function assigned to each edge, representing the distance, cost, or time to travel from city \(v_{i}\) to \(v_{j}\).
• The Output: A permutation of vertices (a cycle) that minimizes the total edge weight:
\(\text{Minimize\ }\sum _{i=1}^{n-1}d(v_{\pi (i)},v_{\pi (i+1)})+d(v_{\pi (n)},v_{\pi (1)})\)

*/

using namespace std;

// Large value to represent infinity for unreachable paths
const int INF = 1e9;

/**
 * Solves TSP using Memoization (Top-Down Dynamic Programming) with Bitmasking
 * 
 * @param mask     Bitmask representing visited cities
 * @param pos      Current city index
 * @param n        Total number of cities
 * @param dist     The 2D distance matrix
 * @param memo     The caching table for minimum costs
 * @param parent   The tracking table to store the optimal next step
 * @return         Minimum cost from this state
 */
int tsp(int mask, int pos, int n, const vector<vector<int>>& dist, 
        vector<vector<int>>& memo, vector<vector<int>>& parent) {
    
    // Base Case: All cities visited, return cost to go back to starting city (0)
    if (mask == (1 << n) - 1) {
        return dist[pos][0]; 
    }

    // Return cached result if already computed
    if (memo[mask][pos] != -1) {
        return memo[mask][pos];
    }

    int minCost = INF;
    int bestNextCity = -1;

    // Try visiting all other unvisited cities
    for (int nextCity = 0; nextCity < n; ++nextCity) {
        // If the nextCity has NOT been visited yet
        if ((mask & (1 << nextCity)) == 0) {
            int newCost = dist[pos][nextCity] + tsp(mask | (1 << nextCity), nextCity, n, dist, memo, parent);
            
            if (newCost < minCost) {
                minCost = newCost;
                bestNextCity = nextCity;
            }
        }
    }

    // Record the optimal next city for this specific state
    parent[mask][pos] = bestNextCity;

    // Cache and return the cost
    return memo[mask][pos] = minCost;
}

/**
 * Reconstructs and prints the exact tour from the parent table
 */
void printPath(int n, const vector<vector<int>>& parent) {
    int mask = 1; // Start at city 0 (binary: ...0001)
    int currCity = 0;

    cout << "Optimal Path: " << currCity;

    // Trace the path forward until all bits are set
    while (mask != (1 << n) - 1) {
        int nextCity = parent[mask][currCity];
        cout << " -> " << nextCity;
        
        // Update mask to include nextCity, and move to it
        mask |= (1 << nextCity);
        currCity = nextCity;
    }

    // Complete the cycle by returning to the start
    cout << " -> 0" << endl;
}


// main code harness:
int main(int argc, char *argv[]) {
    // Example: Adjacency matrix representation of a graph with 4 cities
    vector<vector<int>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int n = dist.size();

    // Tables of size (2^n) x n
    vector<vector<int>> memo(1 << n, vector<int>(n, -1));
    vector<vector<int>> parent(1 << n, vector<int>(n, -1));

    // Start at city 0, mask is 1 (only city 0 visited)
    int startingMask = 1;
    int startingCity = 0;
    
    int totalMinCost = tsp(startingMask, startingCity, n, dist, memo, parent);

    cout << "The minimum possible travel cost is: " << totalMinCost << endl;
    printPath(n, parent);

    return 0;
}
