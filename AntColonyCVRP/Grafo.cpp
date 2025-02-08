#include "Grafo.h"
#include <cmath>

Grafo::Grafo(int numCidades, int capacity) {
    cidades.reserve(numCidades);  // Opcional: otimiza aloca��o
    distancias.resize(numCidades, std::vector<float>(numCidades, -1));  // -1 indica sem conex�o
    capacidade = capacity;
}

Cidade Grafo::getCidade(int vertice) const {
    return cidades[vertice];
}

float Grafo::getDistancia(int vertice1, int vertice2) const {
    return distancias[vertice1][vertice2];
}