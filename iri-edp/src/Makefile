# Makefile for IRI profile generator (Ubuntu Linux)

# Compilers and flags
CC = gcc
FC = gfortran
CFLAGS = -Wall -g -I.
FCFLAGS = -std=legacy -fPIC
LDFLAGS = -L../iri-src -liri -lgfortran -lm

# Source files
SRC = iri_profile.c iri_interface.c
OBJ = $(SRC:.c=.o)
EXEC = iri_profile

# Targets
all: $(EXEC)

$(EXEC): $(OBJ) ../iri-src/libiri.so
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

../iri-src/libiri.so:
	$(MAKE) -C ../iri-src libiri.so

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	LD_LIBRARY_PATH=../iri-src ./$(EXEC)

install: $(EXEC)
	mkdir -p $(DESTDIR)/usr/local/bin
	install -m 755 $(EXEC) $(DESTDIR)/usr/local/bin
	install -m 755 run_iri_profile.sh $(DESTDIR)/usr/local/bin
	mkdir -p $(DESTDIR)/usr/local/lib
	install -m 644 ../iri-src/libiri.so $(DESTDIR)/usr/local/lib
	ldconfig

.PHONY: all clean run install
