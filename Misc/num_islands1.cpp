#include <iostream>
#include <vector>

using namespace std;

class Solution {

public:

    // compute the num of islands:
    int numIslands(vector<vector<char>>& grid)
    {
        if (grid.empty())
            return 0;

        int rows = grid.size();
        int cols = grid[0].size();

        vector<vector<bool>> visited(rows, vector<bool>(cols, false));   // fill all the rows and cols with false

        int islands = 0;

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (grid[row][col] == '1' && !visited[row][col])
                {
                    islands++;

                    dfs(row, col, grid, visited);
                }
            }
        }

        return islands;
    }

private:

    void dfs(
        int row,
        int col,
        vector<vector<char>>& grid,
        vector<vector<bool>>& visited)
    {
        if (!isValid(row, col, grid, visited))
            return;

        visited[row][col] = true;

        dfs(row - 1, col, grid, visited);
        dfs(row + 1, col, grid, visited);
        dfs(row, col - 1, grid, visited);
        dfs(row, col + 1, grid, visited);
    }

    bool isValid(
        int row,
        int col,
        vector<vector<char>>& grid,
        vector<vector<bool>>& visited)
    {
        if (row < 0 || row >= grid.size())
            return false;#include <iostream>
#include <vector>

using namespace std;

class Solution {

public:

    int numIslands(vector<vector<char>>& grid)
    {
        if (grid.empty())
            return 0;

        int rows = grid.size();
        int cols = grid[0].size();

        vector<vector<bool>> visited(
            rows,
            vector<bool>(cols, false));

        int islands = 0;

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (grid[row][col] == '1' &&
                    !visited[row][col])
                {
                    islands++;

                    dfs(row, col, grid, visited);
                }
            }
        }

        return islands;
    }

private:

    void dfs(
        int row,
        int col,
        vector<vector<char>>& grid,
        vector<vector<bool>>& visited)
    {
        if (!isValid(row, col, grid, visited))
            return;

        visited[row][col] = true;

        dfs(row - 1, col, grid, visited);
        dfs(row + 1, col, grid, visited);
        dfs(row, col - 1, grid, visited);
        dfs(row, col + 1, grid, visited);
    }

    bool isValid(
        int row,
        int col,
        vector<vector<char>>& grid,
        vector<vector<bool>>& visited)
    {
        if (row < 0 || row >= grid.size())
            return false;

        if (col < 0 || col >= grid[0].size())
            return false;

        if (visited[row][col])
            return false;

        return grid[row][col] == '1';
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

    for (const auto& row : grid)
    {
        for (char c : row)
            cout << c << " ";

        cout << endl;
    }

    int answer = sol.numIslands(grid);

    cout << "\nNumber of Islands = "
         << answer << endl;

    cout << "\nGrid After DFS\n";

    for (const auto& row : grid)
    {
        for (char c : row)
            cout << c << " ";

        cout << endl;
    }

    return 0;
}

        if (col < 0 || col >= grid[0].size())
            return false;

        if (visited[row][col])
            return false;

        return grid[row][col] == '1';
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

    for (const auto& row : grid)
    {
        for (char c : row)
            cout << c << " ";

        cout << endl;
    }

    int answer = sol.numIslands(grid);

    cout << "\nNumber of Islands = "
         << answer << endl;

    cout << "\nGrid After DFS\n";

    for (const auto& row : grid)
    {
        for (char c : row)
            cout << c << " ";

        cout << endl;
    }

    return 0;
}
