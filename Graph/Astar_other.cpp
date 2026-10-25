#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

// Define 20x20 grid dimensions
const int ROW = 20;
const int COL = 20;

struct Point {
    int r, c;
    bool operator==(const Point& o) const { return r == o.r && c == o.c; }
};

// Complete cell architecture structure
struct Cell {
    int parent_r, parent_c;
    int g, h, f;
};

struct Node {
    int f;
    Point p;
    bool operator>(const Node& other) const { return f > other.f; }
};

bool isValid(int r, int c) {
    return (r >= 0) && (r < ROW) && (c >= 0) && (c < COL);
}

bool isUnblocked(const std::vector<std::vector<int>>& grid, int r, int c) {
    return grid[r][c] == 0;
}

int calculateHValue(int r, int c, Point dest) {
    return std::abs(r - dest.r) + std::abs(c - dest.c);
}

// Visually maps out the full 20x20 matrix
void printVisualGrid(const std::vector<std::vector<int>>& grid, const std::vector<Point>& path, Point src, Point dest) {
    std::vector<std::vector<char>> displayGrid(ROW, std::vector<char>(COL));

    for (int r = 0; r < ROW; ++r) {
        for (int c = 0; c < COL; ++c) {
            if (grid[r][c] == 1) {
                displayGrid[r][c] = '#';
            } else {
                displayGrid[r][c] = '.';
            }
        }
    }

    for (const auto& p : path) {
        displayGrid[p.r][p.c] = '*';
    }

    displayGrid[src.r][src.c] = 'S';
    displayGrid[dest.r][dest.c] = 'E';

    std::cout << "\n================ VISUAL PATH ================\n\n";
    for (int r = 0; r < ROW; ++r) {
        std::cout << "  ";
        for (int c = 0; c < COL; ++c) {
            if (displayGrid[r][c] == '#') {
                std::cout << "▒ "; 
            } else {
                std::cout << displayGrid[r][c] << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n=============================================\n";
}

void tracePath(const std::vector<std::vector<Cell>>& cellDetails, const std::vector<std::vector<int>>& grid, Point src, Point dest) {
    int r = dest.r;
    int c = dest.c;

    std::vector<Point> path;

    // During the initialization phase of our A* algorithm, the starting cell (src) 
    // was uniquely configured to point to itself as its own parent. Every other cell in 
    // the grid points to whatever previous node stepped onto it.
    while (!(cellDetails[r][c].parent_r == r && cellDetails[r][c].parent_c == c)) {
        path.push_back({r, c});
        int temp_r = cellDetails[r][c].parent_r;
        int temp_c = cellDetails[r][c].parent_c;
        r = temp_r;
        c = temp_c;
    }
    path.push_back({r, c});
    std::reverse(path.begin(), path.end());

    std::cout << "\nPath coordinates: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << "(" << path[i].r << "," << path[i].c << ")";
        if (i < path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";

    printVisualGrid(grid, path, src, dest);
}

void aStarSearch(const std::vector<std::vector<int>>& grid, Point src, Point dest) {
    if (!isValid(src.r, src.c) || !isValid(dest.r, dest.c)) return;
    if (!isUnblocked(grid, src.r, src.c) || !isUnblocked(grid, dest.r, dest.c)) return;
    if (src == dest) return;

    std::vector<std::vector<bool>> closedList(ROW, std::vector<bool>(COL, false));    // note how it is being defined
    std::vector<std::vector<Cell>> cellDetails(ROW, std::vector<Cell>(COL));

    // Initialize:
    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
            cellDetails[r][c].f = 2e9;
            cellDetails[r][c].g = 2e9;
            cellDetails[r][c].parent_r = -1;
            cellDetails[r][c].parent_c = -1;
        }
    }

    int r = src.r, c = src.c;
    cellDetails[r][c].f = 0;
    cellDetails[r][c].g = 0;
    cellDetails[r][c].parent_r = r;    // point to itself as its own parent
    cellDetails[r][c].parent_c = c;    // point to itself as its own parent

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;      // min heap
    openList.push({0, {r, c}});        

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!openList.empty()) {
        Node curr = openList.top();
        openList.pop();

        r = curr.p.r;
        c = curr.p.c;
        closedList[r][c] = true;

        for (int i = 0; i < 4; i++) {
            int newR = r + dr[i];
            int newC = c + dc[i];

            if (isValid(newR, newC)) {
                // reached destination:
                if (newR == dest.r && newC == dest.c) {
                    cellDetails[newR][newC].parent_r = r;
                    cellDetails[newR][newC].parent_c = c;
                    std::cout << "The destination cell was found!\n";
                    tracePath(cellDetails, grid, src, dest);
                    return;
                } // else
                else if (!closedList[newR][newC] && isUnblocked(grid, newR, newC)) {
                    int gNew = cellDetails[r][c].g + 1;
                    int hNew = calculateHValue(newR, newC, dest);
                    int fNew = gNew + hNew;
                    // compares all the adjoining points:
                    if (cellDetails[newR][newC].f == 2e9 || cellDetails[newR][newC].f > fNew) {
                        openList.push({fNew, {newR, newC}});     // min heap, so will always store the min
                        cellDetails[newR][newC].f = fNew;
                        cellDetails[newR][newC].g = gNew;
                        cellDetails[newR][newC].parent_r = r;
                        cellDetails[newR][newC].parent_c = c;
                    }
                }
            }
        }
    }
    std::cout << "Failed to find the Destination Cell\n";
}


// main code harness:
int main(int argc, char** argv) {
    // 20x20 Grid using full matrix mapping layers
    // 0 = Walkable, 1 = ▒ Obstacle Block
    std::vector<std::vector<int>> grid = {
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0}, // src is at index [1][1]
        {0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0},
        {1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // dest is at index [18][18]
        {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}
    };

    // Non-zero layout start setup
    Point src = {1, 1};  
    Point dest = {18, 18}; 

    aStarSearch(grid, src, dest);

    return 0;
}
