CC := gcc
FC := gfortran
CFLAGS = -g -std=c99 -Wall -Werror -I.
FCFLAGS := -std=legacy -g -O0 -fbacktrace -fPIC
LDFLAGS = -L. -lirif -lgfortran -lm -Wl,-rpath,'$$ORIGIN'

# IRI Fortran, including the iritest program
IRI_SRC := \
  irisub.for \
  irifun.for \
  iritec.for \
  iridreg.for \
  iriflip.for \
  igrf.for \
  rocdrift.for \
  cira.for
IRI_OBJ := $(IRI_SRC:.for=.o)
IRITEST_SRC := iritest.for
IRITEST_OBJ := $(IRITEST_SRC:.for=.o)
IRILIB := libirif.so
IRITEST := iritest

# C interface and command-line program
CLI_SRC := iri_interface.c iri.c
CLI_OBJ := $(CLI_SRC:.c=.o)
CLI := iri

all: $(IRILIB) $(IRITEST) $(CLI)

$(IRILIB): $(IRI_OBJ)
	$(FC) $(FCFLAGS) -shared $^ -o $@

$(IRITEST): $(IRI_OBJ) $(IRITEST_OBJ)
	$(FC) $(FCFLAGS) $^ -o $@

$(CLI): $(CLI_OBJ) $(IRILIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.for
	$(FC) $(FCFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(CLI)
	./$(CLI)

clean:
	rm -f $(IRI_OBJ) $(IRITEST_OBJ) $(CLI_OBJ) $(IRITEST) $(IRILIB) $(CLI)

.PHONY: all run clean
