/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		instructions.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Mikroinstrukce - rozhrani
******************************************************************************/

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "error.h"

/*
 * Mikroinstrukce vnitřního kódu
 * Detailnejsi popis v docs/mikroinstrukce.txt
 * Externi cast
 */
typedef enum {
NoInstr,  // prazdna instrukce
INIT,     // NUM_GLOBALS     PTR_MAIN
PUSH,     // IND_VARIABLE
CALL,     // NULL            PTR_ADRESS
ALLOC,    // NUM_LOCALS      NUM_ARGUMENTS
RETURN,   // IND_VALUE
ERROR,
RUN_STD,  // ENUM_FUNCTION
GOTO,     // NULL            PTR_LABEL
GOTO_IF,  // IND_CONDITION   PTR_LABEL
LABEL,

MOV,       // IND_DEST        IND_SRC
LOAD_INT,  // IND_VARIABLE    VALUE
LOAD_DBL,  // IND_VARIABLE    VALUE
LOAD_STR,  // IND_VARIABLE    PTR_VALUE

INT2DBL,   // IND_DEST        IND_SRC
DBL2INT,   // IND_DEST        IND_SRC
STR2BOOL,  // IND_DEST        IND_SRC

ADD,       // IND_DEST        IND_SRC1 + IND_SRC2
ADD_DBL,   // IND_DEST        IND_SRC1 + IND_SRC2
SUB,       // IND_DEST        IND_SRC1 - IND_SRC2
SUB_DBL,
MUL,       // IND_DEST        IND_SRC1 * IND_SRC2
MUL_DBL,
DIV,       // IND_DEST        IND_SRC1 / IND_SRC2 celociselne
DIV_DBL,   // IND_DEST        IND_SRC1 / IND_SRC2 presne
POW,	   // IND_DEST	      IND_SRC1 ^ IND_SRC2 -- vysledek je double
POW_DBL,

CONCAT,    // IND_DEST       IND_SRC1    IND_SRC2

CMP_LES,   // IND_DEST       IND_SRC1 <  IND_SRC2
CMP_LES_EQ,// IND_DEST       IND_SRC1 <= IND_SRC2
CMP_GRE,   // IND_DEST       IND_SRC1 >  IND_SRC2
CMP_GRE_EQ,
CMP_EQU,   // IND_DEST       IND_SRC1 == IND_SRC2
CMP_NOT_EQ,// IND_DEST       IND_SRC1 != IND_SRC2

CMP_LES_DBL,
CMP_LES_EQ_DBL,
CMP_GRE_DBL,
CMP_GRE_EQ_DBL,
CMP_EQU_DBL,
CMP_NOT_EQ_DBL,
CMP_LES_STR,
CMP_LES_EQ_STR,
CMP_GRE_STR,
CMP_GRE_EQ_STR,
CMP_EQU_STR,
CMP_NOT_EQ_STR
} EInstruction;

/*
 * Obecny typ parametru mikroinstrukce
 */
typedef union {
	int      IntVal;
	char    *StrVal;
	double   DblVal;
	struct MicroInstruction *PtrVal;
} TInstructionParam;

/* vlastni mikroinstrukce */

typedef struct MicroInstruction {
	EInstruction type; // typ mikroinstrukce

	/* parametry A a C jsou vzdy jen enumy a indexy - vzdy int
	 * unie je tu jen pro jednotnost a moznost rozsireni */
	union {
		int IntVal;
	} a, c;

	// parametr B je promenny
	TInstructionParam b;

	struct MicroInstruction *next;       // Ukazatel na další mikroinstrukci
} TMicroInstruction;

/*
 * Interni cast - pro interpret
 */

/* hlavicky funkci pro mikroinstrukce */

TErrCode do_INIT (TMicroInstruction * actInst);
TErrCode do_PUSH (TMicroInstruction * actInst);
TErrCode do_CALL (TMicroInstruction * actInst);
TErrCode do_ALLOC (TMicroInstruction * actInst);
TErrCode do_RETURN (TMicroInstruction * actInst);
TErrCode do_ERROR (TMicroInstruction * actInst);
TErrCode do_RUN_STD (TMicroInstruction * actInst);
TErrCode do_GOTO (TMicroInstruction * actInst);
TErrCode do_GOTO_IF (TMicroInstruction * actInst);
TErrCode do_LABEL (TMicroInstruction * actInst);
TErrCode do_MOV (TMicroInstruction * actInst);
TErrCode do_LOAD_INT (TMicroInstruction * actInst);
TErrCode do_LOAD_DBL (TMicroInstruction * actInst);
TErrCode do_LOAD_STR (TMicroInstruction * actInst);
TErrCode do_INT2DBL (TMicroInstruction * actInst);
TErrCode do_DBL2INT (TMicroInstruction * actInst);
TErrCode do_STR2BOOL (TMicroInstruction * actInst);
TErrCode do_ADD (TMicroInstruction * actInst);
TErrCode do_ADD_DBL (TMicroInstruction * actInst);
TErrCode do_SUB (TMicroInstruction * actInst);
TErrCode do_SUB_DBL (TMicroInstruction * actInst);
TErrCode do_MUL (TMicroInstruction * actInst);
TErrCode do_MUL_DBL (TMicroInstruction * actInst);
TErrCode do_DIV (TMicroInstruction * actInst);
TErrCode do_DIV_DBL (TMicroInstruction * actInst);
TErrCode do_POW (TMicroInstruction * actInst);
TErrCode do_POW_DBL (TMicroInstruction * actInst);
TErrCode do_CONCAT (TMicroInstruction * actInst);
TErrCode do_CMP_LES (TMicroInstruction * actInst);
TErrCode do_CMP_LES_EQ (TMicroInstruction * actInst);
TErrCode do_CMP_GRE (TMicroInstruction * actInst);
TErrCode do_CMP_GRE_EQ (TMicroInstruction * actInst);
TErrCode do_CMP_EQU (TMicroInstruction * actInst);
TErrCode do_CMP_NOT_EQ (TMicroInstruction * actInst);
TErrCode do_CMP_LES_DBL (TMicroInstruction * actInst);
TErrCode do_CMP_LES_EQ_DBL (TMicroInstruction * actInst);
TErrCode do_CMP_GRE_DBL (TMicroInstruction * actInst);
TErrCode do_CMP_GRE_EQ_DBL (TMicroInstruction * actInst);
TErrCode do_CMP_EQU_DBL (TMicroInstruction * actInst);
TErrCode do_CMP_NOT_EQ_DBL (TMicroInstruction * actInst);
TErrCode do_CMP_LES_STR (TMicroInstruction * actInst);
TErrCode do_CMP_LES_EQ_STR (TMicroInstruction * actInst);
TErrCode do_CMP_GRE_STR (TMicroInstruction * actInst);
TErrCode do_CMP_GRE_EQ_STR (TMicroInstruction * actInst);
TErrCode do_CMP_EQU_STR (TMicroInstruction * actInst);
TErrCode do_CMP_NOT_EQ_STR (TMicroInstruction * actInst);

#endif /* #ifndef INSTRUCTIONS_H */

/* Konec souboru instructions.h */
