/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		functions.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Funkce provadejici prislusne mikroinstrukce
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include "code_list.h"
#include "constants.h"
#include "functions.h"
#include "shellsort.h"
#include "instructions.h"
#include "strptr_stack.h"
#include "stacks.h"
#include "error.h"
#include "debug.h"

/* Base pointer - pocatek parametru funkce */
int BP;

/*
 * Funkce pro prepocet indexu lokalnich x globalnich promennych
 */
inline int VarIndex(int index)
{
	return ((index < 0) ? (-index - (GLOBAL_CONST_SHIFT-1)) : (index + BP));
}


/*
 * Funkce pro provadeni mikroinstrukci
 */


/* inicializace programu
 * vytvoreni globalnich promennych a zavolani main
 */
TErrCode do_INIT (TMicroInstruction * actInst)
{
	// inicializuje zasobniky
	if (stackD_init () != ERR_SUCCESS || stackP_init () != ERR_SUCCESS) return ERR_LOWMEM;

	// vyhradi na zasobniku misto pro globalni promenne
	// pokud na ne neni pamet vraci chybu
	if (pushD_reserve(actInst->a.IntVal + (GLOBAL_CONST_SHIFT-1)) != ERR_SUCCESS) return ERR_LOWMEM;

	// ulozi na zasobnik navratovou adresu mainu - konec programu
	SItemP ret_state;
	ret_state.BP = 0;
	ret_state.retInst = NULL;
	if (pushP_value (ret_state) != ERR_SUCCESS) return ERR_LOWMEM;

	// skoci na main
	actInst = actInst->b.PtrVal;
	code_list_set_addr(actInst);

	// inicializuje BP mainu
	BP = stackD_top();

	// vyhradi lokalni promenne pro main
	assert(actInst->type == ALLOC);
	if (pushD_reserve(actInst->a.IntVal) != ERR_SUCCESS) return ERR_LOWMEM;

	return ERR_SUCCESS;
}


// pridani promenne dane indexem na datovy zasobnik
TErrCode do_PUSH (TMicroInstruction * actInst)
{
	SItemD tmp;
	// precte promennou
	tmp = getD_variable (VarIndex(actInst->a.IntVal));
	// hodi ji na zasobnik a vrati zda nedosla pamet
	return pushD_value (tmp);
}

// volani funkce
TErrCode do_CALL (TMicroInstruction * actInst)
{

	// ulozi aktualni BP a navratovou adresu
	SItemP ret_state;
	ret_state.BP = BP;
	ret_state.retInst = actInst;

	// presune se na adresu volani (na instr. ALLOC)
	actInst = actInst->b.PtrVal;
	code_list_set_addr(actInst);

	// k BP a navratove adrese prida pocet lokalnich dat a celou strukturu ulozi na zasobnik
	ret_state.locals = actInst->a.IntVal + actInst->b.IntVal; //= promenne + argumenty
	if (pushP_value(ret_state) != ERR_SUCCESS) return ERR_LOWMEM;


	// inicializuje BP (odecte nahazene argumenty)
	BP = stackD_top() - actInst->b.IntVal;

	// vyhradi prostor pro lokalni promenne
	assert(actInst->type == ALLOC);
	if (pushD_reserve(actInst->a.IntVal) != ERR_SUCCESS) return ERR_LOWMEM;

	return ERR_SUCCESS;
}

/*
 * prvni instrukce funkce (na kterou se skace). Po provedeni skoku
 * se prechazi vzdy na dalsi instrukci -> tato instrukce se neprovadi
 * Slouzi pouze jako informace o poctu argumentu a lokalnich dat funkce
 */
TErrCode do_ALLOC (TMicroInstruction * actInst)
{
	// tento prikaz by se nikdy nemel sam o sobe vykonavat
	// slouzi pouze pro ulehceni prace CALL
	assert(actInst->type != ALLOC);
	(void)actInst; // at prekladac zbytecne nevaruje
	return ERR_FAILURE;
}

// navrat z funkce
TErrCode do_RETURN (TMicroInstruction * actInst)
{
	SItemD ret;
	// precte navratovou hodnotu
	ret = getD_variable(VarIndex(actInst->a.IntVal));
	// ulozi navratovou hodnotu na index 1 - dohoda
	putD_variable (VarIndex(RETURN_VALUE_INDEX),ret);

	// vyzvedne navratovou adresu, BP a pocet lokalnich dat
	SItemP ret_state = popP_value ();
	BP = ret_state.BP;

	// pokud se vracim z mainu vyvolam vyjimku, aby se ukoncil program
	if (ret_state.retInst == NULL)
	{
		assert (stackP_is_empty()); // zasobnik funkci uz musi byt prazdny
		return ERR_RUNTIME;
	}

	// odstrani lokalni data ze zasobniku
	popD_block (ret_state.locals);

	// skoci na navratovou adresu
	actInst = ret_state.retInst;
	code_list_set_addr(actInst);
	assert(actInst->type == CALL); // mel bych vzdy skakat zpet na call

	return ERR_SUCCESS;
}

