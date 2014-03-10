/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		stacks.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Funkce pro praci se zasobniky.
******************************************************************************/

#ifndef _STACKS_H_
#define _STACKS_H_

#include <stdbool.h>
#include "error.h"
#include "instructions.h"

/* Velikost prvni alokace */
#define DATA_ALLOC 20
#define PTR_ALLOC 4

/* Typ union pro data */
typedef union {
		int      IntVal;
		char    *StrVal;
		double   DblVal;
	} SItemD;

/* Typ union pro ukazatele */
typedef struct {
		int      BP; 		// base pointer
		TMicroInstruction * retInst; 	// navratova adresa mikroinstrukce
		int      locals;    // pocet lokalnich dat funkce (argumenty,lokalni a pomocne prom.)
	} SItemP;


/* Typ datovy zasobnik */
typedef struct {
		int top; 	// index vrcholku zasobniku
		int size; 	// velikost naalokovaneho zasobniku
		SItemD * val;	// pole hodnot
	} StackD;

/* Typ ukazatelovy zasobnik */
typedef struct {
		int top; 	// index vrcholku zasobniku
		int size; 	// velikost naalokovaneho zasobniku
		SItemP * ptr;	// pole ukazatelu
	} StackP;


/* Funkce pro praci s datovym zasobnikem */
/* ===================================== */

/*
 * Funkce inicializuje datovy zasobnik. Alokuje pamet.
 * Nelze-li alokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode stackD_init (void);

/*
 * Funkce ulozi predanou hodnotu na datovy zasobnik. Pokud je plny, realokuje jej.
 * Nelze-li realokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode pushD_value (SItemD value);

/*
 * Funkce reservuje prislusny pocet polozek na datovem zasobniku pro lokalni promenne.
 * Pokud je plny, realokuje jej. Nelze li realokovat pro nedostatek pameti
 * vraci chybovy kod.
 */
TErrCode pushD_reserve (int number);

/*
 * Funkce vyzvedne prislusny pocet polozek z datoveho zasobniku.
 */
void popD_block (int number);

/*
 * Funkce vrati polozku na datovem zasobniku danou indexem.
 */
SItemD getD_variable (int index);

/*
 * Funkce naplni polozku danou indexem na datovem zasobniku danou indexem.
 */
void putD_variable (int index, SItemD val);

/*
 * Funkce zrusi datovy zasobnik. Dealokuje pamet.
 */
void stackD_dispose (void);

/*
 * Funkce vrati vrcholek zasobniku.
 */
int stackD_top (void);



/* Funkce pro praci s ukazatelovym zasobnikem */
/* ========================================== */

/*
 * Funkce inicializuje ukazatelovy zasobnik. Alokuje pamet.
 * Nelze-li alokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode stackP_init (void);

/*
 * Funkce ulozi predanou hodnotu na ukazatelovy zasobnik. Pokud je plny, realokuje jej.
 * Nelze-li realokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode pushP_value (SItemP value);

/*
 * Funkce vyzvedne polozku z vrcholku ukazateloveho zasobniku a vrati.
 */
SItemP popP_value (void);

/*
 * Funkce zrusi ukazatelovy zasobnik. Dealokuje pamet.
 */
void stackP_dispose (void);

/*
 * Funkce vrati 1 je-li zasobnik prazdny jnak vraci 0.
 */
bool stackP_is_empty (void);

#endif /* #ifndef _STACKS_H_ */

/* Konec souboru stacks.h */
