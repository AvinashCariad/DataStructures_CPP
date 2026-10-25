#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Environment boundaries
const double MAP_WIDTH = 100.0;
const double MAP_HEIGHT = 100.0;

// RRT Parameters
const double STEP_SIZE = 5.0;       // Length of each growth step
const double GOAL_RADIUS = 3.0;     // Proximity to goal to declare success
const int MAX_ITERATIONS = 5000;    // Maximum tree nodes to generate
const double GOAL_SAMPLE_RATE = 0.1; // 10% bias to sample directly towards the goal

struct Node {
    double x, y;
    int parent_idx;
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
    const int checks = 10; // Number of interpolation samples along the line segment
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
    // Introduce goal bias so the tree explores aggressively towards the destination
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

// Main RRT Planner
void planRRT(Node start, Node goal, const std::vector<Obstacle>& obstacles) {
    std::vector<Node> tree;
    start.parent_idx = -1;
    tree.push_back(start);

    bool reached_goal = false;
    int goal_node_idx = -1;
    
    // Iterate:
    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        // 1. Sample a random point
        Node sample = getRandomSample(goal.x, goal.y);

        // 2. Find nearest node in the tree
        int nearest_idx = getNearestNodeIdx(tree, sample);
        Node nearest_node = tree[nearest_idx];

        // 3. Extend towards sample point by STEP_SIZE
        double angle = std::atan2(sample.y - nearest_node.y, sample.x - nearest_node.x);
        Node new_node;
        new_node.x = nearest_node.x + STEP_SIZE * std::cos(angle);
        new_node.y = nearest_node.y + STEP_SIZE * std::sin(angle);
        new_node.parent_idx = nearest_idx;

        // 4. Collision check
        if (new_node.x >= 0 && new_node.x <= MAP_WIDTH && new_node.y >= 0 && new_node.y <= MAP_HEIGHT) {
            if (!isSegmentColliding(nearest_node.x, nearest_node.y, new_node.x, new_node.y, obstacles)) {
                tree.push_back(new_node);
                int current_new_idx = tree.size() - 1;

                // 5. Check if we are close enough to the goal
                double dist_to_goal = std::hypot(new_node.x - goal.x, new_node.y - goal.y);
                if (dist_to_goal <= GOAL_RADIUS) {
                    reached_goal = true;
                    goal_node_idx = current_new_idx;
                    break;
                }
            }
        }
    }

    // 6. Trace and display the path if found
    if (reached_goal) {
        std::cout << "Path successfully located!\n";
        std::vector<Node> path;
        int current_idx = goal_node_idx;

        while (current_idx != -1) {
            path.push_back(tree[current_idx]);
            current_idx = tree[current_idx].parent_idx;
        }
        std::reverse(path.begin(), path.end());

        std::cout << "\nPath Coordinates (Start -> Goal):\n";
        for (const auto& p : path) {
            std::cout << "(" << p.x << ", " << p.y << ")\n";
        }
        // Explicit final goal snap
        std::cout << "(" << goal.x << ", " << goal.y << ")\n";
    } else {
        std::cout << "RRT failed to find a valid path within the iteration limit.\n";
    }
}


// main code harness:
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    Node start = {10.0, 10.0, -1};
    Node goal  = {90.0, 90.0, -1};

    // Set up a few circular obstacles blocking the middle of the field
    std::vector<Obstacle> obstacles = {
        {40.0, 40.0, 12.0},
        {50.0, 60.0, 10.0},
        {30.0, 70.0, 8.0}
    };

    planRRT(start, goal, obstacles);

    return 0;
}
