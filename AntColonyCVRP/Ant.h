#pragma once
#include <vector>
#include "Grafo.h"
class Ant
{
	int custo;
	int capacidade;
	int capacidadeAtual;
	int cidadesVisitadas;
	std::vector<int> solucao;


public:
	Ant(int capacity);

	int getCusto();

	std::vector<int> getSolucao();

	void CriaSolucao(std::vector<Cidade> cidades, const std::vector<std::vector<int>>& distancias, 
		const std::vector<std::vector<double>>& probabilidades);

	int EscolheProxVertice(const std::vector<bool>& visitados, const std::vector<Cidade>& cidades,
		const std::vector<double>& probabilidades);

	void Two_Opt(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias);

	void Swap(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias);

	void ImprimeRotas();
};

