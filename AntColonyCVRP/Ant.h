#pragma once
#include <vector>
#include "Grafo.h"
class Ant
{
	int custo;
	int capacidade;
	int capacidadeAtual;
	int cidadesVisitadas;
	int iteracao;
	std::vector<int> solucao;


public:
	Ant(int capacity);

	int GetCusto();

	std::vector<int> GetSolucao();

	void CalculaCusto(const std::vector<std::vector<int>>& distancias);

	void CriaSolucao(std::vector<Cidade> cidades, const std::vector<std::vector<int>>& distancias, 
		std::vector<std::vector<double>>& probabilidades);

	int EscolheProxVertice(const std::vector<bool>& visitados, const std::vector<Cidade>& cidades,
		const std::vector<double>& probabilidades, const std::vector<int>& distancias);

	void Two_Opt(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias);

	void Swap(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias);

	void ImprimeRotas();

	std::vector<std::vector<int>> SplitRotas();

	void Shift(std::vector<std::vector<int>>& rotas, const std::vector<Cidade>& cidades);

	void ReconstruirSolucao(const std::vector<std::vector<int>>& rotas);

	bool RotaViavel(std::vector<int>& rotas, const std::vector<Cidade>& cidades);
};

