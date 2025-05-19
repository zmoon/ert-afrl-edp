FC := gfortran
FCFLAGS := -std=legacy

OBJS := \
  irisub.o \
  irifun.o \
  iritec.o \
  iridreg.o \
  iriflip.o \
  igrf.o \
  rocdrift.o \
  cira.o

OBJS_TEST := \
  iritest.o

PROGRAM := iritest

# Targets
.PHONY: all clean
all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(OBJS_TEST)
	$(FC) $(FCFLAGS) $^ -o $@ $(LIBS) $(INC)

%.o: %.for
	$(FC) $(FCFLAGS) $(INC) -c $<

clean:
	rm -f *.o *.mod $(PROGRAM)

