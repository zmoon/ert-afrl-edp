# IRI-EDP: electron density profile generator

- IRI: the [International Reference Ionosphere](https://irimodel.org/) model
- EDP: electron density profile

This project provides a C interface and command-line tool for generating
vertical profiles of electron density and other parameters
using the IRI-2020 model, which is a legacy Fortran code.

### IRI-2020 version

I was instructed to use IRI-2020 (12/20/2024), but the current IRI-2020 is dated 03/20/2025.
The Wayback Machine has the previous version:
<https://web.archive.org/web/20250215171731/https://irimodel.org/IRI-2020/>.
Unfortunately, `00_iri.zip` wasn't archived,
and essential files like [`irifun.for`](https://web.archive.org/web/20250215171731/https://irimodel.org/IRI-2020/irifun.for)
seem to have been incompletely captured,
so I elected to use the current IRI-2020 instead.

## Usage

Build the project (`make`, `gfortran`, and `gcc` required):

```
cd src
make
```

Run a case, outputting CSV text to stdout:

```
./iri --case 1
```

Run the two cases of interest, save the data to `.csv` files, and view the plots
(`gnuplot` required):

```
./plots.sh
```

## Notes

- IRI expects the data files it needs to load to be found in the current working directory.
  Preprocessor macros could be used to set a relative or absolute (for install) directory
  to load those files from to improve the run experience.
- Building a C API this way requires care in setting the correct types in the C side,
  including passing the correct integers for Booleans (Fortran `logical`),
  ensuring integer/float precision is the same as on the Fortran side,
  and considering the opposite multi-dimensional array storage order.
  Personally, I would prefer to write the C interface in Fortran
  using `iso_c_binding` (F2003).
- Accounting for Fortran name mangling manually is fragile, as it may be different
  for other platforms and Fortran compilers.
  Writing the C interface in Fortran with `iso_c_binding` (F2003) would help alleviate this issue.
- We could write the plotting script in C instead without much fuss:
  ```c
  FILE *gnuplot = popen("gnuplot -p", "w");
  if (!gnuplot) {
      perror("Failed to open gnuplot");
      return 1;
  }
  fprintf(gnuplot, "plot ...\n");
  ```
- I feel a bit bad calling the directory `src`, since it has everything in it.
  Ultimately I'd want to separate the IRI input data files from the Fortran and C source files,
  and place the plotting script elsewhere.
- Using getopt (`getopt_long`) would make the arg parsing more robust.

## See also

- <https://github.com/space-physics/iri2020>: Python (and Matlab) wrapper for
  an older version of IRI-2020 (~ 2023),
  with a command-line IRI [driver written in Modern Fortran](https://github.com/space-physics/iri2020/blob/a609211dce6f101e4e84dae30083942e70272786/src/iri2020/src/iri_driver.f90)
  that writes the outputs to stdout.
  The Python driver calls this CLI.
  It gets around the data location issue
  [by using](https://github.com/space-physics/iri2020/blob/a609211dce6f101e4e84dae30083942e70272786/src/iri2020/base.py#L56)
  a child process with working directory changed to the data directory.
- <https://github.com/timduly4/pyglow>: Python wrapper of IRI and other similar models.
  [Uses f2py](https://github.com/timduly4/pyglow/blob/1988757f3b6a4bd5ed98266a3fb1dc64f2513fc5/src/pyglow/iri.py#L195). It doesn't (currently) have IRI-2020, only older versions.
