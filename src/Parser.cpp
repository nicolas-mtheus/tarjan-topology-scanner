#include "Parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool Parser::loadFromFile(const std::string& filepath, Graph& graph) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo: "
                  << filepath << "\n";
        return false;
    }

    std::string line;
    bool headerProcessed = false;

    while (std::getline(file, line)) {
        // Ignora linhas vazias
        if (line.empty()) {
            continue;
        }

        // Ignora comentarios no padrao Matrix Market (%) ou script (#)
        if (line[0] == '%' || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        std::vector<int> tokens;
        int val;
        while (ss >> val) {
            tokens.push_back(val);
        }

        // Caso 1: Cabecalho Matrix Market (ex: 7476 7476 56943)
        if (!headerProcessed && tokens.size() >= 3) {
            headerProcessed = true;
            continue;
        }

        // Caso 2: Aresta entre dois vertices (u v)
        if (tokens.size() >= 2) {
            graph.addEdge(tokens[0], tokens[1]);
        }
    }

    file.close();
    return true;
}
