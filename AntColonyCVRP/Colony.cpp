#include "Colony.h"
#include <chrono>
#include <vector>
#include <iostream>


Colony::Colony(double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas, const std::string& caminho) 
	: Grafo(caminho), evaporationRate(evaporationRate), alpha(alpha), beta(beta), numFormigas(numFormigas), tempoExec(tempoExec) {
	formigas.resize(numFormigas, Ant(capacidade));
	probabilidades.resize(numCidades, std::vector<double>(numCidades, -1));
	feromonios.resize(numCidades, std::vector<double>(numCidades, 0.001));
	AtualizaProbabilidades();
}

void Colony::AtualizaFeromonio() {
	// Etapa 1: Evaporação
	for (size_t i = 0; i < feromonios.size(); ++i) {
		for (size_t j = 0; j < feromonios[i].size(); ++j) {
			feromonios[i][j] *= (1.0 - evaporationRate);
		}
	}
	// Etapa 2: Depósito de feromônio pelas formigas
	for (Ant& ant : formigas) {
		int custo = ant.getCusto();
		if (custo <= 0) continue;  // Evita divisão por zero ou valores inválidos
		double deltaFeromonio = 1.0 / custo;  // Contribuição da formiga para cada aresta usada

		// Obtém a solução da formiga (supondo que seja um vetor de tours)
		std::vector<int> sol = ant.getSolucao();
		for (size_t k = 0; k < sol.size() - 1; ++k) {
			// Se os IDs das cidades começam em 1, converta para índice (0-based)
			int cidade1 = sol[k];
			int cidade2 = sol[k + 1];
			feromonios[cidade1][cidade2] += deltaFeromonio;
			feromonios[cidade2][cidade1] += deltaFeromonio;  // Se a matriz é simétrica
		}
	}
}

void Colony::AtualizaProbabilidades() {
	// Número de cidades (assumindo que 'cidades' está corretamente preenchido)
	int n = cidades.size();

	// Para cada cidade i, calcule a soma do termo para todas as cidades j (exceto i)
	for (int i = 0; i < n; i++) {
		double denominator = 0.0;
		for (int j = 0; j < n; j++) {
			if (i == j)
				continue;
			//Feromônio na aresta (i,j)
			double tau = feromonios[i][j];
			// Informacao heurística: 1/distância, se a distância for maior que 0
			double eta = (distancias[i][j] > 0 ? 1.0 / distancias[i][j] : 0.0);
			// Usando os parâmetros: 
			// - 'beta' para o feromônio 
			// - 'alpha' para a heurística 
			double term = std::pow(tau, beta) * std::pow(eta, alpha);
			denominator += term;
		}
		// Calcula a probabilidade de ir de i para cada j
		for (int j = 0; j < n; j++) {
			if (i == j) {
				probabilidades[i][j] = 0;
			}
			else {
				double tau = feromonios[i][j];
				double eta = (distancias[i][j] > 0 ? 1.0 / distancias[i][j] : 0.0);
				double term = std::pow(tau, beta) * std::pow(eta, alpha);
				probabilidades[i][j] = (denominator > 0 ? term / denominator : 0.0);
			}
		}
	}
}

std::tuple<std::vector<int>, int, float, std::vector<int>> Colony::CriaSolucoes() {
	auto start = std::chrono::high_resolution_clock::now();
	int bestCusto = INT_MAX;
	std::vector<int> bestSolu;
	std::vector<int> localCosts;
	auto bestTime = std::chrono::high_resolution_clock::now();
	while (true) {
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

		if (elapsed >= tempoExec) { // Sai após tempoExec segundos
			break;
		}

		int bestCustoLocal = INT_MAX;
		std::vector<int> bestSoluLocal;
		for (int i = 0; i < numFormigas; i++) {
			formigas[i].CriaSolucao(this->cidades, this->distancias, this->probabilidades);
			if (bestCustoLocal > formigas[i].getCusto()) {
				bestCustoLocal = formigas[i].getCusto();
				bestSoluLocal = formigas[i].getSolucao();
				//formigas[i].ImprimeRotas();
			}
		}
		localCosts.push_back(bestCustoLocal);
		if (bestCusto > bestCustoLocal) {
			bestCusto = bestCustoLocal;
			bestSolu = bestSoluLocal;
			bestTime = std::chrono::high_resolution_clock::now();
		}
		AtualizaFeromonio();
		AtualizaProbabilidades();
	}
	std::cout << bestCusto << "tempo" << std::chrono::duration<float>(bestTime - start).count();
	return std::make_tuple(bestSolu, bestCusto, std::chrono::duration<float>(bestTime - start).count(), localCosts);
}