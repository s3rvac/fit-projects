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
#   - compile project:			make
#   - clean tmp files:			make clean
#	- run tests:				make test
#	- compress (pack)			make pack
#

# C compiler&linker
CC = gcc

# Flags
CFLAGS = -std=c99 -pedantic -Wall -W -g

# Programs
PROGS = ifj06 sort-alone happyscanner parser

# Rules
all: $(PROGS)

# Main program
ifj06: ./obj/shellsort.o ./obj/hashtable.o ./obj/symbol_table.o ./obj/symtable_stack.o ./obj/error.o ./obj/token_debug.o ./obj/code_list.o ./obj/scanner.o ./obj/parser.o ./obj/predictive_sa.o ./obj/precedence_sa.o ./obj/parser_semantic.o ./obj/strptr_stack.o ./obj/interpret.o ./obj/generator.o ./obj/functions.o ./obj/stacks.o ./obj/debug.o
	$(CC) $(CFLAGS) -lm -o ifj06 ./obj/debug.o ./obj/shellsort.o ./obj/hashtable.o ./obj/symbol_table.o ./obj/symtable_stack.o ./obj/error.o ./obj/token_debug.o ./obj/code_list.o ./obj/scanner.o ./obj/parser.o ./obj/predictive_sa.o ./obj/precedence_sa.o ./obj/parser_semantic.o ./obj/strptr_stack.o ./obj/interpret.o ./obj/instructions.o ./obj/generator.o ./obj/functions.o ./obj/stacks.o ./src/ifj06.c
# Sorting
sort-alone: ./src/shellsort.h ./tests/sort-alone.c ./obj/shellsort.o ./obj/strptr_stack.o ./obj/error.o ./obj/scanner.o
	$(CC) $(CFLAGS) -o ./tests/sort-alone ./tests/sort-alone.c ./obj/shellsort.o ./obj/strptr_stack.o ./obj/error.o ./obj/scanner.o
# Scanner
happyscanner: ./obj/error.o ./obj/token_debug.o ./obj/strptr_stack.o ./obj/scanner.o
	$(CC) $(CFLAGS) -o ./tests/happyscanner ./obj/error.o ./obj/token_debug.o ./obj/strptr_stack.o ./obj/scanner.o ./tests/happyscanner.c
# Parser
parser: ./obj/shellsort.o ./obj/scanner.o ./obj/token_debug.o ./obj/strptr_stack.o ./obj/predictive_sa.o ./obj/precedence_sa.o ./obj/error.o ./obj/hashtable.o ./obj/symbol_table.o ./obj/symtable_stack.o ./obj/code_list.o ./obj/parser_semantic.o ./obj/parser.o ./obj/generator.o ./obj/functions.o
	$(CC) $(CFLAGS) -lm -o ./tests/parser ./obj/debug.o ./obj/shellsort.o ./obj/scanner.o ./obj/token_debug.o ./obj/strptr_stack.o ./obj/predictive_sa.o ./obj/precedence_sa.o ./obj/error.o ./obj/hashtable.o ./obj/symbol_table.o ./obj/symtable_stack.o ./obj/code_list.o ./obj/parser_semantic.o ./obj/parser.o ./obj/generator.o ./obj/functions.o ./tests/parser.c

stacks_test: ./obj/stacks.o
	$(CC) $(CFLAGS) -o ./tests/stack_test ./obj/stacks.o ./tests/stack_test.c

./obj/shellsort.o: ./src/shellsort.c ./src/shellsort.h ./src/debug.h
	$(CC) $(CFLAGS) -o ./obj/shellsort.o -c ./src/shellsort.c
./obj/hashtable.o: ./src/hashtable.c ./src/hashtable.h ./src/debug.h ./src/instructions.h ./src/data_type.h
	$(CC) $(CFLAGS) -o ./obj/hashtable.o -c ./src/hashtable.c
