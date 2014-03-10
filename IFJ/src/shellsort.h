/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		shellsort.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Implementace radici funkce - Shell sort - rozhrani
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#ifndef SHELLSORT_H
#define SHELLSORT_H

/*
 * Razeni metodou Shell sort.
 * Jako parametr prijima retezec obsahujici jednotliva slova ve formatu:
 * slovo1$slovo2$slovo3...slovoN (retezec je samozrejme ukoncen '\0')
 */
char *sort(const char *str);

#endif /* #ifndef SHELLSORT_H */

/* Konec souboru shellsort.h */
