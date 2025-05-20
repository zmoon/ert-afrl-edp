#!/bin/bash
#
# Run and plot the two cases with gnuplot

set -eu

make -j 2

./iri -c 1 -o case1.csv
./iri -c 2 -o case2.csv

gnuplot -p -e " \
  set datafile separator ','; \
  set title 'Electron density profiles'; \
  set xlabel 'n_e [m^{-3}]'; \
  set ylabel 'altitude [km]'; \
  plot 'case1.csv' using 2:1 with lines title 'case 1', \
       'case2.csv' using 2:1 with lines title 'case 2'; \
  "

