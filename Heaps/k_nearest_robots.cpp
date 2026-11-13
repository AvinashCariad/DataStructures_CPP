#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// Define Point:
struct Point {
    double x, y;
};


// Define Robot:
struct Robot {
    int id;
    Point location;
};

// Helper function to compute squared Euclidean distance.
// We avoid std::sqrt() during comparisons for better performance and precision.
double getSquaredDistance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;      // note the square of the dist 
}

std::vector<Robot> findKNearestRobots(const std::vector<Robot>& robots, const Point& query, int k) {
    if (k <= 0 || robots.empty()) {
        return {};
    }

    // Store pairs of {squared_distance, robot_index}.
    // By default, std::priority_queue is a Max-Heap, keeping the farthest
    // of the K closest robots at the top for easy removal.
    using DistIndexPair = std::pair<double, int>;
    std::priority_queue<DistIndexPair> maxHeap;                                           // think why we are using max-heap here insted of min-heap

    for (int i = 0; i < static_cast<int>(robots.size()); ++i) {
        double distSq = getSquaredDistance(robots[i].location, query);

        if (static_cast<int>(maxHeap.size()) < k) {
            maxHeap.push({distSq, i});
        } else if (distSq < maxHeap.top().first) {
            maxHeap.pop();
            maxHeap.push({distSq, i});
        }
    }

    // Extract the K nearest robots from the heap
    std::vector<Robot> nearestRobots;
    // In C++, std::vector::reserve() preallocates memory
    // for a specified number of elements without actually
    // creating those elements or changing the vector's size.
    nearestRobots.reserve(maxHeap.size());                                                // maxHeap.size() -> k, so reserves k slots in vector<Robot> 

    // Iterate while the loop is not empty:
    while (!maxHeap.empty()) {
        nearestRobots.push_back(robots[maxHeap.top().second]);
        maxHeap.pop();
    }

    // Optional: Reverse so the output is ordered from closest to farthest
    std::reverse(nearestRobots.begin(), nearestRobots.end());

    return nearestRobots;
}

// main code harness:
int main(int argc, char** argv) {
    std::vector<Robot> robots = {
        {101, {2.0, 3.0}},
        {102, {5.0, 4.0}},
        {103, {9.0, 6.0}},
        {104, {4.0, 7.0}},
        {105, {1.0, 1.0}}
    };

    Point queryPoint = {3.0, 3.0};
    int k = 3;

    std::vector<Robot> closest = findKNearestRobots(robots, queryPoint, k);               // find the K nearest neighbors 

    std::cout << "The " << k << " nearest robots to (" << queryPoint.x << ", " << queryPoint.y << ") are:\n";
    for (const auto& robot : closest) {
        std::cout << "Robot ID: " << robot.id 
                  << " at (" << robot.location.x << ", " << robot.location.y << ")\n";
    }

    return 0;
}


/*

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// Robot information
struct Robot {
    int id;
    int x, y;
};

// Structure used in the max-heap
struct RobotDistance {
    long long dist;
    int id;
    int x, y;

    // Max-heap: farthest robot stays on top
    bool operator<(const RobotDistance& r) const {
        return dist < r.dist;
    }
};

// Calculate squared distance from origin (0, 0)
long long getDistance(const Robot& robot) {
    return 1LL * robot.x * robot.x
         + 1LL * robot.y * robot.y;
}

// Find K nearest robots
vector<Robot> findKNearestRobots(const vector<Robot>& robots, int k) {

    if (k <= 0) {
        return {};
    }

    // Max-heap
    priority_queue<RobotDistance> max_heap;

    for (const auto& robot : robots) {

        max_heap.emplace(RobotDistance{
            getDistance(robot),
            robot.id,
            robot.x,
            robot.y
        });

        // Remove farthest robot if heap size > K
        if (max_heap.size() > static_cast<size_t>(k)) {
            max_heap.pop();
        }
    }

    // Extract K nearest robots
    vector<Robot> nearestRobots;
    nearestRobots.reserve(max_heap.size());

    while (!max_heap.empty()) {

        auto robot = max_heap.top();

        nearestRobots.emplace_back(
            Robot{robot.id, robot.x, robot.y}
        );

        max_heap.pop();
    }

    // Reverse to get nearest to farthest
    reverse(nearestRobots.begin(), nearestRobots.end());

    return nearestRobots;
}


// main code harness:
int main(int argc, char** argv) {

    vector<Robot> robots = {
        {1, -1, 0},
        {2, 5, 5},
        {3, 2, 3},
        {4, 9, 9},
        {5, 1, 0}
    };

    int k = 2;

    vector<Robot> nearest =
        findKNearestRobots(robots, k);

    for (const auto& robot : nearest) {
        cout << "Robot ID: " << robot.id
             << " Position: (" << robot.x
             << ", " << robot.y << ")"
             << " Distance squared: "
             << getDistance(robot)
             << endl;
    }

    return 0;
}


// same code as above except the distance is from some refrence point as query: 
//========================================================================================
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Robot {
    int id;
    int x, y;
};

struct Coordinate {
    int x, y;
};

// Structure stored in max-heap
struct RobotDistance {
    long long dist;
    Robot robot;

    // Farthest robot at the top
    bool operator<(const RobotDistance& other) const {
        return dist < other.dist;
    }
};

// Distance from any reference point
long long getDistance(
    const Robot& robot,
    const Coordinate& reference) {

    long long dx = 1LL * robot.x - reference.x;
    long long dy = 1LL * robot.y - reference.y;

    return dx * dx + dy * dy;
}

// Find K nearest robots to reference point
vector<Robot> findKNearestRobots(
    const vector<Robot>& robots,
    const Coordinate& reference,
    int k) {

    if (k <= 0) {
        return {};
    }

    priority_queue<RobotDistance> max_heap;

    for (const auto& robot : robots) {

        max_heap.push({
            getDistance(robot, reference),
            robot
        });

        // Remove the farthest robot
        if (max_heap.size() > static_cast<size_t>(k)) {
            max_heap.pop();
        }
    }

    vector<Robot> nearestRobots;
    nearestRobots.reserve(max_heap.size());

    // Extract K nearest robots
    while (!max_heap.empty()) {
        nearestRobots.push_back(max_heap.top().robot);
        max_heap.pop();
    }

    // Nearest first
    reverse(nearestRobots.begin(), nearestRobots.end());

    return nearestRobots;
}



// main code harness:
int main(int argc, char** argv) {

    vector<Robot> robots = {
        {1, -1, 0},
        {2, 5, 5},
        {3, 2, 3},
        {4, 9, 9},
        {5, 1, 0}
    };

    Coordinate reference = {3, 2};

    int k = 2;

    vector<Robot> nearest =
        findKNearestRobots(robots, reference, k);

    cout << "Reference point: ("
         << reference.x << ", "
         << reference.y << ")\n";

    for (const auto& robot : nearest) {

        cout << "Robot ID: " << robot.id
             << " Position: (" << robot.x
             << ", " << robot.y << ")"
             << " Distance squared: "
             << getDistance(robot, reference)
             << endl;
    }

    return 0;
}













*/