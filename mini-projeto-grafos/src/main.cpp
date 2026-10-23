#include "Graph.hpp"
#include "Parser.hpp"
#include "TarjanSPOF.hpp"
#include <iostream>
#include <iomanip>

/**
 * @brief Exibe o cabecalho e informacoes gerais da topologia auditada.
 */
static void printHeader(const AuditReport& report) {
    std::cout << "====================================================\n";
    std::cout << "   AUDITORIA DE RESILIENCIA E PONTOS UNICOS DE FALHA\n";
    std::cout << "           (Single Points of Failure - SPOFs)       \n";
    std::cout << "====================================================\n";
    std::cout << "Vertices (Ativos de Rede): "
              << report.totalVertices << "\n";
    std::cout << "Arestas (Enlaces de Rede): "
              << report.totalEdges << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Grau Medio: "
              << report.averageDegree << "\n";
    std::cout << "Componentes Conexos: "
              << report.connectedComponents << "\n";
    std::cout << "----------------------------------------------------\n";
}

/**
 * @brief Exibe as vulnerabilidades estruturais mapeadas (pontos e pontes).
 */
static void printVulnerabilities(const AuditReport& report) {
    double artPct = (report.totalVertices > 0)
        ? (100.0 * report.articulationPoints.size()) / report.totalVertices
        : 0.0;
    double bridgePct = (report.totalEdges > 0)
        ? (100.0 * report.bridges.size()) / report.totalEdges
        : 0.0;

    std::cout << "Pontos de Articulacao (Roteadores SPOF): "
              << report.articulationPoints.size()
              << " (" << artPct << "% dos ativos)\n";
    std::cout << "Pontes Identificadas (Enlaces Criticos): "
              << report.bridges.size()
              << " (" << bridgePct << "% dos enlaces)\n";
    std::cout << "----------------------------------------------------\n";

    // Exibe os hubs de maior criticidade estrutural
    std::cout << "Top Ativos de Maior Impacto em Caso de Falha:\n";
    int limit = std::min(static_cast<int>(report.topRiskHubs.size()), 5);
    for (int i = 0; i < limit; ++i) {
        std::cout << "  " << (i + 1) << ". Vertice "
                  << report.topRiskHubs[i].second
                  << " -> Grau: " << report.topRiskHubs[i].first
                  << " conexoes diretas\n";
    }

    // Se for uma amostra reduzida, lista detalhadamente os elementos
    if (report.totalVertices <= 30) {
        std::cout << "\nLista de Pontos de Articulacao: ";
        for (size_t i = 0; i < report.articulationPoints.size(); ++i) {
            std::cout << report.articulationPoints[i]
                      << (i + 1 < report.articulationPoints.size() ? ", " : "");
        }
        std::cout << "\nLista de Pontes (u - v):\n";
        for (const auto& b : report.bridges) {
            std::cout << "  Link: " << b.u << " <---> " << b.v << "\n";
        }
    }

    std::cout << "----------------------------------------------------\n";
    std::cout << "Tempo de Execucao (Tarjan O(V+E)): "
              << report.executionTimeMs << " ms\n";
    std::cout << "====================================================\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_grafo>\n";
        return 1;
    }

    std::string filepath = argv[1];
    Graph network;

    if (!Parser::loadFromFile(filepath, network)) {
        std::cerr << "Falha ao carregar o grafo do arquivo especificado.\n";
        return 1;
    }

    TarjanSPOF detector;
    AuditReport report = detector.run(network);

    printHeader(report);
    printVulnerabilities(report);

    return 0;
}
