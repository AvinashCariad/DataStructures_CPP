#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Large value to represent infinity for unreachable paths
const int INF = 1e9;


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
    int numStates = 1 << n; // 2^n states

    // DP Table: dp[mask][u] stores the min cost to visit remaining unvisited cities
    // given that 'mask' represents the cities ALREADY visited, and we are currently at city 'u'.
    vector<vector<int>> dp(numStates, vector<int>(n, INF));
    
    // Parent Table for tracking the path taken to reconstruct the tour
    vector<vector<int>> parent(numStates, vector<int>(n, -1));

    // Base Case: When all cities are visited (mask is all 1s), 
    // the only remaining cost is traveling back from the current city 'u' to the start (city 0).
    int allVisitedMask = numStates - 1;
    for (int u = 0; u < n; ++u) {
        dp[allVisitedMask][u] = dist[u][0];
    }

    // Bottom-Up Tabulation: Iterating from largest masks down to the smallest masks.
    // We skip the all-visited mask since it's already filled.
    for (int mask = allVisitedMask - 1; mask >= 1; --mask) {
        for (int currCity = 0; currCity < n; ++currCity) {
            
            // Optimization check: If the current city is not even marked as visited in this mask, 
            // this state is invalid, so skip it.
            if (!(mask & (1 << currCity))) continue;

            // Try traveling to any next city that hasn't been visited yet
            for (int nextCity = 0; nextCity < n; ++nextCity) {
                if (!(mask & (1 << nextCity))) { // If nextCity is unvisited
                    
                    int nextMask = mask | (1 << nextCity);
                    int newCost = dist[currCity][nextCity] + dp[nextMask][nextCity];
                    
                    if (newCost < dp[mask][currCity]) {
                        dp[mask][currCity] = newCost;
                        parent[mask][currCity] = nextCity; // Track the best decision
                    }
                }
            }
        }
    }

    // The starting state: Only city 0 is visited (mask = 1) and we are sitting at city 0.
    int minTotalCost = dp[1][0];
    cout << "The minimum possible travel cost is: " << minTotalCost << endl;

    // --- Path Reconstruction ---
    int currMask = 1;
    int currCity = 0;
    cout << "Optimal Path: " << currCity;

    while (currMask != allVisitedMask) {
        int nextCity = parent[currMask][currCity];
        cout << " -> " << nextCity;
        
        currMask |= (1 << nextCity);
        currCity = nextCity;
    }
    cout << " -> 0" << endl; // Complete the cycle back to start

    return 0;
}
