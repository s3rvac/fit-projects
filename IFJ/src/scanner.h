/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		scanner.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Rozhranni ke scanneru
******************************************************************************/

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "token.h"

/*
 * Pokyny pro scanner - vyuzivano funkci get_position();
 */
typedef enum {
	GET,
	INC_ROW,
	INC_LEXEME
} Tgp_flag;

/*
 * Struktura funkce get_position(). Uchovava aktualni cislo radku a lexemy.
 */
typedef struct {
	int row;
	int lexeme;
} Tgp_info;

/*
 * Citac pozice scanneru v souboru.
 * GET - vraci strukturu popisujici pozici.
 * INC_ROW - inkrementuj radek, nuluje lexem, vraci pozici.
 * INC_LEXEME - inkrementuje lexem, vraci pozici.
 */
Tgp_info get_position(Tgp_flag flag);

/*
 * Vrat nasledujici token ze vstupniho souboru (wrapper fsm())
 * input - vstupni soubor/stream
 * Vzdy vraci token
 * Na konci souboru vraci TOK_EOF
 * V pripade jakekoliv chyby vypise informace a vraci TOK_SCANNERERROR
 * Typy chyb: malloc(), neznamy znak v FSM_S, chybny lexem, preteceni
 *            (neznamy stav, vyskoceni z for(;;))
 * Alokovanou pamet, kterou predava (pres ukazatel Token.attribute.ptr)
 * u tokenu typu TOK_IDENTIFIER a TOK_CONSTSTRING, _automaticky_ uklada
 * do zasobniku ukazatelu na retezce.
 */
Ttoken get_next_token(FILE *input);

/*
 * Vyvoz rozhrani pro funkce readint a readdouble
 */
inline int ismydigit(int c);
inline int ismyspace(int c);

#endif /* #ifndef _SCANNER_H_ */

/* Konec souboru scanner.h */