// ukonceni non-void funkce bez return - zpusobi runtime chybu
TErrCode do_ERROR (TMicroInstruction * actInst)
{
	assert(actInst->type == ERROR);
	(void)actInst; // Kvuli varovani prekladace
	// runtime chyba - dosel na konec non-void funkce
	return ERR_RUNTIME;
}

// volani vestavenych funkci
TErrCode do_RUN_STD (TMicroInstruction * actInst)
{
	SItemD RetVal; // navratova hodnota funkce
	SItemD par1;   // pomocna - 1 parametr funkce
	TErrCode Error = ERR_SUCCESS;  // zda funkce neskoncila runtime chybou

	/* Volani vestavenych funkci
	 * vyzvedne parametry ze zasobniku a zavola funkci z functions.h
	 * navratovou hodnotu vklada do unie odpovidajici polozce zasobniku
	 */
	switch (actInst->a.IntVal)
	{
		case INT2STR :
			par1 = getD_variable(BP);
			// char *int2str(int);
			RetVal.StrVal = int2str(par1.IntVal);
			if (RetVal.StrVal == NULL) Error = ERR_LOWMEM;
			break;
		case DOUBLE2STR :
			par1 = getD_variable(BP);
			// char *double2str(double);
			RetVal.StrVal = double2str(par1.DblVal);
			if (RetVal.StrVal == NULL) Error = ERR_LOWMEM;
			break;
		case STR2DOUBLE :
			par1 = getD_variable(BP);
			// double str2double(const char *);
			Error = str2double(par1.StrVal, &RetVal.DblVal);
			break;
		case DOUBLE2INT :
			par1 = getD_variable(BP);
			// int double2int(double);
			Error = double2int(par1.DblVal, &RetVal.IntVal);
			break;
		case PRINTINT :
			par1 = getD_variable(BP);
			// void printint(int);
			printint(par1.IntVal);
			break;
		case PRINTDOUBLE :
			par1 = getD_variable(BP);
			// void printdouble(double);
			printdouble(par1.DblVal);
			break;
		case PRINTSTRING :
			par1 = getD_variable(BP);
			// void printstring(const char *);
			Error = printstring(par1.StrVal);
			break;
		case READINT :
			// int readint(void);
			Error = readint(&RetVal.IntVal);
			break;
		case READDOUBLE :
			// double readdouble(void);
			Error = readdouble(&RetVal.DblVal);
			break;
		case READSTRING :
			// char *readstring(void);
			RetVal.StrVal = readstring();
			if (RetVal.StrVal == NULL) Error = ERR_RUNTIME;
			break;
		case STRLEN :
			par1 = getD_variable(BP);
			// int bstrlen(const char *);
			RetVal.IntVal = bstrlen(par1.StrVal);
			break;
		case SORT :
			par1 = getD_variable(BP);
			// char *sort(const char *str);
			RetVal.StrVal = sort(par1.StrVal);
			if (RetVal.StrVal == NULL) Error = ERR_LOWMEM;
			break;
		default:
			error(RAW, "Volani nedefinovane vestavene funkce (ID=%d).\n",actInst->a.IntVal);
			return ERR_FAILURE;
	}

	// zkontroluje zda pri volani funkce nedoslo k chybe
	if (Error != ERR_SUCCESS) return Error;

	// ulozi navratovou hodnotu na index 1 - dohoda
	putD_variable (VarIndex(RETURN_VALUE_INDEX),RetVal);


	// vyzvedne navratovou adresu BP a pocet promennych (zde jen argumentu)
	SItemP ret_state = popP_value ();
	BP = ret_state.BP;

	// odstrani lokalni data ze zasobniku (zde jen argumenty)
	popD_block (ret_state.locals);

	// skoci na navratovou adresu
	actInst = ret_state.retInst;
	code_list_set_addr(actInst);
	assert(actInst->type == CALL); // mel bych vzdy skakat zpet na call

	return ERR_SUCCESS;
}

TErrCode do_GOTO (TMicroInstruction * actInst)
{
	assert (actInst->type == GOTO && actInst->b.PtrVal != NULL && actInst->b.PtrVal->type == LABEL);
	// skoci na adresu
	actInst = actInst->b.PtrVal;
	code_list_set_addr(actInst);
	return ERR_SUCCESS;
}

