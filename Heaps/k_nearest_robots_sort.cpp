#include <iostream>
#include <vector>
#include <algorithm> // Required for std::sort

// Define Point:
struct Point {
    double x, y;
};

// Define Robot:
struct Robot {
    int id;
    Point location;
};


// function signature:
std::vector<Robot> findKNearestRobots(std::vector<Robot> robots, const Point& query, int k) {
    if (k <= 0 || robots.empty()) {
        return {};
    }
     
    // Sort all robots from closest to farthest using an inline lambda
    // The square brackets [] in a C++ lambda are called the capture clause. 
    // They specify which variables from the surrounding scope the lambda can
    // access and whether it accesses them by copy or by reference.
    std::sort(robots.begin(), robots.end(), [&query](const Robot& a, const Robot& b) {       // lambda can access query from the surounding  
        double distSqA = (a.location.x - query.x) * (a.location.x - query.x) +
                         (a.location.y - query.y) * (a.location.y - query.y);
        double distSqB = (b.location.x - query.x) * (b.location.x - query.x) +
                         (b.location.y - query.y) * (b.location.y - query.y);
        return distSqA < distSqB;
    });

    // Keep only the first K elements (safely handling cases where k > robots.size())
    if (k < static_cast<int>(robots.size())) {
        robots.resize(k);
    }

    return robots;
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
