CC = gcc
LD = $(CC)
CFLAGS = -Wall -Werror -I../include
LDFLAGS = -L../src

EXTRA_CFLAGS ?= -O2
EXTRA_LDFLAGS ?=

LIBS = -l:libr.a -lgsl -lgslcblas -lm

TIMEOUT ?= 1s

build: main

test: main
	timeout --preserve-status $(TIMEOUT) ./$<

main: main.o xorshift.o
	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $<

clean:
	rm -rf main *.o

.PHONY: build test clean
