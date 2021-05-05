# plotAll.plt: script to plot and compare all the sorting algorithms:

set terminal pdfcairo enhanced color notransparent font "Latin Modern Math"
set output "plotAllSearch.pdf"

set style line 6 lc rgb "#e6e6e6"
set xlabel "Input Size"
set ylabel "Running time (ns)" offset -0.5
set grid xtics ytics back ls 6

set key box top left spacing 1.5

set title "Comparison of Best-case Running Times"
plot "dataLSearch.txt" using 1:2 with lines ls 1 title "Linear", \
     "dataBSearch.txt" using 1:2 with lines ls 2 title "Binary"

set title "Comparison of Worst-case Running Times"
plot "dataLSearch.txt" using 1:3 with lines ls 1 title "Linear", \
     "dataBSearch.txt" using 1:3 with lines ls 2 title "Binary"

set title "Comparison of Average-case Running Times"
plot "dataLSearch.txt" using 1:4 with lines ls 1 title "Linear", \
     "dataBSearch.txt" using 1:4 with lines ls 2 title "Binary"

set output
