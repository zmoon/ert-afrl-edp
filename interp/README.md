# Interpolation of unstructured grid data

I implemented naive inverse distance weighting (IDW).
In this method, for a given grid point,
all input points contribute to the grid point's value,
with weights based on their distance from the grid point.

## Usage

Run the script, which creates (and optionally shows) the plots (requires `uv`):

```
uv run ./plots.py
```

## Notes

- A more optimized IDW implementation could use a _k_-d tree
  to include only near neighbors in the calculation,
  reducing memory and CPU usage needed to compute the weights.
- Natural neighbor interpolation would be more interesting to implement,
  but it needs Delaunay triangulation.
  May add it later.

## See also

- [`metpy.interpolate`](https://unidata.github.io/MetPy/v1.7/api/generated/metpy.interpolate.html)
  includes many interpolation methods, including natural neighbor
