

CC=gcc
#CFLAGS=-DSAFETY

EXEC=pan

SPIN=../spin/spin

GLOBAL_INCLUDE=for.h

all: channels nfa construction

channels: channels.pml channels.prp $(GLOBAL_INCLUDE)
	$(SPIN) -a -F channels.prp channels.pml
	$(CC) $(CFLAGS) -o $(EXEC) pan.c
	./$(EXEC) -f -a -m10000000

nfa: nfa.pml $(GLOBAL_INCLUDE)
	for i in `seq 1 20` ; do  $(SPIN) -n`cat /dev/urandom | head -c 1 | hexdump -d | head -n 1 | sed 's/[^[:digit:]]//g'`1  -T nfa.pml | head -n 1 ; done ;

construction: construction.pml construction.prp $(GLOBAL_INCLUDE)
	$(SPIN) -a -F construction.prp construction.pml
	$(CC) $(CFLAGS) -o $(EXEC) pan.c
	./$(EXEC) -f -a -m10000000
