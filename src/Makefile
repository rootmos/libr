CC ?= cc
LD ?= $(CC)
CFLAGS = -Wall -Werror -I../include

EXTRA_CFLAGS ?= -O2
EXTRA_LDFLAGS ?=

build: libr.a

libr.a: util.o logging.o fail.o mark.o stopwatch.o xorshift.o test.o
	ar -cr $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $<

clean:
	rm -rf *.o *.a

.PHONY: build clean
