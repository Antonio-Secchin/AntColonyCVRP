#include "Colony.h"
#include <chrono>
#include <vector>


Colony::Colony(double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas, const std::string& caminho) 
	: Grafo(caminho), evaporationRate(evaporationRate), alpha(alpha), beta(beta), numFormigas(numFormigas), tempoExec(tempoExec) {
	formigas.resize(numFormigas);
	probabilidades.resize(numCidades, std::vector<double>(numCidades, -1));
}

void Colony::AtualizaFeromonio() {
	// Etapa 1: Evapora��o
	for (size_t i = 0; i < feromonios.size(); ++i) {
		for (size_t j = 0; j < feromonios[i].size(); ++j) {
			feromonios[i][j] *= (1.0 - evaporationRate);
		}
	}
	// Etapa 2: Dep�sito de ferom�nio pelas formigas
	for (Ant& ant : formigas) {
		int custo = ant.getCusto();
		if (custo <= 0) continue;  // Evita divis�o por zero ou valores inv�lidos
		double deltaFeromonio = 1.0 / custo;  // Contribui��o da formiga para cada aresta usada

		// Obt�m a solu��o da formiga (supondo que seja um vetor de tours)
		std::vector<std::vector<int>> sol = ant.getSolucao();
		for (const auto& tour : sol) {
			// Para cada aresta na rota
			for (size_t k = 0; k < tour.size() - 1; ++k) {
				// Se os IDs das cidades come�am em 1, converta para �ndice (0-based)
				int cidade1 = tour[k] - 1;
				int cidade2 = tour[k + 1] - 1;
				feromonios[cidade1][cidade2] += deltaFeromonio;
				feromonios[cidade2][cidade1] += deltaFeromonio;  // Se a matriz � sim�trica
			}
		}
	}
}

void Colony::AtualizaProbabilidades() {
	// N�mero de cidades (assumindo que 'cidades' est� corretamente preenchido)
	int n = cidades.size();

	// Ajusta a matriz de probabilidades para ter o mesmo tamanho
	probabilidades.resize(n, std::vector<double>(n, 0.0));

	// Para cada cidade i, calcule a soma do termo para todas as cidades j (exceto i)
	for (int i = 0; i < n; i++) {
		double denominator = 0.0;
		for (int j = 0; j < n; j++) {
			if (i == j)
				continue;
			// Ferom�nio na aresta (i,j)
			double tau = feromonios[i][j];
			// Informacao heur�stica: 1/dist�ncia, se a dist�ncia for maior que 0
			double eta = (distancias[i][j] > 0 ? 1.0 / distancias[i][j] : 0.0);
			// Usando os par�metros: 
			// - 'beta' para o ferom�nio 
			// - 'alpha' para a heur�stica 
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

std::vector<std::vector<int>> Colony::CriaSolucoes(int time_max) {
	auto start = std::chrono::high_resolution_clock::now();
	int bestCusto = 0;
	std::vector<std::vector<int>> bestSolu;
	auto bestTime = std::chrono::high_resolution_clock::now();
	while (true) {
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

		if (elapsed >= tempoExec) { // Sai ap�s tempoExec segundos
			break;
		}

		int bestCustoLocal = 0;
		std::vector<std::vector<int>> bestSoluLocal;
		for (int i = 0; i < numFormigas; i++) {
			//formigas[i].CriaSolucao(this);
			if (i = 0 || bestCustoLocal > formigas[i].getCusto()) {
				bestCustoLocal = formigas[i].getCusto();
				bestSoluLocal = formigas[i].getSolucao();
			}
		}
		if (bestCusto > bestCustoLocal) {
			bestCusto = bestCustoLocal;
			bestSolu = bestSoluLocal;
			bestTime = std::chrono::high_resolution_clock::now();
		}
		AtualizaFeromonio();
		AtualizaProbabilidades();
	}
}