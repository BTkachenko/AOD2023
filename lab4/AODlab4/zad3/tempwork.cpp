//Rabotaet
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
    file << "println(\"Maksymalne skojarzenie: \", objective_value(model))\n\n";

    file.close();
}


//NE RABOTAET
void generateLPModel(const std::string &filename, int n) {
    std::ofstream file(filename);

    file << "using JuMP, GLPK\n\n";
    file << "model = Model(GLPK.Optimizer)\n\n";

    // Dodawanie zmiennych decyzyjnych
    file << "# Dodawanie zmiennych decyzyjnych\n";
    std::map<std::pair<int, int>, std::string> varName;
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            // Upewniamy się, że dodajemy każdą krawędź tylko raz
            if (u < v) {
                varName[{u, v}] = "x_" + std::to_string(u) + "_" + std::to_string(v);
                file << "@variable(model, 0 <= " << varName[{u, v}] << " <= 1, Int)\n";
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
        bool firstInRow = true;
        for(auto it = varName.begin(); it != varName.end(); ++it) {
            if(it->first.first == u || it->first.second == u) {
                if(!firstInRow){
                    file << " + ";
                }else{
                    file << "@constraint(model, ";
                    firstInRow = false;
                }
                file << it->second;
            }
        }
        if(!firstInRow){
            file << " <= 1)\n";
        }
    }
    file << "\n";

    // Rozwiązanie modelu
    file << "# Rozwiązanie modelu\n";
    file << "optimize!(model)\n\n";

    // Wyświetlanie wyniku
    file << "# Wyświetlanie wyniku\n";
    file << "println(\"Maksymalne skojarzenie: \", objective_value(model))\n\n";

    file.close();
}
