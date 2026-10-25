#include <iostream>
using namespace std;


/*
While recursion works perfectly for small grids, it calculates the exact same subproblems repeatedly.
For a larger grid (like 10 × 10), this code will run incredibly slow because its time complexity is
exponential: \(O(2^{n+m})\).

*/

// n = total rows, m = total columns
int countPaths(int i, int j, int n, int m)
{
    // Out of bounds boundary check
    if (i >= n || j >= m)
    {
        return 0;
    }

    // Shortcut: If you reach the last row or last column, 
    // there is only 1 straight path left to the destination.
    if (i == n - 1 || j == m - 1)
    {
        // Fixed: i is checked against n-1, j against m-1
        return 1; 
    }

    // Move right (increases column j) + Move down (increases row i)
    return countPaths(i, j + 1, n, m) + countPaths(i + 1, j, n, m);
}


// main code harness:
int main(int argc, char** argv)
{
    int n = 5, m = 5; 
    cout << "Total paths: " << countPaths(0, 0, n, m) << endl; // Output: 6
    return 0;
}




/*

// 1. Dynamic Programming (Tabulation) — \(O(n \times m)\) Time
// Math Formula (Combinatorics) — O(n) Time, O(1) SpaceBecause 
// you must always take exactly (n-1) down moves and (m-1) right moves, this is a permutation problem.
// The answer is simply a combination formula:\(\text{Total\ Paths}={(n-1)+(m-1) \choose n-1}=\frac{((n-1)+(m-1))!}{(n-1)!\times (m-1)!}\)

int countPathsDP(int n, int m) {
    int dp[n][m];
    
    // Fill the grid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i == 0 || j == 0) dp[i][j] = 1; // Base edges only have 1 path
            else dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }
    return dp[n-1][m-1];
}



*/