TErrCode do_GOTO_IF (TMicroInstruction * actInst)
{
	assert (actInst->type == GOTO_IF && actInst->b.PtrVal != NULL && actInst->b.PtrVal->type == LABEL);

	// zjisti si vysledek podminky
	SItemD tmp;
	tmp = getD_variable (VarIndex(actInst->a.IntVal));
	if (tmp.IntVal != 0)
	{
		// podminka splnena - skoci na adresu
		actInst = actInst->b.PtrVal;
		code_list_set_addr(actInst);
	}

	return ERR_SUCCESS;
}

TErrCode do_LABEL (TMicroInstruction * actInst)
{
	/* navesti - pokud se na nej dostaneme z kodu,
	 * nic se nedeje a pokracuje se dale*/
	assert(actInst->type == LABEL);
	(void)actInst; // at prekladac zbytecne nevaruje
	return ERR_SUCCESS;
}

TErrCode do_MOV (TMicroInstruction * actInst)
{
	SItemD tmp;
	// precte promennou
	tmp = getD_variable (VarIndex(actInst->b.IntVal));
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp);
	return ERR_SUCCESS;
}

TErrCode do_LOAD_INT (TMicroInstruction * actInst)
{
	SItemD tmp;
	// nastavi promennou
	tmp.IntVal = actInst->b.IntVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp);
	return ERR_SUCCESS;
}

TErrCode do_LOAD_DBL (TMicroInstruction * actInst)
{
	SItemD tmp;
	// nastavi promennou
	tmp.DblVal = actInst->b.DblVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp);
	return ERR_SUCCESS;
}

TErrCode do_LOAD_STR (TMicroInstruction * actInst)
{
	SItemD tmp;

	// test na inicializovany ukazatel
	if (actInst->b.StrVal == NULL)
	{
		error(RAW, "Mikroinstrukce LOAD_STR: Predan ukazatel s hodnotou NULL\n");
		return ERR_RUNTIME;
	}

	// nastavi promennou
	tmp.StrVal = actInst->b.StrVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp);
	return ERR_SUCCESS;
}

TErrCode do_INT2DBL (TMicroInstruction * actInst)
{
	SItemD tmp;
	// zkonvertuje a nastavi promennou
	tmp = getD_variable (VarIndex(actInst->b.IntVal));
	tmp.DblVal = tmp.IntVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp);
	return ERR_SUCCESS;
}

TErrCode do_DBL2INT (TMicroInstruction * actInst)
{
	SItemD tmp;
	// zkonvertuje a nastavi promennou
	tmp = getD_variable (VarIndex(actInst->b.IntVal));

	// test na preteceni - podteceni
	if (tmp.DblVal > (double) INT_MAX || tmp.DblVal < (double) INT_MIN)
	{
		error(RAW, "Hodnotu typu double -=%f=- nelze prevest na integer - hodnota je mimo rozsah\n",tmp.DblVal);
		return ERR_RUNTIME;
	}

	tmp.IntVal = tmp.DblVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp);
	return ERR_SUCCESS;
}

// urci delku retezce, aby se dala pouzit v podmince
TErrCode do_STR2BOOL (TMicroInstruction * actInst)
{
	SItemD tmp;
	// nacte string
	tmp = getD_variable (VarIndex(actInst->b.IntVal));
	// ochrana pred neinicializovanym retezcem
	if (tmp.StrVal == NULL) tmp.StrVal = "";
	// urci delku
	tmp.IntVal = strlen(tmp.StrVal);

	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp);
	return ERR_SUCCESS;
}

