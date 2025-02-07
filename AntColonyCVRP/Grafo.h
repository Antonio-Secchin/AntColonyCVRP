#pragma once
#include "Cidade.h"
#include <vector>
class Grafo
{
	std::vector<Cidade> cidades;  // Lista de cidades
	std::vector<std::vector<float>> distancias;

	Grafo(int numCidades);

	Cidade getCidade(int vertice) const;

	float getDistancia(int vertice1, int vertice2) const;
};

