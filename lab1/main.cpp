#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <list>

using namespace std;

enum Directionality {
    DIRECTED,
    UNDIRECTED
};

class Graph {
    Directionality directionality; // zmienna przechowująca kierunek grafu
    int V; // liczba wierzchołków w grafie
    int E; // liczba krawędzi w grafie
    vector<vector<int>> neighbors; // lista sąsiedztwa wierzchołków grafu
    vector<vector<int>> reverse_neighbors; // lista odwrotna sąsiedztwa wierzchołków grafu

    void add_edge(int u, int v) { // funkcja dodająca krawędź do grafu
        neighbors[u].push_back(v); // dodaj krawędź u->v do listy sąsiedztwa
        reverse_neighbors[v].push_back(u); // dodaj krawędź v->u do listy odwrotnej sąsiedztwa
        if (directionality == UNDIRECTED) { // jeśli graf jest nieskierowany, dodaj również krawędź v->u do listy sąsiedztwa i u->v do listy odwrotnej sąsiedztwa
            neighbors[v].push_back(u);
            reverse_neighbors[u].push_back(v);
        }
    }

public:
    
    Graph(string filename) { // konstruktor tworzący graf z pliku
        ifstream myfile(filename);

        char flag;
        myfile >> flag;
        directionality = (flag == 'D') ? DIRECTED : UNDIRECTED; // odczytaj kierunek grafu z pliku
        myfile >> V; // odczytaj liczbę wierzchołków z pliku
        myfile >> E; // odczytaj liczbę krawędzi z pliku

        neighbors.assign(V + 1, vector<int>()); // zainicjuj listę sąsiedztwa
        reverse_neighbors.assign(V + 1, vector<int>()); // zainicjuj listę odwrotną sąsiedztwa

        int u, v;
        for(int i = 0; i < E; i++) { // odczytaj krawędzie z pliku i dodaj je do grafu
            myfile >> u >> v;
            add_edge(u, v);
        }

        myfile.close();
    }
    //Zadanie 1
    void bfs(int start, bool print) {
        vector<int> visited;
        visited.assign(V + 1, 0);
        queue<int> que;
        que.push(start);
        visited[start] = 1;

        while(!que.empty()) {
            int u = que.front();
            que.pop();

            if(print == true) {
                cout << u << " ";
            }

            for(int v : neighbors[u]) {
                if(visited[v] == 0) {
                    visited[v] = 1;
                    que.push(v);
                }
            }
        }

        cout << endl;
    }

    //Zadanie 1
    void dfs(int start, bool print) {
        vector<int> visited; // Wektor odwiedzonych wierzchołków.
        visited.assign(V + 1, 0);
        stack<int> st; // Stos do przechowywania wierzchołków.
        st.push(start); // Dodanie wierzchołka startowego do stosu.
        visited[start] = 1; // Oznaczenie wierzchołka startowego jako odwiedzonego.

        while(!st.empty()) {
            int u = st.top();
            st.pop();

            if(print == true ) {
                cout << u << " ";
            }

            for(int v : neighbors[u]) { // Przeszukiwanie sąsiadów wierzchołka u.
                if(visited[v] == 0) { // Jeśli sąsiad nie był odwiedzony...
                visited[v] = 1; // Oznaczanie sąsiada jako odwiedzonego.
                st.push(v); // Dodawanie sąsiada do stosu.
        }
    }
        }

        cout << endl;
    }

    void DFSWithComponents(int start, vector<int>& visited, vector<int>& component, bool reversed = false) {
        visited[start] = 1;
        stack<int> st;

        st.push(start);

        while(!st.empty()) {
            int u = st.top();
            st.pop();

            component.push_back(u);

            if(!reversed) {
                for(int v : neighbors[u]) {
                    if(visited[v] == 0) {
                        visited[v] = 1;
                        st.push(v);
                    }
                }
            }
            else {
                for(int v : reverse_neighbors[u]) {
                    if(visited[v] == 0) {
                        visited[v] = 1;
                        st.push(v);
                    }
                }
            }
        }
    }

    //Zadanie2
    // Funkcja topological_sort wykonuje sortowanie topologiczne na skierowanym grafie acyklicznym (DAG).
// Zwraca wartość true, jeśli liczba wierzchołków w wyniku (results) jest inna niż liczba wierzchołków w grafie (V).
    bool topological_sort() {
        vector<int> results; // Wektor wyników sortowania topologicznego.
        vector<int> Ei; // Wektor przechowujący ilość krawędzi wchodzących do wierzchołków.
        Ei.assign(V + 1, 0);

        for(int u = 1; u <= V; u++) {
            for(int v : neighbors[u]) {
                Ei[v]++;
            }
        }

        queue<int> que;

        for(int u = 1; u <= V; u++) {
            if(Ei[u] == 0) {
                que.push(u);
            }
        }

        while(!que.empty()) {
            int u = que.front();
            que.pop();
            results.push_back(u);

            // Aktualizacja ilości krawędzi wchodzących do sąsiadów wierzchołka u.
        for(int v : neighbors[u]) {
            Ei[v]--;
            if(Ei[v] == 0) {
            que.push(v); // Dodanie sąsiada do kolejki, jeśli nie ma krawędzi wchodzących.
                }
    }
        }

        if(V <= 200) {
            for(int u : results) {
                cout << u << " ";
            }
            cout << endl;
        }

        int size = results.size();

        return (size != V);
    }

    

