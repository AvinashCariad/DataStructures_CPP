#include <iostream>
#include <vector>
#include <functional>

using namespace std;

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {

        if (grid.empty())
            return 0;

        int rows = grid.size();
        int cols = grid[0].size();

        vector<vector<bool>> visited(
            rows,
            vector<bool>(cols, false));

        int islands = 0;

        // Recursive lambda
        function<void(int,int)> dfs = [&](int row, int col)
        {
            // Base cases
            if (row < 0 || row >= rows ||
                col < 0 || col >= cols ||
                grid[row][col] != '1' ||
                visited[row][col])
            {
                return;
            }

            visited[row][col] = true;

            dfs(row - 1, col); // Up
            dfs(row + 1, col); // Down
            dfs(row, col - 1); // Left
            dfs(row, col + 1); // Right
        };

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (grid[row][col] == '1' &&
                    !visited[row][col])
                {
                    islands++;
                    dfs(row, col);
                }
            }
        }

        return islands;
    }
};


// main code harness:
int main(int argc, char** argv)
{
    vector<vector<char>> grid =
    {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };

    Solution sol;

    cout << "Original Grid:\n";

    for (const auto &row : grid)
    {
        for (char c : row)
            cout << c << " ";
        cout << endl;
    }

    int answer = sol.numIslands(grid);

    cout << "\nNumber of Islands = " << answer << endl;

    return 0;
}
