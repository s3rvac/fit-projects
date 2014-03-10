#
# Project: Client/server file transfer
# Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile:                  make
#   - clean:                    clean
#   - pack (gzip):              pack
#

# C++ compiler&linker
CXX = g++

# Flags
CXXFLAGS = -std=c++98 -pedantic -Wall -W -g

# Programs
PROGS = server client

# Object files
OBJS =  common.o Client.o Server.o Protocol.o

.PHONY : all clean pack

.DEFAULT : all

# Rules
all: depend $(PROGS)

depend: *.cc *.h
	$(CXX) -MM *.cc > depend

# Objects
%.o : %.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

# Main program
server: $(OBJS) server.cc depend
	$(CXX) $(CXXFLAGS) -o server $(OBJS) server.cc
# Main program
client: $(OBJS) server.cc depend
	$(CXX) $(CXXFLAGS) -o client $(OBJS) client.cc

# Cleaning
clean:
	rm -f $(PROGS) *.o depend

# Packing
pack:
	make clean
	tar -cf xzemek02.tar Makefile *.cc *.h test.sh README protocol.pdf ./_server/* ./_client/*
	gzip xzemek02.tar
