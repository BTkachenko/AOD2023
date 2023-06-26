#pragma once
#include <unordered_map>
#include <vector>

using Graph = std::unordered_map<int, std::vector<std::pair<int, int>>>;

std::vector<int> radix_heap_algorithm(const Graph &graph, int source);


