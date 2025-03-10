#include "Ant.h"
#include <list>
#include <iostream>
#include <cmath>

//Construtor da classe Ant
Ant::Ant(int capacity) {
	capacidade = capacity;
	capacidadeAtual = capacity;
	custo = 0;
	cidadesVisitadas = 0;
	iteracao = 0;
}

//Retorna o custo da solução
int Ant::GetCusto() {
	return custo;
}

//Retorna a solução
std::vector<int> Ant::GetSolucao() {
	return solucao;
}

//Calcula o custo da solução
void Ant::CalculaCusto(const std::vector<std::vector<int>>& distancias) {
	custo = 0;
	for (size_t i = 0; i < solucao.size() - 1; i++) {
		custo += distancias[solucao[i]][solucao[i + 1]];
	}
}

//Cria a solução de cada formiga, colocando a solução e o custo dentro dos parametros da classe
void Ant::CriaSolucao(std::vector<Cidade> cidades, const std::vector<std::vector<int>>& distancias,
	std::vector<std::vector<double>>& probabilidades) {
	//Reinicia os parametros para uma nova solução
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
	iteracao++;
	//Fica em loop enquanto não visitar todas as cidades
	while (cidadesVisitadas < cidadesTotais) {
		int cidadeProx = EscolheProxVertice(visitadas, cidades, probabilidades[cidadeAnt], distancias[cidadeAnt]);
		visitadas[cidadeProx] = true;
		//Diminui a capacidade e coloca a proxima cidade na solução
		capacidadeAtual -= cidades[cidadeProx].getDemand();
		solucao.push_back(cidadeProx);
		cidadeAnt = cidadeProx;
	}
	//Adiciona o depósito no fim
	solucao.push_back(0);

	//Faz as operações de vizinhança na mesma rota
	Swap(cidades, distancias);
	Two_Opt(cidades, distancias);

	//Busca local entre rotas
	if (((double)rand() / RAND_MAX) <= 20) {
		std::vector<std::vector<int>> rotas = SplitRotas();
		Shift(rotas, cidades);
		ReconstruirSolucao(rotas);
	}
	CalculaCusto(distancias);
}

//Função que escolhe o próximo vertice
int Ant::EscolheProxVertice(const std::vector<bool>& visitados, const std::vector<Cidade>& cidades,
	const std::vector<double>& probabilidades, const std::vector<int>& distancias) {
	double soma = 0.0;

	//Probabilidade de escolher uma cidade apenas heuristicamente
	if (((double)rand() / RAND_MAX) < (0.8 * exp(-iteracao / 100.0))){
		int best_city_dist = INT_MAX;
		int best_city = 0;
		for (size_t i = 1; i < cidades.size(); i++) {
			if (!visitados[i] && distancias[i] != 0 && capacidadeAtual >= cidades[i].getDemand() && best_city_dist > distancias[i]) {
				best_city_dist = distancias[i];
				best_city = i;
			}
		}
		if (best_city != 0) {
			cidadesVisitadas++;
		}
		//Se for o depósito não há mais cidade para ir
		else {
			capacidadeAtual = capacidade;
		}
		return best_city;
	}
	else {
		//Ignora o 0 porque eh o armazem
		//Calcula a soma das probabilidades
		for (size_t i = 1; i < probabilidades.size(); i++) {
			if (!visitados[i]) {
				soma += probabilidades[i];
			}
		}

		double randomValue = ((double)rand() / RAND_MAX) * soma;
		double acumulado = 0.0;
		
		//Verifica qual cidade foi escolhida
		for (size_t i = 1; i < probabilidades.size(); i++) {
			if (!visitados[i]) {
				acumulado += probabilidades[i];
				if (acumulado >= randomValue && capacidadeAtual >= cidades[i].getDemand()) {
					cidadesVisitadas++;
					return i;  // Retorna a cidade escolhida
				}
			}
		}
		//Se chegar aqui não há mais cidade para ir logo vai pro depósito
		capacidadeAtual = capacidade;
		return 0; // Retorna zero pois nao ha cidade que consegue abastecer
	}
}

//Realiza a operação de two-opt
void Ant::Two_Opt(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias) {
	int n = solucao.size();
	bool improved = true;
	//Fica em loop até não haver melhoria em todos os caminhos
	while (improved) {
		improved = false;

		for (int i = 1; i < n - 1; i++) {
			if (solucao[i + 1] == 0 || solucao[i] == 0) {
				continue;
			}
			for (int j = i + 1; j < n - 1; j++) {
				if (solucao[j + 1] == 0) {
					break;
				}
				int delta = (distancias[solucao[i - 1]][solucao[j]] + distancias[solucao[i]][solucao[j + 1]]) -
					(distancias[solucao[i - 1]][solucao[i]] + distancias[solucao[j]][solucao[j + 1]]);

				if (delta < 0) {
					std::reverse(solucao.begin() + i, solucao.begin() + j + 1);
					improved = true;
				}
			}
		}
	}
	//Verifica se a troca foi certa, é apenas para teste no começo do trabalho
	int capacidadeTeste = capacidade;
	for (int i = 1; i < n; i++) {
		if (solucao[i] == 0) {
			capacidadeTeste = capacidade;
		}
		else {
			capacidadeTeste -= cidades[solucao[i]].getDemand();
			if (capacidadeTeste < 0) {
				std::cout << "Erro na capacidade no Two_Opt";
			}
		}
	}
}

