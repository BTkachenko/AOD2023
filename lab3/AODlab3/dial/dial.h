#pragma once
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>

using Graph = std::unordered_map<int, std::vector<std::pair<int, int>>>;

std::vector<int> dial_algorithm(const Graph &graph, int source, int max_cost);


