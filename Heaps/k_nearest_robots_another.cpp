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

    // Comparator written directly inside std::priority_queue
    std::priority_queue maxHeap(
    [&query](const Robot& a, const Robot& b) {                                         // query is redundant, it just tells the lambda to acquire from near scope
        double distSqA = (a.location.x - query.x) * (a.location.x - query.x) +
                         (a.location.y - query.y) * (a.location.y - query.y);
        double distSqB = (b.location.x - query.x) * (b.location.x - query.x) +
                         (b.location.y - query.y) * (b.location.y - query.y);
        return distSqA < distSqB;
    },
    std::vector<Robot>());                                                             // compare type and then comparator

    // Iterate:
    for (const auto& robot : robots) {
        maxHeap.push(robot);
        if (static_cast<int>(maxHeap.size()) > k) {
            maxHeap.pop();
        }
    }

    std::vector<Robot> nearestRobots;
    nearestRobots.reserve(maxHeap.size());                                             // note the use of reserve

    // condition:
    while (!maxHeap.empty()) {
        nearestRobots.push_back(maxHeap.top());
        maxHeap.pop();
    }

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
#include <functional> // Required for std::function

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

    // Priority queue with explicit < > types and inline lambda comparator
    std::priority_queue<
        Robot,
        std::vector<Robot>,
        std::function<bool(const Robot&, const Robot&)>
    > maxHeap(
        [&query](const Robot& a, const Robot& b) {
            double distSqA = (a.location.x - query.x) * (a.location.x - query.x) +
                             (a.location.y - query.y) * (a.location.y - query.y);
            double distSqB = (b.location.x - query.x) * (b.location.x - query.x) +
                             (b.location.y - query.y) * (b.location.y - query.y);
            return distSqA < distSqB;
        }
    );

    // Maintain a Max-Heap of at most K nearest robots
    for (const auto& robot : robots) {
        maxHeap.push(robot);
        if (static_cast<int>(maxHeap.size()) > k) {
            maxHeap.pop();
        }
    }

    // Extract the K nearest robots from the heap
    std::vector<Robot> nearestRobots;
    nearestRobots.reserve(maxHeap.size());

    while (!maxHeap.empty()) {
        nearestRobots.push_back(maxHeap.top());
        maxHeap.pop();
    }

    // Reverse so the result is ordered from closest to farthest
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








*/






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
    
    // lambda functions:
    auto getDistSq = [&query](const Robot& r) {
        // get sq distance:
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


*/

/*

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional> // Required for std::bind

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

    // 1. Lambda with 'query' inside () and empty capture []
    auto cmp = [](const Robot& a, const Robot& b, const Point& q) {
        double distSqA = (a.location.x - q.x) * (a.location.x - q.x) +
                         (a.location.y - q.y) * (a.location.y - q.y);
        double distSqB = (b.location.x - q.x) * (b.location.x - q.x) +
                         (b.location.y - q.y) * (b.location.y - q.y);
        return distSqA < distSqB;
    };

    // 2. Bind 'query' as the 3rd argument so the queue can call it with just (a, b)
    using namespace std::placeholders;
    auto boundCmp = std::bind(cmp, _1, _2, query);

    std::priority_queue<Robot, std::vector<Robot>, decltype(boundCmp)> maxHeap(boundCmp);

    for (const auto& robot : robots) {
        maxHeap.push(robot);
        if (static_cast<int>(maxHeap.size()) > k) {
            maxHeap.pop();
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














*/
