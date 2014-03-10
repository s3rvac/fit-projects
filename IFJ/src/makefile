#
# Makefile - GNU - gcc
# Project: IFJ06
# Authors:
# 	Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
#	Libor Polcak, xpolca03@stud.fit.vutbr.cz
#	Boris Prochazka, xproch63@stud.fit.vutbr.cz
#	Martin Tomec, xtomec05@stud.fit.vutbr.cz
#	Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#  - compile project:                  make
#  - clean tmp files:                  make clean
#  - compress (pack):                  make pack
#

# C compiler&linker
CC = gcc

# Flags
CFLAGS = -std=c99 -pedantic -Wall -W

# Programs
PROG = ifj06

# Object files
OBJS = debug.o strptr_stack.o shellsort.o hashtable.o symbol_table.o symtable_stack.o error.o token_debug.o code_list.o scanner.o parser.o predictive_sa.o precedence_sa.o parser_semantic.o interpret.o instructions.o generator.o functions.o stacks.o


.PHONY : all clean pack

.DEFAULT : all

# Rules
all: depend $(PROG)

depend: *.c *.h
	gcc -MM *.c > depend

-include depend

# Objects
%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

# Main program
ifj06: $(OBJS) ifj06.c depend
	$(CC) $(CFLAGS) -lm -o $(PROG) $(OBJS) $(PROG).c

# Cleaning
clean:
	rm -f $(PROG) *.o depend

# Packing
pack:
	make clean
	zip -r xzemek02.zip *.c *.h dokumentace.pdf makefile rozdeleni
