#include <bits/stdc++.h>
using namespace std;

vector<int> *adj;
int *pairU, *pairV, *dist;

bool bfs(int n) {
    queue<int> Q;

    for (int u = 0; u < n; u++) {
        if (pairU[u] == -1) {
            dist[u] = 0;
            Q.push(u);
        } else {
            dist[u] = INT_MAX;
        }
    }

    dist[-1] = INT_MAX;

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();

        if (u != -1) {
            for (int v : adj[u]) {
                if (dist[pairV[v]] == INT_MAX) {
                    dist[pairV[v]] = dist[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
    }

    return dist[-1] != INT_MAX;
}

bool dfs(int u) {
    if (u == -1) {
        return true;
    }

    for (int v : adj[u]) {
        if (dist[pairV[v]] == dist[u] + 1 && dfs(pairV[v])) {
            pairU[u] = v;
            pairV[v] = u;
            return true;
        }
    }

    dist[u] = INT_MAX;
    return false;
}

int HopcroftKarp(int n) {
    fill(pairU, pairU + n, -1);
    fill(pairV, pairV + n, -1);

    int matching = 0;
    while (bfs(n)) {
        for (int u = 0; u < n; u++) {
            if (pairU[u] == -1 && dfs(u)) {
                matching++;
            }
        }
    }

    return matching;
}

void generateLPModel(const std::string &filename, int n) {
    std::ofstream file(filename);

    file << "using JuMP, GLPK\n\n";
    file << "# Tworzenie modelu\n";
    file << "model = Model(GLPK.Optimizer)\n\n";

    // Dodawanie zmiennych decyzyjnych
    file << "# Dodawanie zmiennych decyzyjnych\n";
    std::map<std::pair<int, int>, std::string> varName;
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            // Sprawdzamy czy taka para już istnieje, jeżeli nie, to dodajemy
            if(varName.find({std::min(u,v), std::max(u,v)}) == varName.end()){
                varName[{std::min(u,v), std::max(u,v)}] = "x_" + std::to_string(u) + "_" + std::to_string(v);
                file << "@variable(model, 0 <= " << varName[{std::min(u,v), std::max(u,v)}] << " <= 1, Int)\n";
            }
        }
    }
    file << "\n";

    // Ustalanie funkcji celu
    file << "# Ustalanie funkcji celu\n";
    file << "@objective(model, Max, ";
    for(auto it = varName.begin(); it != varName.end(); ++it) {
        if (it != varName.begin()) {
            file << " + ";
        }
        file << it->second;
    }
    file << ")\n\n";

    // Dodawanie ograniczeń
    file << "# Dodawanie ograniczeń\n";
    for (int u = 0; u < n; ++u) {
        bool hasEdges = false;
        for(auto it = varName.begin(); it != varName.end(); ++it) {
            if(it->first.first == u || it->first.second == u) {
                if(hasEdges){
                    file << " + ";
                }else{
                    file << "@constraint(model, ";
                }
                file << it->second;
                hasEdges = true;
            }
        }
        if(hasEdges){
            file << " <= 1)\n";
        }
    }
    file << "\n";

    // Rozwiązanie modelu
    file << "# Rozwiązanie modelu\n";
    file << "optimize!(model)\n\n";

    // Wyświetlanie wyniku
    file << "# Wyświetlanie wyniku\n";
    file << "println(\"Maksymalne skojarzenie: \", 2*objective_value(model))\n\n";

    file.close();
}



int main(int argc, char *argv[]) {
    int k = atoi(argv[2]);
    int i = atoi(argv[4]);
    int n = 2 * (1 << k);

    adj = new vector<int>[n];
    pairU = new int[n];
    pairV = new int[n];
    dist = new int[n];

    for (int u = 0; u < (1 << k); u++) {
        for (int j = 0; j < i; j++) {
            int v = (1 << k) + rand() % (1 << k);
            adj[u].push_back(v);
            adj[v].push_back(u);  // Add edge in the opposite direction
        }
    }

    clock_t start = clock();
    int matching = HopcroftKarp(n);
    double time = (double)(clock() - start) / CLOCKS_PER_SEC;

    cout << "Maksymalne skojarzenie: " << matching << endl;
    cerr << "Czas działania: " << time << "s" << endl;

    if (argc > 5 && strcmp(argv[5], "--printMatching") == 0) {
        for (int u = 0; u < n; u++) {
            if (pairU[u] != -1) {
                cout << u << " " << pairU[u] << endl;
            }
        }
    }
    if (argc > 6 && strcmp(argv[6], "--glpk") == 0) {
        string filename = argv[7];
        generateLPModel(filename, n);
    }

    return 0;
}