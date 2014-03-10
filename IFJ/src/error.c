/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		error.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Chybove kody a specialni funkce vypisujici chybu. Implementace.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "debug.h"

/*
 * Chybova funkce, VRACI SE
 * Zpusob zpracovani udava argument 'action'
 * Doporuceny format:
 * cast_prekladace: duvod
 * pr:
 * error("scanner: Nedostatek pameti pro lexem!\n");
 */
void error(TErrEnum action, char *format, ...)
{
	va_list arguments;
	Tgp_info info;
	static bool enable = true; /* Povol vypis. */

	if (enable == false)
		return;

	info = get_position(GET);
	va_start(arguments, format);

	fprintf(stderr, "ifj06: Chyba: ");
	switch(action)
	{
		case RAW:
			fprintf(stderr, "\n");
			break;
		case ROWONLY:
			fprintf(stderr, "Radek %d\n", info.row);
			break;
		case ROWLEX:
			fprintf(stderr, "Radek %d: Lexem %d\n", info.row, info.lexeme);
			break;
		default:
			fprintf(stderr, "Neznamy chybovy vystup\n");
			break;
	}
	vfprintf(stderr, format, arguments);
	va_end(arguments);

#ifndef ERROR_DEBUG
	enable = false;
#endif

	return;
}

/* Konec souboru error.c */
