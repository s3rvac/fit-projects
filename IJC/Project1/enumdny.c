// Encoding: ISO-8859-2 
/* 
 * Soubor:  enumdny.c - prelozeno gcc
 * Datum:   7.3.2006
 * Autor:   Petr Zemek, FIT, xzemek02@stud.fit.vutbr.cz
 * Projekt: IJC - DU1
 * Popis:   Modul pro tisk a cteni hodnot vyctoveho datoveho typu
 */
 
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "enumdny.h" /* Tisk a cteni hodnot vyctove typu */
#include "error.h"   /* Tisk chybovych zprav */

/* Minimalni a maximalni korektni hodnota dne */
#define MIN_DAY 1
#define MAX_DAY 7

/* Maxmalni delka nazvu dne */
#define MAX_DAY_LENGTH 7

/* Pocet moznych formatu dne */
#define DAY_FMT_COUNT 4

/* 
 * Vycet poradi formatu retezcu v poli daysList 
 */
enum rank { Short, ShortASCII, Long, LongASCII };

/* 
 * Staticke pole obsahujici korektni zkratky pro dny v tydnu, 
 * podle kterych se porovna zadany den pri nacteni / vypisu.
 * Obsahuje nazvy s diakritikou i bez.
 */
static const char *daysList[MAX_DAY_LENGTH+1][DAY_FMT_COUNT] = {     
  [Po] = { "Po", "Po", "Pondělí", "Pondeli" },
  [Ut] = { "Út", "Ut", "Úterý", "Utery" },
  [St] = { "St", "St", "Středa", "Streda"  },
  [Ct] = { "Čt", "Ct", "Čtvrtek", "Ctvrtek" },
  [Pa] = { "Pá", "Pa", "Pátek", "Patek" },
  [So] = { "So", "So", "Sobota", "Sobota" },
  [Ne] = { "Ne", "Ne", "Neděle", "Nedele" }
};

/**
 * Tiskne retezec odpovidajici zadane hodnote 
 * vyctoveho typu 'česky' ("Po", "Út", "St", "Čt" ...). 
 */
void PrintDayShort(enum dny den) {  
  // kontrola, zda je hodnota predaneho dne platna
  if (den < MIN_DAY || den > MAX_DAY)
    Error("PrintDay: hodnota %d mimo rozsah\n", den);   
    
  // tisk
  printf("%s", daysList[den][Short]);    
}

/**
 * Tiskne retezec odpovidajici zadane hodnote 
 * vyctoveho typu 'cesky' ("Po", "Ut", "St", Ct, ...). 
 */
void PrintDayShortASCII(enum dny den) {
  // kontrola, zda je hodnota predaneho dne platna
  if (den < MIN_DAY || den > MAX_DAY)
    Error("PrintDay: hodnota %d mimo rozsah\n", den);
    
  // tisk
  printf("%s", daysList[den][ShortASCII]);  
}

/**
 * Tiskne retezec odpovidajici zadane hodnote
 * vyctoveho typu 'česky' ("Pondělí", "Úterý", ...). 
 */
void PrintDayLong(enum dny den) {
  // kontrola, zda je hodnota predaneho dne platna
  if (den < MIN_DAY || den > MAX_DAY)
    Error("PrintDay: hodnota %d mimo rozsah\n", den);
  
  // tisk
  printf("%s", daysList[den][Long]); 
}

/**
 * Tiskne retezec odpovidajici zadane hodnote
 * vyctoveho typu 'cesky' ("Pondeli", "Utery", ...). 
 */
void PrintDayLongASCII(enum dny den) {
  // kontrola, zda je hodnota predaneho dne platna
  if (den < MIN_DAY || den > MAX_DAY)
    Error("PrintDay: hodnota %d mimo rozsah\n", den);

  // tisk
  printf("%s", daysList[den][LongASCII]); 
}

/**
 * Cte hodnotu vyctoveho typu ze standardniho vstupu. 
 */
enum dny ReadDay(void) { 
  // pole znaku pro ulozeni zkratky dne (+1 pro '\0')   
  char daySht[MAX_DAY_LENGTH + 1]; 
  int c;
  // pocet nactenych znaku
  int loaded = 0;
  // indikator, ze se zrovna nacita den
  int isInWord = 0; 
                   
  // nacteni zkratky ze standardniho vstupu
  while (loaded <= MAX_DAY_LENGTH && (c = getchar()) != EOF) {
    // zadany znak je 'bily znak'
    if (isspace(c)) {
      // pokud se nacita den, pak je nacitani ukonceno
      if (isInWord)
        break;
      else
        continue;
    }
    // zadany znak je pismeno
    else if (isalpha(c)) { 
      daySht[loaded] = c;
      loaded++;  
      isInWord = 1;
    }
    // nelegalni znak - konec nacitani
    else      
      break;
  }
  
  // ukonceni retezce
  daySht[loaded] = '\0';

  // prevod nazvu dne do formatu z vyctoveho typu dny
  daySht[0] = isupper(daySht[0]) ? daySht[0] : toupper(daySht[0]);
  for (int i = 1; i < MAX_DAY_LENGTH; i++)
    daySht[i] = islower(daySht[i]) ? daySht[i] : tolower(daySht[i]); 
 
  // kontrola, zda zadany den/zkratka je legalni
  for (enum dny den = Po; den <= Ne; den++)
    for (int i = 0; i < DAY_FMT_COUNT; i++)
      if (strcmp(daySht, daysList[den][i]) == 0)
        return den;
  
  // den/zkratka neni legalni
  return 0;
}

/* Konec souboru enumdny.c */
