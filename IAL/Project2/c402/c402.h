
/* Hlavièkový soubor pro c402.c - nerekurzívní implementace operací nad BVS,
obsahuje jednak nutné knihovny a externí promìnné, ale rovnìž 
definici datových typù, se kterými se pracuje v pøíslušných 
úlohách. Nemodifikovat! */

/* ********************** SOUBOR S HLAVIÈKOU ********************** */
/* ********************** ------------------ ********************** */

/*  vytvoèil: Martin Tuèek
    pøedmìt: Algoritmy (IAL) - FIT (Fakulta Informacnich Technologií)
    hlavicka pro soubor: c402.c
    datum: záøí 2005                                                 */
/* ***************************************************************** */

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>

#define FALSE 0
#define TRUE 1
#define MAXSTACK 30 			/* maximální poèet prvkù v zásobníku */

extern int solved; 			/* globální promìnná naznaèující, zda bylo øešeno èi nikoliv */
extern int errflg;			/* pøíznak ošetøení chyby */


typedef struct tBTNode	{		/* uzel stromu */
    int Cont;      			/* obsah uzlu  */
    struct tBTNode * LPtr;   		/* ukazatel na levý podstrom  */
    struct tBTNode * RPtr; 	  	/* ukazatel na pravý podstrom */
} *tBTNodePtr;				/* typ ukazatel na uzel */

/* datove struktury pro praci se zasobniky */

typedef struct	{ 			/* Zásobník na hodnoty typu tBTNodePtr */
    tBTNodePtr a[MAXSTACK];
    int top;
} tStackP;

typedef struct {  		    	/* Zásobník na boolovské hodnoty. */
    bool a[MAXSTACK];
    int top;
} tStackB;


/***prototypes***/
void BTInit (tBTNodePtr * );
void BTInsert (tBTNodePtr *, int );

void Leftmost_Preorder(tBTNodePtr, tStackP *);
void BTPreorder(tBTNodePtr );

void Leftmost_Inorder(tBTNodePtr, tStackP *);
void BTInorder(tBTNodePtr );

void Leftmost_Postorder(tBTNodePtr, tStackP *, tStackB *);
void BTPostorder(tBTNodePtr );

void BTDisposeTree(tBTNodePtr *);
/********end of prototypes******/

