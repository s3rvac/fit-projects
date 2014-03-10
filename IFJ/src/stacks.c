/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		stacks.c
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

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "stacks.h"

/* Instance zasobniku pro data*/
StackD StackData;
/* Instance zasobniku pro data*/
StackP StackPtr;


/* Funkce pro praci s datovym zasobnikem */
/* ===================================== */

/*
 * Funkce inicializuje datovy zasobnik. Alokuje pamet.
 * Nelze-li alokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode stackD_init (void)
{
	/* Inicializace */
  	StackData.top = 0;

  	/* Alokace */
	StackData.val = malloc(DATA_ALLOC * sizeof(SItemD));
  	if (StackData.val == NULL)
  	{ // chyba alokace
    		error(RAW, "Datovy zasobnik: Nedostatek pameti\n");
		return ERR_LOWMEM;
	}

	StackData.size = DATA_ALLOC; // velikost zasobniku

	return ERR_SUCCESS;
}

/*
 * Funkce ulozi predanou hodnotu na datovy zasobnik. Pokud je plny, realokuje jej.
 * Nelze-li realokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode pushD_value (SItemD value)
{
	/* Aserce datoveho pole */
	assert(StackData.val != NULL);

	/* Test na plny zasobnik */
	if (StackData.top >= StackData.size)
	{ // zasobnik je plny - realokace
		StackData.size *= 2; // zvetsime velikost zasobniku dvakrat
		StackData.val = realloc(StackData.val, StackData.size * sizeof(SItemD));
		if (StackData.val == NULL)
  		{ // chyba alokace
    			error(RAW, "Datovy zasobnik: Nedostatek pameti\n");
			return ERR_LOWMEM;
		}
	}

	StackData.val[StackData.top] = value; // ulozime hodnotu na zasobnik
	(StackData.top)++; // zvysime vrcholek

	return ERR_SUCCESS;
}

/*
 * Funkce reservuje prislusny pocet polozek na datovem zasobniku pro lokalni promenne.
 * Pokud je plny, realokuje jej. Nelze li realokovat pro nedostatek pameti
 * vraci chybovy kod.
 */
TErrCode pushD_reserve (int number)
{
	/* Aserce datoveho pole */
	assert(StackData.val != NULL);

	/* Test na prilis plny zasobnik */
	while (StackData.top + number > StackData.size)
	{ // zasobnik je stale prilis plny - realokace
		StackData.size *= 2; // zvetsime velikost zasobniku dvakrat
		StackData.val = realloc(StackData.val, StackData.size * sizeof(SItemD));
		if (StackData.val == NULL)
  		{ // chyba alokace
    			error(RAW, "Datovy zasobnik: Nedostatek pameti\n");
			return ERR_LOWMEM;
		}
	}
	int i = StackData.top; // pomocna promenna
	StackData.top += number; // zvysime vrcholek o pocet reservovanych polozek
	for ( ; i < StackData.top; i++) // inicializace ukazatelu na retezce
	  StackData.val[i].StrVal = NULL;
	return ERR_SUCCESS;
}

/*
 * Funkce vyzvedne prislusny pocet polozek z datoveho zasobniku.
 */
void popD_block (int number)
{
	/* Test poctu polozek v zasobniku */
	assert(number <= StackData.top);

	StackData.top -= number; // snizime vrcholek o pocet reservovanych polozek

	return;
}

/*
 * Funkce vrati polozku na datovem zasobniku danou indexem.
 */
SItemD getD_variable (int index)
{
	/* Test poctu polozek v zasobniku */
	assert(index < StackData.top);

	return StackData.val[index]; // vracime pozadovanou hodnotu
}

/*
 * Funkce naplni polozku danou indexem na datovem zasobniku danou indexem.
 */
void putD_variable (int index, SItemD val)
{
	/* Test poctu polozek v zasobniku */
	assert(StackData.val != NULL && index < StackData.top);

	StackData.val[index] = val; // priradime hodnotu prislusne polozce

	return;
}

/*
 * Funkce zrusi datovy zasobnik. Dealokuje pamet.
 */
void stackD_dispose (void)
{
  	free(StackData.val); // uvolnime pamet
	return;
}

/*
 * Funkce vrati vrcholek zasobniku.
 */
int stackD_top (void)
{
	return StackData.top;
}

/* Funkce pro praci s ukazatelovym zasobnikem */
/* ========================================== */

/*
 * Funkce inicializuje ukazatelovy zasobnik. Alokuje pamet.
 * Nelze-li alokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode stackP_init (void)
{
	/* Inicializace */
  	StackPtr.top = 0;

  	/* Alokace */
	StackPtr.ptr = malloc(PTR_ALLOC * sizeof(SItemP));
  	if (StackPtr.ptr == NULL)
  	{ // chyba alokace
    		error(RAW, "Ukazatelovy zasobnik: Nedostatek pameti\n");
		return ERR_LOWMEM;
	}

	StackPtr.size = PTR_ALLOC;

	return ERR_SUCCESS;
}

/*
 * Funkce ulozi predanou hodnotu na ukazatelovy zasobnik. Pokud je plny, realokuje jej.
 * Nelze-li realokovat pro nedostatek pameti vraci chybovy kod
 */
TErrCode pushP_value (SItemP value)
{
	/* Aserce datoveho pole */
	assert(StackPtr.ptr != NULL);

	/* Test na plny zasobnik */
	if (StackPtr.top >= StackPtr.size)
	{ // zasobnik je plny - realokace
		StackPtr.size *= 2; // zvetsime velikost zasobniku dvakrat
		StackPtr.ptr = realloc(StackPtr.ptr, StackPtr.size * sizeof(SItemP));
		if (StackPtr.ptr == NULL)
  		{ // chyba alokace
    			error(RAW, "Ukazatelovy zasobnik: Nedostatek pameti\n");
			return ERR_LOWMEM;
		}
	}

	StackPtr.ptr[StackPtr.top] = value; // ulozime hodnotu na zasobnik
	StackPtr.top++; // zvysime vrcholek

	return ERR_SUCCESS;
}

/*
 * Funkce vyzvedne polozku z vrcholku ukazateloveho zasobniku a vrati.
 */
SItemP popP_value (void)
{
	/* Test poctu polozek v zasobniku */
	assert(StackPtr.top != 0);


	StackPtr.top--; // snizime vrcholek o 1

	return StackPtr.ptr[StackPtr.top]; // vracime polozku z vrcholku zasobniku
}

/*
 * Funkce zrusi ukazatelovy zasobnik. Dealokuje pamet.
 */
void stackP_dispose (void)
{
  	free(StackPtr.ptr); // uvolnime pamet
	return;
}

/*
 * Funkce vrati true je-li zasobnik prazdny jinak vraci false.
 */
inline bool stackP_is_empty (void)
{
	return (StackPtr.top == 0);
}

/* Konec souboru stacks.c */
