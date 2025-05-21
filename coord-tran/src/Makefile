CC=gcc
CFLAGS=-g -std=c99 -Wall -Werror -O2 -lm
BINDIR=../bin
TARGETS=$(BINDIR)/g2r $(BINDIR)/r2g $(BINDIR)/test

all: prep $(TARGETS)

prep:
	@mkdir -p $(BINDIR)

$(BINDIR)/g2r: g2r.c lib.c lib.h
	$(CC) g2r.c lib.c -o $@ $(CFLAGS)

$(BINDIR)/r2g: r2g.c lib.c lib.h
	$(CC) r2g.c lib.c -o $@ $(CFLAGS)

$(BINDIR)/test: test.c lib.c lib.h
	$(CC) test.c lib.c -o $@ $(CFLAGS)

test: prep $(BINDIR)/test
	$(BINDIR)/test

clean:
	rm -rf $(BINDIR)

.PHONY: all clean prep test
