// AntColonyCVRP.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Colony.h"

namespace fs = std::filesystem;

std::string lerArquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo: " << caminho << std::endl;
        return NULL;
    }
    fs::path arq_ext(caminho);
    if (arq_ext.extension() == ".vrp") {
        return caminho;
    }
    else if (arq_ext.extension() == ".sol") {
        std::string linha;
        while (std::getline(arquivo, linha)) {
            if (linha.find("Cost") != std::string::npos) {
                std::string palavra;
                std::string valor;
                std::istringstream stream(linha);
                stream >> palavra;
                stream >> valor;
                std::cout << palavra << "::" << valor << std::endl;
            }
        }
    }
    else {
        std::cerr << "Formato de arquivo nao reconhecido";
    }
    return NULL;
}

void lerArquivosNoDiretorio(const std::string& diretorio) {
    if (!fs::exists(diretorio) || !fs::is_directory(diretorio)) {
        std::cerr << "Erro: Diretorio não encontrado -> " << diretorio << std::endl;
        return;
    }

    for (const auto& entry : fs::directory_iterator(diretorio)) {
        if (fs::is_regular_file(entry.path())) {
            std::cout << "📄 Lendo arquivo: " << entry.path().filename() << "\n";
            lerArquivo(entry.path().string());
            std::cout << "----------------------\n";
        }
    }
}

void Inicializa(const std::string& caminho, double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas) {
    if (fs::is_directory(caminho)) {
        lerArquivosNoDiretorio(caminho);
    }
    else if (fs::is_regular_file(caminho)) {
        std::string conteudo = lerArquivo(caminho);
        Colony colonia(evaporationRate, alpha, beta, tempoExec, numFormigas, conteudo);
        colonia.CriaSolucoes();
    }
    else {
        std::cerr << "Erro: Caminho invalido -> " << caminho << std::endl;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 7) {
        std::cerr << "Uso: " << argv[0] << " <diretorio ou arquivo>" << std::endl;
        return 1;
    }
    
    //double evaporationRate, double alpha, double beta, int tempoExec, int numFormigas
    std::string entrada = argv[1];
    double evaporationRate = atof(argv[2]);
    double alpha = atof(argv[3]);
    double beta = atof(argv[4]);
    int tempoExec = std::atoi(argv[5]);
    int numFormigas = std::atoi(argv[6]);
    std::srand(42); // Seed fixa

    Inicializa(entrada, evaporationRate, alpha, beta, tempoExec, numFormigas);

    return 0;
}


// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
