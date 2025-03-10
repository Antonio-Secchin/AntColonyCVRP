set title "Caminho da Solucao"
set xlabel "X"
set ylabel "Y"
set grid
set key off

# Definição de cores variadas (você pode adicionar mais)
colors = "#FF0000 #00FF00 #0000FF #FF00FF #00FFFF #FFFF00 #800000 #008000 #000080 #808000 #800080 #008080 #808080 #C00000 #00C000 #0000C0 #C0C000 #C000C0 #00C0C0 #C0C0C0 #FF4500 #32CD32 #1E90FF #FF1493 #7FFF00 #8A2BE2 #FFD700 #DC143C #00CED1 #FF8C00 #8B008B #00FF7F #483D8B #FF69B4 #00BFFF #FF6347 #8FBC8F #9400D3 #20B2AA #FFA07A #7B68EE #00FA9A #DAA520 #FF00FF #7CFC00 #ADFF2F #FF4500 #6A5ACD #FF7F50 #3CB371 #B22222 #87CEEB #FFDAB9 #4682B4 #D2691E #5F9EA0 #FFE4B5 #6495ED #CD5C5C #4169E1 #F08080 #8B4513 #00FF00 #BA55D3 #FFA500 #66CDAA #9370DB #FFC0CB #8B0000 #00FFFF #FFEBCD #B0C4DE #FF0000 #7FFF00 #D8BFD8 #FF7F50 #F0E68C #00FF7F #FF69B4 #ADD8E6 #FF4500 #90EE90 #FF00FF #FFD700 #87CEFA #FF8C00 #98FB98 #FF1493 #FFA07A #00BFFF #FF6347 #8FBC8F #9400D3 #20B2AA #FFA500 #7B68EE #00FA9A #DAA520 #FF00FF #7CFC00 #ADFF2F"

# Pegando o nome do arquivo passado como argumento
filename = ARG1

# Contar quantos conjuntos de dados temos no arquivo
stats filename nooutput
n = STATS_blocks - 1  # Número de segmentos (blocos separados por linha em branco)


# Encontrar posição final de ".dat"
ext_pos = strstrt(filename, ".dat")

# Se ".dat" for encontrado, remover ele e adicionar ".png"
if (ext_pos > 0) {
    pngname = filename[:ext_pos-1] . ".png"
} else {
    pngname = filename . ".png"  # Caso não tenha ".dat", adiciona ".png" direto
}

# Exibir gráfico na tela primeiro
set terminal wxt
# Loop para plotar todos os segmentos de forma automática
plot for [i=0:n] filename index i with linespoints lc rgb word(colors, (i % words(colors)) + 1) title sprintf("Trecho %d", i+1)


# Pergunta ao usuário se deseja salvar
print "Deseja salvar o gráfico como PNG? (s/n)"
pause -1 "Pressione Enter para continuar..."

# Captura a resposta do usuário no Windows (PowerShell)
save_as_png = system("powershell -Command \"Read-Host 'Salvar como PNG (s/n)'\"")

# Se o usuário disser 's', salva o gráfico
if (save_as_png eq "s") {
    set terminal png
    set output pngname
    replot
    print "Gráfico salvo como '" . pngname . "'!"
}
