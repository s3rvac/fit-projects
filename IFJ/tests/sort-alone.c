/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		sort-alone.c
	Projekt:	IFJ06 - test

	Popis: Testovani samostatneho modulu razeni
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../src/shellsort.h"

/*
 * Main
 */
int main(int argc, char**argv)
{
	if (argc != 2)
		printf("%s\n", "");
	else
		printf("%s\n", sort(argv[1]));

    return EXIT_SUCCESS;
}

/* Konec souboru sort-alone.c */
