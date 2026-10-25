#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) {
            return;
        }

        int rows = static_cast<int>(board.size());
        int cols = static_cast<int>(board[0].size());

        // Step 1:
        // Temporarily change every 'O' to 'P'.
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                if (board[row][col] == 'O') {
                    board[row][col] = 'P';
                }
            }
        }

        // Step 2: - discard the boundaries - left/right/top/bottom
        // Restore all regions connected to the top and bottom borders.
        for (int col = 0; col < cols; ++col) {
            dfsCore(0, col, board);
            dfsCore(rows - 1, col, board);
        }

        // Restore all regions connected to the left and right borders.
        for (int row = 0; row < rows; ++row) {
            dfsCore(row, 0, board);
            dfsCore(row, cols - 1, board);
        }

        // Step 3:
        // Any remaining 'P' cell is fully surrounded.
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                if (board[row][col] == 'P') {
                    board[row][col] = 'X';
                }
            }
        }
    }

private:
    void dfsCore(
        int row,
        int col,
        vector<vector<char>>& board)
    {
        int rows = static_cast<int>(board.size());
        int cols = static_cast<int>(board[0].size());

        if (row < 0 || row >= rows ||
            col < 0 || col >= cols ||
            board[row][col] != 'P')
        {
            return;
        }

        // This cell is connected to a border,
        // so restore it to 'O'.
        board[row][col] = 'O';

        dfsCore(row - 1, col, board); // Up
        dfsCore(row + 1, col, board); // Down
        dfsCore(row, col - 1, board); // Left
        dfsCore(row, col + 1, board); // Right
    }
};

void printBoard(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << ' ';
        }
        cout << '\n';
    }
}


// main code harness:
int main(int argc, char** argv) {
    vector<vector<char>> board = {
        {'X', 'X', 'X', 'X'},
        {'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'X'},
        {'X', 'O', 'X', 'X'}
    };

    cout << "Original board:\n";
    printBoard(board);

    Solution solution;
    solution.solve(board);

    cout << "\nBoard after solving:\n";
    printBoard(board);

    return 0;
}
