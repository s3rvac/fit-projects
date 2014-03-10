/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		functions.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Vestavene funkce, ktere jsou primo vyuzitelne v interpretu
******************************************************************************/

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "symbol_table.h"
#include "error.h"

/*
 * Vycet vestavenych funkci pro mikroinstrukci vnitřního kódu RUN_STD
 * Jednotlive prvky vyctu nazvem odpovidaji funkci kterou representuji
 */
typedef enum {
	INT2STR = 0,
	DOUBLE2STR,
	STR2DOUBLE,
	DOUBLE2INT,
	PRINTINT,
	PRINTDOUBLE,
	PRINTSTRING,
	READINT,
	READDOUBLE,
	READSTRING,
	STRLEN,
	SORT
} EFunction;

/*
 * Vlozi vsechny vestavene funkce do globalni tabulky symbolu.
 */
TErrCode insert_all_builtin_functions(TSymtable *global);

/*
 * Prevede hodnotu typu int na string.
 */
char *int2str(int);

/*
 * Prevede hodnotu typu double na string.
 */
char *double2str(double);

/*
 * Prevede retezec na double.
 */
TErrCode str2double(const char *, double *);

/*
 * Prevede hodnotu typu double na int s vyuzitim funkce trunc.
 */
TErrCode double2int(double , int *);

/*
 * Vypise hodnotu typu int na vystup.
 */
void printint(int);

/*
 * Vypise hodnotu typu double na vystup.
 */
void printdouble(double);

/*
 * Vypise hodnotu typu string na vystup.
 * Pravidla:
 * Rozpoznava escape sekvence '\n', '\\', '\"', '\$'
 * Ignoruje '$', ostatni escape sekvence
 * Jiny znak tiskne
 */
TErrCode printstring(const char *);

/*
 * Nacte ze vstupu hodnotu typu int.
 */
TErrCode readint(int *);

/*
 * Nacte ze vstupu hodnotu typu double.
 */
TErrCode readdouble(double *);

/*
 * Nacte ze vstupu hodnotu typu string.
 */
char *readstring(void);

/*
 * Vraci delku zadaneho retezce.
 */
int bstrlen(const char *);

#endif /* #ifndef _FUNCTIONS_H_ */

/* Konec souboru functions.h */
