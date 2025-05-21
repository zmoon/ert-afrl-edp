CC=gcc
CFLAGS=-g -std=c99 -Wall -Werror -O2 -lm
SRCDIR=src
BINDIR=bin
TARGETS=$(BINDIR)/g2r $(BINDIR)/r2g $(BINDIR)/test

all: prepare $(TARGETS)

prepare:
	@mkdir -p $(BINDIR)

$(BINDIR)/g2r: $(SRCDIR)/g2r.c $(SRCDIR)/lib.c $(SRCDIR)/lib.h
	$(CC) $(SRCDIR)/g2r.c $(SRCDIR)/lib.c -o $@ $(CFLAGS)

$(BINDIR)/r2g: $(SRCDIR)/r2g.c $(SRCDIR)/lib.c $(SRCDIR)/lib.h
	$(CC) $(SRCDIR)/r2g.c $(SRCDIR)/lib.c -o $@ $(CFLAGS)

$(BINDIR)/test: $(SRCDIR)/test.c $(SRCDIR)/lib.c $(SRCDIR)/lib.h
	$(CC) $(SRCDIR)/test.c $(SRCDIR)/lib.c -o $@ $(CFLAGS)

clean:
	rm -rf $(BINDIR)

.PHONY: all clean prepare
