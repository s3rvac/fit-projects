
/* Hlavièkový soubor pro c403.c - Vyhledávací tabulka v neseøazeném 
poli se zar. (optimalizace), obsahuje jednak nutné includes a 
externované promìnné, ale rovneø definici datových typù, se kterými 
se pracuje v pøíslušných úlohách. Nemodifikovat! */

/* ********************** SOUBOR S HLAVIÈKOU ********************** */
/* ********************** ------------------ ********************** */

/*  vytvoèil: Martin Tuèek
    pøedmìt: Algoritmy (IAL) - FIT (Fakulta Informacnich Technologií)
    hlavicka pro soubor: c403.c
    datum: záøí 2005                                                 */
/* ***************************************************************** */


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#define ASMaxSize 50        /* Maximální velikost pole pro implementaci
			       vyhledávací tabulky. Øešené procedury však
			       využívají pouze ASSize prvkù pole (viz
			       deklarace této promìnné). */
#define FALSE 0
#define TRUE 1

extern int solved;
extern int errflg;
extern int ASCompNum;       /* Promìnná, která bude uchovávat poèet porovnání */

typedef struct { 
/* datový záznam */
        int Cont;                /* zde se bude ukládat obsah  */
        int Key;     	         /* a zde klíè */
} tASData;

typedef struct  {
/* tabulka implementovaná polem */
   tASData arr[ASMaxSize];  /* pole pro datové položky */
   int last;                /* index poslední využité položky pole */
} tASTable;

int ASSize;          /* Pro úèely testování je vhodné mít možnost
                        volby velikosti pole, kterým je vyhledávací
                        tabulka implementována. Fyzicky je
                        deklarováno pole o rozmìru ASMaxSize, ale
                        pøi implementaci vašich procedur uvažujte
                        velikost ASSize! */


/****prototypes*****/
void ASInit   (tASTable *);
int  ASSearch (tASTable *, int , int* );
void ASInsert (tASTable *, int, int );
/*******end of prototypes**************/

