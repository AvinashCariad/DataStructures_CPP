#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

// Use a sufficiently large value representing infinity for unreachable paths
const double INF = 1e9;

class TSPSolver {
private:
    int num_cities;
    std::vector<std::vector<double>> distance_matrix;
    // memo[mask][pos] stores the minimum cost to visit the remaining unvisited cities
    // represented by the 'mask', starting from city 'pos'.
    std::vector<std::vector<double>> memo;
    // parent[mask][pos] stores the next city to visit, used to reconstruct the optimal tour.
    std::vector<std::vector<int>> parent;

    // Recursive DP function with bitmasking
    double solveTSP(int mask, int pos) {
        // Base Case: If all cities have been visited (mask contains all 1s)
        // Return the distance from the current city back to the starting city (0)
        if (mask == (1 << num_cities) - 1) {
            return distance_matrix[pos][0];
        }

        // Return the result immediately if it has already been computed
        if (memo[mask][pos] != -1.0) {
            return memo[mask][pos];
        }

        double min_cost = INF;
        int best_next_city = -1;

        // Try to visit all next unvisited cities
        for (int next_city = 0; next_city < num_cities; ++next_city) {
            // Check if the next_city has not been visited yet
            if ((mask & (1 << next_city)) == 0) {
                // Calculate the path cost if we travel to next_city
                double current_cost = distance_matrix[pos][next_city] + 
                                      solveTSP(mask | (1 << next_city), next_city);
                
                if (current_cost < min_cost) {
                    min_cost = current_cost;
                    best_next_city = next_city;
                }
            }
        }

        parent[mask][pos] = best_next_city;
        return memo[mask][pos] = min_cost;
    }

public:
    TSPSolver(const std::vector<std::vector<double>>& matrix) {
        distance_matrix = matrix;
        num_cities = matrix.size();
        memo.assign(1 << num_cities, std::vector<double>(num_cities, -1.0));
        parent.assign(1 << num_cities, std::vector<int>(num_cities, -1));
    }

    void computeAndPrintTour() {
        // Start at city 0, with only city 0 marked as visited (mask = 1)
        double total_distance = solveTSP(1, 0);

        std::cout << "Minimum Distance: " << std::fixed << std::setprecision(2) << total_distance << "\n";
        
        // Reconstruct and print the optimal path sequence
        std::cout << "Optimal Tour Path: ";
        int mask = 1;
        int curr_city = 0;
        
        std::cout << curr_city;
        while (true) {
            int next_city = parent[mask][curr_city];
            if (next_city == -1) break;
            std::cout << " -> " << next_city;
            mask |= (1 << next_city);
            curr_city = next_city;
        }
        std::cout << " -> 0\n"; // Return to start
    }
};



// main code harness:
int main(int argc, char** argv) {
    // Example 4x4 Distance Matrix (Symmetric TSP)
    // Rows and columns represent cities 0, 1, 2, and 3
    std::vector<std::vector<double>> graph = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    TSPSolver solver(graph);
    solver.computeAndPrintTour();

    return 0;
}
