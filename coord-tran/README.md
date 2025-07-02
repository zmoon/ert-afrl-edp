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

## Special cases

If we are at a pole, the final longitude depends entirely on bearing.

```
> ./bin/r2g 0 90 222 45
135.000000, 88.003506
```

```
> ./bin/g2r 0 90 $(./bin/r2g 0 90 222 45)
222.000 45.0000
```

At the south pole, they are equal.

```
> ./bin/r2g 0 -90 222 90
90.000000, -88.003506
```

```
> ./bin/g2r 0 -90 $(./bin/r2g 0 -90 222 90)
222.000 90.0000
```
