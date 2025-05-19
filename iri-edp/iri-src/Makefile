FC := gfortran
FCFLAGS := -std=legacy -fPIC

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
LIBRARY := libiri.so

# Targets
.PHONY: all clean
all: $(PROGRAM) $(LIBRARY)

$(PROGRAM): $(OBJS) $(OBJS_TEST)
	$(FC) $(FCFLAGS) $^ -o $@ $(LIBS) $(INC)

$(LIBRARY): $(OBJS)
	$(FC) $(FCFLAGS) -shared $^ -o $@ $(LIBS) $(INC)

%.o: %.for
	$(FC) $(FCFLAGS) $(INC) -c $<

clean:
	rm -f *.o *.mod $(PROGRAM) $(LIBRARY)
