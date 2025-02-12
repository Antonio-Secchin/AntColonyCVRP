#pragma once
#include <vector>
#include "Grafo.h"
class Ant
{
	int custo;
	int capacidade;
	std::vector<std::vector<int>> solucao;


public:
	void CriaSolucao(const Grafo& grafo);


};

