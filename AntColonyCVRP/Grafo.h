#pragma once
#include "Cidade.h"
#include <vector>
#include <string>
#include <vector>

class Grafo
{
protected:
	std::vector<Cidade> cidades;  // Lista de cidades
	std::vector<std::vector<int>> distancias;
	int numCidades;
	int capacidade;

public:

	Grafo(int numCidades, int capacidade);

	Grafo(const std::string& conteudo);

	Cidade getCidade(int vertice) const;

	std::vector<Cidade> getCidades();

	int getDistancia(int vertice1, int vertice2) const;

	std::string toString() const;
};

