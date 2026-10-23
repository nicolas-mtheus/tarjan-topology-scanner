#ifndef TARJAN_SPOF_HPP
#define TARJAN_SPOF_HPP

#include "Graph.hpp"
#include <vector>
#include <utility>

/**
 * @brief Representacao de uma aresta critica (Ponte).
 */
struct Bridge {
    int u;
    int v;
};

/**
 * @brief Estrutura que consolida o resultado da auditoria de resiliencia.
 */
struct AuditReport {
    int totalVertices;
    int totalEdges;
    double averageDegree;
    int connectedComponents;
    std::vector<int> articulationPoints;
    std::vector<Bridge> bridges;
    std::vector<std::pair<int, int>> topRiskHubs; // {degree, externalId}
    double executionTimeMs;
};

/**
 * @brief Implementacao do Algoritmo de Tarjan para identificacao de
 * Single Points of Failure (SPOFs) em tempo O(V + E).
 */
class TarjanSPOF {
private:
    int timer;
    std::vector<int> tin;
    std::vector<int> low;
    std::vector<bool> visited;
    std::vector<bool> isArticulation;
    std::vector<Bridge> bridgesList;

    void dfs(int u, int parent, const Graph& graph);

public:
    TarjanSPOF();
    AuditReport run(const Graph& graph);
};

#endif // TARJAN_SPOF_HPP
