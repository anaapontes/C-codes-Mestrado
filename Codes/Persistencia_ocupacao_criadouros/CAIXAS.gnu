# Configurações para gerar um gráfico em PDF
set term pdf enhanced color font 'Helvetica,12'
set output "caixas-comparacao.pdf"
set encoding utf8

# Layout do gráfico
set multiplot

# Configuração do gráfico principal
set origin 0,0
set size 1,1
set tics font ", 12"
set style line 1 linecolor rgb "red" lw 1
set style line 2 linecolor rgb "green" lw 1
set style line 3 linecolor rgb "orange" lw 1
set style line 4 linecolor rgb "blue" lw 1
set style line 5 linecolor rgb "magenta" lw 1
set style line 6 linecolor rgb "yellow" lw 1
set style line 7 linecolor rgb "cyan" lw 1
set style line 8 linecolor rgb "violet" lw 1
set style line 9 linecolor rgb "brown" lw 1
set style line 10 dashtype 2 linecolor rgb "#800000" lw 2
set style line 11 dashtype 2 linecolor rgb "#006400" lw 2
set key top left
set xrange [0:1400]
set yrange [0:]
set ylabel "{/*1.1{Mosquitos não infectados}}"
set xlabel "{/*1.1{Número de criadouros (NC)}}"

# Primeiro, plote as barras de erro com um símbolo único para todos
plot "mosquitos_regular.dat" using 1:2:3 with errorbars ls 2 pt 4 notitle, \
     "mosquitos_aleat.dat" using 1:2:3 with errorbars ls 3 pt 4 notitle, \
     "mosquitos_agrupada.dat" using 1:2:3 with errorbars ls 4 pt 4 notitle

# Agora, plote apenas as linhas, sem símbolos de pontos
plot "mosquitos_regular.dat" using 1:2 with lines ls 2 title "Distribuição regular", \
     "mosquitos_aleat.dat" using 1:2 with lines ls 3 title "Distribuição aleatória", \
     "mosquitos_agrupada.dat" using 1:2 with lines ls 4 title "Distribuição agrupada"


# Configuração do gráfico menor dentro do principal
#set origin 0.36,0.13
#set size 0.6,0.65
#set key top right
#set key at screen 0.91, 0.44
#set title "Porcentagem de ocupação nos criadouros"
#set ylabel "{/*1.1{Porcentagem (%)}}"
#set xlabel "{/*1.1{NC}}"
#set xrange [0:1000]

# Primeiro, plote as barras de erro, sem pontos ou linhas
#plot 'ocupacaoDIF7-50M.dat' using 1:3:4 with errorbars ls 4 notitle

# Agora, plote apenas as linhas, sem símbolos de pontos
#plot 'ocupacaoDIF7-50M.dat' using 1:3 with lines ls 4 title 'Filtrada (A)', \
     'ocupacaoDIF7-50M.dat' using 1:2 with lines ls 7 title 'Direta (A)', \
     'ocupacao_regular.dat' using 1:3 with lines ls 6 title 'Filtrada (R)', \
     'ocupacao_regular.dat' using 1:2 with lines ls 5 title 'Direta (R)'

# Finaliza o multiplot
#unset multiplot

