CC ?= gcc
CFLAGS := -g -std=c99 -Wall -Werror -O2 -lm
BINDIR := ../bin
TARGETS := $(BINDIR)/g2r $(BINDIR)/r2g $(BINDIR)/test

all: prep $(TARGETS)

prep:
	@mkdir -p $(BINDIR)

$(BINDIR)/%: %.c lib.c
	$(CC) $^ -o $@ $(CFLAGS)

test: prep $(BINDIR)/test
	$(BINDIR)/test

clean:
	rm -rf $(BINDIR)

.PHONY: all clean prep test
