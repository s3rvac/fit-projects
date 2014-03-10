#
# Project: Serial-TCP/IP bridge
# Author:  Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile project:       make
#   - compile testing unit:  make test
#   - create documentation:  make docs
#   - clean:                 make clean
#   - pack (zip):            make pack
#

# Debugging enabled (1)/disabled (0)
DEBUG = 0

# Program
PROG = bridge

# Testing program
TESTPROG = tester

# Source directory
SRCDIR = src

# Object files directory
OBJDIR = obj

# Object files
OBJS = $(OBJDIR)/base_exception.o \
	$(OBJDIR)/config.o \
	$(OBJDIR)/config_parser.o \
	$(OBJDIR)/connection.o \
	$(OBJDIR)/conversions.o \
	$(OBJDIR)/debug.o \
	$(OBJDIR)/error.o \
	$(OBJDIR)/io.o \
	$(OBJDIR)/serial_config.o \
	$(OBJDIR)/serial_connection.o \
	$(OBJDIR)/shared_buffer.o \
	$(OBJDIR)/tcpip_config.o \
	$(OBJDIR)/tcpip_connection.o \
	$(OBJDIR)/tcpip_server.o \
	$(OBJDIR)/threading_error.o \
	$(OBJDIR)/validations.o

# C++ compiler&linker
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++98 -pedantic -Wall -W \
	-D_REENTRANT -D_POSIX_C_SOURCE=200112L
ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -DDEBUG
endif

# Linker flags
LDFLAGS = -lpthread

.PHONY : all clean pack

.DEFAULT : all

# Rules
all: depend $(PROG)

depend: $(SRCDIR)/*.cc $(SRCDIR)/*.h
	$(CXX) -MM $(SRCDIR)/*.cc > depend

# Objects
$(OBJDIR)/%.o : $(SRCDIR)/%.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

# Main program
bridge: $(OBJS) $(SRCDIR)/bridge.cc depend
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o bridge $(OBJS) $(SRCDIR)/bridge.cc

# Testing program (CPPUnit >= 1.12 must be installed properly!)
test: $(OBJS) $(SRCDIR)/tester.cc depend
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lcppunit -o $(TESTPROG) $(OBJS) $(SRCDIR)/tester.cc

# Documentation
docs:
	make -C ./doc/

# Cleaning
clean:
	rm -f $(PROG) $(TESTPROG) $(OBJDIR)/*.o depend

# Packing
pack:
	zip -r xzemek02.zip src doc obj Makefile bridge.cfg COPYING README

# End of file
