export module Arquivo;

import <iostream>;
import <fstream>;
import <string>;
import <vector>;
import <filesystem>;
import "Colony.h";

namespace fs = std::filesystem;

//Estrutura para armazenar os resultados para escrever a tabela de resultados
export struct Resultado {
    std::string instancia; // Nome da instancia (ex: "A-n32-k5")
    int fsol_otima;       // Valor ótimo extraído do comentário
    int custo_min;       // Menor custo encontrado
    double custo_media;     // Custo médio
    float tempo_min;     // Menor tempo (em segundos)
    float tempo_media;   // Tempo médio
    double gap_min;       // Gap mínimo (%)
    double gap_media;     // Gap médio (%)
};

//Estrutura para armazenar os resultados para escrever a tabela de parametros
export struct Parametros {
    std::string instancia;       // Nome da instancia (ex: "A-n32-k5")
    double alpha;                // Parametro alpha
    double beta;                 // Parametro beta
    double evaporationRate;      // Taxa de evaporaçao
    double resultado;            // Resultado (ex: custo total)
};

// Função para extrair o valor otimo
export int ExtrairValorOtimo(const std::string& conteudoArquivo) {
    std::istringstream stream(conteudoArquivo);
    std::string linha;
    while (std::getline(stream, linha)) {
        if (linha.find("COMMENT") != std::string::npos) {
            size_t pos = linha.find("Optimal value:");
            if (pos != std::string::npos) {
                std::string valorStr = linha.substr(pos + 14); // "Optimal value: " tem 14 caracteres
                return std::stoi(valorStr);
            }
        }
    }
    return -1; // Retorna -1 se não encontrar
}

// Função para extrair o nome da instância do caminho do arquivo
export std::string ExtrairNomeInstancia(const std::string& caminhoArquivo) {
    fs::path path(caminhoArquivo);
    std::string nomeArquivo = path.filename().string();
    size_t pos = nomeArquivo.find_last_of(".");
    return (pos != std::string::npos) ? nomeArquivo.substr(0, pos) : nomeArquivo;
}

//Função que gera a tabela de resultados em formato latex
export void GerarTabelaResultadosLatex(const std::vector<Resultado>& dados, const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo + "tabela_resultado.txt");
    if (arquivo.is_open()) {
        std::stringstream latex;

        // Configuração do documento
        latex << "\\documentclass{article}\n";
        latex << "\\usepackage{longtable}\n";
        latex << "\\usepackage{geometry}\n";
        latex << "\\geometry{a4paper, margin=1cm}\n";
        latex << "\\begin{document}\n";
        latex << "\\centering\n";
        latex << "\\scriptsize\n";

        // Tabela com 8 colunas e cabeçalho de duas linhas
        latex << "\\begin{longtable}{|p{2cm}|p{1.2cm}|p{0.8cm}|p{0.8cm}|p{0.8cm}|p{0.8cm}|p{1cm}|p{1cm}|}\n";

        // Cabeçalho principal
        latex << "\\hline\n";
        latex << "\\textbf{Instância} & \\textbf{fsol\\_otima} & \\multicolumn{2}{c|}{\\textbf{fun}} & \\multicolumn{2}{c|}{\\textbf{Tempo(s)}} & \\textbf{gap\\_min} & \\textbf{gap\\_media} \\\\\n";

        // Subcabeçalhos
        latex << "\\hline\n";
        latex << " & & \\textbf{Mins} & \\textbf{Medias} & \\textbf{Mins} & \\textbf{Medias} & & \\\\\n";
        latex << "\\hline\n";
        latex << "\\endhead\n";

        // Linhas de dados
        for (const auto& res : dados) {
            latex << res.instancia << " & "
                << res.fsol_otima << " & "
                << std::fixed << std::setprecision(2) << res.custo_min << " & "
                << res.custo_media << " & "
                << std::fixed << std::setprecision(2) << res.tempo_min << " & "
                << res.tempo_media << " & "
                << std::fixed << std::setprecision(2) << res.gap_min << "\\% & "
                << res.gap_media << "\\% \\\\\n";
            latex << "\\hline\n";
        }

        latex << "\\end{longtable}\n";
        latex << "\\end{document}\n";

        arquivo << latex.str();
        arquivo.close();
        std::cout << "Tabela LaTeX gerada em: " << caminhoArquivo << std::endl;
    }
    else {
        std::cerr << "Erro ao criar arquivo!" << std::endl;
    }
}