TErrCode do_ADD (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal += tmp2.IntVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_ADD_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.DblVal += tmp2.DblVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_SUB (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal -= tmp2.IntVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_SUB_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.DblVal -= tmp2.DblVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_MUL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal *= tmp2.IntVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_MUL_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.DblVal *= tmp2.DblVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_DIV (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// kontrola na deleni 0
	if (tmp2.IntVal == 0) {
		error(RAW, "interpret: Nelze delit nulou pri operaci %d div %d.\n",
			tmp1.IntVal, tmp2.IntVal);
		return ERR_RUNTIME;
	}
	// provede operaci
	tmp1.IntVal /= tmp2.IntVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_DIV_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// kontrola na deleni 0
	if (tmp2.DblVal == 0.0) {
		error(RAW, "interpret: Nelze delit nulou pri operaci %g / %g.\n",
			tmp1.DblVal, tmp2.DblVal);
		return ERR_RUNTIME;
	}
	// provede operaci
	tmp1.DblVal /= tmp2.DblVal;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_POW (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.DblVal = pow (tmp1.IntVal, tmp2.IntVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_POW_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	double result = pow (tmp1.DblVal, tmp2.DblVal);
	// kontrola na NAN
	if (isnan(result)) {
		error(RAW, "interpret: Nedefinovany vysledek operace pri umocnovani %g^%g.\n",
			tmp1.DblVal, tmp2.DblVal);
		return ERR_RUNTIME;
	}
	tmp1.DblVal = result;
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

// spoji 2 retezce do 1 -> alokuje novy retezec
TErrCode do_CONCAT (TMicroInstruction * actInst)
{
	SItemD tmpNew;
	SItemD tmp1;
	SItemD tmp2;

	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));

	// neinicializovany retezec je automaticky prazdny
	if (tmp1.StrVal == NULL) tmp1.StrVal = "";
	if (tmp2.StrVal == NULL) tmp2.StrVal = "";

	// kopie prvniho retezce
	tmpNew.StrVal = strmalloc(strlen(tmp1.StrVal) + strlen(tmp2.StrVal) + 1);
  	if (tmpNew.StrVal == NULL)
  	{
  		// chyba alokace
    	error(RAW, "Datovy zasobnik: Nedostatek pameti.\n");
		return ERR_LOWMEM;
	}
	tmpNew.StrVal = strcpy(tmpNew.StrVal, tmp1.StrVal);
	// konkatenace
	tmpNew.StrVal = strcat(tmpNew.StrVal, tmp2.StrVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmpNew);
	return ERR_SUCCESS;
}

TErrCode do_CMP_LES (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.IntVal < tmp2.IntVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
 }

TErrCode do_CMP_LES_EQ (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.IntVal <= tmp2.IntVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_GRE (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.IntVal > tmp2.IntVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_GRE_EQ (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.IntVal >= tmp2.IntVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_EQU (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.IntVal == tmp2.IntVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_NOT_EQ (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.IntVal != tmp2.IntVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_LES_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.DblVal < tmp2.DblVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_LES_EQ_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.DblVal <= tmp2.DblVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_GRE_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.DblVal > tmp2.DblVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_GRE_EQ_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.DblVal >= tmp2.DblVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_EQU_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.DblVal == tmp2.DblVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_NOT_EQ_DBL (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// provede operaci
	tmp1.IntVal = (tmp1.DblVal != tmp2.DblVal);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_LES_STR (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));

	// neinicializovany retezec je automaticky prazdny
	if (tmp1.StrVal == NULL) tmp1.StrVal = "";
	if (tmp2.StrVal == NULL) tmp2.StrVal = "";

	// provedeni porovnani
	tmp1.IntVal = (strcmp(tmp1.StrVal, tmp2.StrVal) < 0);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_LES_EQ_STR (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));

	// neinicializovany retezec je automaticky prazdny
	if (tmp1.StrVal == NULL) tmp1.StrVal = "";
	if (tmp2.StrVal == NULL) tmp2.StrVal = "";

	// provedeni porovnani
	tmp1.IntVal = (strcmp(tmp1.StrVal, tmp2.StrVal) <= 0);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_GRE_STR (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));
	// test na inicializovany ukazatel

	// neinicializovany retezec je automaticky prazdny
	if (tmp1.StrVal == NULL) tmp1.StrVal = "";
	if (tmp2.StrVal == NULL) tmp2.StrVal = "";

	// provedeni porovnani
	tmp1.IntVal = (strcmp(tmp1.StrVal, tmp2.StrVal) > 0);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_GRE_EQ_STR (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));

	// neinicializovany retezec je automaticky prazdny
	if (tmp1.StrVal == NULL) tmp1.StrVal = "";
	if (tmp2.StrVal == NULL) tmp2.StrVal = "";

	// provedeni porovnani
	tmp1.IntVal = (strcmp(tmp1.StrVal, tmp2.StrVal) >= 0);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_EQU_STR (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));

	// neinicializovany retezec je automaticky prazdny
	if (tmp1.StrVal == NULL) tmp1.StrVal = "";
	if (tmp2.StrVal == NULL) tmp2.StrVal = "";

	// provedeni porovnani
	tmp1.IntVal = (strcmp(tmp1.StrVal, tmp2.StrVal) == 0);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}

TErrCode do_CMP_NOT_EQ_STR (TMicroInstruction * actInst)
{
	SItemD tmp1;
	SItemD tmp2;
	// nacte promenne
	tmp1 = getD_variable (VarIndex(actInst->b.IntVal));
	tmp2 = getD_variable (VarIndex(actInst->c.IntVal));

	// neinicializovany retezec je automaticky prazdny
	if (tmp1.StrVal == NULL) tmp1.StrVal = "";
	if (tmp2.StrVal == NULL) tmp2.StrVal = "";

	// provedeni porovnani
	tmp1.IntVal = (strcmp(tmp1.StrVal, tmp2.StrVal) != 0);
	// zapise ji na prislusne misto
	putD_variable (VarIndex(actInst->a.IntVal), tmp1);
	return ERR_SUCCESS;
}
