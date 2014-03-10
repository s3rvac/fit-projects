/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		scanner_debug.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Retezce obsahujici nazvy stavu pro debugging ucely.
******************************************************************************/

#ifndef _SCANNER_DEBUG_C_
#define _SCANNER_DEBUG_C_

/*
 * Retezce znacici jednotlive stavy automatu pro ladici ucely
 */
const char *Efsm_state_name[] = {
	"FSM_S",
	"FSM_F_COMMA",
	"FSM_F_SEMICOLON",
	"FSM_F_LABEL",
	"FSM_F_ASSIGNMENT",
	"FSM_F_POWER",
	"FSM_F_PLUS",
	"FSM_F_MINUS",
	"FSM_F_MULTIPLE",
	"FSM_F_DIVDOUBLE",
	"FSM_COMMENT",
	"FSM_F_LESS",
	"FSM_F_LESSEQUAL",
	"FSM_F_GREATER",
	"FSM_F_GREATEREQUAL",
	"FSM_Q_EQUAL",
	"FSM_F_EQUAL",
	"FSM_Q_NOTEQUAL",
	"FSM_F_NOTEQUAL",
	"FSM_F_LBRACKET",
	"FSM_F_RBRACKET",
	"FSM_F_LVINCULUM",
	"FSM_F_RVINCULUM",
	"FSM_Q1_STRING",
	"FSM_Q2_STRING",
	"FSM_F_STRING",
	"FSM_F_CONSTINT",
	"FSM_F1_CONSTDOUBLE",
	"FSM_F2_CONSTDOUBLE",
	"FSM_Q1_CONSTDOUBLE",
	"FSM_Q2_CONSTDOUBLE",
	"FSM_F3_CONSTDOUBLE",
	"FSM_F_IDENTIFIERORKEYWORD"
};

#endif /* #ifndef _SCANNER_DEBUG_C_ */

/* Konec souboru scanner_debug.c */
