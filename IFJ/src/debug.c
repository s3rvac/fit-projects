/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		debug.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Vypis ladicich informaci
******************************************************************************/

#include "debug.h"
#include "instructions.h"
/* Nazvy prikazu pro ladici vypisy */

#ifdef PRINTINSTR
#include <stdio.h>

/*
 * Retezce pro ladici vypisy
 */
static const char *InsStr[] = {
	[NoInstr] = "Chyba!",
	[INIT] = "INIT",
	[PUSH] = "PUSH",
	[CALL] = "CALL",
	[ALLOC] = "ALLOC",
	[RETURN] = "RETURN",
	[ERROR] = "ERROR",
	[RUN_STD] = "RUN_STD",
	[GOTO] = "GOTO",
	[GOTO_IF] = "GOTO_IF",
	[LABEL] = "LABEL",
	[MOV] = "MOV",
	[LOAD_INT] = "LOAD_INT",
	[LOAD_DBL] = "LOAD_DBL",
	[LOAD_STR] = "LOAD_STR",
	[INT2DBL] = "INT2DBL",
	[DBL2INT] = "DBL2INT",
	[STR2BOOL] = "STR2BOOL",
	[ADD] = "ADD",
	[ADD_DBL] = "ADD_DBL",
	[SUB] = "SUB",
	[SUB_DBL] = "SUB_DBL",
	[MUL] = "MUL",
	[MUL_DBL] = "MUL_DBL",
	[DIV] = "DIV",
	[DIV_DBL] = "DIV_DBL",
	[POW] = "POW",
	[POW_DBL] = "POW_DBL",
	[CONCAT] = "CONCAT",
	[CMP_LES] = "CMP_LES",
	[CMP_LES_EQ] = "CMP_LES_EQ",
	[CMP_GRE] = "CMP_GRE",
	[CMP_GRE_EQ] = "CMP_GRE_EQ",
	[CMP_EQU] = "CMP_EQU",
	[CMP_NOT_EQ] = "CMP_NOT_EQ",
	[CMP_LES_DBL] = "CMP_LES_DBL",
	[CMP_LES_EQ_DBL] = "CMP_LES_EQ_DBL",
	[CMP_GRE_DBL] = "CMP_GRE_DBL",
	[CMP_GRE_EQ_DBL] = "CMP_GRE_EQ_DBL",
	[CMP_EQU_DBL] = "CMP_EQU_DBL",
	[CMP_NOT_EQ_DBL] = "CMP_NOT_EQ_DBL",
	[CMP_LES_STR] = "CMP_LES_STR",
	[CMP_LES_EQ_STR] = "CMP_LES_EQ_STR",
	[CMP_GRE_STR] = "CMP_GRE_STR",
	[CMP_GRE_EQ_STR] = "CMP_GRE_EQ_STR",
	[CMP_EQU_STR] = "CMP_EQU_STR",
	[CMP_NOT_EQ_STR] = "CMP_NOT_EQ_STR",
};

/*
 * Vytiskne instrukci na standardni chybovy vystup
 */
void PrintInstruction(TMicroInstruction *instr)
{
	static int LineNum = 0;
	LineNum++; // pocitadlo radku mezikodu
	TMicroInstruction i = *instr; // pro prehlednost

	// tisk cisla radku a instrukce s parametry
	fprintf(stderr, "%3d.[%8p]%15s[int:%4d] [int:%4d dbl:% 7le ptr:%8p] [int:%4d]\n",
	LineNum, (void *)instr, InsStr[i.type], i.a.IntVal, i.b.IntVal, i.b.DblVal, (void *)i.b.PtrVal, i.c.IntVal );
}

/*
 * Vypise celou posloupnst instrukci
 */
void PrintInstructionList(TMicroInstruction *instr)
{
	while (instr != NULL)
	{
		PrintInstruction(instr);
		instr = instr->next;
	}
}

#endif /* #ifdef PRINTINSTR */

/* Konec souboru debug.c */
