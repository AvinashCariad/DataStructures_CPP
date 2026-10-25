#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Environment boundaries
const double MAP_WIDTH = 100.0;
const double MAP_HEIGHT = 100.0;

// Grid rendering resolution (100x100 space mapped to a 25x25 display grid)
const int DISPLAY_ROWS = 25;
const int DISPLAY_COLS = 25;

// RRT Parameters
const double STEP_SIZE = 6.0;       
const double GOAL_RADIUS = 4.0;     
const int MAX_ITERATIONS = 5000;    
const double GOAL_SAMPLE_RATE = 0.15; 

struct Node {
    double x, y;
    int parent_idx;
};

struct Obstacle {
    double x, y, radius;
};

bool isPointColliding(double x, double y, const std::vector<Obstacle>& obstacles) {
    for (const auto& obs : obstacles) {
        double dist = std::hypot(x - obs.x, y - obs.y);
        if (dist <= obs.radius) return true;
    }
    return false;
}

bool isSegmentColliding(double x1, double y1, double x2, double y2, const std::vector<Obstacle>& obstacles) {
    const int checks = 10; 
    for (int i = 0; i <= checks; ++i) {
        double t = static_cast<double>(i) / checks;
        double cx = x1 + t * (x2 - x1);
        double cy = y1 + t * (y2 - y1);
        if (isPointColliding(cx, cy, obstacles)) return true;
    }
    return false;
}

Node getRandomSample(double goal_x, double goal_y) {
    Node sample;
    double r = static_cast<double>(rand()) / RAND_MAX;
    if (r < GOAL_SAMPLE_RATE) {
        sample.x = goal_x;
        sample.y = goal_y;
    } else {
        sample.x = (static_cast<double>(rand()) / RAND_MAX) * MAP_WIDTH;
        sample.y = (static_cast<double>(rand()) / RAND_MAX) * MAP_HEIGHT;
    }
    sample.parent_idx = -1;
    return sample;
}

int getNearestNodeIdx(const std::vector<Node>& tree, const Node& sample) {
    int nearest_idx = 0;
    double min_dist = std::hypot(tree[0].x - sample.x, tree[0].y - sample.y);

    for (size_t i = 1; i < tree.size(); ++i) {
        double dist = std::hypot(tree[i].x - sample.x, tree[i].y - sample.y);
        if (dist < min_dist) {
            min_dist = dist;
            nearest_idx = i;
        }
    }
    return nearest_idx;
}

// Maps continuous coordinates to our smaller, discrete console character matrix
void drawVisualGrid(const std::vector<Node>& path, Node start, Node goal, const std::vector<Obstacle>& obstacles) {
    std::vector<std::vector<char>> display(DISPLAY_ROWS, std::vector<char>(DISPLAY_COLS, '.'));

    // 1. Render Obstacles
    for (int r = 0; r < DISPLAY_ROWS; ++r) {
        for (int c = 0; c < DISPLAY_COLS; ++c) {
            // Map grid indices back up to continuous space coordinates to check boundaries
            double map_x = (static_cast<double>(c) / DISPLAY_COLS) * MAP_WIDTH;
            double map_y = (static_cast<double>(r) / DISPLAY_ROWS) * MAP_HEIGHT;
            
            if (isPointColliding(map_x, map_y, obstacles)) {
                display[r][c] = '#'; // Mark obstacles with a standard ASCII cross hatch
            }
        }
    }

    // 2. Render Path Trajectory
    for (const auto& p : path) {
        int c = std::min(static_cast<int>((p.x / MAP_WIDTH) * DISPLAY_COLS), DISPLAY_COLS - 1);
        int r = std::min(static_cast<int>((p.y / MAP_HEIGHT) * DISPLAY_ROWS), DISPLAY_ROWS - 1);
        display[r][c] = '*';
    }

    // 3. Render Explicit Start and End Marks
    int start_c = std::min(static_cast<int>((start.x / MAP_WIDTH) * DISPLAY_COLS), DISPLAY_COLS - 1);
    int start_r = std::min(static_cast<int>((start.y / MAP_HEIGHT) * DISPLAY_ROWS), DISPLAY_ROWS - 1);
    display[start_r][start_c] = 'S';

    int goal_c = std::min(static_cast<int>((goal.x / MAP_WIDTH) * DISPLAY_COLS), DISPLAY_COLS - 1);
    int goal_r = std::min(static_cast<int>((goal.y / MAP_HEIGHT) * DISPLAY_ROWS), DISPLAY_ROWS - 1);
    display[goal_r][goal_c] = 'E';

    // 4. Print results to standard out (inverted row printing to represent Y-axis ascending vertically)
    std::cout << "\n================ RRT PATH VISUALIZATION ================\n\n";
    for (int r = DISPLAY_ROWS - 1; r >= 0; --r) {
        std::cout << "  ";
        for (int c = 0; c < DISPLAY_COLS; ++c) {
            std::cout << display[r][c] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n========================================================\n";
}

void planRRT(Node start, Node goal, const std::vector<Obstacle>& obstacles) {
    std::vector<Node> tree;
    start.parent_idx = -1;
    tree.push_back(start);

    bool reached_goal = false;
    int goal_node_idx = -1;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        Node sample = getRandomSample(goal.x, goal.y);
        int nearest_idx = getNearestNodeIdx(tree, sample);
        Node nearest_node = tree[nearest_idx];

        double angle = std::atan2(sample.y - nearest_node.y, sample.x - nearest_node.x);
        Node new_node;
        new_node.x = nearest_node.x + STEP_SIZE * std::cos(angle);
        new_node.y = nearest_node.y + STEP_SIZE * std::sin(angle);
        new_node.parent_idx = nearest_idx;

        if (new_node.x >= 0 && new_node.x <= MAP_WIDTH && new_node.y >= 0 && new_node.y <= MAP_HEIGHT) {
            if (!isSegmentColliding(nearest_node.x, nearest_node.y, new_node.x, new_node.y, obstacles)) {
                tree.push_back(new_node);
                int current_new_idx = tree.size() - 1;

                double dist_to_goal = std::hypot(new_node.x - goal.x, new_node.y - goal.y);
                if (dist_to_goal <= GOAL_RADIUS) {
                    reached_goal = true;
                    goal_node_idx = current_new_idx;
                    break;
                }
            }
        }
    }

    if (reached_goal) {
        std::cout << "Path successfully located!\n";
        std::vector<Node> path;
        int current_idx = goal_node_idx;

        while (current_idx != -1) {
            path.push_back(tree[current_idx]);
            current_idx = tree[current_idx].parent_idx;
        }
        std::reverse(path.begin(), path.end());

        // Generate output visual grid matrix layout
        drawVisualGrid(path, start, goal, obstacles);
    } else {
        std::cout << "RRT failed to locate a valid continuous path.\n";
    }
}

// main code harness:
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    Node start = {10.0, 10.0, -1};
    Node goal  = {90.0, 90.0, -1};

    // Continuous map circular blocking elements (X, Y, Radius)
    std::vector<Obstacle> obstacles = {
        {40.0, 45.0, 14.0},
        {60.0, 65.0, 14.0},
        {35.0, 75.0, 10.0}
    };

    planRRT(start, goal, obstacles);

    return 0;
}
