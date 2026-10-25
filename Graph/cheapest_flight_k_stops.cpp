#include <iostream>
#include <vector>
#include <queue>
#include <climits>

/*









*/

class Solution {
public:
    int findCheapestPrice(int n, std::vector<std::vector<int>>& flights, int src, int dst, int k) {
        // 1. Build adjacency list: u -> {v, price}
        std::vector<std::vector<std::pair<int, int>>> adj(n);
        for (const auto& flight : flights) {
            adj[flight[0]].push_back({flight[1], flight[2]});
        }

        // 2. Track minimum cost to reach each node
        std::vector<int> minCost(n, INT_MAX);
        minCost[src] = 0;

        // 3. BFS Queue stores: {stops, {current_node, current_cost}}
        std::queue<std::pair<int, std::pair<int, int>>> q;
        q.push({0, {src, 0}});

        while (!q.empty()) {
            auto [stops, nodeInfo] = q.front();
            int u = nodeInfo.first;
            int cost = nodeInfo.second;
            q.pop();

            // If we have exceeded K stops, we cannot move further from this node
            if (stops > k) continue;

            // Traverse all neighboring flights
            for (const auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int price = neighbor.second;

                // Relaxation condition: if a cheaper price is found, update and push to queue
                if (cost + price < minCost[v]) {
                    minCost[v] = cost + price;
                    q.push({stops + 1, {v, minCost[v]}});
                }
            }
        }

        // Return the min cost to destination, or -1 if unreachable within K stops
        return minCost[dst] == INT_MAX ? -1 : minCost[dst];
    }
};


// main code harness:
int main(int argc, char** argv) {
    Solution solver;
    int n = 4;
    std::vector<std::vector<int>> flights = {
        {0, 1, 100}, {1, 2, 100}, {2, 0, 100}, {1, 3, 600}, {2, 3, 200}
    };
    int src = 0, dst = 3, k = 1;

    int result = solver.findCheapestPrice(n, flights, src, dst, k);
    std::cout << "Cheapest flight price: " << result << std::endl; // Output: 700

    return 0;
}
