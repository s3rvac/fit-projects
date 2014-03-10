/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		precedence_sa.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Precedencni syntakticka analyza - cte vyrazy ze standardniho
			vstupu a vyhodnocuje je - pokud je nalezen derivacni strom,
			pak se vypise OK a pravy rozbor, jinak CHYBA a nazev tokenu + cislo radku,
			na kterem SA skoncila.
			Rozhrani.
******************************************************************************/

#ifndef _PRECEDENCE_SA_H_
#define _PRECEDENCE_SA_H_

#include <stdio.h>
#include "parser_define.h"
#include "symbol_table.h"
#include "token.h"

/*
 * Vola prcedencni syntaktickou analyzu a vraci 0, pokud probehlo vse v poradku, nebo nenulove cislo
 * v pripade chyby.
 * Popis parametru:
 *  stream - popisovac souboru se zdrojovym kodem
 *  phase - faze, ve ktere se SA zrovna nachazi
 *  global_st - globalni tabulka symbolu
 *  local_st - lokalni tabulka symbolu
 *  cur_token - aktualni token
 *  last_token - posledni nacteny token
 *  return_item - navratova hodnota z analyzy
 */
int precedence_SA(FILE *stream, TPhases *phase, TSymtable *global_st, TSymtable *local_st,
	Ttoken cur_token, Ttoken *last_token, TItem *return_item);

#endif /* #ifndef _PRECEDENCE_SA_H_ */

/* Konec souboru precedence_sa.h */
