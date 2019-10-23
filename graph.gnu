set terminal png size 800,600
set output 'Data/graph.png'

#Key means label
#set key inside bottom right

set xlabel 'Matrix size'
set ylabel 'Time (sec)'
set title 'Matrix multiplication speed on Wolfgang Cluster'

plot "GraphData.txt" using 1:2 title 'non-parallel' with linespoint, "GraphData.txt" using 1:3 title 'SIMD' with linespoint
