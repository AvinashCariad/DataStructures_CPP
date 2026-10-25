#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

//compile using the following command:  g++ -std=c++17 astar_visual.cpp -o astar && ./astar



const int ROW = 9;
const int COL = 10;

struct Point {
    int r, c;
    bool operator==(const Point& o) const { return r == o.r && c == o.c; }
};

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

// Visually maps out and prints the grid with the path drawn on it
void printVisualGrid(const std::vector<std::vector<int>>& grid, const std::vector<Point>& path, Point src, Point dest) {
    // Create a copy of the grid using characters for display
    std::vector<std::vector<char>> displayGrid(ROW, std::vector<char>(COL));

    for (int r = 0; r < ROW; ++r) {
        for (int c = 0; c < COL; ++c) {
            if (grid[r][c] == 1) {
                displayGrid[r][c] = '#'; // Temporary wall marker
            } else {
                displayGrid[r][c] = '.'; // Empty path
            }
        }
    }

    // Overlay the calculated path onto the display grid
    for (const auto& p : path) {
        displayGrid[p.r][p.c] = '*';
    }

    // Overlay explicit start and end symbols
    displayGrid[src.r][src.c] = 'S';
    displayGrid[dest.r][dest.c] = 'E';

    // Print the visual grid layout to terminal
    std::cout << "\n================ VISUAL PATH ================\n\n";
    for (int r = 0; r < ROW; ++r) {
        std::cout << "  ";
        for (int c = 0; c < COL; ++c) {
            if (displayGrid[r][c] == '#') {
                std::cout << "▒ "; // Obstacle block
            } else {
                std::cout << displayGrid[r][c] << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n=============================================\n";
}

// Traces the final path from destination to source using parent pointers
void tracePath(const std::vector<std::vector<Cell>>& cellDetails, const std::vector<std::vector<int>>& grid, Point src, Point dest) {
    int r = dest.r;
    int c = dest.c;

    std::vector<Point> path;

    while (!(cellDetails[r][c].parent_r == r && cellDetails[r][c].parent_c == c)) {
        path.push_back({r, c});
        int temp_r = cellDetails[r][c].parent_r;
        int temp_c = cellDetails[r][c].parent_c;
        r = temp_r;
        c = temp_c;
    }
    path.push_back({r, c});
    std::reverse(path.begin(), path.end());

    // Print text coordinates
    std::cout << "\nPath coordinates: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << "(" << path[i].r << "," << path[i].c << ")";
        if (i < path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";

    // Pass information to build the visual grid map
    printVisualGrid(grid, path, src, dest);
}

void aStarSearch(const std::vector<std::vector<int>>& grid, Point src, Point dest) {
    if (!isValid(src.r, src.c) || !isValid(dest.r, dest.c)) return;
    if (!isUnblocked(grid, src.r, src.c) || !isUnblocked(grid, dest.r, dest.c)) return;
    if (src == dest) return;

    std::vector<std::vector<bool>> closedList(ROW, std::vector<bool>(COL, false));
    std::vector<std::vector<Cell>> cellDetails(ROW, std::vector<Cell>(COL));

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
    cellDetails[r][c].parent_r = r;
    cellDetails[r][c].parent_c = c;

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
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
                if (newR == dest.r && newC == dest.c) {
                    cellDetails[newR][newC].parent_r = r;
                    cellDetails[newR][newC].parent_c = c;
                    std::cout << "The destination cell was found!\n";
                    tracePath(cellDetails, grid, src, dest);
                    return;
                }
                else if (!closedList[newR][newC] && isUnblocked(grid, newR, newC)) {
                    int gNew = cellDetails[r][c].g + 1;
                    int hNew = calculateHValue(newR, newC, dest);
                    int fNew = gNew + hNew;

                    if (cellDetails[newR][newC].f == 2e9 || cellDetails[newR][newC].f > fNew) {
                        openList.push({fNew, {newR, newC}});
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

int main() {
    std::vector<std::vector<int>> grid = {
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 1, 1, 0 },
        { 0, 0, 1, 1, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    Point src = {0, 0};  
    Point dest = {8, 9}; 

    aStarSearch(grid, src, dest);

    return 0;
}
