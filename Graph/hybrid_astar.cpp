#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iomanip>

// --- Configuration Parameters ---
const int MAP_W = 20; // Grid Width (X)
const int MAP_H = 20; // Grid Height (Y)
const int NUM_THETAS = 72; // Discrete heading bins (360 degrees / 5-degree increments)

const double GRID_RESOLUTION = 1.0; 
const double WHEELBASE = 1.5;       // Vehicle length metric (L)
const double MAX_STEER = 0.6;       // Maximum steering angle in radians (~35 degrees)
const double MOVE_STEP = 0.8;       // Forward driving distance per step

// Structure representing continuous real-world states
struct State {
    double x, y, theta;
    int direction; // 1 = Forward, -1 = Backward
};

// Node structure utilized inside the Priority Queue
struct Node {
    double f, g;
    State state;
    int parent_idx; // Index tracking inside our memory block
    int self_idx;
};

// Custom Comparator to build a Min-Heap Priority Queue
struct CompareNode {
    bool operator()(const Node* a, const Node* b) {
        return a->f > b->f;
    }
};

// Convert continuous poses into discrete 3D index bins
int stateTo3DIndex(double x, double y, double theta) {
    int xi = std::min(std::max(0, static_cast<int>(x / GRID_RESOLUTION)), MAP_W - 1);
    int yi = std::min(std::max(0, static_cast<int>(y / GRID_RESOLUTION)), MAP_H - 1);
    
    // Normalize heading between 0 and 2*PI
    while (theta < 0) theta += 2 * M_PI;
    while (theta >= 2 * M_PI) theta -= 2 * M_PI;
    
    int ti = static_cast<int>((theta / (2 * M_PI)) * NUM_THETAS) % NUM_THETAS;
    
    return xi + yi * MAP_W + ti * (MAP_W * MAP_H);
}

// Inline standard Euclidean distance heuristic
double getEuclideanHeuristic(double x1, double y1, double x2, double y2) {
    return std::hypot(x1 - x2, y1 - y2);
}

// Bicycle Kinematic Model Update Loop
State moveVehicle(const State& current, double steering_angle, double step_size, int direction) {
    State next;
    double d = step_size * direction;
    
    // If steering is nearly straight, move linear to avoid divide-by-zero
    if (std::abs(steering_angle) < 1e-4) {
        next.x = current.x + d * std::cos(current.theta);
        next.y = current.y + d * std::sin(current.theta);
        next.theta = current.theta;
    } else {
        // Apply Kinematic equations based on steering orientation
        double turn_radius = WHEELBASE / std::tan(steering_angle);
        double change_in_theta = d / turn_radius;
        
        next.theta = current.theta + change_in_theta;
        next.x = current.x + turn_radius * (std::sin(next.theta) - std::sin(current.theta));
        next.y = current.y - turn_radius * (std::cos(next.theta) - std::cos(current.theta));
    }
    
    next.direction = direction;
    return next;
}

// Check boundary criteria and cross-reference 2D wall data
bool isValid(double x, double y, const std::vector<std::vector<int>>& grid) {
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H) return false;
    int xi = static_cast<int>(x / GRID_RESOLUTION);
    int yi = static_cast<int>(y / GRID_RESOLUTION);
    return grid[yi][xi] == 0; // Walkable if 0
}

