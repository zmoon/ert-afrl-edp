#!/usr/bin/env -S uv run --script
"""
Demonstrate interpolating unstructured point data to a fine grid.
"""
# /// script
# dependencies = [
#   "matplotlib ~=3.10.3",
#   "numpy",
# ]
# ///

from __future__ import annotations


def idw(xi, yi, zi, xg, yg, *, power=2):
    import numpy as np

    """Naive inverse distance weighting interpolation.
    (Including all neighbors, not just the nearest ones.)

    Parameters
    ----------
    xi, yi, zi : array-like
        Coordinates and values of the input points.
        (1-D arrays of the same length.)
    xg, yg : array
        1-D arrays of the coordinates to interpolate to.
    power : float
        Power of the distance in the weighting function.
        (Default is 2, which is the most common choice.)
        The higher the power, the more weight is given to the nearest points.

    Returns
    -------
    zg : array
        Interpolated values at the grid points.
        (1-D array of the same length as `xg` and `yg`.)
    """
    # Calculate the distance from each point to the grid points
    # We need an array of shape (xg.size, xi.size),
    # so this becomes memory-intensive pretty quickly.
    d = np.sqrt((xg[:, np.newaxis] - xi) ** 2 + (yg[:, np.newaxis] - yi) ** 2)
    d[d == 0] = np.finfo(d.dtype).eps  # Avoid division by zero

    # Calculate the normalized weights
    w = 1 / d**power
    w /= w.sum(axis=1)[:, np.newaxis]

    # Calculate the interpolated values
    zg = (w * zi).sum(axis=1)

    return zg


def plots(*, show: bool = False) -> None:
    from io import StringIO
    from textwrap import dedent

    import matplotlib.pyplot as plt
    import numpy as np

    # Load the data
    s_input_data = """\
    lon	lat	value
    121.39	13.51	1.494
    126.19	12.02	1.934
    130.27	13.11	2.148
    127.42	10.09	9.155
    126.14	15.33	2.221
    125.96	14	8.1
    123.15	10.88	2.039
    130.5	11.18	1.916
    129.08	15.78	3.729
    122.74	15.82	7.137
    """
    arr = np.loadtxt(StringIO(dedent(s_input_data)), delimiter="\t", skiprows=1)
    lon, lat, value = arr.T

    # Define the structured grid
    lon_grid_range = (121, 131, 70)
    lat_grid_range = (10, 16, 50)
    lon_grid, lat_grid = np.meshgrid(
        np.linspace(*lon_grid_range),
        np.linspace(*lat_grid_range),
    )

    #
    # Recreate the point locations plot
    #

    fig, ax = plt.subplots(figsize=(4, 3.5))

    ax.plot(lon, lat, "*", ms=10, color="blue")
    ax.plot(lon_grid, lat_grid, ".", ms=5, mec="none", color="red", alpha=0.7)

    ax.set_title("Point Locations")

    fig.tight_layout()
    fig.savefig("point-locations.png")

    #
    # Inverse distance weighting
    #

    powers = [1.2, 2, 3]

    fig, axs = plt.subplots(
        len(powers),
        1,
        figsize=(4.2, 8),
        sharex=True,
        sharey=True,
        layout="constrained",
    )

    color_kws = dict(cmap="viridis", vmin=value.min(), vmax=value.max())
    for power, ax in zip(powers, axs.flat):
        values_grid = np.reshape(
            idw(lon, lat, value, lon_grid.ravel(), lat_grid.ravel(), power=power),
            lon_grid.shape,
        )

        ax.pcolormesh(
            lon_grid,
            lat_grid,
            values_grid,
            shading="auto",
            alpha=0.95,
            **color_kws,
        )
        ax.scatter(
            lon,
            lat,
            c=value,
            s=150,
            edgecolor="black",
            linewidth=0.5,
            **color_kws,
        )
        ax.text(
            0.01,
            0.99,
            rf"$1 / d^{{{power}}}$",
            ha="left",
            va="top",
            transform=ax.transAxes,
            color="firebrick",
        )

    fig.colorbar(
        axs.flat[0].collections[-1],
        ax=axs,
        orientation="vertical",
        shrink=0.5,
        label="value",
    )
    fig.suptitle("Inverse Distance Weighting")

    fig.savefig("idw.png")

    if show:
        plt.show()


def main() -> None:
    import argparse

    parser = argparse.ArgumentParser(
        description="Demonstrate interpolating unstructured point data.",
    )
    parser.add_argument(
        "--show",
        action="store_true",
        help="Show the plots.",
    )

    args = parser.parse_args()

    plots(show=args.show)


if __name__ == "__main__":
    main()
