#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int numEnclaves(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0;
        }

        int rows = grid.size();
        int cols = grid[0].size();

        // Remove land connected to left and right boundaries.
        for (int row = 0; row < rows; row++) {
            dfsCore(row, 0, grid);
            dfsCore(row, cols - 1, grid);
        }

        // Remove land connected to top and bottom boundaries.
        for (int col = 0; col < cols; col++) {
            dfsCore(0, col, grid);
            dfsCore(rows - 1, col, grid);
        }

        int count = 0;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (grid[row][col] == 1) {
                    count++;
                }
            }
        }

        return count;
    }

private:
    void dfsCore(int row,
                 int col,
                 vector<vector<int>>& grid)
    {
        int rows = grid.size();
        int cols = grid[0].size();

        if (row < 0 || row >= rows ||
            col < 0 || col >= cols ||
            grid[row][col] != 1)
        {
            return;
        }

        grid[row][col] = 0;

        dfsCore(row - 1, col, grid);
        dfsCore(row + 1, col, grid);
        dfsCore(row, col - 1, grid);
        dfsCore(row, col + 1, grid);
    }
};


// main code harness:
int main(int argc, char** argv){
    vector<vector<int>> grid = {
        {0, 0, 0, 0},
        {1, 0, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };

    Solution solution;

    int result = solution.numEnclaves(grid);

    cout << "Number of enclave land cells: "
         << result << endl;

    return 0;
}