./obj/symbol_table.o: ./src/symbol_table.c ./src/symbol_table.h ./src/hashtable.h ./src/debug.h ./src/data_type.h ./src/constants.h ./src/error.h ./src/generator.h ./src/symtable_stack.h
	$(CC) $(CFLAGS) -o ./obj/symbol_table.o -c ./src/symbol_table.c
./obj/symtable_stack.o: ./src/symtable_stack.c ./src/symtable_stack.h ./src/error.h ./src/debug.h
	$(CC) $(CFLAGS) -o ./obj/symtable_stack.o -c ./src/symtable_stack.c
./obj/error.o: ./src/error.c ./src/error.h ./src/scanner.h
	$(CC) $(CFLAGS) -o ./obj/error.o -c ./src/error.c
./obj/token_debug.o: ./src/token_debug.c ./src/token.h ./src/token_debug.h
	$(CC) $(CFLAGS) -o ./obj/token_debug.o -c ./src/token_debug.c
./obj/code_list.o: ./src/code_list.c ./src/code_list.h
	$(CC) $(CFLAGS) -o ./obj/code_list.o -c ./src/code_list.c
./obj/scanner.o: ./src/scanner.c ./src/scanner.h ./src/debug.h ./src/error.h ./src/token.h ./src/scanner_define.h
	$(CC) $(CFLAGS) -o ./obj/scanner.o -c ./src/scanner.c
./obj/parser.o: ./src/parser.c ./src/parser.h ./src/parser_define.h ./src/parser_semantic.h
	$(CC) $(CFLAGS) -o ./obj/parser.o -c ./src/parser.c
./obj/predictive_sa.o: ./src/predictive_sa.c ./src/predictive_sa.h ./src/precedence_sa.h ./src/parser_define.h ./src/debug.h ./src/token.h ./src/token_debug.h ./src/scanner.h
	$(CC) $(CFLAGS) -o ./obj/predictive_sa.o -c ./src/predictive_sa.c
./obj/precedence_sa.o: ./src/precedence_sa.c ./src/precedence_sa.c ./src/debug.h ./src/token.h ./src/token_debug.h ./src/scanner.h ./src/parser_define.h ./src/predictive_sa.h
	$(CC) $(CFLAGS) -o ./obj/precedence_sa.o -c ./src/precedence_sa.c
./obj/parser_semantic.o: ./src/parser_semantic.c ./src/parser_semantic.h
	$(CC) $(CFLAGS) -o ./obj/parser_semantic.o -c ./src/parser_semantic.c
./obj/strptr_stack.o: ./src/strptr_stack.c ./src/strptr_stack.h
	$(CC) $(CFLAGS) -o ./obj/strptr_stack.o -c ./src/strptr_stack.c
./obj/generator.o: ./src/generator.c ./src/generator.h
	$(CC) $(CFLAGS) -o ./obj/generator.o -c ./src/generator.c
./obj/functions.o: ./src/functions.c ./src/functions.h
	$(CC) $(CFLAGS) -o ./obj/functions.o -c ./src/functions.c
./obj/debug.o: ./src/debug.h ./src/debug.c
	$(CC) $(CFLAGS) -o ./obj/debug.o -c ./src/debug.c

# Interpret
./obj/interpret.o: ./obj/instructions.o ./src/interpret.c ./src/interpret.h
	$(CC) $(CFLAGS) -o ./obj/interpret.o -c ./src/interpret.c
./obj/instructions.o: ./obj/stacks.o ./src/instructions.c ./src/instructions.h
	$(CC) $(CFLAGS) -o ./obj/instructions.o -c ./src/instructions.c
./obj/stacks.o: ./obj/error.o
	$(CC) $(CFLAGS) -o ./obj/stacks.o -c ./src/stacks.c


# Testing
test:
	make
	sh ./tests/ifj06-test.sh --verbose --all

# Cleaning
clean:
	rm -f $(PROGS) ./obj/*.o
	cd ./tests/ && rm -f $(PROGS)

# Packing
pack:
	make clean
	zip -r ifj06.zip docs obj tests src Makefile
