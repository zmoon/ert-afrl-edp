#!/bin/bash

set -euo pipefail

# Classic Mandelbrot, zoomed out
./nodejs/cli.js -x '-0.7' -y 0 -q -w 600 -h 400 -s 0.005 | uv run ./plot.py -o default.jpg

# Seahorse tail
./nodejs/cli.js -x '-0.7435669' -y 0.1314023 -w 600 -h 400 -s 0.0000038 -n 200 -q | uv run ./plot.py -o seahorse-tail.jpg

# Seahorse valley
./nodejs/cli.js -x '-0.74364409961' -y 0.13182604688 -w 1200 -h 800 -s 0.00000000055 -n 3000 -q | uv run ./plot.py -o seahorse-valley.jpg
