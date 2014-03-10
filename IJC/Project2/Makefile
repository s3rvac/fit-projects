#
# Makefile - GNU - gcc
# Project: IJC - DU2
# Petr Zemek, FIT, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile:			make
#   - clean:			clean
#	- pack:				pack
#

# Meta data
NAME = IJC-DU2
VERSION = 1.0

# C/C++ compiler&linker
CC = gcc
CPP = g++

# Flags
CFLAGS = -std=c99 -pedantic -Wall -W -O2
CPPFLAGS = -std=c++98 -pedantic -Wall -W -O2

# Archiver and indexer
AR = /usr/bin/ar
RANLIB = /usr/bin/ranlib

# Library's object files
LIB_OBJS = htable_init.o htable_clear.o htable_free.o htable_hash.o htable_lookup.o htable_print.o

# Library's archive file
LIB_FILE = htable.a

# Library to use when linking the main program
LIBS = -L. htable.a

# Program's object files
PROG_1_OBJS = wordcount.o io.o
PROG_2_OBJS = tail.o
PROG_3_OBJS = tail2.o

# Program's executable files
PROG_1 = wordcount
PROG_2 = tail
PROG_3 = tail2

# Rules
all: $(LIB_FILE) $(PROG_1) $(PROG_2) $(PROG_3)
	
# -- Wordcount --
$(PROG_1): $(PROG_1_OBJS)
	$(CC) $(CFLAGS) $(PROG_1_OBJS) $(LIBS) -static -o $(PROG_1)

# Create library
$(LIB_FILE): $(LIB_OBJS)
	$(AR) rc $(LIB_FILE) $(LIB_OBJS)
	$(RANLIB) $(LIB_FILE)

# Compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# -- Tail --
$(PROG_2): $(PROG_2_OBJS)
	$(CC) $(CFLAGS) $(PROG_2_OBJS) -o $(PROG_2)

# -- Tail2 --
$(PROG_3): $(PROG_3_OBJS)
	$(CPP) $(CPPFLAGS) $(PROG_3_OBJS) -o $(PROG_3)

# Packing
pack:
	zip xzemek02.zip *.c *.cc *.h Makefile 
	
# Cleaning
clean:
	rm -f *.o $(PROG_1) $(PROG_2) $(PROG_3) $(LIB_FILE)
 