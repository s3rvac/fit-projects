/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		debug.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Zapnuti/vypnuti ladicich informaci, ladici symboly a knihovny
******************************************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_

// Klasicky assert
#define NDEBUG
#include <assert.h>

// Pokud je ERROR_DEBUG definovane, fce error() vypisuje chyby i vicekrat
//#define ERROR_DEBUG

/*
 * Vlastni ladici symboly pro jednotlive moduly
 */

// Scanner
#define SCANNER_DEBUG 0

// Parser
//#define PREC_SA_DEBUG
//#define PRED_SA_DEBUG

// Ukecany generator instrukci - vypisuje instrukce na stderr
//#define CHATTY_GENERATOR

// Interpret
//#define INTERPRET_DEBUG
//#define CHATTY_INTERPRET

//#define PRINTINSTR
/* Ladici vypisy instrukci - include jen kdyz je potreba */
#ifdef PRINTINSTR
#include "instructions.h"
void PrintInstruction(TMicroInstruction *instr);
void PrintInstructionList(TMicroInstruction *instr);
#endif /* def PRINTINSTR */

#endif /* #ifndef _DEBUG_H_ */

/* Konec souboru debug.h */
