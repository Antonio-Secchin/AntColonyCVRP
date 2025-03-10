// AntColonyCVRP.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "Colony.h"
import Arquivo;

namespace fs = std::filesystem;

std::vector<double> alphas = {0.5, 1.5, 2.0};
std::vector<double> betas = { 1.0, 2.0,8.0};
std::vector<double> evaporationRates = { 0.2, 0.5, 0.9};
std::vector<int> seeds = { 11,10,2001,23,32 };

//Função que inicializa o algoritmo, lendo as entradas, criando a colônia e executando os testes dos parâmetros
void InicializaTestePar(const std::string& caminho, int tempoExec, int numFormigas, std::string outDir) {
    //Se for um diretório roda todos os arquivos nele
    if (fs::is_directory(caminho)) {
        std::vector<Parametros> valores;
        std::vector<std::string> arquivos = LerArquivosNoDiretorio(caminho);
        for (std::string arquivo : arquivos) {
            std::string instancia = ExtrairNomeInstancia(arquivo);
            std::string conteudo = LerArquivo(arquivo);
            for (double alpha : alphas)
                for (double beta : betas)
                    for (double evaporationRate : evaporationRates) {
                        Colony colonia(evaporationRate, alpha, beta, tempoExec, numFormigas, conteudo);
                        std::tuple results = colonia.CriaSolucoes();
                        // Armazenar os resultados para depois escrever a tabela
                        Parametros p;
                        p.instancia = instancia;
                        p.alpha = alpha;
                        p.beta = beta;
                        p.evaporationRate = evaporationRate;
                        p.resultado = std::get<1>(results);
                        valores.push_back(p);
                    }
        }
        GerarTabelaLatex(valores, outDir);
    }
    else {
        std::cerr << "Erro: Caminho invalido -> " << caminho << std::endl;
        return;
    }
}

//Função que inicializa o algoritmo, lendo as entradas, criando a colônia e executando o algoritmo 5 vezes
void Inicializa(const std::string& caminho, double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas, std::string outDir) {
    if (fs::is_directory(caminho)) {
        std::vector<Resultado> valores;
        std::vector<std::string> arquivos = LerArquivosNoDiretorio(caminho);
        for (std::string arquivo : arquivos) {
            std::string conteudo = LerArquivo(arquivo);
            std::string instancia = ExtrairNomeInstancia(arquivo);
            Resultado res;
            res.instancia = instancia;
            res.fsol_otima = ExtrairValorOtimo(conteudo);
            res.custo_min = INT_MAX;
            int sumCusto = 0;
            float sumTime = 0.0;
            std::vector<int> bestSolu;
            std::vector<int> localCosts;
            std::vector<Cidade> aux;

            for (int seed : seeds) {
                std::srand(seed);
                Colony colonia(evaporationRate, alpha, beta, tempoExec, numFormigas, conteudo);
                std::tuple results = colonia.CriaSolucoes();
                //Armazena os resultados para depois escrever na tabela
                if (res.custo_min > std::get<1>(results)) {
                    res.custo_min = std::get<1>(results);
                    res.tempo_min = std::get<2>(results);
                    localCosts = std::get<3>(results);
                    bestSolu = std::get<0>(results);
                    aux = colonia.getCidades();
                }
                sumCusto += std::get<1>(results);
                sumTime += std::get<2>(results);
            }

            res.custo_media = sumCusto * 1.0 / seeds.size();
            res.tempo_media = sumTime / seeds.size();
            res.gap_min = (res.custo_min - res.fsol_otima) * 100.0 / res.fsol_otima;
            res.gap_media = (res.custo_media - res.fsol_otima) * 100.0 / res.fsol_otima;
            valores.push_back(res);
            //Faz o gráfico de custos e do caminho da solução
            EscreverArqGraficoCustos(localCosts, res.fsol_otima, arquivo, outDir);
            EscreverArqGraficoSolucao(bestSolu, aux, arquivo, outDir);
        }
        GerarTabelaResultadosLatex(valores, outDir);
    }
    //Executa o código se for um arquivo
    else if (fs::is_regular_file(caminho)) {
        std::string conteudo = LerArquivo(caminho);
        int bestSol = ExtrairValorOtimo(conteudo);
        Colony colonia(evaporationRate, alpha, beta, tempoExec, numFormigas, conteudo);
        std::tuple results = colonia.CriaSolucoes();
        std::cout << "tempo: " << std::get<2>(results) << " melhor custo  " << std::get<1>(results);

        EscreverArqGraficoCustos(std::get<3>(results), bestSol, caminho, outDir);
        EscreverArqGraficoSolucao(std::get<0>(results), colonia.getCidades(), caminho, outDir);
    }
    else {
        std::cerr << "Erro: Caminho invalido -> " << caminho << std::endl;
        return;
    }
}

// Recebe como argumento o caminho do arquivo, o valor da taxa de evaporação, alpha, beta, tempo de execução, numero de formigas, diretório de saída e a operação
int main(int argc, char* argv[]) {
    //Verifica se tem todos os argumentos
    if (argc < 9) {
        std::cerr << "Uso: " << argv[0] << " <diretorio ou arquivo>" << std::endl;
        return 1;
    }
    
    // evaporationRate, double alpha, double beta, int tempoExec, int numFormigas
    std::string entrada = argv[1];
    double evaporationRate = atof(argv[2]);
    double alpha = atof(argv[3]);
    double beta = atof(argv[4]);
    int tempoExec = std::atoi(argv[5]);
    int numFormigas = std::atoi(argv[6]);
    std::string outDir = argv[7];
    int oper = std::atoi(argv[8]);

    std::srand(11); // Seed fixa
    //std::srand(static_cast<unsigned>(std::time(nullptr)));

    //oper = 1 faz a execução com todas as seeds
    if (oper == 1)
        Inicializa(entrada, evaporationRate, alpha, beta, tempoExec, numFormigas, outDir);
    //oper == 2 faz o teste de parametros
    else if (oper == 2)
        InicializaTestePar(entrada, tempoExec, numFormigas, outDir);

    return 0;
}