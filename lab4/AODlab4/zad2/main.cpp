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
    return 0;
}