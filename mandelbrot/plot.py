"""
Run and plot
"""
# /// script
# dependencies = [
#   "matplotlib",
#   "numpy",
#   "nodejs-wheel-binaries",
# ]
# ///

import json
from pathlib import Path
from textwrap import indent

import matplotlib.pyplot as plt
import numpy as np
from nodejs_wheel import node

HERE = Path(__file__).parent

# Run the script
params = {"width": 600, "height": 400, "scale": 0.005}
cmd = [(HERE / "nodejs/cli.js").as_posix(), "--quiet"]
for k, v in params.items():
    cmd.extend([f"--{k}" if len(k) > 1 else f"-{k}", str(v)])
cp = node(
    cmd,
    return_completed_process=True,
    capture_output=True,
    text=True,
)
if cp.returncode != 0:
    info = indent(cp.stderr, "| ", lambda _: True)
    raise RuntimeError(f"Failed to generate Mandelbrot data:\n{info}")

data = json.loads(cp.stdout)

if not data["points"]:
    print("no points")
    print(data["metadata"])
    raise SystemExit(1)

# Extract points
xs = []
ys = []
ns = []
for d in data["points"]:
    xs.append(d["x"])
    ys.append(d["y"])
    ns.append(d["iterations"])

# Plot points
plt.scatter(xs, ys, c=ns, s=3, linewidths=0, cmap="hot_r", marker=".")
nx, ny = data["metadata"]["width"], data["metadata"]["height"]
plt.imshow(
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
plt.axis("equal")
plt.tight_layout()

plt.show()
