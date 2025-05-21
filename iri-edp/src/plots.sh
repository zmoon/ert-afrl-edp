#!/bin/bash
#
# Run and plot the two cases with gnuplot

set -eu

make -j 2

./iri -c 1 -o case1.csv
./iri -c 2 -o case2.csv

# TODO: save with
# set term pngcairo size 800,600; \
# set output 'edp.png'; \
gnuplot -p -e " \
  set datafile separator ','; \
  set title 'Electron density profiles at 37.8°N, 75.4°W'; \
  set xlabel 'n_e [m^{-3}]'; \
  set ylabel 'altitude [km]'; \
  plot 'case1.csv' using 2:1 with lines title '2021-03-03 11:00 UTC', \
       'case2.csv' using 2:1 with lines title '2021-03-04 23:00 UTC'; \
  "

gnuplot -p -e " \
  set datafile separator ','; \
  set title 'Temperature profiles at 37.8°N, 75.4°W'; \
  set xlabel 'temperature [K]'; \
  set ylabel 'altitude [km]'; \
  plot 'case1.csv' using 3:1 with lines title '2021-03-03 11:00 UTC T_n', \
       '' using 4:1 with lines title 'T_i', \
       '' using 5:1 with lines title 'T_e', \
       'case2.csv' using 3:1 with lines title '2021-03-04 23:00 UTC T_n', \
       '' using 4:1 with lines title 'T_i', \
       '' using 5:1 with lines title 'T_e', \
  "

gnuplot -p -e " \
  set datafile separator ','; \
  set title 'Ratio of plasma frequency to gyrofrequency profiles at 37.8°N, 75.4°W'; \
  set xlabel 'f_p/f_g [1]'; \
  set ylabel 'altitude [km]'; \
  plot 'case1.csv' using 13:1 with lines title '2021-03-03 11:00 UTC', \
       'case2.csv' using 13:1 with lines title '2021-03-04 23:00 UTC'; \
  "
