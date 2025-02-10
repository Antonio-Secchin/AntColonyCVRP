#pragma once
#include "Cidade.h"
#include <vector>
#include <string>

class Grafo
{
	std::vector<Cidade> cidades;  // Lista de cidades
	std::vector<std::vector<double>> distancias;
	int capacidade;

public:

	Grafo(int numCidades, int capacidade);

	Grafo(const std::string& caminho);

	Cidade getCidade(int vertice) const;

	double getDistancia(int vertice1, int vertice2) const;

	std::string toString() const;
};

