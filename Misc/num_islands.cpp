#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    // num-islands:
    int numIslands(vector<vector<char>>& grid) {

        if (grid.empty())
            return 0;

        int count = 0;

        for (int row = 0; row < grid.size(); row++) {

            for (int col = 0; col < grid[0].size(); col++) {

                if (grid[row][col] == '1') {

                    count++;

                    dfs(row, col, grid);      // explore the regions elsewhere
                }
            }
        }

        return count;
    }

private:
    // use dfs:
    void dfs(int row, int col, vector<vector<char>>& grid) {

        if (!isValid(row, col, grid))
            return;

        // Mark current cell as visited - mark this as visited
        grid[row][col] = '2';

        // Explore all four directions
        dfs(row - 1, col, grid); // Up
        dfs(row + 1, col, grid); // Down
        dfs(row, col - 1, grid); // Left
        dfs(row, col + 1, grid); // Right
    }

    bool isValid(int row, int col, vector<vector<char>>& grid) {

        if (row < 0 || row >= grid.size())
            return false;

        if (col < 0 || col >= grid[0].size())
            return false;

        return grid[row][col] == '1';
    }
};


// main code harness:
int main(int argc, char** argv) {

    vector<vector<char>> grid = {

        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}

    };

    Solution sol;

    cout << "Original Grid:\n";

    for (const auto& row : grid) {
        for (char c : row)
            cout << c << " ";
        cout << endl;
    }

    int islands = sol.numIslands(grid);

    cout << "\nNumber of Islands = " << islands << endl;

    cout << "\nGrid After DFS (visited cells marked as '2'):\n";

    for (const auto& row : grid) {
        for (char c : row)
            cout << c << " ";
        cout << endl;
    }

    return 0;
}
