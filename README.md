# ERT-AFRL space weather

_Assessment exercises for an ERT-AFRL scientific software engineer position related to impacts of the ionosphere_

[![CI workflow status](https://github.com/zmoon/ert-afrl-edp/actions/workflows/ci.yml/badge.svg)](https://github.com/zmoon/ert-afrl-edp/actions/workflows/ci.yml)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/zmoon/ert-afrl-edp/main.svg)](https://results.pre-commit.ci/latest/github/zmoon/ert-afrl-edp/main)

Each exercise has its own subdirectory:

| Exercise                    | Description                                                                        |
| :-------------------------- | :--------------------------------------------------------------------------------- |
| [coord-tran](./coord-tran/) | Coordinate transformation between radar and geodetic coordinates                   |
| [interp](./interp/)         | Interpolation of scattered point data to a structured grid                         |
| [iri-edp](./iri-edp/)       | C interface and tools for generating vertical profiles using the IRI-2020 model    |
| [mandelbrot](./mandelbrot/) | CLI tools to generate and visualize Mandelbrot set images using Node.js and Python |
| [noaa-rtsw](./noaa-rtsw/)   | Fetch and analyze NOAA real-time solar wind data                                   |

The [CI workflow](https://github.com/zmoon/ert-afrl-edp/actions/workflows/ci.yml)
checks that the code works and stores the outputs (e.g. images)
as temporary artifacts.
