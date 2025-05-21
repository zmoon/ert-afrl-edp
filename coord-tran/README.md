# Coordinate transformation: radar ↔ geodetic

We assume a spherical Earth to simplify the calculations.
The formulas of interest are:

- Haversine: great-circle distance from one point to another
- initial bearing for that great-circle path
- inverses of these two

## Usage

Build (requires `make` and `gcc`):

```
make -C src
```

Get range and bearing for the example locations of interest:

```
> ./bin/g2r -75 37 -66 18
2288.664 154.9632
```

Recover final location:

```
> ./bin/r2g -75 37 $(./bin/g2r -75 37 -66 18)
-65.999993, 17.999999
```
