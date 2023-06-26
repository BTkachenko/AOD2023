#include <vector>
#include <queue>
#include <limits>
#include "radix_heap.h"

using namespace std;

using Graph = unordered_map<int, vector<pair<int, int>>>;

std::vector<int> radix_heap_algorithm(const Graph& graph, int source) {
    int n = graph.size();
    std::vector<int> distance(n + 1, std::numeric_limits<int>::max());
    std::vector<bool> processed(n + 1, false);

    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // https://www.geeksforgeeks.org/implement-min-heap-using-stl/
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distance[source] = 0;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a vertex
        for (auto i = graph.at(u).begin(); i != graph.at(u).end(); ++i) {
            int v = (*i).first;
            int weight = (*i).second;

            // If there is shorter path to v through u.
            if (distance[v] > distance[u] + weight) {
                // Updating distance of v
                distance[v] = distance[u] + weight;
                pq.push(make_pair(distance[v], v));
            }
        }
    }

    return distance;
}
