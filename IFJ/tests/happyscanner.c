/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		happyscanner.c
	Projekt:	IFJ06 - test

	Popis: Testovani lexikalniho analyzatoru (scanneru)
******************************************************************************/

#include <stdio.h>
#include "../src/scanner.h"
#include "../src/token_debug.h"
#include "../src/strptr_stack.h"

/*
 * Vypise pouziti programu.
 */
void usage(char *program)
{
	printf("%s soubor_s_programem_v_ifj06\n", program);
}

/*
 * Main
 */
int main(int argc, char *argv[])
{
	FILE *fr;
	Ttoken tok;

	// Kontrola vstupnich parametru
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	printf("-->%s<--\n", argv[1]);

	if ((fr = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Soubor %s se nepodarilo otevrit.\n", argv[1]);
		return 1;
	}

	strptr_stack_init();
	// Nacitani tokenu az do konce souboru
	while ((tok = get_next_token(fr)),tok.type != TOK_EOF && tok.type != TOK_SCANNERERROR) {
  		print_token(tok);
	}

	if (fclose(fr) == EOF) {
		fprintf(stderr, "Soubor %s se nepodarilo zavrit.\n", argv[1]);
		return 1;
	}
 	strptr_stack_free();

	return 0;
}

/* Konec souboru happyscanner.c */
