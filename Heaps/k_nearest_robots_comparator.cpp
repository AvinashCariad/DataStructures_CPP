#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Point {
    double x, y;
};

struct Robot {
    int id;
    Point location;
};

// Custom comparator struct for the Max-Heap
struct CompareDistance {
    Point query;

    // Constructor captures the query point
    CompareDistance(const Point& q) : query(q) {}

    // Returns true if 'a' is closer to query than 'b'.
    // In std::priority_queue, returning true means 'a' has LOWER priority than 'b',
    // placing the FARTHEST robot at the top (Max-Heap).
    bool operator()(const Robot& a, const Robot& b) const {
        double distSqA = (a.location.x - query.x) * (a.location.x - query.x) +
                         (a.location.y - query.y) * (a.location.y - query.y);
        double distSqB = (b.location.x - query.x) * (b.location.x - query.x) +
                         (b.location.y - query.y) * (b.location.y - query.y);
        
        return distSqA < distSqB;          // arrange in ascending order
    }
};


// find the K nearest neighbor:
std::vector<Robot> findKNearestRobots(const std::vector<Robot>& robots, const Point& query, int k) {
    if (k <= 0 || robots.empty()) {
        return {};
    }

    // Initialize priority_queue with:
    // 1. Element type: Robot
    // 2. Container type: std::vector<Robot>
    // 3. Comparator type: CompareDistance
    CompareDistance comp(query);
    std::priority_queue<Robot, std::vector<Robot>, CompareDistance> maxHeap(comp);          // note how the priority queue is being written

    for (const auto& robot : robots) {
        if (static_cast<int>(maxHeap.size()) < k) {
            maxHeap.push(robot);
        } else if (comp(robot, maxHeap.top())) { 
            // comp(robot, maxHeap.top()) is true if 'robot' is closer than 'maxHeap.top()'
            maxHeap.pop();
            maxHeap.push(robot);
        }
    }

    // Extract the K nearest robots
    std::vector<Robot> nearestRobots;
    nearestRobots.reserve(maxHeap.size());

    while (!maxHeap.empty()) {
        nearestRobots.push_back(maxHeap.top());
        maxHeap.pop();
    }

    // Reverse to order from closest to farthest
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

    std::vector<Robot> closest = findKNearestRobots(robots, queryPoint, k);

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

struct Point {
    double x, y;
};

struct Robot {
    int id;
    Point location;
};

std::vector<Robot> findKNearestRobots(const std::vector<Robot>& robots, const Point& query, int k) {
    if (k <= 0 || robots.empty()) {
        return {};
    }

    auto getDistSq = [&query](const Robot& r) {
        double dx = r.location.x - query.x;
        double dy = r.location.y - query.y;
        return dx * dx + dy * dy;
    };

    // Pass the lambda directly as the comparator
    auto maxHeap = std::priority_queue(
        [&](const Robot& a, const Robot& b) {
            return getDistSq(a) < getDistSq(b);
        },
        std::vector<Robot>()
    );

    for (const auto& robot : robots) {
        if (static_cast<int>(maxHeap.size()) < k) {
            maxHeap.push(robot);
        } else if (getDistSq(robot) < getDistSq(maxHeap.top())) {
            maxHeap.pop();
            maxHeap.push(robot);
        }
    }

    std::vector<Robot> nearestRobots;
    nearestRobots.reserve(maxHeap.size());

    while (!maxHeap.empty()) {
        nearestRobots.push_back(maxHeap.top());
        maxHeap.pop();
    }

    std::reverse(nearestRobots.begin(), nearestRobots.end());

    return nearestRobots;
}

int main() {
    std::vector<Robot> robots = {
        {101, {2.0, 3.0}},
        {102, {5.0, 4.0}},
        {103, {9.0, 6.0}},
        {104, {4.0, 7.0}},
        {105, {1.0, 1.0}}
    };

    Point queryPoint = {3.0, 3.0};
    int k = 3;

    std::vector<Robot> closest = findKNearestRobots(robots, queryPoint, k);

    std::cout << "The " << k << " nearest robots to (" << queryPoint.x << ", " << queryPoint.y << ") are:\n";
    for (const auto& robot : closest) {
        std::cout << "Robot ID: " << robot.id 
                  << " at (" << robot.location.x << ", " << robot.location.y << ")\n";
    }

    return 0;
}





















*/