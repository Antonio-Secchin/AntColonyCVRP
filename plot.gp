set title "Custo por Itera��o"
set xlabel "Itera��es"
set ylabel "Custo"
set grid
set key off

# Pegando o nome do arquivo passado como argumento
filename = ARG1

# Encontrar posi��o final de ".dat"
ext_pos = strstrt(filename, ".dat")

# Se ".dat" for encontrado, remover ele e adicionar ".png"
if (ext_pos > 0) {
    pngname = filename[:ext_pos-1] . ".png"
} else {
    pngname = filename . ".png"  # Caso n�o tenha ".dat", adiciona ".png" direto
}

# Exibir gr�fico na tela primeiro
set terminal wxt
plot filename using 1:2 with linespoints title "Custo", \
     filename using 1:3 with lines lc rgb "#FF0000" title "Valor �timo"

# Pergunta ao usu�rio se deseja salvar
print "Deseja salvar o gr�fico como PNG? (s/n)"
pause -1 "Pressione Enter para continuar..."

# Captura a resposta do usu�rio no Windows (PowerShell)
save_as_png = system("powershell -Command \"Read-Host 'Salvar como PNG (s/n)'\"")

# Se o usu�rio disser 's', salva o gr�fico
if (save_as_png eq "s") {
    set terminal png
    set output pngname
    replot
    print "Gr�fico salvo como '" . pngname . "'!"
}
