#include <iostream>
#include <vector>

using namespace std;


class Solution {
private:
    // Helper function to perform DFS using a tracking matrix
    void dfs(const std::vector<std::vector<char>>& grid, std::vector<std::vector<bool>>& visited, int r, int c) {     // note the visited is also the same dims as grid
        int rows = grid.size();
        int cols = grid[0].size();

        // Base cases: boundary check, water check, or already visited check - note the base case
        if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] == '0' || visited[r][c]) {
            return;
        }

        // Mark the current cell as visited - cell visited is marked inside the island
        visited[r][c] = true;

        // Recursively visit all 4 neighboring directions
        dfs(grid, visited, r + 1, c); // Down
        dfs(grid, visited, r - 1, c); // Up
        dfs(grid, visited, r, c + 1); // Right
        dfs(grid, visited, r, c - 1); // Left
    }

public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = grid.size();
        int cols = grid[0].size();
        int islandCount = 0;

        // Initialize a 2D visited grid with 'false' values matching the dimensions
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

        // Traverse every cell in the 2D grid
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                // If we hit unvisited land, it's a new island
                if (grid[r][c] == '1' && !visited[r][c]) {
                    islandCount++;         // note where the increment is being done
                    // Run DFS to map out and mark the entire connected component
                    dfs(grid, visited, r, c);      // this marks all the connected components as visited
                }
            }
        }

        return islandCount;
    }
};


// main code harness:
int main(int argc, char** argv) {
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
