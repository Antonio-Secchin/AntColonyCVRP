#pragma once
#include <vector>
#include <utility>
#include "Grafo.h"
#include "Ant.h"
class Colony :
    public Grafo
{
    //Taxa que os feromonios diminuem
    double evaporationRate;
    
    //Peso para a influencia da informacao heuristica
    double alpha;
    
    //Peso para a influencia dos feromonios
    double beta;

    int tempoExec;

    int numFormigas;

    std::vector<Ant> formigas;
    std::vector<std::vector<double>> feromonios;
    std::vector<std::vector<double>> probabilidades;

public:

    Colony(double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas, const std::string& conteudo);

    void AtualizaFeromonio(std::vector<int>& bestSolu, int& bestCusto);

    void AtualizaProbabilidades();

    std::tuple<std::vector<int>, int, float, std::vector<int>> CriaSolucoes();
};

