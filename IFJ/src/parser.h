/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		parser.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Syntakticky a semanticky analyzator - je rozdelen na 2 casti
			a to prediktivni SA, ktera prochazi deklarace/definice funkci
			a paramteru a precedencni SA, ktera prochazi vyrazy.
			Rozhrani.
******************************************************************************/

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include "error.h"

/*
 * Syntakticka a semanticka analyza zdrojoveho souboru.
 * Pokud je nalezena chyba, vraci ERR_FAILURE a je vypsana chybova hlaska,
 * jinak ERR_SUCCESS. Postupne bude generovat instrukce (ty se budou ukladat do
 * modulu code_list a pak vykonavat, neni tudiz treba predavat parametr na tuto strukturu,
 * protoze bude zapouzdrena v danem modulu).
 */
TErrCode parser(FILE *input_file);

#endif /* #ifndef _PARSER_H_ */

/* Konec souboru parser.h */
