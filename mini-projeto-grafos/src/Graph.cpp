#include "Graph.hpp"
#include <algorithm>

Graph::Graph() : numVertices(0), numEdges(0) {}

int Graph::getOrCreateVertex(int externalId) {
    auto it = externalToInternal.find(externalId);
    if (it != externalToInternal.end()) {
        return it->second;
    }
    int internalId = numVertices++;
    externalToInternal[externalId] = internalId;
    internalToExternal.push_back(externalId);
    adjacencyList.emplace_back();
    return internalId;
}

void Graph::addEdge(int externalU, int externalV) {
    if (externalU == externalV) {
        return; // Ignora auto-lacos
    }

    int u = getOrCreateVertex(externalU);
    int v = getOrCreateVertex(externalV);

    adjacencyList[u].push_back(v);
    adjacencyList[v].push_back(u);
    numEdges++;
}

int Graph::getNumVertices() const {
    return numVertices;
}

int Graph::getNumEdges() const {
    return numEdges;
}

int Graph::getDegree(int u) const {
    if (u >= 0 && u < numVertices) {
        return static_cast<int>(adjacencyList[u].size());
    }
    return 0;
}

double Graph::getAverageDegree() const {
    if (numVertices == 0) {
        return 0.0;
    }
    return (2.0 * static_cast<double>(numEdges)) /
           static_cast<double>(numVertices);
}

const std::vector<int>& Graph::getNeighbors(int u) const {
    return adjacencyList[u];
}

int Graph::toExternalId(int internalId) const {
    if (internalId >= 0 && internalId < numVertices) {
        return internalToExternal[internalId];
    }
    return -1;
}
