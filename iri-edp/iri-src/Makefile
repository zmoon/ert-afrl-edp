FC := gfortran
FCFLAGS := -std=legacy -fPIC -g -fbacktrace

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

all: $(PROGRAM) $(LIBRARY)

$(PROGRAM): $(OBJS) $(OBJS_TEST)
	$(FC) $(FCFLAGS) $^ -o $@

$(LIBRARY): $(OBJS)
	$(FC) $(FCFLAGS) -shared $^ -o $@

%.o: %.for
	$(FC) $(FCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(PROGRAM) $(LIBRARY)

.PHONY: all clean
