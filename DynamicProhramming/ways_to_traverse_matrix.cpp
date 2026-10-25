/*
    Find the number of ways in which you can reach the right most bottom (n-1, m-1) 
    location of 2d array starting from 0,0. We can move only right and down.

    Input: array dimensions
    Output: int

    Solution:
        (n-1, m-1) can be reached only from its top or left location. And those
        locations can be reached only in a similar way. So for each location (i,j)
        we need to find the no. of ways to reach (i-1, j) and (i, j-1).

        L(i, j) = Top + Left
        Top : L(i-1, j) when i >= 1, 0 otherwise 
        Left : L(i, j-1) when j >= 1, 0 otherwise

        TC: O(nm), n: no. of rows and m: no. of columns
        SC: O(nm)

        This code solves the classic Unique Paths problem using recursion + memoization.
*/

#include <iostream>
#include <vector>
using namespace std;

// finds the no. of ways to reach bottom right
int waysToReachMatrixEndHelper(int n, int m, vector<vector<int>>& ways_to_reach) {       // step to include memoization
    // if the location has been reached
    if(n == 0 && m == 0)
        return 1;
    
    if(ways_to_reach[n][m] == 0) {
        // check the no. of ways the left and top locations can be reached.
        // Also it is checked if it is the top row or first column or not

        // ways to reach the left location
        int reach_left = m == 0 ? 0 : waysToReachMatrixEndHelper(n, m-1, ways_to_reach);
        // ways to reach the top location
        int reach_top = n == 0 ? 0 : waysToReachMatrixEndHelper(n-1, m, ways_to_reach);

        ways_to_reach[n][m] = reach_left + reach_top;
    }

    return ways_to_reach[n][m];
}

int waysToReachMatrixEndHelper(int n, int m) {
    // each entry (i, j) stores the no. of ways that location can be 
    // from their left and top
    vector<vector<int>> ways_to_reach(n, vector<int>(m, 0));
    return waysToReachMatrixEndHelper(n - 1, m - 1, ways_to_reach); 
}


// main code harness:
int main(int argc, char** argv) {
	vector<vector<int>> matrix(5, vector<int>(5, 1));
	cout << "ways to reach the desired position: ";
    cout << waysToReachMatrixEndHelper(matrix.size(), matrix[0].size());
    cout << endl;
    return 0;
}



/*

// Another approach for the same problem above

#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int uniquePaths(int m, int n) {
        // Create a 2D DP array initialized to 1
        vector<vector<int>> dp(m, vector<int>(n, 1));    // note this initializes the starting step

        // Fill the DP table
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                // Ways to reach current cell = ways from above + ways from left
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }

        // Return the bottom-right corner value
        return dp[m - 1][n - 1];
    }
};


// main code harness:
int main(int argc, char** argv) {
    Solution sol;
    int m = 3, n = 7;
    cout << "Unique Paths: " << sol.uniquePaths(m, n) << endl;
    return 0;
}


*/