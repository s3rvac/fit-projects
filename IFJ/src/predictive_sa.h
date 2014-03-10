/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		predictive_sa.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Prediktivni syntakticka analyza - cte deklarace/definice/tela funkci (mimo vyrazy)
			z predaneho popisovace souboru a vyhodnocuje je - pokud je nalezen derivacni strom,
			pak vraci 0, jinak nenulove cislo. Rozhrani.
******************************************************************************/

#ifndef _PREDICTIVE_SA_H_
#define _PREDICTIVE_SA_H_

#include <stdio.h>
#include "parser_define.h"
#include "symbol_table.h"
#include "token.h"

/*
 * Vola prediktivni syntaktickou analyzu a vraci 0, pokud vse probehlo v poradku a nenulove cislo v pripade chyby.
 * Popis parametru:
 *  stream - popisovac souboru se zdrojovym kodem
 *  phase - faze, ve ktere se SA zrovna nachazi
 *  global_st - globalni tabulka symbolu
 *  local_st - lokalni tabulka symbolu
 *  cur_token - aktualni token
 *  end_token - ukoncujici token
 *  last_token - posledni nacteny token
 *  starting_nonterm - pocatecni non-terminal, od ktereho bude SA zacinat
 *  return_item - navratova hodnota z analyzy (muze byt NULL)
 *  last_called_func - nazev posledne volane funkce
 */
int predictive_SA(FILE *stream,	TPhases *phase, TSymtable *global_st, TSymtable *local_st,
	Ttoken cur_token, Ttoken end_token, Ttoken *last_token,
	TNonTerm starting_nonterm, TItem *return_item, const char *last_called_func);

#endif /* #ifndef _PREDICTIVE_SA_H_ */

/* Konec souboru predictive_sa.h */
