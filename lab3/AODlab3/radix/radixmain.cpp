#include "radix_heap.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>
#include <map>


using namespace std;

using Graph = unordered_map<int, vector<pair<int, int>>>;

Graph parse_input(const string &file_path) {
    ifstream file(file_path);
    string line;
    Graph graph;

    while (getline(file, line)) {
        if (line.empty()) continue;

        char identifier = line[0];
        istringstream iss(line.substr(1));

        if (identifier == 'p') {
            int n, m;
            string problem, problem_type;
            iss >> problem >> problem_type >> n >> m;
            for (int i = 1; i <= n; ++i) {
                graph[i] = vector<pair<int, int>>();
            }
        } else if (identifier == 'a') {
            int u, v, w;
            iss >> u >> v >> w;
            graph[u].emplace_back(v, w);
        }
    }

    return graph;
}



vector<int> read_source_nodes(const string &file_path) {
    ifstream file(file_path);
    vector<int> source_nodes;
    int node;

    while (file >> node) {
        source_nodes.push_back(node);
    }

    return source_nodes;
}

map<string, string> parse_args(int argc, char *argv[]) {
    map<string, string> options;
    for (int i = 1; i < argc; i += 2) {
        options[string(argv[i])] = string(argv[i + 1]);
    }

    return options;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " -d input_file -ss sources_file -oss output_file" << endl;
        cerr << "or" << endl;
        cerr << "Usage: " << argv[0] << " -d input_file -p2p pairs_file -op2p output_file" << endl;
        return 1;
    }

    auto options = parse_args(argc, argv);

    Graph graph = parse_input(options["-d"]);

    if (options.find("-ss") != options.end()) {
        auto source_nodes = read_source_nodes(options["-ss"]);
        ofstream output(options["-oss"]);

        for (int source : source_nodes) {
            auto start_time = chrono::high_resolution_clock::now();
            vector<int> distances = radix_heap_algorithm(graph, source);
            auto end_time = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> time_taken = end_time - start_time;
            output << "Source node: " << source << endl;
            output << "Time taken: " << time_taken.count() << " milliseconds" << endl;

            for (size_t i = 1; i < distances.size(); ++i) {
                if (i != source) {
                    output << "Node " << i << ": " << distances[i] << endl;
                }
            }

            output << endl;
        }

        output.close();

    } else if (options.find("-p2p") != options.end()) {
        ifstream pairs_file(options["-p2p"]);
        ofstream output(options["-op2p"]);
        int source, destination;

        while (pairs_file >> source >> destination) {
            vector<int> distances = radix_heap_algorithm(graph, source);
            output << "Shortest path from " << source << " to " << destination << ": " << distances[destination] << endl;
        }

        output.close();
    } else {
        cerr << "Invalid arguments provided" << endl;
        return 1;
    }

    return 0;
}
