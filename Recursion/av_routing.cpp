/*

Autonomous Vehicle Routing Coding Challenge

Ride-hailing companies like Uber and Lyft have become ubiquitous means to get around cities. Efficient vehicle routing,
or figuring out the fastest and most efficient way to direct vehicles to pick up and drop off multiple passengers at a
time, has become a crucial component of their success.

In the near future you are the proud owner of an autonomous vehicle! You want to let some friends use it whenever they
want so you make an app to let them call the vehicle to come pick them up. In this problem you will develop a system to
keep track of the state of your car and your friends.

Here are some more constraints to help you code your solution:
The city is a grid of perpendicular blocks. At initialization your code should take as inputs the number of streets in
both the x and y directions (ex. x=10, y=10).

The car can move in any direction exactly 1 block at a time. For our purposes, we only care about when the car is at a
vertex of two intersecting streets, so continuing our example, a car can be at the intersection (0, 0) in the upper-left
corner of our city, or (9, 9) in the bottom-right corner.

There should be a function to advance time by one time-step (and hence the vehicle by one block in any direction). This
function should accept a list of ride requests from your friends. These ride requests come in the form of a list of JSON
elements like so:

{ requests: [
    {
      'name': 'Elon',
      'start': (3,5),
      'end': (8,7)
    },
    {
      'name': 'George',
      'start': (1,2),
      'end': (4,3)
    }
  ]
}

At each time step this list of requests can be an empty list (ex. []), or infinitely long. At each time step you should
print out the current position of the vehicle and the names of all of the passengers currently in the vehicle, as well
as anybody being dropped off or being picked up at this intersection. (We will assume your car can fit infinitely many
people at a time.)

Your goal is to schedule your car to get people where they want to go as fast as possible. Of course, you will likely
have to make some trade-offs in the process. There is no right answer, just make sure you explain your decisions clearly
in your write-up.

Submission: Please return all of your code, as well as a write up of your solution, as well as any tests you have
written in a zip file with your name in the title.

Please code your solution in C++.

Let me know if you have any questions and happy coding!




*/


#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <memory>

// Structure to represent a 2D coordinate intersection
struct Location {
    int x;
    int y;

    bool operator==(const Location& other) const {
        return x == other.x && y == other.y;
    }
};

// Structure representing a friend's ride request
struct Request {
    std::string name;
    Location start;
    Location end;
    bool is_picked_up = false;
};

class AutonomousVehicleSystem {
private:
    int city_width;
    int city_height;
    Location car_pos;
    
    std::vector<Request> active_requests; // Tracks waiting and onboard passengers

public:
    // Initialize the city bounds and start the car at (0, 0)
    AutonomousVehicleSystem(int x_bound, int y_bound) 
        : city_width(x_bound), city_height(y_bound), car_pos{0, 0} {}

    // Main function to advance time by 1 unit and accept new requests
    void advanceTimeStep(const std::vector<Request>& new_requests) {
        // 1. Ingest new requests into the global queue
        for (const auto& req : new_requests) {
            active_requests.push_back(req);
        }

        // 2. Determine the next target location to move toward
        Location target = getNextTarget();

        // 3. Move the vehicle 1 block toward the target (if a target exists)
        if (!(target == car_pos)) {
            if (car_pos.x != target.x) {
                car_pos.x += (target.x > car_pos.x) ? 1 : -1;
            } else if (car_pos.y != target.y) {
                car_pos.y += (target.y > car_pos.y) ? 1 : -1;
            }
        }

        // 4. Process pickups and dropoffs at the NEW intersection
        std::vector<std::string> picked_up_now;
        std::vector<std::string> dropped_off_now;
        std::vector<std::string> currently_onboard;

        auto it = active_requests.begin();
        while (it != active_requests.end()) {
            // Dropoff check
            if (it->is_picked_up && it->end == car_pos) {
                dropped_off_now.push_back(it->name);
                it = active_requests.erase(it); // Remove from tracking entirely
                continue;
            }
            // Pickup check
            if (!it->is_picked_up && it->start == car_pos) {
                it->is_picked_up = true;
                picked_up_now.push_back(it->name);
            }
            // If they are onboard after processing pickups
            if (it->is_picked_up) {
                currently_onboard.push_back(it->name);
            }
            ++it;
        }

        // 5. Print State Output
        printState(currently_onboard, picked_up_now, dropped_off_now);
    }

private:
    // Helper to calculate Manhattan distance on a grid
    int manhattanDistance(Location a, Location b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    // Routing Logic: Returns the optimal next coordinate target
    Location getNextTarget() {
        if (active_requests.empty()) {
            return car_pos; // Stay put if no one needs a ride
        }

        Location best_target = car_pos;
        int min_distance = 1e9;
        bool dropoff_available = false;

        // Prioritize dropping people off who are already inside the car
        for (const auto& req : active_requests) {
            if (req.is_picked_up) {
                int dist = manhattanDistance(car_pos, req.end);
                if (!dropoff_available || dist < min_distance) {
                    min_distance = dist;
                    best_target = req.end;
                    dropoff_available = true; // Hard lock focus onto dropping passengers off
                }
            }
        }

        // If no one is onboard, find the closest person waiting to be picked up
        if (!dropoff_available) {
            for (const auto& req : active_requests) {
                int dist = manhattanDistance(car_pos, req.start);
                if (dist < min_distance) {
                    min_distance = dist;
                    best_target = req.start;
                }
            }
        }

        return best_target;
    }

    // Formatting string lists for output
    void printState(const std::vector<std::string>& onboard, 
                    const std::vector<std::string>& picked, 
                    const std::vector<std::string>& dropped) {
        std::cout << "Vehicle Position: (" << car_pos.x << ", " << car_pos.y << ")\n";
        
        std::cout << "  Onboard: [ ";
        for (const auto& name : onboard) std::cout << name << " ";
        std::cout << "]\n";

        std::cout << "  Picked Up: [ ";
        for (const auto& name : picked) std::cout << name << " ";
        std::cout << "]\n";

        std::cout << "  Dropped Off: [ ";
        for (const auto& name : dropped) std::cout << name << " ";
        std::cout << "]\n";
        std::cout << "-------------------------------------------\n";
    }
};

// Unit test harness simulation
int main(int argc, char** argv) {
    // Initialize a 10x10 Grid city
    AutonomousVehicleSystem av(10, 10);

    std::cout << "--- Starting AV Routing Simulation ---\n\n";

    // Time-step 1: Elon and George make requests
    std::cout << "Time Step 1: Receiving requests from Elon and George\n";
    std::vector<Request> t1 = {
        {"Elon", {3, 5}, {8, 7}},
        {"George", {1, 2}, {4, 3}}
    };
    av.advanceTimeStep(t1);

    // Time-step 2-5: Advance empty time steps while car drives toward George (closest)
    for (int i = 2; i <= 5; ++i) {
        std::cout << "Time Step " << i << ":\n";
        av.advanceTimeStep({});
    }

    // Time-step 6: Another request comes in mid-transit
    std::cout << "Time Step 6: Receiving request from Shivon\n";
    std::vector<Request> t6 = {
        {"Shivon", {2, 2}, {2, 5}}
    };
    av.advanceTimeStep(t6);

    // Run until empty to watch the system clear out passenger goals
    for (int i = 7; i <= 20; ++i) {
        std::cout << "Time Step " << i << ":\n";
        av.advanceTimeStep({});
    }

    return 0;
}