void hybridAStar(const std::vector<std::vector<int>>& grid, State start, State goal) {
    int total3DBins = MAP_W * MAP_H * NUM_THETAS;
    
    // Arrays for tracking costs, closures, and pointers
    std::vector<bool> closedList(total3DBins, false);
    std::vector<double> gCosts(total3DBins, 2e9);
    std::vector<Node> nodeRegistry(total3DBins);

    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

    // Set up launch node
    int startIdx = stateTo3DIndex(start.x, start.y, start.theta);
    gCosts[startIdx] = 0;
    
    nodeRegistry[startIdx] = {
        getEuclideanHeuristic(start.x, start.y, goal.x, goal.y),
        0.0, start, -1, startIdx
    };
    openList.push(&nodeRegistry[startIdx]);

    // Discrete action options for steering input: Left, Straight, Right
    std::vector<double> steeringInputs = {-MAX_STEER, 0.0, MAX_STEER};
    std::vector<int> directions = {1, -1}; // 1 = Forward, -1 = Reverse Gear

    bool success = false;
    int goalContainerIdx = -1;

    while (!openList.empty()) {
        Node* curr = openList.top();
        openList.pop();

        if (closedList[curr->self_idx]) continue;
        closedList[curr->self_idx] = true;

        // Proximity threshold to safely catch goal
        if (getEuclideanHeuristic(curr->state.x, curr->state.y, goal.x, goal.y) < 1.0) {
            success = true;
            goalContainerIdx = curr->self_idx;
            break;
        }

        // Branch out kinematic possibilities
        for (int dir : directions) {
            for (double steer : steeringInputs) {
                State nextState = moveVehicle(curr->state, steer, MOVE_STEP, dir);

                if (isValid(nextState.x, nextState.y, grid)) {
                    int nextIdx = stateTo3DIndex(nextState.x, nextState.y, nextState.theta);

                    if (closedList[nextIdx]) continue;

                    // Calculate customized movement costs (Penalize reverse driving to favor forward movement)
                    double edgeCost = MOVE_STEP;
                    if (dir == -1) edgeCost *= 2.5; // Penalize backing up
                    if (curr->state.direction != dir && curr->state.direction != 0) edgeCost += 1.0; // Penalize shifting gears

                    double newG = curr->g + edgeCost;

                    if (newG < gCosts[nextIdx]) {
                        gCosts[nextIdx] = newG;
                        double h = getEuclideanHeuristic(nextState.x, nextState.y, goal.x, goal.y);
                        
                        nodeRegistry[nextIdx] = {newG + h, newG, nextState, curr->self_idx, nextIdx};
                        openList.push(&nodeRegistry[nextIdx]);
                    }
                }
            }
        }
    }

    // --- Trace and Visually Render Path Output ---
    if (success) {
        std::cout << "Target State Reached Flawlessly!\n";
        std::vector<State> finalPath;
        int trace = goalContainerIdx;
        
        while (trace != -1) {
            finalPath.push_back(nodeRegistry[trace].state);
            trace = nodeRegistry[trace].parent_idx;
        }
        std::reverse(finalPath.begin(), finalPath.end());

        // Overlay calculated trajectory path onto display grid
        std::vector<std::vector<char>> display(MAP_H, std::vector<char>(MAP_W, '.'));
        for (int r = 0; r < MAP_H; ++r) {
            for (int c = 0; c < MAP_W; ++c) {
                if (grid[r][c] == 1) display[r][c] = '▒';
            }
        }

        for (const auto& s : finalPath) {
            int xi = static_cast<int>(s.x);
            int yi = static_cast<int>(s.y);
            if (xi >= 0 && xi < MAP_W && yi >= 0 && yi < MAP_H) {
                display[yi][xi] = '*';
            }
        }
        
        // Dynamic Overlays
        display[static_cast<int>(start.y)][static_cast<int>(start.x)] = 'S';
        display[static_cast<int>(goal.y)][static_cast<int>(goal.x)] = 'E';

        std::cout << "\n================ HYBRID A* PATH VISUALIZATION ================\n\n";
        for (int r = MAP_H - 1; r >= 0; --r) { // Print top-down orientation accurately
            std::cout << std::setw(2) << r << "  ";
            for (int c = 0; c < MAP_W; ++c) {
                std::cout << display[r][c] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "    ";
        for (int c = 0; c < MAP_W; ++c) std::cout << c % 10 << " ";
        std::cout << "\n\n==============================================================\n";
    } else {
        std::cout << "Kinematic constraints prevented locating an applicable pathway.\n";
    }
}


// main code harness:
int main(int argc, char** argv) {
    // 20x20 Local Grid Setup with an obstacle block wall dividing the room midway
    std::vector<std::vector<int>> grid(MAP_H, std::vector<int>(MAP_W, 0));
    for (int i = 4; i < 16; ++i) {
        grid[10][i] = 1; // Creates a horizontal wall in the middle
    }

    // Set positions using continuous coordinate variables (X, Y, Heading-Theta, Initial Gear Direction)
    State start = { 2.5,  2.5, 0.0, 0 };                // Bottom-Left, Facing Right
    State goal  = { 17.5, 17.5, M_PI / 2.0, 0 };        // Top-Right, Facing Straight Up

    hybridAStar(grid, start, goal);

    return 0;
}
