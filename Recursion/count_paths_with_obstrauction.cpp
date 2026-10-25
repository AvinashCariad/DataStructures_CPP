#include <iostream>
#include <vector>
using namespace std;


/*
To handle obstructions in the maze, your recursion needs an extra parameter: 
a 2D grid representation where an obstruction is marked (commonly with a 1 for
a wall and 0 for an open path).You must add an explicit safety check at the very
beginning of your recursive function to instantly return 0 if the current cell is
blocked. Additionally, you can no longer use the shortcut if (i == n - 1 || j == m - 1) return 1;
because a wall might block that straight path. Instead, you check if you have successfully reached the final cell (n-1, m-1).
*/

// n = total rows, m = total columns
int countPathsWithObstacles(int i, int j, int n, int m, const vector<vector<int>>& grid)
{
    // 1. Boundary check OR if the current cell is an obstruction
    if (i >= n || j >= m || grid[i][j] == 1)
    {
        return 0;
    }

    // 2. Base case: Successfully reached the target cell (bottom-right corner)
    if (i == n - 1 && j == m - 1)
    {
        return 1;
    }

    // Move right (increases column j) + Move down (increases row i)
    int rightPaths = countPathsWithObstacles(i, j + 1, n, m, grid);
    int downPaths  = countPathsWithObstacles(i + 1, j, n, m, grid);

    return rightPaths + downPaths;
}

// main code harness:
int main(int argc, char** argv)
{
    // 0 = Empty Space, 1 = Obstruction
    vector<vector<int>> grid = {
        {0, 0, 0},
        {0, 1, 0}, // Obstacle right in the middle
        {0, 0, 0}
    };

    int n = grid.size();       // Number of rows
    int m = grid[0].size();    // Number of columns

    cout << "Total paths with obstacles: " << countPathsWithObstacles(0, 0, n, m, grid) << endl; 
    // Output: 2 (Right->Down->Down->Right OR Down->Right->Right->Down)
    
    return 0;
}


/*

// Optimized approach:
int countPathsDP(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    
    // If the start or end is blocked, no paths are possible
    if (grid[0][0] == 1 || grid[n-1][m-1] == 1) return 0;

    vector<vector<int>> dp(n, vector<int>(m, 0));
    
    // Base case: Starting position
    dp[0][0] = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Skip the starting position or any obstacle
            if ((i == 0 && j == 0) || grid[i][j] == 1) continue;

            int pathsFromTop = (i > 0) ? dp[i-1][j] : 0;
            int pathsFromLeft = (j > 0) ? dp[i][j-1] : 0;

            dp[i][j] = pathsFromTop + pathsFromLeft;
        }
    }
    return dp[n-1][m-1];
}


*/