//Função que gera a tabela em formato latex da analise dos parametros
export void GerarTabelaLatex(const std::vector<Parametros>& dados, const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo + "tabela_teste.txt");
    if (arquivo.is_open()) {
        std::stringstream latex;

        // Iniciar tabela longtable
        latex << "\\begin{longtable}{|p{3cm}|p{1.5cm}|p{1.5cm}|p{1.5cm}|p{2cm}|}\n";
        latex << "\\hline\n";
        latex << "\\textbf{Instância} & \\textbf{$\\alpha$} & \\textbf{$\\beta$} & \\textbf{$\\rho$} & \\textbf{Resultado} \\\\\n";
        latex << "\\hline\n";
        latex << "\\endhead\n"; // Repete o cabeçalho em cada página

        // Agrupar por instância e preencher linhas
        std::vector<std::string> instanciasUnicas;
        for (const auto& p : dados) {
            if (std::find(instanciasUnicas.begin(), instanciasUnicas.end(), p.instancia) == instanciasUnicas.end()) {
                instanciasUnicas.push_back(p.instancia);
            }
        }

        for (const auto& instancia : instanciasUnicas) {
            std::vector<Parametros> dadosInstancia;
            for (const auto& p : dados) {
                if (p.instancia == instancia) dadosInstancia.push_back(p);
            }

            auto melhor = *std::min_element(dadosInstancia.begin(), dadosInstancia.end(),
                [](const Parametros& a, const Parametros& b) { return a.resultado < b.resultado; });

            for (const auto& p : dadosInstancia) {
                bool ehMelhor = (p.resultado == melhor.resultado);

                latex << p.instancia << " & ";
                latex << (ehMelhor ? "\\textbf{" : "") << p.alpha << (ehMelhor ? "}" : "") << " & ";
                latex << (ehMelhor ? "\\textbf{" : "") << p.beta << (ehMelhor ? "}" : "") << " & ";
                latex << (ehMelhor ? "\\textbf{" : "") << p.evaporationRate << (ehMelhor ? "}" : "") << " & ";
                latex << (ehMelhor ? "\\textbf{" : "") << p.resultado << (ehMelhor ? "}" : "") << " \\\\\n";
                latex << "\\hline\n";
            }
        }

        latex << "\\end{longtable}\n";
        latex << "\\end{document}\n";
        arquivo << latex.str();
        arquivo.close();
        std::cout << "Tabela salva em: " << caminhoArquivo << std::endl;
    }
    else {
        std::cerr << "Erro ao criar o arquivo!" << std::endl;
    }
    
}

//Função que lê um arquivo
export std::string LerArquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo: " << caminho << std::endl;
        return NULL;
    }
    fs::path arq_ext(caminho);
    if (arq_ext.extension() == ".vrp") {
        std::stringstream buffer;
        buffer << arquivo.rdbuf(); // Lê todo o conteúdo de uma vez
        return buffer.str();;
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
        arquivo.close();
    }
    else {
        std::cerr << "Formato de arquivo nao reconhecido";
    }
    return NULL;
}

//Função que lê os arquivos no diretorio
export std::vector<std::string> LerArquivosNoDiretorio(const std::string& diretorio) {
    if (!fs::exists(diretorio) || !fs::is_directory(diretorio)) {
        std::cerr << "Erro: Diretorio não encontrado -> " << diretorio << std::endl;
        exit(1);
    }
    std::vector<std::string> conteudos;
    for (const auto& entry : fs::directory_iterator(diretorio)) {
        if (fs::is_regular_file(entry.path())) {
            conteudos.push_back(entry.path().string());
        }
    }
    return conteudos;
}

//Função que escreve o gráfico dos custos da execução no outDir
export void EscreverArqGraficoCustos(const std::vector<int>& localCustos, int valorOtimo,const std::string& caminho, const std::string& outDir) {
    std::ifstream arquivo(caminho);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo do grafico: " << caminho << std::endl;
        return;
    }
    fs::path arq_nome(caminho);
    std::string nome = outDir + arq_nome.filename().string() + "_Custos.dat";
    arquivo.close();
    
    std::ofstream file(nome); // Cria ou sobrescreve o arquivo

    if (!file) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return;
    }
    int i = 0;
    for (int result : localCustos) {
        file << ++i << " " << result << " " << valorOtimo << "\n";
    }
    file.close();
}

//Função que escreve o gráfico da solução no outDir
export void EscreverArqGraficoSolucao(const std::vector<int>& solucao, const std::vector<Cidade> cidades,const std::string& caminho, const std::string& outDir) {
    std::ifstream arquivo(caminho);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo do grafico solucao: " << caminho << std::endl;
        return;
    }
    fs::path arq_nome(caminho);
    std::string nome = outDir + arq_nome.filename().string() + "_Solucao.dat";

    std::ofstream file(nome); // Cria ou sobrescreve o arquivo

    bool first = true;
    for (int result : solucao) {
        file << cidades[result].getPosX() << " " << cidades[result].getPosY() << "\n";
        if (result == 0) {
            if (first) {
                first = false;
            }
            else {
                file << "\n\n";
                file << cidades[result].getPosX() << " " << cidades[result].getPosY() << "\n";
            }
        }
    }
    file.close();
}