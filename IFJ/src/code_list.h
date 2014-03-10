/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		code_list.h
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
			uvnitr tohoto modulu. Rozhrani.
******************************************************************************/

#ifndef _CODE_LIST_H_
#define _CODE_LIST_H_

#include <stdbool.h>
#include "error.h"
#include "instructions.h"

/*
 * Inicializace. Pri nedostatku pameti vraci ERR_FAILURE.
 */
TErrCode code_list_init(void);

/*
 * Pokud je seznam prazdny (neobsahuje ani jednu intrukci), vraci true.
 */
inline bool code_list_empty(void);

/*
 * Vraci ukazatel na prvni instrukci. Pokud neexistuje vraci NULL
 */
inline TMicroInstruction *code_list_head(void);

/*
 * Pokud existuje aktualni intrukce, vraci na ni ukazatel, jinak NULL.
 */
inline TMicroInstruction *code_list_act(void);

/*
 * Zruseni vsech polozek a uvolneni pameti - stejny stav, jako po inicializaci.
 */
void code_list_clean(void);

/*
 * Zarazeni nove instrukce do kodu. Nic nealokuje.
 */
void code_list_add_instr(TMicroInstruction *instr);

/*
 * Nastaveni aktualni instrukce na jinou. Pouziva se napriklad pri skoku.
 */
inline void code_list_set_addr(TMicroInstruction *instr);

/*
 * Prejde na dalsi instrukci. Pokud neexistuje vraci NULL, jinak adresu instrukce,
 * na kterou jsme se prave dostali.
 */
TMicroInstruction *code_list_next(void);

#endif /* #ifndef _CODE_LIST_H_ */

/* Konec souboru code_list.h */
