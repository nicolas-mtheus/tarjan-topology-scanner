#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_map>

/**
 * @brief Representacao de um grafo nao-direcionado usando Lista de Adjacencia.
 * Mapeia identificadores externos arbitrarios para indices internos (0-based).
 */
class Graph {
private:
    int numVertices;
    int numEdges;
    std::vector<std::vector<int>> adjacencyList;
    std::unordered_map<int, int> externalToInternal;
    std::vector<int> internalToExternal;

public:
    Graph();

    int getOrCreateVertex(int externalId);
    void addEdge(int externalU, int externalV);

    int getNumVertices() const;
    int getNumEdges() const;
    int getDegree(int u) const;
    double getAverageDegree() const;

    const std::vector<int>& getNeighbors(int u) const;
    int toExternalId(int internalId) const;
};

#endif // GRAPH_HPP
