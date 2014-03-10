

/* Hlavièkový soubor pro c401.c - rekurzívní implementace operaci nad BVS,
obsahuje jednak nutné knihovny a externí promìnné, ale rovnìž 
definici datových typù, se kterými se pracuje v pøíslušných 
úlohách. Nemodifikovat! */

/* ********************** SOUBOR S HLAVIÈKOU ********************** */
/* ********************** ------------------ ********************** */

/*  vytvoèil: Martin Tuèek
    pøedmìt: Algoritmy (IAL) - FIT (Fakulta Informacnich Technologií)
    hlavicka pro soubor: c401.c
    datum: záøí 2005                                                 */
/* ***************************************************************** */

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#define TRUE 1
#define FALSE 0

extern int solved; 			/* globální promìnná naznaèující, zda bylo øešeno èi nikoliv */
extern int errflg;

typedef struct tBSTNode {
	char Key;			/* podle tohoto klíèe se vyhledává */
	int BSTNodeCont;
	struct tBSTNode * LPtr;
	struct tBSTNode * RPtr;
} *tBSTNodePtr;	

/* prototypes */

void BSTInit   (tBSTNodePtr *);
int BSTSearch  (tBSTNodePtr ,char , int *);
void BSTInsert (tBSTNodePtr *, char, int );
void BSTDelete (tBSTNodePtr *, char );
void BSTDispose(tBSTNodePtr *);

