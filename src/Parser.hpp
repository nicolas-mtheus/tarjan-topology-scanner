#ifndef PARSER_HPP
#define PARSER_HPP

#include "Graph.hpp"
#include <string>

/**
 * @brief Modulo responsavel pela leitura e carregamento do grafo a partir
 * de arquivos de texto (compativel com formato Matrix Market .mtx e edges).
 */
class Parser {
public:
    static bool loadFromFile(const std::string& filepath, Graph& graph);
};

#endif // PARSER_HPP
