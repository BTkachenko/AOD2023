#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX

struct Edge {
    int v, flow, C, rev;
};

class Graph {
    int V;
    vector<vector<Edge>> adj;
    map<pair<int, int>, int> flows;
    vector<int> level;
    vector<int> start;

public:
    Graph(int V) : V(V), adj(V), level(V), start(V) {}

    void addEdge(int u, int v, int C) {
        Edge a{v, 0, C, (int) adj[v].size()};
        Edge b{u, 0, 0, (int) adj[u].size()};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;

        queue<int> Q;
        Q.push(s);

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (const auto &e : adj[u]) {
                if (level[e.v] < 0 && e.flow < e.C) {
                    level[e.v] = level[u] + 1;
                    Q.push(e.v);
                }
            }
        }

        return level[t] >= 0;
    }

    int sendFlow(int u, int flow, int t, vector<int>& start) {
        if (u == t)
            return flow;

        while (start[u] < adj[u].size()) {
            Edge &e = adj[u][start[u]];
            if (level[e.v] == level[u] + 1 && e.flow < e.C) {
                int curr_flow = min(flow, e.C - e.flow);
                int temp_flow = sendFlow(e.v, curr_flow, t, start);

                if (temp_flow > 0) {
                    e.flow += temp_flow;
                    adj[e.v][e.rev].flow -= temp_flow;
                    flows[{u, e.v}] = e.flow;
                    return temp_flow;
                }
            }
            start[u]++;
        }

        return 0;
    }

    int Dinic(int s, int t) {
        if (s == t)
            return -1;

        int total = 0;

        while (bfs(s, t) == true) {
            fill(start.begin(), start.end(), 0);
            int flow = sendFlow(s, INF, t, start);
            while (flow) {
                total += flow;
                flow = sendFlow(s, INF, t, start);
            }
        }

        return total;
    }

    void printFlows() {
        for (auto &flow: flows) {
            cout << "Przepływ z " << flow.first.first << " do " << flow.first.second << ": " << flow.second << endl;
        }
    }
};

int hammingWeight(int n) {
    int weight = 0;
    while (n) {
        weight += n & 1;
        n >>= 1;
    }
    return weight;
}

int main(int argc, char *argv[]) {
    int k = atoi(argv[2]);
    int V = 1 << k;
    Graph g(V);
    for (int i = 0; i < (1 << k); i++) {
        for (int j = 0; j < k; j++) {
            int v = i ^ (1 << j);
            if (i < v) {
                int l = max(max(hammingWeight(i), hammingWeight(v)), max(k - hammingWeight(i), k - hammingWeight(v)));
                int capacity = rand() % (1 << l) + 1;
                g.addEdge(i, v, capacity);
            }
        }
    }

    clock_t start = clock();
    int max_flow = g.Dinic(0, (1 << k) - 1);  // Tutaj używamy algorytmu Dinica
    double time = (double)(clock() - start) / CLOCKS_PER_SEC;

    cout << "Maksymalny przepływ: " << max_flow << endl;
    cerr << "Czas działania: " << time << "s" << endl;

    if (argc > 3 && strcmp(argv[3], "--printFlow") == 0) {
        g.printFlows();
    }

    return 0;
}
