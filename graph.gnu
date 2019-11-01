set terminal png size 800,600
set output 'graph.png'

#Key means label
#set key inside bottom right

set xlabel 'Matrix Entries'
set ylabel 'Time (sec)'
set title 'Matrix multiplication speed on Wolfgang Cluster'

plot "Data/GraphData.txt" using 1:2 title 'Basic' with linespoint, "Data/GraphData.txt" using 1:3 title 'SIMD' with linespoint,"Data/GraphData.txt" using 1:4 title 'OMP' with linespoint, "Data/GraphData.txt" using 1:5 title 'MPI' with linespoint, "Data/GraphData.txt" using 6:7 title 'Basic NS' with linespoint, "Data/GraphData.txt" using 6:8 title 'SIMD NS' with linespoint, "Data/GraphData.txt" using 6:9 title 'OMP NS' with linespoint
