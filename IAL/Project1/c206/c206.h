
/* P�edm�t: Algoritmy (IAL) - FIT VUT v Brn�
 * Hlavi�kov� soubor pro c206.c (Dvousm�rn� v�zan� line�rn� seznam)
 * Vytvo�il: Martin Tu�ek, z��� 2005
 * Upravil: Bohuslav K�ena, ��jen 2006
 *  
 *  Tento soubor, pros�me, neupravujte!  
 */

#include<stdio.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1

#define RESENO 1
#define NERESENO 0

extern int errflg;
extern int solved;
 
typedef struct tDLElem {                /* prvek dvouosm�rn� v�zan�ho seznamu */ 
        int data;                                            /* u�ite�n� data */
        struct tDLElem *lptr;          /* ukazatel na p�edchoz� prvek seznamu */
        struct tDLElem *rptr;        /* ukazatel na n�sleduj�c� prvek seznamu */
} *tDLElemPtr;

typedef struct {                                  /* dvousm�rn� v�zan� seznam */
    tDLElemPtr First;                      /* ukazatel na prvn� prvek seznamu */
    tDLElemPtr Act;                     /* ukazatel na aktu�ln� prvek seznamu */
    tDLElemPtr Last;                    /* ukazatel na posledni prvek seznamu */
} tDLList;

                                             /* prototypy jednotliv�ch funkc� */
void DLInitList (tDLList *);
void DLDisposeList (tDLList *);
void DLInsertFirst (tDLList *, int );
void DLInsertLast(tDLList *, int );
void DLFirst (tDLList *);
void DLLast (tDLList *);
int DLCopyFirst (tDLList *);
int DLCopyLast (tDLList *);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
void DLPostInsert (tDLList *, int );
void DLPreInsert (tDLList *, int );
int DLCopy (tDLList *);
void DLActualize (tDLList *, int);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);

