#include <vector>
#include <list>
#include <limits>
#include <iostream>
#include <sstream>
#include <map>
#include "dial.h"



std::vector<int> dial_algorithm(const Graph &graph, int source, int max_cost) {
    int n = graph.size();
    std::vector<int> distance(n + 1, std::numeric_limits<int>::max());
    distance[source] = 0;

    std::vector<std::deque<int>> buckets(max_cost * n + 1);

    buckets[0].push_back(source);

    for (int i = 0; i <= max_cost * n; ++i) {
        while (!buckets[i].empty()) {
            int u = buckets[i].front();
            buckets[i].pop_front();

            if (graph.find(u) == graph.end()) {
                continue;
            }

            for (const auto &edge : graph.at(u)) {
                int v = edge.first;
                int w = edge.second;
                if (distance[u] + w < distance[v]) {
                    distance[v] = distance[u] + w;
                    buckets[distance[v]].push_back(v);
                }
            }
        }
    }

    return distance;
}
