#
# Makefile - GNU - gcc
# Projekt: IJC - DU1
# Petr Zemek, FIT, xzemek02@stud.fit.vutbr.cz
#
# Pouziti:
#   - preklad:			make
#   - packovani:		pack
#   - cisteni:			clean
#

NAME=ijc-du1
VERSION=1.0

CC=gcc
CFLAGS=-std=c99 -O2 -pedantic -Wall -W -g

all: enumtest prvocisla

enumtest: enumtest.o enumdny.o error.o
	$(CC) $(CFLAGS) -o enumtest enumtest.o enumdny.o error.o

prvocisla: prvocisla.o error.o
	$(CC) $(CFLAGS) -o prvocisla prvocisla.o error.o

prvocisla.o: prvocisla.c
	$(CC) $(CFLAGS) -c prvocisla.c

enumtest.o: enumtest.c enumdny.h
	$(CC) $(CFLAGS) -c enumtest.c

enumdny.o: enumdny.c enumdny.h
	$(CC) $(CFLAGS) -c enumdny.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

pack:
	zip xzemek02.zip *.c *.h Makefile

clean:
	rm -f *.o prvocisla enumtest
