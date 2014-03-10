/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		parser.c
	Projekt:	IFJ06
	Datum:
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Syntakticky a semanticky analyzator - je rozdelen na 2 casti a to prediktivni SA,
			ktera prochazi deklarace/definice funkci a paramteru a precedencni SA,
			ktera prochazi vyrazy.
			Implementace.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/parser.h"
#include "../src/parser_define.h"
#include "../src/parser_semantic.h"
#include "../src/predictive_sa.h"
#include "../src/code_list.h"
#include "../src/error.h"
#include "../src/constants.h"
#include "../src/token.h"
#include "../src/token_debug.h"
#include "../src/symbol_table.h"
#include "../src/scanner.h"
#include "../src/strptr_stack.h"

/*
 * Zkontroluje vstupni parametry a vrati popisovac souboru na zdrojovy soubor
 * nebo stdin, pokud nazev vstupniho souboru nebyl zadan. Pokud dojde k chybe,
 * dojde k vypsani chyby a ukonceni.
 */
static FILE *get_input_fd(int argc, char **argv)
{
	if (argc == 1) {
		// Bude se cist ze vstupu
		return stdin;
	}

	FILE *fd = fopen(argv[1], "r");
	if (!fd) {
		fprintf(stderr, "Nelze otevrit vstupni soubor: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	return fd;
}

/*
 * Main
 */
int main(int argc, char **argv)
{
	// Zdrojovy soubor (nebo stdin)
	FILE *stream = get_input_fd(argc, argv);

	// Ulozeni funkci strptr_stack_free(); a code_list_clean();
	// do atexit() posloupnosti, aby se na ni nezapomelo
	atexit(strptr_stack_free);
	atexit(code_list_clean);

	// Inicializace modulu pro ukladani posloupnsti instrukci
	if (code_list_init() != ERR_SUCCESS) {
		// Nedostatek pameti
		fclose(stream);
		return ERR_FAILURE;
	}

	if (parser(stream) != ERR_SUCCESS) {
		// Chyba
		if (stream != stdin)
			fclose(stream);
		return ERR_FAILURE;
	}

	// SA probehla v poradku
	printf("OK\n");

	if (stream != stdin)
			fclose(stream);

    return ERR_SUCCESS;
}

/* Konec souboru parser.c */
