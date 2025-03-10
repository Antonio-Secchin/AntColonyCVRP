#include "Grafo.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

//Construtor do grafo sem o arquivo
Grafo::Grafo(int numCidades, int capacity) {
    cidades.reserve(numCidades);  // Opcional: otimiza alocação
    distancias.resize(numCidades, std::vector<int>(numCidades, -1));  // -1 indica sem conexão
    capacidade = capacity;
    this->numCidades = numCidades;
}

//Construtor do grafo com a string do arquivo
Grafo::Grafo(const std::string& conteudo) {
    this->capacidade = 0;
    std::string linha;
    std::istringstream arquivo(conteudo);
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

    this->numCidades = numCidades;
    distancias.resize(numCidades, std::vector<int>(numCidades, -1));
    
    for (int i = 0; i < numCidades; i++)
        for (int j = i; j < numCidades; j++) {
            distancias[i][j] = CalculaDistancia(cidades[i], cidades[j]);
            distancias[j][i] = distancias[i][j];
        }

}

//Retorna a cidade do grafo no vertice dado
Cidade Grafo::getCidade(int vertice) const {
    return cidades[vertice];
}

//Retorna todas as cidades
std::vector<Cidade> Grafo::getCidades() {
    return cidades;
}

//Retorna a distancia entre duas cidades
int Grafo::getDistancia(int vertice1, int vertice2) const {
    return distancias[vertice1][vertice2];
}

//Imprime o grafo
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