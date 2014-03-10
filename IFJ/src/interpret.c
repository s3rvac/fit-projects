/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		interpret.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Interpret instrukci - implementace
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "interpret.h"
#include "instructions.h"
#include "code_list.h"
#include "error.h"
#include "debug.h"
#include "stacks.h"

/*
 * Interpret
 * Code list je globalni
 */

TErrCode interpret()
{
	// pole ukazatelu na instrukce
	TErrCode (* InstructionArray[]) () = {
	do_ERROR , // pro ladici ucely je 1.instrukce nedefinovana
	do_INIT ,
	do_PUSH ,
	do_CALL ,
	do_ALLOC ,
	do_RETURN ,
	do_ERROR ,
	do_RUN_STD ,
	do_GOTO ,
	do_GOTO_IF ,
	do_LABEL ,
	do_MOV ,

	do_LOAD_INT ,
	do_LOAD_DBL ,
	do_LOAD_STR ,
	do_INT2DBL ,
	do_DBL2INT ,
	do_STR2BOOL ,

	do_ADD ,
	do_ADD_DBL ,
	do_SUB ,
	do_SUB_DBL ,
	do_MUL ,
	do_MUL_DBL ,
	do_DIV ,
	do_DIV_DBL ,
	do_POW ,
	do_POW_DBL ,
	do_CONCAT ,

	do_CMP_LES ,
	do_CMP_LES_EQ ,
	do_CMP_GRE ,
	do_CMP_GRE_EQ ,
	do_CMP_EQU ,
	do_CMP_NOT_EQ ,
	do_CMP_LES_DBL ,
	do_CMP_LES_EQ_DBL ,
	do_CMP_GRE_DBL ,
	do_CMP_GRE_EQ_DBL ,
	do_CMP_EQU_DBL ,
	do_CMP_NOT_EQ_DBL ,
	do_CMP_LES_STR ,
	do_CMP_LES_EQ_STR ,
	do_CMP_GRE_STR ,
	do_CMP_GRE_EQ_STR ,
	do_CMP_EQU_STR ,
	do_CMP_NOT_EQ_STR ,
	};

#ifdef INTERPRET_DEBUG
	if (code_list_empty())
	{
		error(RAW, "Interpret: nedostal jsem talir nudli\n");
		return ERR_FAILURE;
	}
	if (code_list_act() == NULL)
	{
		error(RAW, "Interpret: nedostal jsem prvni nudli (code_list->act)\n");
		return ERR_FAILURE;
	}

/* vypis cele nudle kodu */
fprintf(stderr,"CELA NUDLE\n");
	PrintInstructionList(code_list_head());
fprintf(stderr,"KONEC NUDLE\n");

#endif
	TMicroInstruction *act_ins;      // aktualni instrukce
	TErrCode rt_error = ERR_SUCCESS; // run-time chyby
	act_ins = code_list_act();

	while (rt_error == ERR_SUCCESS) // pri vyskoceni z mainu vyvola ERR_RUNTIME
	{
		assert(act_ins != NULL);

#ifdef CHATTY_INTERPRET
		// krokovany tisk interpretovanych funkci
		fprintf(stderr,"in:");
		PrintInstruction(act_ins);
		int c = getchar();
#endif

		// zavola funkci aby provedla aktualni instrukci
		rt_error = InstructionArray[act_ins->type](act_ins);
		// prejde na dalsi instrukci
		act_ins = code_list_next();
	}
	/* uvolnime datovy a ukazatelovy zasobnik */
	stackP_dispose();
	stackD_dispose();

	/* pokud mame prazdny zasobnik funkci, jde o normalni konec programu */
	if (rt_error == ERR_RUNTIME && stackP_is_empty()) rt_error = ERR_SUCCESS;

	/* vytiskne pripadnou neosetrenou chybu */
	if (rt_error == ERR_LOWMEM) error(RAW, "Interpret: Nedostatek pameti pro interpretaci.\n");
	else if (rt_error == ERR_RUNTIME) error(RAW, "Interpret: Chyba pri vykonavani programu.\n");
	else if (rt_error != ERR_SUCCESS) error(RAW, "Neosetrena chyba pri interpretaci. Omlouvam se vsem.\n");

	// vrati zda doslo k nejake chybe
	return rt_error;
}

/* Konec souboru interpret.c */
