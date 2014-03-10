#
# Project: Webclient
# Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile:                  make
#   - clean:                    clean
#   - pack (gzip):              pack
#

# Compiler&linker
CXX = g++ -std=c++98 -pedantic -Wall -W -g

# Rules
all: webclient

webclient: webclient.cc
	$(CXX) -o webclient webclient.cc

# Cleaning
clean:
	rm -f *.o webclient xzemek02.tar.gz

# Packing
pack:
	make clean
	tar -cf xzemek02.tar Makefile test.sh webclient.cc
	gzip xzemek02.tar
