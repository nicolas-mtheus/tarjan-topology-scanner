#include "TarjanSPOF.hpp"
#include <algorithm>
#include <chrono>

TarjanSPOF::TarjanSPOF() : timer(0) {}

void TarjanSPOF::dfs(int u, int parent, const Graph& graph) {
    visited[u] = true;
    tin[u] = low[u] = ++timer;
    int children = 0;

    for (int v : graph.getNeighbors(u)) {
        if (v == parent) {
            continue;
        }

        if (visited[v]) {
            // Back-edge encontrada: atualiza o low com o tin do ancestral
            low[u] = std::min(low[u], tin[v]);
        } else {
            // Tree-edge encontrada: avanca na arvore DFS
            children++;
            dfs(v, u, graph);
            low[u] = std::min(low[u], low[v]);

            // Condicao de Ponto de Articulacao para vertice nao-raiz
            if (low[v] >= tin[u] && parent != -1) {
                isArticulation[u] = true;
            }

            // Condicao de Ponte: enlace critico sem rotas alternativas
            if (low[v] > tin[u]) {
                int extU = graph.toExternalId(u);
                int extV = graph.toExternalId(v);
                bridgesList.push_back({std::min(extU, extV),
                                       std::max(extU, extV)});
            }
        }
    }

    // Condicao de Ponto de Articulacao para a raiz da arvore DFS
    if (parent == -1 && children > 1) {
        isArticulation[u] = true;
    }
}

AuditReport TarjanSPOF::run(const Graph& graph) {
    auto startTime = std::chrono::high_resolution_clock::now();

    int n = graph.getNumVertices();
    timer = 0;
    tin.assign(n, 0);
    low.assign(n, 0);
    visited.assign(n, false);
    isArticulation.assign(n, false);
    bridgesList.clear();

    int components = 0;

    // Executa a DFS sobre todos os vertices (suporte a floresta DFS)
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            components++;
            dfs(i, -1, graph);
        }
    }

    AuditReport report;
    report.totalVertices = n;
    report.totalEdges = graph.getNumEdges();
    report.averageDegree = graph.getAverageDegree();
    report.connectedComponents = components;
    report.bridges = bridgesList;

    // Coleta pontos de articulacao e seus graus
    std::vector<std::pair<int, int>> artHubs;
    for (int i = 0; i < n; ++i) {
        if (isArticulation[i]) {
            int extId = graph.toExternalId(i);
            report.articulationPoints.push_back(extId);
            artHubs.push_back({graph.getDegree(i), extId});
        }
    }

    // Ordena os SPOFs por grau de impacto decrescente
    std::sort(artHubs.begin(), artHubs.end(),
              [](const std::pair<int, int>& a,
                 const std::pair<int, int>& b) {
                  return a.first > b.first;
              });

    report.topRiskHubs = artHubs;

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
    report.executionTimeMs = elapsed.count();

    return report;
}
