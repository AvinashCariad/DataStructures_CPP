#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

const int ROW = 20;
const int COL = 20;

struct Point {
    int r, c;
    bool operator==(const Point& o) const { return r == o.r && c == o.c; }
};

struct Cell {
    int parent_r, parent_c;
    int cost; // Cumulative distance/cost from source
};

struct Node {
    int cost;
    Point p;
    bool operator>(const Node& other) const { return cost > other.cost; }
};

bool isValid(int r, int c) {
    return (r >= 0) && (r < ROW) && (c >= 0) && (c < COL);
}

bool isUnblocked(const std::vector<std::vector<int>>& grid, int r, int c) {
    return grid[r][c] == 0;
}

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

    std::cout << "\n================ DIJKSTRA PATH ================\n\n";
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
    std::cout << "\n===============================================\n";
}

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

    std::cout << "\nPath coordinates count: " << path.size() << "\n";
    printVisualGrid(grid, path, src, dest);
}

void dijkstraSearch(const std::vector<std::vector<int>>& grid, Point src, Point dest) {
    if (!isValid(src.r, src.c) || !isValid(dest.r, dest.c)) return;
    if (!isUnblocked(grid, src.r, src.c) || !isUnblocked(grid, dest.r, dest.c)) return;
    if (src == dest) return;

    std::vector<std::vector<bool>> visited(ROW, std::vector<bool>(COL, false));
    std::vector<std::vector<Cell>> cellDetails(ROW, std::vector<Cell>(COL));

    for (int r = 0; r < ROW; ++r) {
        for (int c = 0; c < COL; ++c) {
            cellDetails[r][c].cost = 2e9;
            cellDetails[r][c].parent_r = -1;
            cellDetails[r][c].parent_c = -1;
        }
    }

    cellDetails[src.r][src.c].cost = 0;
    cellDetails[src.r][src.c].parent_r = src.r;
    cellDetails[src.r][src.c].parent_r = src.r; // wait, correct parent assignment:
    cellDetails[src.r][src.c].parent_r = src.r;
    cellDetails[src.r][src.c].parent_c = src.c;

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    pq.push({0, src});

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();

        Point p = curr.p;
        if (visited[p.r][p.c]) continue;
        visited[p.r][p.c] = true;

        if (p == dest) {
            std::cout << "The destination cell was found via Dijkstra!\n";
            tracePath(cellDetails, grid, src, dest);
            return;
        }

        for (int i = 0; i < 4; ++i) {
            int newR = p.r + dr[i];
            int newC = p.c + dc[i];

            if (isValid(newR, newC) && isUnblocked(grid, newR, newC) && !visited[newR][newC]) {
                int newCost = cellDetails[p.r][p.c].cost + 1;

                if (newCost < cellDetails[newR][newC].cost) {
                    cellDetails[newR][newC].cost = newCost;
                    cellDetails[newR][newC].parent_r = p.r;
                    cellDetails[newR][newC].parent_c = p.c;
                    pq.push({newCost, {newR, newC}});
                }
            }
        }
    }
    std::cout << "Failed to find the Destination Cell\n";
}


// main code harness:
int main(int argc, char** argv) {
    std::vector<std::vector<int>> grid = {
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
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
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}
    };

    Point src = {1, 1};  
    Point dest = {18, 18}; 

    dijkstraSearch(grid, src, dest);

    return 0;
}

