# IRI-EDP: Electron Density Profile Generator

* IRI: the [International Reference Ionosphere](https://irimodel.org/) model
* EDP: electron density profile

This project provides a C interface and command-line tool for generating
vertical profiles of electron density and other parameters
using the IRI-2020 model, which is a legacy Fortran code.

### IRI-2020 Version

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
./plot.sh
```

## Notes

* IRI expects the data files it needs to load to be found in the current working directory.
  Preprocessor macros could be used to set a relative or absolute (for install) directory
  to load those files from to improve the run experience.
* Building a C API this way requires care in setting the correct types in the C side,
  including passing the correct integers for Booleans (Fortran `logical`),
  ensuring integer/float precision is the same as on the Fortran side,
  and considering the opposite multi-dimensional array storage order.
  Personally, I would prefer to write the C interface in Fortran
  using `iso_c_binding` (F2003).
* Accounting for Fortran name mangling manually is fragile, as it may be different
  for other platforms and Fortran compilers.
  Writing the C interface in Fortran with `iso_c_binding` (F2003) would help alleviate this issue.
* We could write the plotting script in C instead without much fuss:
  ```c
  FILE *gnuplot = popen("gnuplot -p", "w");
  if (!gnuplot) {
      perror("Failed to open gnuplot");
      return 1;
  }
  fprintf(gnuplot, "plot ...\n");
  ```
