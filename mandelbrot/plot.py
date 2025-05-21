#!/usr/bin/env python
"""
Run and plot
"""
# /// script
# dependencies = [
#   "matplotlib ~=3.10.3",
#   "numpy",
# ]
# ///

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

HERE = Path(__file__).parent

# Run the script
# params = {"width": 600, "height": 400, "scale": 0.005}


def plot(data) -> None:
    import matplotlib.pyplot as plt
    import numpy as np

    # Extract points
    xs = []
    ys = []
    ns = []
    for d in data["points"]:
        xs.append(d["x"])
        ys.append(d["y"])
        ns.append(d["iterations"])

    nx, ny = data["metadata"]["width"], data["metadata"]["height"]

    aspect = nx / ny
    small_dim = 4
    if aspect > 1:
        h = small_dim
        w = h * aspect
    else:
        w = small_dim
        h = w / aspect
    fig, ax = plt.subplots(figsize=(w, h))

    ax.imshow(
        np.reshape(ns, (ny, nx)),
        extent=[
            data["metadata"]["minX"],
            data["metadata"]["maxX"],
            data["metadata"]["minY"],
            data["metadata"]["maxY"],
        ],
        interpolation="bilinear",
        cmap="hot_r",
    )
    ax.set_aspect("equal")
    fig.tight_layout()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Plot Mandelbrot set data from the nodejs script",
    )
    parser.add_argument(
        "-i", "--input", type=Path,
        help="Input JSON file (stdin takes precedence)",
    )
    parser.add_argument(
        "-o", "--output", type=Path,
        default=Path("./out.jpg"),
        help="Output JPEG file name or stem (default: out.jpg)",
    )
    parser.add_argument(
        "--show", action="store_true",
        help="Show the plot",
    )

    args = parser.parse_args()

    # Get data
    if args.input is None or args.input == Path("-"):
        # Read from stdin
        data = json.loads(sys.stdin.read())
    else:
        # Read from file
        if args.input.is_file():
            with args.input.open("r") as f:
                data = json.load(f)
        else:
            print(f"Input file {args.input.as_posix()} does not exist")
            raise SystemExit(1)

    if not data["points"]:
        print("no points")
        print(data["metadata"])
        raise SystemExit(1)

    plot(data)

    import matplotlib.pyplot as plt

    # Save
    if args.output.suffix == "":
        args.output = args.output.with_suffix(".jpg")
    plt.savefig(args.output.as_posix())

    # Show the plot
    if args.show:
        plt.show()


if __name__ == "__main__":
    main()
