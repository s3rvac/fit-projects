
/* ******************************* c203.h *********************************** */
/*  Pøedmìt: Algoritmy (IAL) - FIT VUT v Brnì                                 */
/*  Hlavièkový soubor pro c203.c (Fronta znakù v poli)                        */
/*  Úkol: c203 - Fronta znakù v poli                                          */
/*  Vytvoøil: Václav Topinka, záøí 2005                                       */
/*  Úpravy: Bohuslav Køena, øíjen 2006                                        */
/* ************************************************************************** */

/* TENTO SOUBOR, PROSÍME, NEUPRAVUJTE! */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>

#define MAX_QUEUE 50
extern int QUEUE_SIZE; 
/* 
 * Hodnota MAX_QUEUE udvává skuteènou velikost statického pole pro ulo¾ení
 * hodnot fronty. Pøi implementaci operací nad ADT fronta v¹ak pøedpokládejte,
 * ¾e velikost tohoto pole je pouze QUEUE_SIZE. Umo¾ní nám to jednodu¹e 
 * mìnit velikost fronty v prùbìhu testování. Pøi implementaci ADT fronta byste
 * tedy hodnotu MAX_QUEUE nemìli vùbec pou¾ít. Pamatujte, ¾e do fronty se vejde
 * maximálnì (QUEUE_SIZE - 1) prvkù. Jedna pozice ve frontì bude v¾dy nevyu¾itá,
 * aby bylo mo¾né odli¹it prázdnou frontu od plné. 
 */

extern int solved;                      /* Indikuje, zda byla operace øe¹ena. */
extern int err_flag;                   /* Indikuje, zda operace volala chybu. */

                                        /* Chybové kódy pro funkci queueError */
#define MAX_QERR    5                                   /* poèet mo¾ných chyb */
#define QERR_UP     1                                   /* chyba pøi stackTop */
#define QERR_FRONT  2                                   /* chyba pøi stackPop */
#define QERR_REMOVE 3                                  /* chyba pøi stackPush */
#define QERR_GET    4                                  /* chyba pøi stackPush */
#define QERR_INIT   5                                     /* chyba pøi malloc */

                               /* ADT fronta implementovaný ve statickém poli */
typedef struct {
	char arr[MAX_QUEUE];                           /* pole pro ulo¾ení hodnot */
	int f_index;                                       /* index prvního prvku */
	int b_index;                                  /* index první volné pozice */
} tQueue;
                                      /* Hlavièky funkcí pro práci s frontou. */
void queueError ( int error_code );
void queueInit ( tQueue* q );
int queueEmpty ( const tQueue* q );
int queueFull ( const tQueue* q );
char queueFront ( const tQueue* q );
void queueRemove ( tQueue* q );
char queueGet ( tQueue* q );
void queueUp ( tQueue* q, char c );

#endif

/* Konec hlavièkového souboru c203.h */
