# Configurações para gerar um gráfico em EPS
set term pdf enhanced color font 'Helvetica,12'   	       # Define o formato de saída para PDF com fonte Helvetica de tamanho 16
set output "COMPARACOES-QI.pdf"                         	       	       # Define o nome do arquivo de saída
set encoding utf8						# Para que os caracteres especiais fiquem corretos

# Layout do gráfico: 2 linhas, 1 coluna
set multiplot layout 3,1

# Configurações gerais do gráfico
set tics font ", 12"                             		# Tamanho da fonte para as marcas nos eixos

# Configurações das linhas e estilos
set style line 1 linecolor rgb "blue" lw 2              # Linha azul
set style line 11 linecolor rgb "black" lw 2     		# Linha preta (usada para a seta)
set style line 2 linecolor rgb "green" lw 2     		# Linha verde
set style line 3 dashtype 2 linecolor rgb "red" lw 2   	# Linha tracejada vermelha
set style line 4 dashtype 2 linecolor rgb "green" lw 2 	# Linha tracejada verde
set style line 5 linecolor rgb "magenta" lw 2   		# Linha magenta
set key top right # Configurações da legenda

# Configurações dos eixos
set xrange[0:]                                		    # Intervalo do eixo x
set ylabel "{/*1.1{Populações}}"               			# Rótulo do eixo y
set xlabel "{/*1.1{Tempo (meses)}}"              		# Rótulo do eixo x

# Plota o primeiro conjunto de dados
set label 'QI=2' at 40,0.15       		
plot 'TEMPO_w_libVIRGEM_virgem-QI=2.dat' u ($1/30):($2/10000) w l ls 1 title 'U','TEMPO_w_libVIRGEM_virgem-QI=2.dat' u ($1/30):($3/10000) w l ls 2 title 'I'

unset label # Remove os rótulos
# Plota o primeiro conjunto de dados
set label 'QI=4' at 40,0.15 
plot 'TEMPO_w_libVIRGEM_virgem-QI=4.dat' u ($1/30):($2/10000) w l ls 1 title 'U','TEMPO_w_libVIRGEM_virgem-QI=4.dat' u ($1/30):($3/10000) w l ls 2 title 'I'

unset label # Remove os rótulos
# Plota o primeiro conjunto de dados
set label 'QI=8' at 40,0.20 
plot 'TEMPO_w_libVIRGEM_virgem-QI=8.dat' u ($1/30):($2/10000) w l ls 1 title 'U','TEMPO_w_libVIRGEM_virgem-QI=8.dat' u ($1/30):($3/10000) w l ls 2 title 'I'

