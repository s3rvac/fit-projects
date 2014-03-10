
PRJ=c403
#
PROGS=$(PRJ)-test $(PRJ)-test2
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

$(PRJ)-test2: $(PRJ).c $(PRJ)-test2.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test2.c


clean:
	rm -f *.o *.out $(PROGS)
#