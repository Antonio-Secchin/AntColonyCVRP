#include "Ant.h"
#include <list>
#include <iostream>

Ant::Ant(int capacity) {
	capacidade = capacity;
	capacidadeAtual = capacity;
	custo = 0;
	cidadesVisitadas = 0;
}


int Ant::getCusto() {
	return custo;
}

std::vector<int> Ant::getSolucao() {
	return solucao;
}

void Ant::CriaSolucao(std::vector<Cidade> cidades, const std::vector<std::vector<int>>& distancias, 
	const std::vector<std::vector<double>>& probabilidades) {
	
	capacidadeAtual = capacidade;
	custo = 0;
	cidadesVisitadas = 0;
	std::vector<bool> visitadas(cidades.size(), false);
	int cidadeAnt = 0;
	solucao = std::vector<int>();  // Recria o vetor zerado
	int cidadesTotais = cidades.size();
	solucao.push_back(0);
	//Visitando o 0
	cidadesVisitadas++;
	while (cidadesVisitadas < cidadesTotais) {
		int cidadeProx = EscolheProxVertice(visitadas, cidades, probabilidades[cidadeAnt]);
		visitadas[cidadeProx] = true;
		custo += distancias[cidadeAnt][cidadeProx];
		capacidadeAtual -= cidades[cidadeProx].getDemand();
		solucao.push_back(cidadeProx);
		cidadeAnt = cidadeProx;
		//std::cout << "Cidades Visitadas:" << cidadesVisitadas << "\n";
		//test++;
	}
	custo += distancias[cidadeAnt][0];
	solucao.push_back(0);
	Swap(cidades, distancias);
}

int Ant::EscolheProxVertice(const std::vector<bool>& visitados, const std::vector<Cidade>& cidades, 
	const std::vector<double>& probabilidades) {
	double soma = 0.0;
	//Ignora o 0 porque eh o armazem
	for (size_t i = 1; i < probabilidades.size(); i++) {
		if (!visitados[i]) {
			soma += probabilidades[i];
		}
	}

	double randomValue = ((double)rand() / RAND_MAX) * soma;
	double acumulado = 0.0;

	for (size_t i = 1; i < probabilidades.size(); i++) {
		if (!visitados[i]) {
			acumulado += probabilidades[i];
			if (acumulado >= randomValue && capacidadeAtual >= cidades[i].getDemand()) {
				//std::cout << "cidadeEscolhida:" << i << "\n";
				cidadesVisitadas++;
				return i;  // Retorna a cidade escolhida
			}
		}
	}
	capacidadeAtual = capacidade;
	return 0; // Retorna zero pois nao ha cidade que consegue abastecer
}

void Ant::Two_Opt(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias) {

}

void Ant::Swap(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias) {
	int n = solucao.size();
	int bestDelta = 0;
	int bestI = -1, bestJ = -1;

	for (int i = 1; i < n - 1; i++) {
		if (i < (n - 1) && solucao[i + 1] == 0) {
			if (bestI != -1) {
				std::swap(solucao[bestI], solucao[bestJ]);
			}
			bestI = -1;
			bestJ = -1;
			bestDelta = 0;
			// Evitar ultrapassar os limites do vetor
			if (i + 2 < n - 1) {
				i += 2;
			}
			else {
				break;
			}
		}
		for (int j = i + 1; j < n - 1; j++) {
			if (j < (n- 1) && solucao[j + 1] == 0) {
				break;
			}
			int delta = (distancias[solucao[i - 1]][solucao[j]] + distancias[solucao[i]][solucao[j + 1]]) -
				(distancias[solucao[i - 1]][solucao[i]] + distancias[solucao[j]][solucao[j + 1]]);

			if (delta < bestDelta) {
				bestDelta = delta;
				bestI = i;
				bestJ = j;
			}
		}
	}

	if (bestI != -1) {
		std::swap(solucao[bestI], solucao[bestJ]);
	}
}