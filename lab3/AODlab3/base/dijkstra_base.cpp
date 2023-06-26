#include "dijkstra_base.h"
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>

using namespace std;

vector<int> dijkstra_base(const Graph &graph, int source) {
    vector<int> distances(graph.size() + 1, numeric_limits<int>::max());
    distances[source] = 0;

    using Node = pair<int, int>;
    priority_queue<Node, vector<Node>, greater<Node>> queue;
    queue.emplace(0, source);

    while (!queue.empty()) {
        auto [current_distance, current_node] = queue.top();
        queue.pop();

        if (current_distance > distances[current_node]) {
            continue;
        }

        for (const auto &[neighbor, weight] : graph.at(current_node)) {
            int distance = current_distance + weight;

            if (distance < distances[neighbor]) {
                distances[neighbor] = distance;
                queue.emplace(distance, neighbor);
            }
        }
    }

    return distances;
}
