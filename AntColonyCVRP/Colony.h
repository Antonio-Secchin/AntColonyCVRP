#pragma once
#include <vector>
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

    Colony(double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas, const std::string& caminho);

    void AtualizaFeromonio();

    void AtualizaProbabilidades();

    std::vector<std::vector<int>> CriaSolucoes(int time_max);
};

