/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		parser_semantic.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Wrapper pro get_next_token() a dalsi funkce
			potrebne pri semanticke analyze - rozhrani.
******************************************************************************/

#ifndef _PARSER_SEMANTIC_H_
#define _PARSER_SEMANTIC_H_

#include "parser_define.h"
#include "data_type.h"
#include "token.h"
#include "symbol_table.h"

/*
 * Zjisti nazev funkce, jejiz deklaraci/definici provadime (nebo jsme zrovna provedli)
 */
inline const char *get_current_func_name(void);

/*
 * Zjisti nazev posledne volane funkce (v tele jine funkce)
 */
inline const char *get_last_called_func_name(void);

/*
 * Vrati posledni nacteny token
 */
inline Ttoken get_last_token(void);

/*
 * Zjisti index a datovy typ promenne a vrati ERR_SUCCESS, pokud byla nalezena
 * (a vysledek ulozi do index a data_type) nebo v pripade neuspechu vrati ERR_FAILURE.
 */
TErrCode get_var_data(Ttoken tok, int *index, TDataType *data_type, TSymtable *global_st, TSymtable *local_st);

/*
 * Prevede datovy typ v tokenu na datovy typ z TDataType
 */
TDataType tok2data_type(Ttoken tok);

/*
 * Wrapper pro get_next_token() - vklada symboly do TS a kontroluje deklarace/definice
 */
int gnt_iist(FILE *stream, TPhases *phase, TSymtable *global_st, TSymtable *local_st, Ttoken *tok);

#endif /* #ifndef _PARSER_SEMANTIC_H_ */

/* Konec souboru parser_semantic.h */
