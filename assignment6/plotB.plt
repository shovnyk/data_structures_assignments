# autogenerated plotting script

set terminal pdfcairo enhanced color notransparent font "Latin Modern Math"
set output "plotB.pdf"

set style line 1 lc rgb "#008b8b" lw 1.5
set style line 2 lc rgb "#8b0000" lw 1.5
set style line 3 lc rgb "#8b8b00" dashtype 2 lw 1.5
set style line 4 lc rgb "#e6e6e6"

set title "Analysis of Bubble Sort"
set xlabel "Input Size"
set ylabel "Running time (ns)" offset -0.5
set grid xtics ytics back ls 4

set key box top left spacing 1.5

plot "dataB.txt" using 1:2 with lines ls 1 title "Best case"
plot "dataB.txt" using 1:3 with lines ls 2 title "Worst case"
plot "dataB.txt" using 1:4 with lines ls 3 title "Avg case"

plot "dataB.txt" using 1:2 with lines ls 1 title "Best case",\
"dataB.txt" using 1:3 with lines ls 2 title "Worst case",\
"dataB.txt" using 1:4 with lines ls 3 title "Avg case"

set output