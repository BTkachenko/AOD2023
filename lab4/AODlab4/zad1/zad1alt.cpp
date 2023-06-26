#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX
#define N 65536
#define M 524288

struct Edge {
    int v, flow, C, rev;
};

class Graph {
    int V;
    vector<Edge> adj[N];
    map<pair<int, int>, int> flows;

public:
    Graph(int V) {
        this->V = V;
    }

    void addEdge(int u, int v, int C) {
        Edge a{v, 0, C, (int)adj[v].size()};
        Edge b{u, 0, 0, (int)adj[u].size()};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool bfs(int s, int t, int parent[]) {
        bool visited[V];
        memset(visited, 0, sizeof(visited));

        queue<int> Q;
        Q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int i = 0; i < adj[u].size(); i++) {
                Edge &e = adj[u][i];
                if (!visited[e.v] && e.flow < e.C) {
                    Q.push(e.v);
                    parent[e.v] = u;
                    visited[e.v] = true;
                }
            }
        }

        return visited[t];
    }

    int EdmondsKarp(int s, int t) {
        int parent[V];
        int max_flow = 0;

        while (bfs(s, t, parent)) {
            int path_flow = INF;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                int i;
                for (i = 0; i < adj[u].size(); i++)
                    if (adj[u][i].v == v)
                        break;
                path_flow = min(path_flow, adj[u][i].C - adj[u][i].flow);
            }

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                int i;
                for (i = 0; i < adj[u].size(); i++)
                    if (adj[u][i].v == v)
                        break;
                adj[u][i].flow += path_flow;
                adj[v][adj[u][i].rev].flow -= path_flow;
                flows[{u, v}] = adj[u][i].flow;
            }

            max_flow += path_flow;
        }

        return max_flow;
    }

    void printFlows() {
        for (auto &flow : flows) {
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
    Graph g(N);
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
    int max_flow = g.EdmondsKarp(0, (1 << k) - 1);
    double time = (double)(clock() - start) / CLOCKS_PER_SEC;

    cout << "Maksymalny przepływ: " << max_flow << endl;
    cerr << "Czas działania: " << time << "s" << endl;

    if (argc > 3 && strcmp(argv[3], "--printFlow") == 0) {
        g.printFlows();
    }

    return 0;
}

