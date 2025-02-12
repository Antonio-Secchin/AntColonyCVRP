#include "Grafo.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

Grafo::Grafo(int numCidades, int capacity) {
    cidades.reserve(numCidades);  // Opcional: otimiza aloca��o
    distancias.resize(numCidades, std::vector<double>(numCidades, -1));  // -1 indica sem conex�o
    capacidade = capacity;
}

Grafo::Grafo(const std::string& caminho) {
    this->capacidade = 0;
    std::string linha;
    std::ifstream arquivo(caminho);
    int numCidades = 0, capacity = 0;
    bool lendoCoordenadas = false, lendoDemanda = false;
    while (std::getline(arquivo, linha)) {
        std::istringstream stream(linha);
        std::string palavra;
        stream >> palavra;

        if (palavra == "DIMENSION") {
            std::string trash;
            stream >> trash;
            stream >> numCidades;
        }
        else if (palavra == "CAPACITY") {
            std::string trash;
            stream >> trash;
            stream >> capacity;  // Lendo a capacidade
            capacidade = capacity;
        }
        else if (palavra == "NODE_COORD_SECTION") {
            lendoCoordenadas = true;
        }
        else if (palavra == "DEMAND_SECTION") {
            lendoCoordenadas = false;
            lendoDemanda = true;
        }
        else if (palavra == "DEPOT_SECTION") {
            break;  // Fim dos dados relevantes
        }
        else if (lendoCoordenadas) {
            // Lendo coordenadas das cidades
            Cidade cidade;
            cidade.id = std::stoi(palavra);
            stream >> cidade.posX >> cidade.posY;
            cidades.push_back(cidade);
        }
        else if (lendoDemanda) {
            // Lendo demandas das cidades
            int id, demanda;
            id = std::stoi(palavra);
            stream >> demanda;
            cidades[id - 1].demand = demanda;  // Ajuste para vetor base 0
        }
    }

    distancias.resize(numCidades, std::vector<double>(numCidades, -1));
    
    std::cout << numCidades;
    for (int i = 0; i < numCidades; i++)
        for (int j = i; j < numCidades; j++) {
            distancias[i][j] = CalculaDistancia(cidades[i], cidades[j]);
            distancias[j][i] = distancias[i][j];
        }

}

Cidade Grafo::getCidade(int vertice) const {
    return cidades[vertice];
}

int Grafo::getDistancia(int vertice1, int vertice2) const {
    return distancias[vertice1][vertice2];
}

std::string Grafo::toString() const {
    std::ostringstream oss;
    oss << "Capacidade: " << capacidade << "\n";
    oss << "Cidades:\n";

    for (const auto& cidade : cidades) {
        oss << "ID: " << cidade.id
            << " | X: " << cidade.posX
            << " | Y: " << cidade.posY
            << " | Demanda: " << cidade.demand << "\n";
    }

    oss << "\nMatriz de Distancias:\n";
    for (size_t i = 0; i < distancias.size(); ++i) {
        for (size_t j = 0; j < distancias[i].size(); ++j) {
            oss << distancias[i][j] << " ";
        }
        oss << "\n";
    }

    return oss.str();
}