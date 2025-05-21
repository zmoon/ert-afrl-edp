# Mandelbrot set CLIs

## Usage

Install the Node.js package dependencies (requires `npm`):

```
cd nodejs
npm install
```

Return and run the Bash script to plot the three cases (requires `uv`):

```
cd ..
./plots.sh
```

Mandelbrot set data from the Node.js-based CLI is piped
into the Python-based plotting CLI as JSON.

## Notes

* There are smoothing techniques to improve the beauty of the rendering.
  For example, fractional escape counts.
