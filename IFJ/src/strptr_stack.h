/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		strptr_stack.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Rozhrani - zasobnik ukazatelu na retezce.
******************************************************************************/

#ifndef _STRPTR_STACK_H_
#define _STRPTR_STACK_H_

#include <stdlib.h>

/*
 * Struktura polozky v zasobniku.
 */
typedef struct strptr_stack_item {
	char *ptr;
	struct strptr_stack_item *next;
} Tstrptr_stack_item;

/*
 * Inicializace zasobniku.
 */
void strptr_stack_init(void);

/*
 * Pridani jiz alokovaneho retezce do zasobniku.
 * V pripade chyby (malo pameti) vraci NULL.
 * Jinak vraci ukazatel na polozku v zasobniku.
 */
Tstrptr_stack_item *strptr_stack_push(char *string);

/*
 * Alokace noveho retezce s vlozenim ukazatele do zasobniku.
 * V pripade chyby (malo pameti) vraci NULL (a zasobnik nerusi).
 */
void *strmalloc(size_t size);

/*
 * Zruseni celeho zasobniku vcetne dealokace retezcu.
 */
void strptr_stack_free();

#endif /* #ifndef _STRPTR_STACK_H_ */

/* Konec souboru strptr_stack.h */
