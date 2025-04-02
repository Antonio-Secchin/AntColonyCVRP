# AntColonyCVRP

Trabalho desenvolvido para a matéria de Tópicos em Otimização, com o objetivo de implementar a meta-heuristica de colônia de formigas no problema de Problema do roteamento de veículos capacitados (CVRP) 

## Configuração para Desenvolvimento

Primeiro é necessário clonar o repositório, usando:
```sh
git clone https://github.com/Antonio-Secchin/AntColonyCVRP.git
```
Em seguida recomendo abrir utilizando o Microsoft Visual Studio, pois para compilar foi utilizado o MSBuild.

### Como executar

Primeiro é necessário baixar as instâncias do problema CVRP da biblioteca [CVRPLIB](http://vrp.galgos.inf.puc-rio.br/index.php/en/).
Em seguida configure o caminho dos arquivos de entrada(InputFile), pasta contendo somente os arquivos .vrp, e saída(DIretorioSaida) no arquivo AntColonyCVRP.vcxproj.

Depois é só executar o comando:
```sh
msbuild /t:RunTeste
```
Depois de algum tempo o resultado será gerado em uma tabela e em arquivos .dat para visualizar o gráfico.

### Gerando os gráficos

Primeiramente instale o [gnuplot](https://sourceforge.net/projects/gnuplot/).

Depois, para os arquivos de "Custos", utilize
```sh
gnuplot -c plot.gp "Caminho do arquivo .dat"
```
Será exibido o gráfico da variação do custo da solução ao longo do tempo.

Para os arquivos "Solucao", utilize
```sh
gnuplot -c plotCaminho.gp "Caminho do arquivo .dat"
```
Será exibido a melhor solução encontrada pelo algoritmo.