    void fillOrder(int start, vector<int>& visited, stack<int>& Stack) {
        visited[start] = true;
        stack<int> st;
        stack<int> temp;
    
        st.push(start);

        while(!st.empty()) {
            int u = st.top();
            st.pop();
            temp.push(u);

            for(int v : neighbors[u]) {
                if(visited[v] == 0) {
                    visited[v] = 1;
                    st.push(v);
                }
            }
        }

        while(!temp.empty()) {
            int u = temp.top();
            temp.pop();
            Stack.push(u);
        }
    }
//Zadanie 3
// Funkcja strong_connect sprawdza silnie spójne składowe w grafie skierowanym.
// Silnie spójna składowa to taki podgraf, w którym istnieje ścieżka między każdą parą wierzchołków.
//Ten kod używa algorytmu Kosaraju do znalezienia silnie spójnych składowych w grafie skierowanym. Algorytm Kosaraju działa w czasie O(|V| + |E|)
    void strong_connect() {
        stack<int> st;
        vector<int> visited;
        visited.assign(V + 1, 0);

        for(int u = 1; u <= V; u++) {
            if(visited[u] == false) {
                fillOrder(u, visited, st);
            }
        }

        for(int u = 1; u <= V; u++) {
            visited[u] = 0;
        }

        int i = 0; // Licznik silnie spójnych składowych.

// Przeszukiwanie grafu w odwrotnym porządku krawędzi.
    while (!st.empty()) {
      int u = st.top(); // Pobieranie wierzchołka z góry stosu.
      st.pop();

     if (visited[u] == 0) { // Jeśli wierzchołek nie był odwiedzony.
        vector<int> component; // Wektor silnie spójnej składowej.
        DFSWithComponents(u, visited, component, true); // DFS na transponowanym grafie.
                i++;

                if(V <= 200) {
                    cout << "Składowa nr " << i << ": \n";

                    for(int v : component) {
                        cout << v << ", ";
                    }
                    cout << endl;
                }
            }
            if(st.empty()) {
                cout << "Graf posiada " << i << " silnie spójne składowe \n";
            }
        }
    }

//Zadanie 4
// Funkcja bipartite sprawdza, czy dany graf jest dwudzielny, czyli czy można go podzielić na dwa zbiory wierzchołków, probojac pokolorowac go na 2 kolory
// takie że każda krawędź łączy wierzchołki z różnych zbiorów.

   void bipartite() {
        queue<int> que;
        vector<int> colors;
        colors.assign(V + 1, 0);
        int x, y, z;

        for(int i = 1; i <= V; i++) {
            if(colors[i] == 0) {
                colors[i] = 1;
                que.push(i);

                while(!que.empty()) {
                    int u = que.front();
                    que.pop();

                    for(int v : neighbors[u]) {
                        if(colors[v] == colors[u]){
                            cout << "Graf nie jest dwudzielny!\n";
                            return;
                        }
                        if(colors[v] == 0) {
                            colors[v] = 3 - colors[u];
                            que.push(v);
                        }
                    }
                }
            }
        }

        cout << "Graf jest dwudzielny!\n";
        if(V <= 200) {
            cout << "Podział numer 1:\n";
            for(int i = 1; i <= V; i++) {
                if(colors[i] == 1) {
                    cout << i;
                    if(i != V) cout << ", ";
                }
            }
            cout << "\nPodział numer 2:\n";
            for(int i = 1; i <= V; i++) {
                if(colors[i] == 2) {
                    cout << i;
                    if(i != V) cout << ", ";
                }
            }
            printf("\n");
        }
    }

    void print_graph(bool neighbors_show = true) {
        cout << "Liczba wierzchołków: " << V << endl;
        cout << "Liczba krawędzi: " << E << endl;
        cout << "Skierowanie: " << (directionality == DIRECTED ? "TAK" : "NIE") << endl;

        if(neighbors_show) {
            cout << "Lista sąsiedztwa:" << endl;

            for (int u = 1; u <= V; u++) {
                cout << u << ": ";
                for (int v : neighbors[u]) {
                    cout << v << " ";
                }
                cout << endl;
            }
        }
    }
};

int main(int argc, char** argv) {

    
  
   //Zadanie
   Graph gbfs("aod_testy1/2/g2b-1.txt");
   gbfs.dfs(2,true);


   //Zadanie 2
   cout<<"_____________________________________________"<<endl;
   cout<<"ZADANIE 2"<<endl;
   cout<<"_____________________________________________"<<endl;
   Graph g2("aod_testy1/2/g2a-2.txt");
   g2.print_graph(false);
   cout << g2.topological_sort() << endl;
  

   Graph g("aod_testy1/2/g2b-5.txt");
   g.print_graph(false);
   cout << g.topological_sort() << endl;
  

   //Zadanie 3
   cout<<"_____________________________________________"<<endl;
   cout<<"ZADANIE 3"<<endl;
   cout<<"_____________________________________________"<<endl;
   Graph g3("aod_testy1/3/g3-6.txt");
   g3.print_graph(false);
   g3.strong_connect();
  

    //Zadanie 4
    cout<<"_____________________________________________"<<endl;
    cout<<"ZADANIE 4"<<endl;
   cout<<"_____________________________________________"<<endl;
    Graph g4("aod_testy1/4/d4a-2.txt");
   g4.print_graph(false);
   g4.bipartite();
  


     Graph g5("aod_testy1/4/u4b-2.txt");
   g5.print_graph(false);
   g5.bipartite();
   

    return 0;
}