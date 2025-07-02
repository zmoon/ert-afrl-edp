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

Get range (km) and bearing (deg) for the example locations of interest:

```
> ./bin/g2r -75 37 -66 18
2288.664 154.9632
```

Recover final location:

```
> ./bin/r2g -75 37 $(./bin/g2r -75 37 -66 18)
-65.999993 17.999999
```

> [!NOTE]
> Longitude comes first in the CLI coordinate pairs for both `g2r` and `r2g`,
> such that `-66 18` means 18°N, 66°W,
> [which is](https://www.google.com/maps/place/18%C2%B000'00.0%22N+66%C2%B000'00.0%22W/)
> a point in Puerto Rico.

## Special cases

If we are at a pole, the `r2g` final longitude depends entirely on bearing.

```
> ./bin/r2g 0 90 222 45
135.000000 88.003506
```

```
> ./bin/g2r 0 90 $(./bin/r2g 0 90 222 45)
222.000 45.0000
```

At the south pole, they are equal.

```
> ./bin/r2g 0 -90 222 90
90.000000 -88.003506
```

```
> ./bin/g2r 0 -90 $(./bin/r2g 0 -90 222 90)
222.000 90.0000
```

If two points are at a pole, we know the distance is zero,
but bearing doesn't really make sense in this case,
so `g2r` errors.

```
> ./bin/g2r 0 90 45 90
Error: Invalid coordinates. Cannot calculate bearing for two pole points.
```

From one pole to the other, we know the distance is $\pi r_e$,
but the bearing is not well-defined.

```
> ./bin/g2r 0 90 0 -90
Error: Invalid coordinates. Cannot calculate bearing for two pole points.
```

## Directions

`g2r` examples, considering initial point 30°N, 0°E.
Recall the output is range and bearing.

```
> ./bin/g2r 0 30 0 30
0.000 0.0000
```

N:

```
> ./bin/g2r 0 30 0 40
1111.949 0.0000
```

S:

```
> ./bin/g2r 0 30 0 20
1111.949 180.0000
```

E:

```
> ./bin/g2r 0 30 10 30
962.670 87.4952
```

W:

```
> ./bin/g2r 0 30 -10 30
962.670 272.5048
```
