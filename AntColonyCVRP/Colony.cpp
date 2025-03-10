#include "Colony.h"
#include <chrono>
#include <vector>
#include <iostream>

//Inicializador da classe colony que recebe os parametros que foi passado como argumento e a string para a construção do grafo
Colony::Colony(double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas, const std::string& conteudo) 
	: Grafo(conteudo), evaporationRate(evaporationRate), alpha(alpha), beta(beta), numFormigas(numFormigas), tempoExec(tempoExec) {
	formigas.resize(numFormigas, Ant(capacidade));
	probabilidades.resize(numCidades, std::vector<double>(numCidades, -1));
	feromonios.resize(numCidades, std::vector<double>(numCidades));
	for (int i = 0; i < numCidades; i++) {
		for (int j = 0; j < numCidades; j++) {
			feromonios[i][j] = 1000.0 / (distancias[i][j] + 1e-6); // Evita divisão por zero
		}
	}
	AtualizaProbabilidades();
}

//Função para atualizar os feromonios presentes nas arestas do grafo
void Colony::AtualizaFeromonio(std::vector<int>& bestSolu, int& bestCusto) {
	// Etapa 1: Evaporação
	for (size_t i = 0; i < feromonios.size(); ++i) {
		for (size_t j = 0; j < feromonios[i].size(); ++j) {
			feromonios[i][j] *= (1.0 - evaporationRate);
		}
	}
	// Etapa 2: Depósito de feromônio pelas formigas
	for (Ant& ant : formigas) {
		int custo = ant.GetCusto();
		if (custo <= 0) continue;  // Evita divisão por zero ou valores inválidos
		double deltaFeromonio = 1000 / custo;  // Contribuição da formiga para cada aresta usada

		// Obtém a solução da formiga (supondo que seja um vetor de tours)
		std::vector<int> sol = ant.GetSolucao();
		for (size_t k = 0; k < sol.size() - 1; ++k) {
			int cidade1 = sol[k];
			int cidade2 = sol[k + 1];
			feromonios[cidade1][cidade2] += deltaFeromonio;
			feromonios[cidade2][cidade1] += deltaFeromonio;  // Se a matriz é simétrica
		}
	}
	double deltaFeromonio = 6000 / bestCusto;
	for (size_t k = 0; k < bestSolu.size() - 1; k++) {
		int cidade1 = bestSolu[k];
		int cidade2 = bestSolu[k + 1];
		feromonios[cidade1][cidade2] += deltaFeromonio;
		feromonios[cidade2][cidade1] += deltaFeromonio;
	}
	for (size_t i = 0; i < feromonios.size(); ++i) {
		for (size_t j = 0; j < feromonios[i].size(); ++j) {
			feromonios[i][j] = std::max(1e-5, std::min(feromonios[i][j], 100.0));
		}
	}
}

//Função para atualizar a matriz de probabilidades de ir para cada cidade
void Colony::AtualizaProbabilidades() {
	int n = cidades.size();

	// Para cada cidade i, calcule o denominador para todas as cidades j (exceto i)
	for (int i = 0; i < n; i++) {
		double denominator = 0.0;
		for (int j = 0; j < n; j++) {
			if (i == j)
				continue;
			//Feromônio na aresta (i,j)
			double tau = feromonios[i][j];
			// Informacao heurística: 1000/distância, se a distância for maior que 0
			double eta = (distancias[i][j] > 0 ? 1000.0 / distancias[i][j] : 0.0);
			// Usando os parâmetros: 
			// - 'alpha' para o feromônio 
			// - 'beta' para a heurística 
			double term = std::pow(tau, alpha) * std::pow(eta, beta);
			denominator += term;
		}
		// Calcula a probabilidade de ir de i para cada j
		for (int j = 0; j < n; j++) {
			// Se for a propria cidade probabilidade zero
			if (i == j) {
				probabilidades[i][j] = 0;
			}
			// Mesma formula usada para o denominador
			else {
				double tau = feromonios[i][j];
				double eta = (distancias[i][j] > 0 ? 1000.0 / distancias[i][j] : 0.0);
				double term = std::pow(tau, alpha) * std::pow(eta, beta);
				probabilidades[i][j] = (denominator > 0 ? term / denominator : 0.0);
			}
		}
	}
}

//Esta função executa o algoritmo em si, criando soluções para cada formiga, verificando a melhor e chama as funções para atualizar as probabilidades e feromonios
std::tuple<std::vector<int>, int, float, std::vector<int>> Colony::CriaSolucoes() {
	//Inicializa algumas variaveis, como tempo, melhor valor, entre outras
	auto start = std::chrono::high_resolution_clock::now();
	int bestCusto = INT_MAX;
	int iteracaoSemMelhoria = 0;
	std::vector<int> bestSolu;
	std::vector<int> localCustos;
	auto bestTime = std::chrono::high_resolution_clock::now();
	double evaporationRateOri = evaporationRate;
	//Loop principal que para quando o tempo limitar passar
	while (true) {
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
		
		//Verifica se passou o tempo
		if (elapsed >= tempoExec) { // Sai após tempoExec segundos
			break;
		}

		int bestCustoLocal = INT_MAX;
		std::vector<int> bestSoluLocal;
		//Cria a solução para cada formiga
		for (int i = 0; i < numFormigas; i++) {
			formigas[i].CriaSolucao(this->cidades, this->distancias, this->probabilidades);
			if (bestCustoLocal > formigas[i].GetCusto()) {
				bestCustoLocal = formigas[i].GetCusto();
				bestSoluLocal = formigas[i].GetSolucao();
				//formigas[i].ImprimeRotas();
			}
		}
		localCustos.push_back(bestCustoLocal);
		iteracaoSemMelhoria++;
		//Verifica se é melhor que a melhor solução até agora
		if (bestCusto > bestCustoLocal) {
			bestCusto = bestCustoLocal;
			bestSolu = bestSoluLocal;
			bestTime = std::chrono::high_resolution_clock::now();
			iteracaoSemMelhoria = 0;
		}
		// Muda o valor de alguns parametros para ver se há alguma melhoria
		if (iteracaoSemMelhoria > 500) {
			alpha *= 0.9;
			evaporationRate *= 1.1;
			iteracaoSemMelhoria = 0;
			if (evaporationRate > 1)
				evaporationRate = evaporationRateOri;
		}
		//Atuailiza os feromonios e as probabilidades
		AtualizaFeromonio(bestSolu, bestCusto);
		AtualizaProbabilidades();
	}
	//Retorna o melhor custo a solução e o tempo que a achou e todos os melhores custos locais da execução
	return std::make_tuple(bestSolu, bestCusto, std::chrono::duration<float>(bestTime - start).count(), localCustos);
}