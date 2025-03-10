# Configurações para gerar um gráfico em EPS
set term pdf enhanced color font 'Helvetica,12'   	      # Define o formato de saída para PDF com fonte Helvetica de tamanho 16
set output "comparacao-area-350.pdf"                      # Define o nome do arquivo de saída
set encoding utf8						                  # Para que os caracteres especiais fiquem corretos

# Layout do gráfico
set multiplot layout 1,1                                  # 1 linha, 1 coluna

# Configurações gerais do gráfico
set lmargin 10                                            # Margem esquerda
set rmargin 10                                            # Margem direita
set tics font ", 15"                                      # Tamanho da fonte para as marcas nos eixos
set style line 1 linecolor rgb "red" lw 2                 # Estilo de linha 1 (vermelha e espessura 3)
set style line 2 linecolor rgb "green" lw 2               # Estilo de linha 2 (verde e espessura 3)
set style line 3 linecolor rgb "orange" lw 2              # Estilo de linha 3 (laranja e espessura 3)
set style line 4 linecolor rgb "blue" lw 2                # Estilo de linha 1 (azul e espessura 2)
set style line 5 linecolor rgb "magenta" lw 2             # Estilo de linha 5 (magenta e espessura 3)
set style line 6 linecolor rgb "yellow" lw 2              # Estilo de linha 6 (amarelo e espessura 3)
set style line 7 linecolor rgb "cyan" lw 2                # Estilo de linha 7 (azul claro e espessura 3)
set style line 9 linecolor rgb "brown" lw 2               # Estilo de linha 9 (marrom e espessura 3)
set style line 10 lc rgb "#55BCC9" lw 2                   # Ciano
set style line 11 lc rgb "#59A14F" lw 2                   # Verde claro
set style line 12 lc rgb "#9B59B6" lw 2                   # Roxo
set style line 13 lc rgb "#E15759" lw 2                   # Vermelho
set key above                                             # Posição da legenda fora do gráfico, no canto superior direito
set ylabel "{/*1.1{J}}"                                   # Rótulo do eixo y
set xlabel "{/*1.1{QI (proporção de infectados com relação a não infectados)}}"      # Rótulo do eixo x
set xrange [0:]                                           # Intervalo do eixo x
set yrange [0:]                                           # Intervalo do eixo y

# Configurações da plotagem
plot "eficacia_area_w_libvirgem_virgem-oficial.dat" u 1:2 w linespoints ls 4 pt 7 title 'Wolbachia - Virgem', \
     "eficacia_area_w_libvirgem_nvirgem-oficial.dat" u 1:2 w linespoints ls 2 pt 7 title 'Wolbachia - Não virgem', \
     "eficacia_area_m_libvirgem_virgem-oficial.dat" u 1:2 w linespoints ls 3 pt 7 title 'Estéreis - Virgem', \
     "eficacia_area_m_libvirgem_nvirgem-oficial.dat" u 1:2 w linespoints ls 12 pt 7 title 'Estéreis - Não virgem'
     
