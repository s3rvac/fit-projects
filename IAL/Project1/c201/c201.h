
/* Pøedmìt: Algoritmy (IAL) - FIT VUT v Brnì
 * Hlavièkový soubor pro c201.c (Jednosmìrnì vázaný lineární seznam)
 * Vytvoøil: Martin Tuèek, záøí 2005
 * Upravil: Bohuslav Køena, øíjen 2006
 *
 *
 * Tento soubor, prosíme, neupravujte!
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#define TRUE 1
#define FALSE 0

#define RESENO 1
#define NERESENO 0

extern int solved;                            // Indikace, zda byl pøíklad øe¹en
extern int errflg;                                            // Indikace chyby

typedef struct tElem {                                        // Prvek seznamu
    struct tElem *ptr;
    int data;
} *tElemPtr;

typedef struct {                                               // Vlastní seznam
    tElemPtr act;
    tElemPtr frst;
} tList;
                                                       // Funkce pro implementaci
void InitList (tList *);
void DisposeList (tList *);
void InsertFirst (tList *, int);
void First (tList *);
void CopyFirst (tList *, int *);
void DeleteFirst (tList *);
void PostDelete (tList *);
void PostInsert (tList *, int);
void Succ (tList *);
void Copy (tList *, int *);
void Actualize (tList *, int );
int  Active (tList *);

