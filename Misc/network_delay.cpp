#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> computeDistance(
        const vector<vector<pair<int, int>>>& graph,
        int source)
    {
        const int INF = numeric_limits<int>::max();

        vector<int> dist(graph.size(), INF);

        // Min-heap storing: {distance, vertex}
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int currentDistance = pq.top().first;
            int currentVertex = pq.top().second;
            pq.pop();

            // Ignore outdated entries
            if (currentDistance > dist[currentVertex]) {
                continue;
            }

            for (const auto& edge : graph[currentVertex]) {
                int neighbor = edge.first;
                int weight = edge.second;

                int newDistance = currentDistance + weight;

                if (newDistance < dist[neighbor]) {
                    dist[neighbor] = newDistance;
                    pq.push({newDistance, neighbor});
                }
            }
        }

        return dist;
    }

    int networkDelayTime(
        const vector<vector<int>>& times,
        int N,
        int K)
    {
        vector<vector<pair<int, int>>> graph(N);

        // Build directed graph
        for (const auto& edge : times) {
            int from = edge[0] - 1;
            int to = edge[1] - 1;
            int weight = edge[2];

            graph[from].push_back({to, weight});
        }

        vector<int> distance = computeDistance(graph, K - 1);

        int answer = 0;

        for (int d : distance) {
            if (d == numeric_limits<int>::max()) {
                return -1;
            }

            answer = max(answer, d);
        }

        return answer;
    }
};



// main code harness:
int main(int argc, char** argv) {
    /*
        Example graph:

        2 ----1----> 1
        |
        1
        |
        v
        3 ----1----> 4

        Input edges:
        2 -> 1, weight 1
        2 -> 3, weight 1
        3 -> 4, weight 1

        Starting node: 2

        Shortest distances:
        node 1 = 1
        node 2 = 0
        node 3 = 1
        node 4 = 2

        Network delay = 2
    */

    vector<vector<int>> times = {
        {2, 1, 1},
        {2, 3, 1},
        {3, 4, 1}
    };

    int N = 4;
    int K = 2;

    Solution solution;

    int result = solution.networkDelayTime(times, N, K);

    cout << "Network delay time: " << result << endl;

    return 0;
}
