#include <iostream>
#include <vector>

class Solution {
private:
    // Helper function to perform DFS and sink the current island
    void dfs(std::vector<std::vector<char>>& grid, int r, int c) {
        int rows = grid.size();
        int cols = grid[0].size();

        // Base cases for recursion: boundary check and water check
        if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] == '0') {
            return;
        }

        // Mark the current cell as visited by turning it into water ('0')   - note this trick
        grid[r][c] = '0';

        // Recursively visit all 4 neighboring directions
        dfs(grid, r + 1, c); // Down
        dfs(grid, r - 1, c); // Up
        dfs(grid, r, c + 1); // Right
        dfs(grid, r, c - 1); // Left
    }

public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int islandCount = 0;
        int rows = grid.size();
        int cols = grid[0].size();

        // Traverse every cell in the 2D grid
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                // If we encounter land ('1'), we found a new island
                if (grid[r][c] == '1') {
                    islandCount++;
                    // Run DFS to "sink" the entire connected island
                    dfs(grid, r, c);
                }
            }
        }

        return islandCount;
    }
};

int main() {
    Solution solver;
    std::vector<std::vector<char>> grid = {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}
    };

    std::cout << "Number of islands: " << solver.numIslands(grid) << std::endl; // Output: 3
    return 0;
}
