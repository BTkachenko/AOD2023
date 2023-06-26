#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX

struct Edge {
    int u,v, flow, C, rev;
};

class Graph {
        int V;
        vector<vector<Edge>> adj;  // Zmieniamy na wektor wektorów
        map<pair<int, int>, int> flows;

        public:
        Graph(int V) : V(V), adj(V) {}  // Inicjalizujemy adj na podstawie V

    void addEdge(int u, int v, int C) {
        Edge a{u, v, 0, C, (int) adj[v].size()};
        Edge b{v, u, 0, 0, (int) adj[u].size()};
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
        for (auto &flow: flows) {
            cout << "Przepływ z " << flow.first.first << " do " << flow.first.second << ": " << flow.second << endl;
        }
    }

    void generateLPModel(const std::string &filename, const Graph& graph) {
        std::ofstream file(filename);

        file << "using JuMP, GLPK\n\n";
        file << "# Tworzenie modelu\n";
        file << "model = Model(GLPK.Optimizer)\n\n";

        // Liczba wierzchołków
        int V = graph.V;
        file << "# Liczba wierzchołków\n";
        file << "V = " << V << "\n\n";

        // Dodawanie zmiennych przepływu
        file << "# Dodawanie zmiennych przepływu\n";
        for (int i = 0; i < V; ++i) {
            for (const Edge& edge : graph.adj[i]) {
                file << "@variable(model, 0 <= x_" << edge.u << "_" << edge.v << " <= " << edge.C << ")\n";
            }
        }
        file << "\n";

        // Ustalanie funkcji celu
        file << "# Ustalanie funkcji celu\n";
        file << "@objective(model, Max, ";
        bool first = true;
        for (const Edge& edge : graph.adj[0]) {
            if (!first) {
                file << " + ";
            }
            file << "x_" << edge.u << "_" << edge.v;
            first = false;
        }
        file << ")\n\n";

        // Dodawanie ograniczeń
        file << "# Dodawanie ograniczeń\n";
        for (int i = 1; i < V - 1; ++i) {
            file << "@constraint(model, ";
            first = true;
            for (const Edge& edge : graph.adj[i]) {
                if (!first) {
                    file << " + ";
                }
                file << "x_" << edge.u << "_" << edge.v;
                first = false;
            }
            file << " - ";
            first = true;
            for (int j = 0; j < V; ++j) {
                for (const Edge& edge : graph.adj[j]) {
                    if (edge.v == i) {
                        if (!first) {
                            file << " - ";
                        }
                        file << "x_" << edge.u << "_" << edge.v;
                        first = false;
                    }
                }
            }
            file << " == 0)\n";
        }
        file << "\n";

        // Rozwiązanie modelu
        file << "# Rozwiązanie modelu\n";
        file << "optimize!(model)\n\n";

        // Wyświetlanie wyniku
        file << "# Wyświetlanie wyniku\n";
        file << "println(\"Maksymalny przepływ: \", objective_value(model))\n\n";


        file.close();
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
    int V = 1 << k;  // Liczba wierzchołków wynosi 2 do potęgi k
    Graph g(V);  // Inicjalizujemy graf z V wierzchołkami
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

    if (argc > 5 && strcmp(argv[4], "--glpk") == 0) {
        g.generateLPModel(argv[5],g);
    }

    return 0;
}
