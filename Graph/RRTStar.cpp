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

// RRT* Parameters
const double STEP_SIZE = 6.0;         // Length of each growth step
const double GOAL_RADIUS = 4.0;       // Proximity to goal to declare success
const int MAX_ITERATIONS = 3000;      // Maximum iterations to optimize the tree
const double GOAL_SAMPLE_RATE = 0.15;  // 15% bias to sample directly towards the goal
const double REWIRE_RADIUS = 15.0;    // Search radius to find near neighbors for optimization

struct Node {
    double x, y;
    int parent_idx;
    double cost; // Total cost accumulated from the start node to this node
};

struct Obstacle {
    double x, y, radius;
};

// Check if a point falls inside any circular obstacle
bool isPointColliding(double x, double y, const std::vector<Obstacle>& obstacles) {
    for (const auto& obs : obstacles) {
        double dist = std::hypot(x - obs.x, y - obs.y);
        if (dist <= obs.radius) return true;
    }
    return false;
}

// Check if a line segment between two points intersects an obstacle
bool isSegmentColliding(double x1, double y1, double x2, double y2, const std::vector<Obstacle>& obstacles) {
    const int checks = 12; // Number of interpolation samples along the line segment
    for (int i = 0; i <= checks; ++i) {
        double t = static_cast<double>(i) / checks;
        double cx = x1 + t * (x2 - x1);
        double cy = y1 + t * (y2 - y1);
        if (isPointColliding(cx, cy, obstacles)) return true;
    }
    return false;
}

// Generate a random sample point with goal biasing
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
    sample.cost = 0.0;
    return sample;
}

// Find the index of the node closest to the sample point
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
            double map_x = (static_cast<double>(c) / DISPLAY_COLS) * MAP_WIDTH;
            double map_y = (static_cast<double>(r) / DISPLAY_ROWS) * MAP_HEIGHT;
            if (isPointColliding(map_x, map_y, obstacles)) {
                display[r][c] = '#'; 
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

    std::cout << "\n================ RRT* PATH VISUALIZATION ================\n\n";
    for (int r = DISPLAY_ROWS - 1; r >= 0; --r) {
        std::cout << "  ";
        for (int c = 0; c < DISPLAY_COLS; ++c) {
            std::cout << display[r][c] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n=========================================================\n";
}

void planRRTStar(Node start, Node goal, const std::vector<Obstacle>& obstacles) {
    std::vector<Node> tree;
    start.parent_idx = -1;
    start.cost = 0.0;
    tree.push_back(start);

    int best_goal_node_idx = -1;
    double min_goal_cost = 2e9;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        // 1. Sample a random point
        Node sample = getRandomSample(goal.x, goal.y);

        // 2. Find nearest node in the tree
        int nearest_idx = getNearestNodeIdx(tree, sample);
        Node nearest_node = tree[nearest_idx];

        // 3. Steer towards sample point by STEP_SIZE
        double angle = std::atan2(sample.y - nearest_node.y, sample.x - nearest_node.x);
        Node new_node;
        new_node.x = nearest_node.x + STEP_SIZE * std::cos(angle);
        new_node.y = nearest_node.y + STEP_SIZE * std::sin(angle);
        
        // Skip if outside map boundaries or instantly colliding
        if (new_node.x < 0 || new_node.x > MAP_WIDTH || new_node.y < 0 || new_node.y > MAP_HEIGHT) continue;
        if (isSegmentColliding(nearest_node.x, nearest_node.y, new_node.x, new_node.y, obstacles)) continue;

        // --- RRT* Optimization Step 1: Choose Best Parent ---
        int best_parent_idx = nearest_idx;
        double min_cost = nearest_node.cost + std::hypot(new_node.x - nearest_node.x, new_node.y - nearest_node.y);
        std::vector<int> near_indices;

        // Gather all existing nodes within REWIRE_RADIUS
        for (size_t i = 0; i < tree.size(); ++i) {
            double d = std::hypot(tree[i].x - new_node.x, tree[i].y - new_node.y);
            if (d <= REWIRE_RADIUS) {
                near_indices.push_back(i);
                
                // Evaluate if connecting to this near node results in a lower total cost
                double potential_cost = tree[i].cost + d;
                if (potential_cost < min_cost) {
                    if (!isSegmentColliding(tree[i].x, tree[i].y, new_node.x, new_node.y, obstacles)) {
                        min_cost = potential_cost;
                        best_parent_idx = i;
                    }
                }
            }
        }

        // Commit the node with its optimized parent and cost
        new_node.parent_idx = best_parent_idx;
        new_node.cost = min_cost;
        tree.push_back(new_node);
        int new_node_idx = tree.size() - 1;

        // --- RRT* Optimization Step 2: Rewire Nearby Nodes ---
        for (int near_idx : near_indices) {
            double d = std::hypot(new_node.x - tree[near_idx].x, new_node.y - tree[near_idx].y);
            double potential_rewire_cost = new_node.cost + d;

            // If pathing THROUGH our new node is cheaper for the neighbor, rewire its parent connection
            if (potential_rewire_cost < tree[near_idx].cost) {
                if (!isSegmentColliding(new_node.x, new_node.y, tree[near_idx].x, tree[near_idx].y, obstacles)) {
                    tree[near_idx].parent_idx = new_node_idx;
                    tree[near_idx].cost = potential_rewire_cost;
                }
            }
        }

        // Keep track of the absolute best path layout touching the goal radius
        double dist_to_goal = std::hypot(new_node.x - goal.x, new_node.y - goal.y);
        if (dist_to_goal <= GOAL_RADIUS) {
            if (new_node.cost < min_goal_cost) {
                min_goal_cost = new_node.cost;
                best_goal_node_idx = new_node_idx;
            }
        }
    }

    // Trace path
    if (best_goal_node_idx != -1) {
        std::cout << "Optimized Path located! Total Cost: " << min_goal_cost << "\n";
        std::vector<Node> path;
        int current_idx = best_goal_node_idx;

        while (current_idx != -1) {
            path.push_back(tree[current_idx]);
            current_idx = tree[current_idx].parent_idx;
        }
        std::reverse(path.begin(), path.end());

        drawVisualGrid(path, start, goal, obstacles);
    } else {
        std::cout << "RRT* failed to locate a valid path.\n";
    }
}


// main code harness:
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    Node start = {10.0, 10.0, -1, 0.0};
    Node goal  = {90.0, 90.0, -1, 0.0};

    // Circular obstacle definitions
    std::vector<Obstacle> obstacles = {
        {40.0, 45.0, 14.0},
        {60.0, 65.0, 14.0},
        {35.0, 75.0, 10.0}
    };

    planRRTStar(start, goal, obstacles);

    return 0;
}

