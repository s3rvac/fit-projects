#
# Makefile - GNU - gcc
# Project: IOS - Project 2
# Authors:  Petr Zemek, xzemek02@stud.fit.vutbr.cz 
#           Michal Ingeli, xingel00@stud.fit.vutbr.cz 
#           Ivan Vogel, xvogel01@stud.fit.vutbr.cz   
#
# Usage:
#   - compile:			make
#   - clean:			clean
#	- pack:				pack
#

# Meta data
NAME = IOS-PROJ2
VERSION = 1.0

# C compiler&linker
CC = gcc

# Flags
CFLAGS = -std=gnu99 -pedantic -Wall -W -g

# Rules
all: proj2

proj2: proj2.o
	$(CC) $(CFLAGS) -o proj2 proj2.o 

# Packing
pack:
	tar -cvf xzemek02.tar *.c Makefile 
	
# Cleaning
clean:
	rm -f *.o proj2
 