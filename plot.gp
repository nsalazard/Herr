set term pdf
set out 'Speed_up.pdf'
set xlabel 'N Threads'
set ylabel 'Speed up'
set title 'Speed up vs N '
plot "easy.txt" u 1:2 w lp lw 2 t "Usando for y reduction", \
"hard.txt" u 1:2 w lp lw 2 t "Mi implementacion"

set term pdf
set out 'efficiency.pdf'
set xlabel 'N Threads'
set ylabel 'Parallel efficiency'
set title 'Parallel efficiency vs N'
plot "easy.txt" u 1:3 w lp lw 2 t "Usando for y reduction", \
"hard.txt" u 1:3 w lp lw 2 t "Mi implementacion"
