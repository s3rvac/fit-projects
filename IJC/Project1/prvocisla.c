// Encoding: ISO-8859-2
/*
 * Soubor:  prvocisla.c - prelozeno gcc
 * Datum:   7.3.2006
 * Autor:   Petr Zemek, FIT, xzemek02@stud.fit.vutbr.cz
 * Projekt: IJC - DU1
 * Popis:   IJC - DU1 b)
 *          Implementace Eratostenova sita
 */

#include <stdio.h>
#include <math.h>
#include "error.h"   /* Tisk chybovych zprav */

/* Pocet prvku pole bitu */
#define N 90000000L

/* Pocet vypisovanych prvocisel */
#define PRV_COUNT 10

/* Pocet bitu jednoho unsigned long cisla */
#define BIT_COUNT_UL (8*sizeof(unsigned long))

/* Vypocet poctu prvku, pro ktere je treba definovat pole pro ulozeni celkem N bitu */
#define BitCountUnits(velikost) \
  ((velikost) / (BIT_COUNT_UL) + (((velikost) % (BIT_COUNT_UL)) == 0 ? 0 : 1))

/* Vypocet indexu prvky na kterem se nachazi bit */
#define BitPos(bit) ((bit) / (BIT_COUNT_UL))

/* Posun bitu s hodnotou 1 na pozici danou bitem b */
#define BitMask(bit) (1 << ((bit) % (BIT_COUNT_UL)))

/*
 * Definice a nulovani pole bitu.
 * Do pomocne konstanty si ulozim velikost pole,
 * abych podle ni mohl kontrolovat meze v nasledujicich
 * makrech / inline funkcich.
 */
#define BitArray(jmeno_pole, velikost) \
  unsigned long (jmeno_pole)[BitCountUnits(velikost)] = {0}; \
  const size_t jmeno_pole##_velikost = (velikost);

/*
 * Neni definovan symbol USE_INLINE, pouziji se makra
 */
#ifndef USE_INLINE
  /*
   * Nastavi zadany bit v poli na hodnotu zadanou vyrazem
   */
  #define SetBit(jmeno_pole, index, vyraz) \
    if ((index) > (jmeno_pole##_velikost) || (long)(index) < 0) \
      Error("Index %ld mimo rozsah 0..%ld", (long)(index), (long)(jmeno_pole##_velikost)); \
    else if (vyraz) \
      ((jmeno_pole)[BitPos(index)] |= BitMask(index)); \
    else \
      ((jmeno_pole)[BitPos(index)] &= ~BitMask(index))

  /*
   * Ziska hodnotu zadaneho bitu
   */
  #define GetBit(jmeno_pole, index) \
    (((index) > (jmeno_pole##_velikost) || (long)(index) < 0) ? \
      (Error("Index %ld mimo rozsah 0..%ld", (long)(index), (long)(jmeno_pole##_velikost)), 0) :\
      (((jmeno_pole)[BitPos(index)] & BitMask(index)) != 0))

/*
 * Pokud je pri prekladu definovan symbol USE_INLINE,
 * misto maker se definuji inline funkce.
 */
#else
  /*
   * Abych mohl kontrolovat meze poli v inline funkcich, musim si vytvorit nove makra,
   * ktere budou pri prekladu umisteny misto puvodnich maker, a tudiz budu moct funkci predat
   * dalsi parametr, kterym bude velikost pole -> snadna kontrola mezi.
   * Zadani je dodrzeno, protoze nedojde ke zmene v testovacim prikladu
   * (dodrzim pocet parametru).
   */
  #define SetBit(jmeno_pole, index, vyraz) \
    SetBit_inline(jmeno_pole, index, vyraz, jmeno_pole##_velikost)

  #define GetBit(jmeno_pole, index) \
    GetBit_inline(jmeno_pole, index, jmeno_pole##_velikost)

  /*
   * Definice inline funkci
   * ----------------------------------------------------
   */

  /*
   * Nastavi zadany bit v poli na hodnotu zadanou vyrazem
   */
  inline void SetBit_inline(unsigned long *jmeno_pole, long index, int vyraz, size_t mez) {
    // kontrola mezi
    if (index > (long)mez || index < 0)
      Error("Index %ld mimo rozsah 0..%ld", index, (long)mez);
    else if (vyraz)
      jmeno_pole[BitPos(index)] |= BitMask(index);
    else
      jmeno_pole[BitPos(index)] &= ~BitMask(index);
  }

  /*
   * Ziska hodnotu zadaneho bitu
   */
  inline int GetBit_inline(unsigned long *jmeno_pole, long index, size_t mez) {
    return (index > (long)mez || index < 0) ?
      (Error("Index %ld mimo rozsah 0..%ld", index, (long)mez), 0) :
      ((jmeno_pole[BitPos(index)] & BitMask(index)) != 0);
  }

#endif

/*
 * Hlavni program s implementaci Eratostenova sita
 */
int main(void) {
  // definice a inicializace N prvkoveho pole bitu
  BitArray(p, N);

  // odmocnina z N, ktera je potreba v podmince Eratostenova sita
  const unsigned long SQRTN = (unsigned long) sqrt(N);
  // ridici promenne do cyklu
  unsigned long i, j;

  // Eratostenovo sito
  for (i = 2; i <= SQRTN; i++) {
    // vynecham vsechny bity s hodnotou 1 - nejsou prvocisla
    while (GetBit(p, i))
      i++;
    // odstranim vsechny nasobky i - nejsou to prvocisla
    for (j = i*i; j <= N; j += i) {
      SetBit(p, j, 1);
    }
  }

  // nacteni poslednich 10 (PRV_COUNT) prvocisel
  unsigned long pp[PRV_COUNT];
  for (i = N, j = 0; j < PRV_COUNT; i--)
    if (GetBit(p, i) == 0) {
      pp[j] = i;
      j++;
    }

  // vypis prvocisel (vzestupne)
  for (int k = PRV_COUNT-1; k >= 0; k--)
    printf("%lu\n", pp[k]);

  return 0;
}

/* Konec souboru prvocisla.c */