//Função que realiza a operação de swap
void Ant::Swap(const std::vector<Cidade>& cidades, const std::vector<std::vector<int>>& distancias) {
	int n = solucao.size();
	int bestDelta = 0;
	int bestI = -1, bestJ = -1;

	//FIca em loop percorrendo todas as rotas e trocando as duas melhores cidades para troca de lugar
	for (int i = 1; i < n - 1; i++) {
		if (solucao[i + 1] == 0 || solucao[i] == 0) {
			if (bestI != -1) {
				std::swap(solucao[bestI], solucao[bestJ]);
			}
			bestI = -1;
			bestJ = -1;
			bestDelta = 0;
			continue;
		}
		for (int j = i + 1; j < n - 1; j++) {
			if (j < (n- 1) && solucao[j + 1] == 0) {
				break;
			}
			int delta = (distancias[solucao[i - 1]][solucao[j]] + distancias[solucao[j]][solucao[i + 1]] +
				distancias[solucao[j - 1]][solucao[i]] + distancias[solucao[i]][solucao[j + 1]]) -
				(distancias[solucao[i - 1]][solucao[i]] + distancias[solucao[i]][solucao[i + 1]] +
					distancias[solucao[j - 1]][solucao[j]] + distancias[solucao[j]][solucao[j + 1]]);

			if (delta < bestDelta) {
				bestDelta = delta;
				bestI = i;
				bestJ = j;
			}
		}
	}
	//Verifica se a troca foi certa, é apenas para teste no começo do trabalho
	int capacidadeTeste = capacidade;
	for (int i = 1; i < n; i++) {
		if (solucao[i] == 0) {
			capacidadeTeste = capacidade;
		}
		else {
			capacidadeTeste -= cidades[solucao[i]].getDemand();
			if (capacidadeTeste < 0) {
				std::cout << "Erro na capacidade no swap";
			}
		}
	}
}

//Função para imprimir a solução
void Ant::ImprimeRotas() {
	std::cout << "Rotas encontradas:" << std::endl;
	std::cout << "Deposito -> ";

	for (size_t i = 1; i < solucao.size(); i++) {
		if (solucao[i] == 0) {
			std::cout << "Deposito" << std::endl;  // Fecha a rota atual
			if (i != solucao.size() - 1) { // Evita imprimir "Depósito ->" no final desnecessariamente
				std::cout << "Deposito -> ";
			}
		}
		else {
			std::cout << solucao[i] << " -> ";
		}
	}

	std::cout << std::endl;
}

//Função para separar a solução em varias rotas
std::vector<std::vector<int>> Ant::SplitRotas() {
	std::vector<std::vector<int>> rotas;
	std::vector<int> rotaAtual;
	for (int cidade : solucao) {
		if (cidade == 0 && !rotaAtual.empty()) {
			rotas.push_back(rotaAtual);
			rotaAtual.clear();
		}
		rotaAtual.push_back(cidade);
	}
	return rotas;
}

//Função que recebe várias rotas e constroi de volta a solução
void Ant::ReconstruirSolucao(const std::vector<std::vector<int>>& rotas) {
	solucao.clear();
	for (const auto& rota : rotas) {
		solucao.insert(solucao.end(), rota.begin(), rota.end());
	}
	solucao.push_back(0);
}

//Função qeu seleciona um cliente(cidade) para shift
std::pair<int, int> SelecionarClienteParaShift(const std::vector<std::vector<int>>& rotas) {
	int idxRotaOrigem = rand() % rotas.size();
	while (rotas[idxRotaOrigem].size() <= 1) { // Evita rotas vazias (apenas depósito)
		idxRotaOrigem = rand() % rotas.size();
	}
	int idxCliente = rand() % (rotas[idxRotaOrigem].size() - 1) + 1; // Ignora o depósito inicial
	return { idxRotaOrigem, idxCliente };
}

//Função que verifica se uma rota é viavel
bool Ant::RotaViavel(std::vector<int> &rotas, const std::vector<Cidade>& cidades) {
	
	if (rotas.front() != 0 || rotas.back() != 0) return false;
	// Verifica capacidade da rota de destino
	int carga = 0;
	for (int cidade : rotas) {
		if (cidade != 0) carga += cidades[cidade].demand;
	}
	if (carga > capacidade) {
		return false;
	}
	return true;
}

//Função que faz o shift
void Ant::Shift(std::vector<std::vector<int>>& rotas, const std::vector<Cidade>& cidades) {
	auto [rotaOrigemIdx, clienteIdx] = SelecionarClienteParaShift(rotas);
	int cliente = rotas[rotaOrigemIdx][clienteIdx];

	// Remove o cliente da rota de origem
	rotas[rotaOrigemIdx].erase(rotas[rotaOrigemIdx].begin() + clienteIdx);

	// Escolhe uma rota de destino (pode ser a mesma ou outra)
	int rotaDestinoIdx = rand() % rotas.size();

	// Insere o cliente na rota de destino (posição aleatória)
	int posDestino = rand() % (rotas[rotaDestinoIdx].size() + 1);
	rotas[rotaDestinoIdx].insert(rotas[rotaDestinoIdx].begin() + posDestino, cliente);

	bool viavel = RotaViavel(rotas[rotaDestinoIdx], cidades);

	if (!viavel) {
		// Reverte a operação se inviável
		rotas[rotaDestinoIdx].erase(rotas[rotaDestinoIdx].begin() + posDestino);
		rotas[rotaOrigemIdx].insert(rotas[rotaOrigemIdx].begin() + clienteIdx, cliente);
	}
}