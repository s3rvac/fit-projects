/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		interpret.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Interpret instrukci - rozhrani
******************************************************************************/

#ifndef _INTERPRET_H_
#define _INTERPRET_H_

#include "code_list.h"
#include "error.h"

/*
 * Rozhraní interpretu
 */
TErrCode interpret();

#endif /* #ifndef _INTERPRET_H_ */

/* Konec souboru interpret.h */
