#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;


/*
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


// Large value to represent infinity for unreachable paths
const int INF = 1e9;

// Type alias to make the state keys easier to read
using State = pair<set<int>, int>;

/**
 * Solves TSP using a set for tracking visited cities and maps for memoization and path tracking.
 * 
 * @param visited  A set containing the indices of all cities visited so far
 * @param pos      The current city index the salesman is at
 * @param n        Total number of cities
 * @param dist     The 2D distance/cost matrix between cities
 * @param memo     Map pairing the unique state to its minimum cost
 * @param parent   Map pairing the unique state to the optimal next city to visit
 * @return         The minimum cost to complete the tour from this state
 */
int tsp(set<int>& visited, int pos, int n, const vector<vector<int>>& dist, 
        map<State, int>& memo, map<State, int>& parent) {
    
    // Base Case: If all cities have been visited, return the cost to go back to the starting city (0)
    if (visited.size() == n) {
        return dist[pos][0]; // Explicitly returning to starting city 0
    }

    State currState = {visited, pos};

    // Return cached result if this state has already been computed
    if (memo.find(currState) != memo.end()) {
        return memo[currState];
    }

    int minCost = INF;
    int bestNextCity = -1;

    // Try visiting all other unvisited cities from the current position
    for (int nextCity = 0; nextCity < n; ++nextCity) {
        // Check if the nextCity is NOT in the visited set
        if (visited.find(nextCity) == visited.end()) {
            
            // Mark the next city as visited
            visited.insert(nextCity);

            // Calculate the cost of this path
            int newCost = dist[pos][nextCity] + tsp(visited, nextCity, n, dist, memo, parent);
            
            if (newCost < minCost) {
                minCost = newCost;
                bestNextCity = nextCity;
            }

            // Backtrack: remove the city from the visited set for other recursive paths
            visited.erase(nextCity);
        }
    }

    // Record the optimal choice for path reconstruction
    parent[currState] = bestNextCity;

    // Cache and return the optimal cost for this specific state
    return memo[currState] = minCost;
}

/**
 * Reconstructs and prints the path using the choices saved in the parent map
 */
void printPath(int n, map<State, int>& parent) {
    set<int> visited;
    int currCity = 0;
    visited.insert(currCity);

    cout << "Optimal Path: " << currCity;

    // Follow the recorded best choices until all cities are visited
    while (visited.size() < n) {
        State state = {visited, currCity};
        int nextCity = parent[state];
        
        cout << " -> " << nextCity;
        
        visited.insert(nextCity);
        currCity = nextCity;
    }

    // Complete the loop by returning to the start
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

    // memo Map:  {visited_set, current_city} -> min_cost
    // parent Map: {visited_set, current_city} -> optimal_next_city
    map<State, int> memo;
    map<State, int> parent;

    // Set to keep track of visited cities. Start by adding the starting city (0).
    set<int> visited;
    visited.insert(0);
    int startingCity = 0;

    int totalMinCost = tsp(visited, startingCity, n, dist, memo, parent);

    cout << "The minimum possible travel cost is: " << totalMinCost << endl;
    printPath(n, parent);

    return 0;
}


/*

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Use a large value to represent infinity for unreachable paths
const int INF = 1e9;


 //* Solves the Traveling Salesman Problem using Memoization (Top-Down Dynamic Programming)
 //* 
 //* @param mask   Bitmask representing the set of visited cities (e.g., 0011 means cities 0 and 1 are visited)
 //* @param pos    The current city index the salesman is at
 //* @param n      Total number of cities
 //* @param dist   The 2D distance/cost matrix between cities
 //* @param memo   The memoization table to cache computed subproblem states
 //* @return       The minimum cost to visit all remaining cities and return to the start (city 0)
 
int tsp(int mask, int pos, int n, const vector<vector<int>>& dist, vector<vector<int>>& memo) {
    // Base Case: If all cities have been visited (mask has all 'n' bits set to 1)
    if (mask == (1 << n) - 1) {
        return dist[pos][0]; // Return the cost to go back to the starting city (0)
    }

    // Return cached result if this state has already been computed
    if (memo[mask][pos] != -1) {
        return memo[mask][pos];
    }

    int minCost = INF;

    // Try visiting all other unvisited cities from the current position
    for (int nextCity = 0; nextCity < n; ++nextCity) {
        // Check if the nextCity has NOT been visited yet
        if ((mask & (1 << nextCity)) == 0) {
            // Calculate the total cost of moving to nextCity and solving the rest of the tour
            int newCost = dist[pos][nextCity] + tsp(mask | (1 << nextCity), nextCity, n, dist, memo);
            minCost = min(minCost, newCost);
        }
    }

    // Cache and return the optimal cost for the current state
    return memo[mask][pos] = minCost;
}


// main code harness:
int main(int argc, char *argv[]) {
    // Example: Adjacency matrix representation of a graph with 4 cities
    // dist[i][j] represents the travel cost from city i to city j
    vector<vector<int>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int n = dist.size();

    // Memoization table of size (2^n) x n initialized to -1
    // Rows represent unvisited/visited combinations (bitmask); columns represent the current city position
    vector<vector<int>> memo(1 << n, vector<int>(n, -1));

    // Start the tour at City 0. The bitmask is initially 1 (binary: 0001), indicating city 0 is visited.
    int startingMask = 1;
    int startingCity = 0;
    
    int totalMinCost = tsp(startingMask, startingCity, n, dist, memo);

    cout << "The minimum possible travel cost is: " << totalMinCost << endl;

    return 0;
}



*/