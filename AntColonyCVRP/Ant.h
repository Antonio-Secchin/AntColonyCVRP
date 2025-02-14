#pragma once
#include <vector>
#include "Grafo.h"
class Ant
{
	int custo;
	int capacidade;
	std::vector<std::vector<int>> solucao;


public:
	int getCusto();

	std::vector<std::vector<int>> getSolucao();

	void CriaSolucao(const Grafo& grafo);


};

