#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <climits>
#include <set>


using namespace std;

/*
Given a weighted, undirected and connected graph where you have given adjacency list adj. You have to find the shortest distance of all the vertices from the source vertex src, and return a list of integers denoting the shortest distance between each node and source vertex src.
Note: The Graph doesn't contain any negative weight edge.

Input: adjList = [[[1, 9]], [[0, 9]]], src = 0
Output: [0, 9]
Explanation: The source vertex is 0. Hence, the shortest distance of node 0 is 0 and the shortest distance from node 0 to 1 is 9

Input: adjList = [[[1, 1], [2, 6]], [[2, 3], [0, 1]], [[1, 3], [0, 6]]], src = 2
Output: [4, 3, 0]
Explanation: For nodes 2 to 0, we can follow the path 2-1-0. This has a distance of 1+3 = 4, whereas the path 2-0 has a distance of 6. So, the Shortest path from 2 to 0 is 4.
The shortest distance from 0 to 1 is 1

Comparison Between Dijkstra's Original PQ Implementation and Modern PQ Implementation :-
With the visited array, we ensure that a node is processed exactly once, but it increases memory usage. Without the visited array, it's possible that the same node could be stored in the priority queue, but if the same node with a smaller distance already exists, it will be processed first. So, both implementations don't affect the result finding. However, considering sparse graphs, if the graph size is large (at max possible in sparse), and you are strictly dealing with memory, then avoid the visited array implementation. Else, when time is strictly a concern, don't use the visited array for better performance.
Note: However, as the overall time and space of both implementations are the same, so generally both are effective. You could use either, but still, if you want to use with too much detailed considerations, then use based on what I said.
Note: Always remember that total number of edges in Dense graphs will never exceed V^2.
Note: Always remember that total number of edges in Sparse graphs will never exceed V.

What Is Edge Relaxation ?
Edge relaxation means checking if going through a specific edge provides a shorter path to a node. If it does, we update the shortest distance to that node.
For example:
You know the shortest distance to a city A.
You check if taking the road (edge) from A to city B gives a shorter distance to B than what you already know.
If yes, update the shortest distance to B.
This is called relaxing the edge.
*/


class BruteForce {    
public:
    typedef pair<int, int> P;
    // O(V*V) & O(V) : Where V = total nodes
    vector<int> shortestPath(vector<vector<P>>& adjList, int src) {
        int n = adjList.size();
        
        vector<bool> isVisited(n, false);
        vector<int> minDistance(n, INT_MAX); // The ith node stores the shortest distance from source to it
        minDistance[src] = 0;
        
        int nodesProcessed = n;

        while(nodesProcessed--) {
            // Select a node which is not visited yet and have the minimum distance among all all the unvisited nodes
            int node = -1, currDistance = INT_MAX;
            for(int i = 0; i < n; ++i) {
                if(!isVisited[i] && minDistance[i] < currDistance) {
                    currDistance = minDistance[i];
                    node = i;
                }
            }
                        
            // Mark node as visited
            isVisited[node] = true;

            // Relax the edges
            for(auto& [neighbor, edgeWeight] : adjList[node]) {
                int newDistance = currDistance + edgeWeight;
                if(!isVisited[neighbor] && minDistance[neighbor] > newDistance) {
                    minDistance[neighbor] = newDistance;
                }
            }
        }
        
        return minDistance;       // this is after processing all the nodes
   }
};
// Note: The above implementation is preferred for Dense Graph

class OriginalImplementationByPQ {
    typedef pair<int, int> P;       // note the definition of Pair
    
public:
    // O(ELogV) & O(V) : Where V = total nodes, E = total edges
    vector<int> dijkstra(vector<vector<P>>& adjList, int src) {
        int n = adjList.size();
        
        vector<bool> isVisited(n, false);
        vector<int> minDistance(n, INT_MAX);
        minDistance[src] = 0;
        
        priority_queue<P, vector<P>, greater<P>> minHeap;
        minHeap.push({0, src});
        
        while(!minHeap.empty()) {
            auto [currDistance, node] = minHeap.top(); minHeap.pop();

            isVisited[node] = true;

            for(auto& [neighbor, edgeWeight] : adjList[node]) { // Relax the edges
                int newDistance = currDistance + edgeWeight;
                if(!isVisited[neighbor] && minDistance[neighbor] > newDistance) {
                    // if the new distance is less than the min distance
                    minDistance[neighbor] = newDistance;
                    minHeap.push({newDistance, neighbor});
                }
            }
        }
        
        return minDistance;
   }
};  
// Note: The above implementation is preferred for Sparse Graph



class OriginalIntuitionBySet {
    typedef pair<int, int> P;
    
public:
    // O(ELogV) & O(V) : Where V = total nodes, E = total edges
    vector<int> dijkstra(vector<vector<P>>& adjList, int src) {
        int n = adjList.size();
        
        vector<bool> isVisited(n, false);
        vector<int> minDistance(n, INT_MAX);
        minDistance[src] = 0;

        set<P> st;        
        st.insert({0, src});
        
        while(!st.empty()) {
            auto it          = st.begin();
            int node         = (*it).second;
            int currDistance = (*it).first;
            st.erase(it);

            isVisited[node] = true;

            for(auto& [neighbor, edgeWeight] : adjList[node]) {
                int newDistance = currDistance + edgeWeight;
                if(!isVisited[neighbor] && minDistance[neighbor] > newDistance) {
                    minDistance[neighbor] = newDistance;
                    st.insert({newDistance, neighbor});
                }
            }
        }
        
        return minDistance;
   }
};  
// Note: The above implementation is preferred for Sparse Graph



class ModernImplementationByPQ {
    typedef pair<int, int> P;
    
public:
    // O(ELogV) & O(V)
    vector<int> dijkstra(vector<vector<P>>& adjList, int src) {
        int n = adjList.size();
        
        vector<int> minDistance(n, INT_MAX);
        minDistance[src] = 0;
        
        priority_queue<P, vector<P>, greater<P>> minHeap;
        minHeap.push({0, src});
                
        while(!minHeap.empty()) {
            auto [currDistance, node] = minHeap.top(); minHeap.pop();

            for(auto& [neighbor, edgeWeight] : adjList[node]) {
                int newDistance = currDistance + edgeWeight;
                if(minDistance[neighbor] > newDistance) {
                    minDistance[neighbor] = newDistance;
                    minHeap.push({newDistance, neighbor});
                }
            }
        }
        
        return minDistance;
   }
};  
// Note: The above implementation is preferred for Sparse Graph




