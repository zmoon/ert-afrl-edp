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
-66.0000 18.0000
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
135.0000 88.0035
```

```
> ./bin/g2r 0 90 $(./bin/r2g 0 90 222 45)
222.001 45.0000
```

At the south pole, they are equal.

```
> ./bin/r2g 0 -90 222 90
90.0000 -88.0035
```

```
> ./bin/g2r 0 -90 $(./bin/r2g 0 -90 222 90)
222.001 90.0000
```

If two points are at a pole, we know the distance is zero,
but bearing doesn't really make sense in this case,
so `g2r` outputs a special value for bearing to indicate undetermined (-999).

```
> ./bin/g2r 0 90 45 90
0.000 -999.0000
```

From one pole to the other, we know the distance is $\pi r_e$,
but the bearing is not well-defined.

```
> ./bin/g2r 0 90 0 -90
20015.087 -999.0000
```

When range is zero, bearing has no effect.

```
> ./bin/r2g 20 30 0 123
Warning: Input range is zero, input bearing has no effect.
20.0000 30.0000
```

If you pass undetermined bearing, you don't get the warning.

```
> ./bin/r2g 20 30 0 -999
20.0000 30.0000
```

Bearing is undetermined for two points in the same location.

```
> ./bin/g2r 20 30 20 30
0.000 -999.0000
```

```
> ./bin/r2g 20 30 $(./bin/g2r 20 30 20 30)
20.0000 30.0000
```

Works for pole-to-pole too:

```
> ./bin/r2g 0 90 $(./bin/g2r 0 90 0 -90)
0.0000 -90.0000
```

## Directions

`g2r` examples, considering initial point 30°N, 0°E.
Recall the output is range and bearing.

```
> ./bin/g2r 0 30 0 30
0.000 -999.0000
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
