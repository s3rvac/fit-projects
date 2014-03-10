/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		code_list.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Struktura pro uchovani posloupnosti instrukci.
			Prace se strukturou bude probihat pouze pomoci funkci k tomu urcenych
			a neni potreba predavat strukturu jako parametr, protoze vse bude reseno
			uvnitr tohoto modulu. Implementace.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"
#include "error.h"
#include "instructions.h"
#include "code_list.h"

/* Struktura pro uchovani posloupnsti intrukci */
typedef struct {
	TMicroInstruction *first; // Pocatecni instrukce
	TMicroInstruction *last;  // Posledni instrukce (kvuli vkladani novych instr.)
	TMicroInstruction *act;   // Aktualne provadena instrukce
} TCodeList;

/*
 * Globalni ukazatel na strukturu s instrukcemi.
 * Vsechny funkce pracuji s timto ukazatelem.
 */
static TCodeList *code_list;

/****** FUNKCE *******/

/*
 * Inicializace. Pri nedostatku pameti vraci ERR_FAILURE.
 */
TErrCode code_list_init(void)
{
	code_list = malloc(sizeof(*code_list));
	if (!code_list) {
		error(RAW, "Instrukcni list: Nedostatek pameti.\n");
		return ERR_FAILURE;
	}

	code_list->first = code_list->last = code_list->act = NULL;
	return ERR_SUCCESS;
}

/*
 * Pokud je seznam prazdny (neobsahuje ani jednu intrukci), vraci true.
 */
inline bool code_list_empty(void)
{
	assert(code_list != NULL);

	return (code_list->first == NULL);
}

/*
 * Vraci ukazatel na prvni instrukci. Pokud neexistuje vraci NULL
 */
inline TMicroInstruction *code_list_head(void)
{
	assert(code_list != NULL);

	return code_list->first;
}


/*
 * Pokud existuje aktualni intrukce, vraci na ni ukazatel, jinak NULL.
 */
inline TMicroInstruction *code_list_act(void)
{
	assert(code_list != NULL);

	return code_list->act;
}

/*
 * Zruseni vsech polozek a uvolneni pameti - stejny stav, jako po inicializaci.
 */
void code_list_clean(void)
{
	assert(code_list != NULL);

	TMicroInstruction *to_delete;
	TMicroInstruction *tmp = code_list->first;
	while (tmp != NULL) {
		to_delete = tmp;
		tmp = tmp->next;
		free(to_delete);
	}

	free(code_list);
	code_list = NULL;
}

/*
 * Zarazeni nove instrukce do kodu. Nic nealokuje.
 */
void code_list_add_instr(TMicroInstruction *instr)
{
	assert(code_list != NULL);

#ifdef CHATTY_GENERATOR
	// Ukecany generator instrukci - vypisuje instrukce na stderr
	PrintInstruction(instr);
#endif//def CHATTY_GENERATOR

	if (code_list->last == NULL) {
		// Seznam instrukci je prazdny, toto bude prvni
		code_list->last = code_list->first = code_list->act = instr;
	}
	else {
		// Pridame dalsi intrukci na konec
		code_list->last->next = instr;
		code_list->last = instr;
		code_list->act = instr;
	}

	instr->next = NULL;
	return;
}

/*
 * Nastaveni aktualni instrukce na jinou. Pouziva se napriklad pri skoku.
 */
inline void code_list_set_addr(TMicroInstruction *instr)
{
	assert(code_list != NULL && instr != NULL);

	code_list->act = instr;
}

/*
 * Prejde na dalsi instrukci. Pokud neexistuje vraci NULL, jinak adresu instrukce,
 * na kterou jsme se prave dostali.
 */
TMicroInstruction *code_list_next(void)
{
	assert(code_list != NULL);

	if (code_list->act == NULL) {
		// Instrukcni seznam je prazdny nebo jsme na konci
		return NULL;
	}

	// Presun
	code_list->act = code_list->act->next;
	return code_list->act;
}

/* Konec souboru code_list.c */
