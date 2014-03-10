/*
 * Soubor:  proj4.c
 * Datum:   20.12.2005
 * Autor:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Projekt: IZP c. 4 - Ceske razeni
 * Popis:   Program provede serazeni vstupniho souboru, obsahujici zaznamy 
 *          o osobach, podle zadaneho slozeneho klice  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/** Vypisy pro odladovani programu */
#define NDEBUG

/** Maximalni pocet parametru/udaju u osoby v programu */
#define MAX_PARAMS 4

/** Maximalni a minimalni pocet znaku parametru klic */
#define MIN_KEY_SIZE 5
#define MAX_KEY_SIZE 8

/** Maximalni pocet klicu */
#define MAX_KEY_COUNT 4

/** Velikost kodovaci tabulky */
#define TABLE_SIZE 256

/** Maximalni delka jmena, prijmeni a retezce obecne */
#define MAX_STRING 20

/** Velikost bloku pro alokovani pameti pro data */
const unsigned int BS_INCREMENT = 16;

/** 
 * Ukazatel na funkci vracejici int s dvema obecnymi ukazateli 
 * pro porovnavani retezcu/cisel.
 */
typedef int (*PCmpFunc)(void *, void *);

/** Prvni kodovaci tabulka, podle ktere se bude radit */
const unsigned char FIRSTTABLE[TABLE_SIZE] = 
{
  [(unsigned char)'A'] = 1,  [(unsigned char)'a'] = 1,  
  [(unsigned char)'¡'] = 1,  [(unsigned char)'·'] = 1, 
  [(unsigned char)'ƒ'] = 1,  [(unsigned char)'‰'] = 1,
  [(unsigned char)'B'] = 3,  [(unsigned char)'b'] = 3,
  [(unsigned char)'C'] = 4,  [(unsigned char)'c'] = 4, 
  [(unsigned char)'∆'] = 4,  [(unsigned char)'Ê'] = 4,
  [(unsigned char)'»'] = 5,  [(unsigned char)'Ë'] = 5,
  [(unsigned char)'D'] = 6,  [(unsigned char)'d'] = 6,  
  [(unsigned char)'œ'] = 6,  [(unsigned char)'Ô'] = 6,
  [(unsigned char)'E'] = 8,  [(unsigned char)'e'] = 8,  
  [(unsigned char)'…'] = 8,  [(unsigned char)'È'] = 8,  
  [(unsigned char)'Ã'] = 8,  [(unsigned char)'Ï'] = 8, 
  [(unsigned char)'À'] = 8,  [(unsigned char)'Î'] = 8,
  [(unsigned char)'F'] = 11, [(unsigned char)'f'] = 11,
  [(unsigned char)'G'] = 12, [(unsigned char)'g'] = 12,
  [(unsigned char)'H'] = 13, [(unsigned char)'h'] = 13,
  // 'CH' a 'ch' = 14 - neni znak
  [(unsigned char)'I'] = 15, [(unsigned char)'i'] = 15, 
  [(unsigned char)'Õ'] = 15, [(unsigned char)'Ì'] = 15,
  [(unsigned char)'J'] = 17, [(unsigned char)'j'] = 17,
  [(unsigned char)'K'] = 18, [(unsigned char)'k'] = 18,
  [(unsigned char)'L'] = 19, [(unsigned char)'l'] = 19, 
  [(unsigned char)'≈'] = 19, [(unsigned char)'Â'] = 19, 
  [(unsigned char)'º'] = 19, [(unsigned char)'æ'] = 19,
  [(unsigned char)'M'] = 20, [(unsigned char)'m'] = 20,
  [(unsigned char)'N'] = 21, [(unsigned char)'n'] = 21, 
  [(unsigned char)'—'] = 21, [(unsigned char)'Ò'] = 21, 
  [(unsigned char)'“'] = 21, [(unsigned char)'Ú'] = 21,
  [(unsigned char)'O'] = 23, [(unsigned char)'o'] = 23, 
  [(unsigned char)'”'] = 23, [(unsigned char)'Û'] = 23, 
  [(unsigned char)'÷'] = 23, [(unsigned char)'ˆ'] = 23,
  [(unsigned char)'P'] = 25, [(unsigned char)'p'] = 25,
  [(unsigned char)'Q'] = 26, [(unsigned char)'q'] = 26,
  [(unsigned char)'R'] = 27, [(unsigned char)'r'] = 27, 
  [(unsigned char)'¿'] = 27, [(unsigned char)'‡'] = 27,
  [(unsigned char)'ÿ'] = 28, [(unsigned char)'¯'] = 28,
  [(unsigned char)'S'] = 29, [(unsigned char)'s'] = 29, 
  [(unsigned char)'å'] = 29, [(unsigned char)'ú'] = 29,
  [(unsigned char)'ä'] = 30, [(unsigned char)'ö'] = 30,
  [(unsigned char)'T'] = 31, [(unsigned char)'t'] = 31, 
  [(unsigned char)'ç'] = 31, [(unsigned char)'ù'] = 31,
  [(unsigned char)'U'] = 33, [(unsigned char)'u'] = 33, 
  [(unsigned char)'⁄'] = 33, [(unsigned char)'˙'] = 33, 
  [(unsigned char)'Ÿ'] = 33, [(unsigned char)'˘'] = 33, 
  [(unsigned char)'‹'] = 33, [(unsigned char)'¸'] = 33,
  [(unsigned char)'V'] = 36, [(unsigned char)'v'] = 36,
  [(unsigned char)'W'] = 37, [(unsigned char)'w'] = 37,
  [(unsigned char)'X'] = 38, [(unsigned char)'x'] = 38,
  [(unsigned char)'Y'] = 39, [(unsigned char)'y'] = 39, 
  [(unsigned char)'›'] = 39, [(unsigned char)'˝'] = 39,
  [(unsigned char)'Z'] = 41, [(unsigned char)'z'] = 41, 
  [(unsigned char)'è'] = 41, [(unsigned char)'ü'] = 41,
  [(unsigned char)'é'] = 42, [(unsigned char)'é'] = 42,
  [(unsigned char)'.'] = 43,
  [(unsigned char)','] = 44,
  [(unsigned char)':'] = 45,
  [(unsigned char)'"'] = 46,
  [(unsigned char)'-'] = 47,
  [(unsigned char)'&'] = 48
};
  
/** Druha kodovaci tabulka pro porovnani pismen s urcitou diakritikou */
const unsigned char SECONDTABLE[TABLE_SIZE] = 
{
  [(unsigned char)'A'] = 1,  [(unsigned char)'a'] = 1,
  [(unsigned char)'¡'] = 2,  [(unsigned char)'·'] = 2,
  [(unsigned char)'ƒ'] = 3,  [(unsigned char)'‰'] = 3,
  [(unsigned char)'B'] = 4,  [(unsigned char)'b'] = 4,
  [(unsigned char)'C'] = 5,  [(unsigned char)'c'] = 5,
  [(unsigned char)'∆'] = 6,  [(unsigned char)'Ê'] = 6,
  [(unsigned char)'»'] = 7,  [(unsigned char)'Ë'] = 7,
  [(unsigned char)'D'] = 8,  [(unsigned char)'d'] = 8,
  [(unsigned char)'œ'] = 9,  [(unsigned char)'Ô'] = 9,
  [(unsigned char)'E'] = 10, [(unsigned char)'e'] = 10,
  [(unsigned char)'…'] = 11, [(unsigned char)'È'] = 11,
  [(unsigned char)'Ã'] = 12, [(unsigned char)'Ï'] = 12,
  [(unsigned char)'À'] = 13, [(unsigned char)'Î'] = 13,
  [(unsigned char)'F'] = 14, [(unsigned char)'f'] = 14,
  [(unsigned char)'G'] = 15, [(unsigned char)'g'] = 15,
  [(unsigned char)'H'] = 16, [(unsigned char)'h'] = 16,
  // 'CH' a 'ch' - 17 neni znak
  [(unsigned char)'I'] = 18, [(unsigned char)'i'] = 18,
  [(unsigned char)'Õ'] = 19, [(unsigned char)'Ì'] = 19,
  [(unsigned char)'J'] = 20, [(unsigned char)'j'] = 20,
  [(unsigned char)'K'] = 21, [(unsigned char)'k'] = 21,
  [(unsigned char)'L'] = 22, [(unsigned char)'l'] = 22,
  [(unsigned char)'≈'] = 23, [(unsigned char)'Â'] = 23,
  [(unsigned char)'º'] = 24, [(unsigned char)'æ'] = 24,
  [(unsigned char)'M'] = 25, [(unsigned char)'m'] = 25,
  [(unsigned char)'N'] = 26, [(unsigned char)'n'] = 26,
  [(unsigned char)'—'] = 27, [(unsigned char)'Ò'] = 27,
  [(unsigned char)'“'] = 28, [(unsigned char)'Ú'] = 28,
  [(unsigned char)'O'] = 29, [(unsigned char)'o'] = 29,
  [(unsigned char)'”'] = 30, [(unsigned char)'Û'] = 30,
  [(unsigned char)'÷'] = 31, [(unsigned char)'ˆ'] = 31,
  [(unsigned char)'P'] = 32, [(unsigned char)'p'] = 32,
  [(unsigned char)'Q'] = 33, [(unsigned char)'q'] = 33,
  [(unsigned char)'R'] = 34, [(unsigned char)'r'] = 34,
  [(unsigned char)'¿'] = 35, [(unsigned char)'‡'] = 35,
  [(unsigned char)'ÿ'] = 36, [(unsigned char)'¯'] = 36,
  [(unsigned char)'S'] = 37, [(unsigned char)'s'] = 37,
  [(unsigned char)'å'] = 38, [(unsigned char)'ú'] = 38,
  [(unsigned char)'ä'] = 39, [(unsigned char)'ö'] = 39,
  [(unsigned char)'T'] = 40, [(unsigned char)'t'] = 40,
  [(unsigned char)'ç'] = 41, [(unsigned char)'ù'] = 41,
  [(unsigned char)'U'] = 42, [(unsigned char)'u'] = 42,
  [(unsigned char)'⁄'] = 43, [(unsigned char)'˙'] = 43,
  [(unsigned char)'Ÿ'] = 44, [(unsigned char)'˘'] = 44,
  [(unsigned char)'‹'] = 45, [(unsigned char)'¸'] = 45,
  [(unsigned char)'V'] = 46, [(unsigned char)'v'] = 46,
  [(unsigned char)'W'] = 47, [(unsigned char)'w'] = 47,
  [(unsigned char)'X'] = 48, [(unsigned char)'x'] = 48,
  [(unsigned char)'Y'] = 49, [(unsigned char)'y'] = 49,
  [(unsigned char)'›'] = 50, [(unsigned char)'˝'] = 50,
  [(unsigned char)'Z'] = 51, [(unsigned char)'z'] = 51,
  [(unsigned char)'è'] = 52, [(unsigned char)'ü'] = 52,
  [(unsigned char)'é'] = 53, [(unsigned char)'û'] = 53,
  [(unsigned char)'.'] = 54,
  [(unsigned char)','] = 55,
  [(unsigned char)':'] = 56,
  [(unsigned char)'"'] = 57,
  [(unsigned char)'-'] = 58,
  [(unsigned char)'&'] = 59
};


/** Hodnoty klice */
typedef enum Key 
{
  KEY_NONE,     // klic neni urcen
  KEY_PRIJMENI, // prijmeni
  KEY_JMENO,    // jmeno
  KEY_POHLAVI,  // pohlavi 
  KEY_NAROZEN   // rok narozeni   
} EKey;

/** Hodnoty pohlavi */
typedef enum Pohlavi 
{
  P_MUZ,     // muz
  P_ZENA     // zena	 
} EPohlavi;

/** Hodnoty porovnani dvou hodnot/retezcu */
typedef enum Values
{
  EQU,  // shodne hodnoty
  V1,   // prvni hodnota/retezec je 'vetsi'
  V2    // druha hodnota/retezec je 'vetsi'
} EValues;

/** Jednotlive polozky, ktere se budou radit */
typedef struct person
{
  char prijmeni[MAX_STRING+1]; // 20 znaku string 1 ukoncujici znak '\0'
  char jmeno[MAX_STRING+1];
  int pohlavi;
  int narozen;
} TPerson;

/** Struktura dat*/
typedef struct data
{
  TPerson *pData;           // ukazatel na data o osobe 
  unsigned int count;       // pocet udaju o osobach
  unsigned int *indexTable; // pole indexu jednotlivych polozek s daty
} TData;

/** Struktura obsahujici hodnoty parametru prikazove radky. */
typedef struct params
{ // ukazatele na retezce z prikazove radky
  char *key;        // slozeny klic 
  char *inputFile;  // vstupni soubor
  char *outputFile; // vystupni soubor  
} TParams;

/** Vycet chybovych/stavovych kodu */
typedef enum Errors
{
  EOK,
  EPARAMS, EPKEY,
  EKPRIOR,
  EFOPEN, EFCLOSE, EFWRITE,
  EALLOC,
  ELOAD,EFORM
  
} ECode;

/** Chybove hlasky */
const char *ERRMSG[] =
{
  // bez chyby
  "",
  // prikazovy radek    
  "Program byl spusten s chybnymi parametry!\n",
  // parametr klic
  "Parametr klic ma spatny format!\n",
  "Dva klice nemohou mit stejnou prioritu!\n",
  // prace se soubory
  "Soubor se nepodarilo otevrit!\n",	
  "Soubor se nepodarilo uzavrit!\n",
  // zapis dat
  "Nepodarilo se zapsat data do souboru!\n",
  // alokace pameti
  "Nedostatek pameti!\n",
  // nacitani dat
  "Data v souboru jsou ve spatnem formatu!\n",
  // znaky v datech
  "Data obsahuji nezname znaky!\n"
 
};

/** 
 * Vypisuje na standardni chybovy vystup oznameni o moznosti 
 * spusteni programu s parametrem -h 
 */
inline void printRunHelp(void)
{
  fprintf(stderr, "Spustte program s parametrem -h pro napovedu.\n");
}

/** 
 * Vypise chybovou hlasku na standardni chybovy vystup 
 * @param code Chybovy kod
 */
inline void printMsg(ECode code)
{
  fprintf(stderr, ERRMSG[code]);
}

/**
 * Vytiskne na standardni vystup text s napovedou.
 */
void printHelp(void)
{
  printf("******************************************************************\n"
         "*    Program Ceske razeni.                                       *\n"
         "*    Autor: Petr Zemek                                           *\n"
         "*    Program provede serazeni vstupniho souboru, obsahujici      *\n"
         "*    zaznamy o osobach, podle zadaneho slozeneho klice.          *\n"
         "*    Data se nacitaji ze souboru zadanem pri spusteni programu.  *\n"
         "*    Maximalni delka jmena a prijmeni je 20 znaku.  *\n"
         "*    Pouziti: proj4 -h                                           *\n"
         "*             proj4 key=klic vstupni.soubor vystupni.soubor      *\n"
         "*    Popis parametru:                                            *\n"
         "*      -h               Vypise tuto obrazovku s napovedou.       *\n"
         "*      klic             Priorita polozek (viz. dale)             *\n"
         "*      vstupni.soubor   Soubor se vstupnimi udaji                *\n"
         "*      vystupni.soubor  Soubor s vystupnimi udaji                *\n"
         "*    Hodnota klic udava prioritu jednotlivych polozek pri        *\n"
         "*    porovnani (napr. key=1234).                                 *\n"
         "*      1 - Prijmeni                                              *\n"
         "*      2 - Jmeno                                                 *\n"
         "*      3 - Pohlavi                                               *\n"
         "*      4 - Rok narozeni                                          *\n"
         "******************************************************************\n"
         );
}  // printHelp()

/**
 * Otestuje, zda posledni alokacni operace probehla v poradku. Pokud ne, 
 * vypise chybovou hlasku a ukonci program.
 * @param ptr Ukazatel na posledne alokovanou pamet.
 */
inline int testAlloc(void *ptr)
{
  if (ptr == NULL)
    return EALLOC;
  else 
    return EOK;
} // testAlloc()

/**
 * Zpracuje argumenty prikazoveho radku a vrati je ve strukture TParams.
 * Pokud je format argumentu chybny, ukonci program a vypise chybovou hlasku.
 * @param argc Pocet argumentu.
 * @param argv Pole textovych retezcu s argumenty.
 * @return Vraci analyzovane argumenty prikazoveho radku.
 */
TParams getParams(int argc, char *argv[])
{
  TParams result = 
  { // inicializace struktury
    .key = NULL,       
    .inputFile = NULL,  
    .outputFile = NULL 
  };

  // byl zadan pouze 1 parametr a byl to parametr -h 
  if (argc == 2 && !strcmp("-h", argv[1]))
  { // tisk napovedy
    printHelp();
    exit(EXIT_SUCCESS);
  }

  // test, zda nebylo zadano malo/moc parametru
  else if (argc != MAX_PARAMS)
  { // zadany spatny pocet parametru 
    printMsg(EPARAMS);
    printRunHelp();
    exit(EXIT_FAILURE);
  }
 
  // nacteni parametru klic
  unsigned int keySize = strlen(argv[1]);
  if (strncmp("key=", argv[1], MIN_KEY_SIZE-1) || (keySize < MIN_KEY_SIZE) || 
      (keySize > MAX_KEY_SIZE))
  { // nezadan parametr klic ve spravnem tvaru
    printMsg(EPKEY);
    printRunHelp();
    exit(EXIT_FAILURE);  
  }
  else
  { // zadan parametr key=, nacteni ukazatele na retezec obsahujici klic(e)
    result.key = &argv[1][MIN_KEY_SIZE-1];
  }
      
  // nacteni ukazatelu na nazvy souboru
  result.inputFile = argv[2];
  result.outputFile = argv[3];
   
  return result;
} // getParams()


/**
 * Nacte slozeny klic z parametru prikazove radky
 * @param keyTable Ukazatel na priority klicu
 * @param key Ukazatel na retezec, ktery obsahuje klic(e)
 * @return Vraci chybovy kod/stav
 */
int readKey(EKey *keyTable, char *key)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(keyTable != NULL); 
  assert(key != NULL);
  
  unsigned int keyLength = strlen(key); // pocet prvku v klici
  unsigned int keyUsed[MAX_KEY_COUNT];  // pole obsahujici udaje o pouziti daneho klice  
  unsigned int keyValue;                // hodnota jednoho klice
        
  // Inicializace poli (priorita klice a pouziti klice)
  for (unsigned int i = 0; i < MAX_KEY_COUNT; i++)
  {
  	keyTable[i] = KEY_NONE;
  	keyUsed[i+1] = 0;
  }
  
  // prirazeni klicu podle priority     
  for (unsigned int i = 0; i < keyLength; i++)
  {
    keyValue = key[i] - '0';
    if ((keyValue <= 0) || (keyValue > MAX_KEY_COUNT))
    { // spatny format klice
      return EPKEY; 
    }
    else if (!keyUsed[keyValue]) 
    { // klic nebyl pouzit
      keyTable[i] = keyValue;
      keyUsed[keyValue]++;
    }
    else  // dva klice nemohou mit stejnou prioritu
      return EKPRIOR; 
  }
     
  return EOK;
} // readKey()

/**
 * Nacte vstupni data ze souboru.
 * @param pData Ukazatel na strukturu s daty
 * @param inputFile Ukazatel na soubor
 * @return Vraci chybovy/stavovy kod
 */
int loadData(TData *pData, FILE *inputFile)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(inputFile != NULL);
  
  int state;
  unsigned int blockSize; // velikost bloku pameti
  char tmpPohlavi;
  TPerson *pPerson; // ukazatel na informace o osobe
  
  // alokace pameti pro data
  blockSize = BS_INCREMENT;
  pData->pData = malloc(blockSize * sizeof(TPerson));
  if (testAlloc(pData->pData) != EOK)
    return EALLOC;
    
  // vynulovani poctu nactenych osob
  pData->count = 0;
  
  // nacteni osob ze souboru
  do
  {
    // ukazatel na prave nacitanou adresu osoby
    pPerson = pData->pData + pData->count;
    
    state = fscanf(inputFile, "%20s %20s %c %d", pPerson->prijmeni, 
                                                 pPerson->jmeno, 
                                                 &tmpPohlavi,
                                                 &pPerson->narozen);
    // test nacteni dat
    if (state == EOF)
      break;
    else if (state != MAX_PARAMS)
      return ELOAD;
       
    // prirazeni pohlavi podle nacteneho znaku  
    if (tmpPohlavi == 'M')
      pPerson->pohlavi = P_MUZ;
    else if (tmpPohlavi == 'é')
      pPerson->pohlavi = P_ZENA;
    else
      return ELOAD;
        
    // inkrementace poctu nactenych osob
    pData->count++;
  
    #ifndef NDEBUG
      printf("pData->count = %u\n", pData->count);
      printf("blockSize = %d\n", blockSize);
    #endif
  
    if ((pData->count % blockSize) == 0)
    { // pokud je na konci bloku, je potreba jej natahnout
      blockSize += BS_INCREMENT;
      pData->pData = realloc(pData->pData, blockSize * sizeof(TPerson));
      if (testAlloc(pData->pData) != EOK)
        return EALLOC;
    }
  } while (state != EOF);
  
  if (pData->count == 0) // nebyla nactena ani jedna osoba
    return ELOAD;
   
  // alokace pameti pro pole indexu
  pData->indexTable = malloc(pData->count * sizeof(unsigned int));
  if (testAlloc(pData->indexTable) != EOK)
  { // chyba, uvolnim alokovanou pamet pro data
    free(pData->pData);                          
    return EALLOC;
  }
  
  // inicializace pole indexu
  for (unsigned int i = 0; i < pData->count; i++)
    pData->indexTable[i] = i;
      
  return EOK;
} // loadData()

/**
 * Porovna dve cisla typu int predane odkazem.
 * @param arg1 Ukazatel na prvni cislo
 * @param arg2 Ukazatel na druhe cislo
 * @return Vraci vysledek porovnani (ktere cislo je vetsi nebo zda jsou stejne)
 */
int numCmp(void *arg1, void *arg2)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(arg1 != NULL); 
  assert(arg2 != NULL); 
      
  // pretypovani ukazatelu na ukazatele na int
  int *n1 = (int*) arg1;
  int *n2 = (int*) arg2;
    
  // porovnani cisel
  if (*n1 > *n2)
    return V1;
  else if (*n1 < *n2)
    return V2;
  else
    return EQU; // cisla jsou shodna
    
} // numCmp()

/**
 * Porovna dva retezce predane odkazem podle ceske normy dvoupruchodovym
 * algoritmem pomoci prekodovanych retezcu pomoci dvou tabulek. 
 * @param arg1 Prvni retezec
 * @param arg2 Druhy retezec
 * @return Vraci porovnani (ktery retezec je vetsi nebo zda jsou stejne)
 */
int strCmpCz(void *arg1, void *arg2)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(arg1 != NULL); 
  assert(arg2 != NULL); 
    
  // pretypovani ukazatelu na ukazatele na char
  char *str1 = (char*) arg1;
  char *str2 = (char*) arg2;
    
  // pole, ktere obsahuje obe tabulky pro oba dva pruchody
  const unsigned char *TABLE[] = 
  { 
    FIRSTTABLE, 
    SECONDTABLE 
  };
   
  // pomocne promenne
  unsigned int serie;    // cislo pruchodu cyklem (0,1)
  unsigned char s1, s2;  // znak v retezci
  unsigned int i,j; 
  unsigned char sCH;     // znak 'CH' a 'ch'  
    
  /**
   * Cyklus porovnani dvou retezcu.
   * Tento cyklus ma dva pruchody, pri kazdem je retezec prekodovan podle
   * dane kodovaci tabulky a nasledne dochazi k porovnani retezcu.
   * Pokud jsou retezce po prvnim pruchodu shodne, tak je proveden druhy pruchod,
   * kde dojde k prekodovani a porovnani podle diakritiky.
   * V cyklu je take testovana pritomnost znaku 'CH'.
   */ 
  for (serie = 0; serie <= 1; serie++)
  { 
    // prirazeni indexu znaku 'CH' podle tabulky pruchodu 
    if (serie == 0)
      sCH = 14;
    else // (serie == 1)
      sCH = 17;
      
    // pruchod znaky retezcu
    for (i = j = 0; (str1[i] != '\0') && (str2[j] != '\0'); i++, j++)
    {
      // prekodovani znaku prvniho retezce
      s1 = TABLE[serie][(unsigned char) str1[i]];
      // test na legalitu znaku
      if (s1 == 0)
        return EFORM;
      // test na pismeno 'CH'  
      if ((str1[i] == 'c') || (str1[i]== 'C'))
        if ((str1[i+1] == 'h') || (str1[i+1]== 'H'))
        { // znak je 'CH'
          s1 = sCH;
          i++;
        }

      // prekodovani znaku druheho retezce
      s2 = TABLE[serie][(unsigned char) str2[j]];
      // test na legalitu znaku
      if (s2 == 0)
        return EFORM;
      // test na pismeno 'CH'  
      if ((str2[j] == 'c') || (str2[j]== 'C'))
        if ((str2[j+1] == 'h') || (str2[j+1]== 'H'))
      { // znak je 'CH'
        s2 = sCH;
        j++;
      }

      // porovnani hodnot znaku
      if (s1 > s2)
       return V1;
      if (s2 > s1)
       return V2;
    }

    // porovnavane casti retezcu jsou stejne, rozhodne delka retezcu
    if ((unsigned char)str1[i] > (unsigned char)str2[j])
      return V1;
    if ((unsigned char)str1[i] < (unsigned char)str2[j])
      return V2;
  }  
   
  return EQU;  // retezce jsou shodne
} // strCmpCz()

/**
 * Seradi data podle predaneho klice (jednoho).
 * Pri razeni se meni pouze hodnoty pole indexu, nedoachazi k presunu dat.
 * @param pData Ukazatel na strukturu s daty
 * @param key Klic, podle ktereho se bude radit
 * @return Vraci chybovy/stavovy kod
 */
int sortByKey(TData *pData, EKey key)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(pData != NULL); 
  
  TPerson *pItem;    // ukazatel na udaj, ktery se bude porovnavat
  PCmpFunc pCmpFunc; // ukazatel na funkci, kterou se bude porovnavat 
  
  // prirazeni ukazatelu podle klice
  if (key == KEY_PRIJMENI)
  {
    pCmpFunc = &strCmpCz;
    pItem = (TPerson*) &pData->pData->prijmeni;
  }
  else if (key == KEY_JMENO)
  {
    pCmpFunc = &strCmpCz;
    pItem = (TPerson*) &pData->pData->jmeno;
  }
  else if (key == KEY_POHLAVI)
  {
    pCmpFunc = &numCmp;
    pItem = (TPerson*) &pData->pData->pohlavi;
  }
  else // (key == KEY_NAROZEN)
  {
    pCmpFunc = &numCmp;
    pItem = (TPerson*) &pData->pData->narozen;
  }
    
  #ifndef NDEBUG
    printf("Sort start...\n");
  #endif
  
  // pomocne promenne
  int state;
  unsigned int i, j, tmp;
      
  /**
   * Vlastni razeni metodou Insert-sort.
   * Pole s udaji (struktury s informacema o osobach) je rozdeleno na serazenou 
   * a neserazenou cast (ta ma na zacatku delku jedna).
   * V serazene casti se nalezne pozice, na kterou prijde vlozit prvni prvek 
   * z neserazenÈ casti a od teto pozice az do konce serazene casti 
   * se prvky odsunou.
   * To se opakuje tak dlouho, dokud neserazena cast obsahuje nejaky prvek. 
   * Pokud se v retezcich objevi nelegalni znak, tak algoritmus konci 
   * a algoritmus je ukoncen (funkce nasledne vrati chybovy kod).
   * Pri razeni se meni pouze hodnoty pole indexu, nedoachazi k presunu dat.
   */
  for (i = 1; i < pData->count; i++)
  { // ulozeni aktualni hodnoty z pole indexu
    tmp = pData->indexTable[i];
    j = i;
    while (j > 0) 
    {
      state = pCmpFunc(pItem + tmp, pItem + pData->indexTable[j-1]);  
      // test podminek  
      if (state == EFORM) // nelegalni znak
        return EFORM; 
      if (state != V2) // neni splnena podminka cyklu razeni Insert-sort
        break; 
      // posun prvku o jednicku doprava
      pData->indexTable[j] = pData->indexTable[j-1];
      j--;
    }
    pData->indexTable[j] = tmp; // prirazeni ulozene hodnoty z pole indexu
  }
   
  #ifndef NDEBUG
    printf("Sort: OK!\n");
  #endif
   
  return EOK;
} // sortByKey()

/**
 * Seradi data podle zadaneho slozeneho klice.
 * Radi se od nejnizsi priority po nejvyssi a pokazde 
 * dochazi k zavolani funkce sortByKey.
 * @param pData Ukazatel na strukturu s daty
 * @param keyTable Tabulka priority slozenych klicu
 * @return Vraci chybovy/stavovy kod
 */
int sortData(TData *pData, EKey *keyTable)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(pData != NULL); 
  assert(keyTable != NULL);

  #ifndef NDEBUG
    printf("Razeni:\n");
    printf("-------\n");
  #endif
  
  // vlastni razeni podle klice
  for (int i = MAX_KEY_COUNT-1; i >= 0; i--)
  {
    // pokud klic nebyl zadan pokracuje dalsim klicem
    if (keyTable[i] == KEY_NONE)
      continue;
    
    #ifndef NDEBUG
      printf("Razeni podle klice: %d\n", keyTable[i]);
    #endif
        
    // seradi udaje podle klice
    int state;
    state = sortByKey(pData, keyTable[i]);
    if (state != EOK)
      return state; // doslo k chybe pri razeni
  }
    
  return EOK;
} // sortData()

/**
 * Zapise vystupni data do souboru.
 * @param pData Ukazatel na strukturu s daty
 * @param outputFile Ukazatel na soubor
 * @return Vraci chybovy/stavovy kod
 */
int saveData(TData *pData, FILE *outputFile)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(pData != NULL); 
  assert(outputFile != NULL);
      
  char tmpPohlavi;
  int state;
  TPerson *pPerson; // ukazatel na informace o osobe
  
  // zapis do souboru
  for (unsigned int i = 0; i < pData->count; i++)
  {
    // ukazatel na adresu prave zapisovane osoby
    pPerson = pData->pData + pData->indexTable[i];
        
    // pohlavi
    if (pPerson->pohlavi == P_MUZ)
      tmpPohlavi = 'M';
    else // (pPerson->pohlavi == P_ZENA)
      tmpPohlavi = 'é';
    
    // vlastni zapis   
    state = fprintf(outputFile, "%s %s %c %d\n", pPerson->prijmeni,
                                                 pPerson->jmeno,
                                                 tmpPohlavi,
                                                 pPerson->narozen);  
    if (state <= 0)
      return EFWRITE; // nepodarilo se zapsat data do souboru
  }
      
  return EOK;
} // saveData()

/**
 * Uvolni alokovanou pamet pro data a pole indexu
 * @param pData Ukazatel na strukturu s daty
 */
inline void freeData(TData *pData)
{
  // test, zda byly parametry predany odkazem v poradku
  assert(pData != NULL);
  assert(pData->indexTable != NULL);
  
  // uvolneni pameti
  free(pData->pData);
  free(pData->indexTable);
} // freeData()

/**
 * Hlavni program
 */
int main(int argc, char *argv[])
{
  // vyhodnotim parametry prikazove radky
  TParams params = getParams(argc, argv); 
    
  int state;                    // stavovy/chybovy kod
  FILE *inputFile, *outputFile; // I/O soubory
  EKey keyTable[MAX_KEY_COUNT]; // tabulka priority klicu
  TData data;                   // struktura obsahujici informace o datech
  
  // nactu klic, podle ktereho se bude radit
  state = readKey(keyTable, params.key);
  if (state != EOK)
  {
    printMsg(state);
    return EXIT_FAILURE;
  }
  
  #ifndef NDEBUG
    printf("Priorita klice : ");
    for (int k = 0; k < MAX_KEY_COUNT; k++)
      printf("%d", keyTable[k]);
    printf("\n\n");
  #endif
  
  // otevreni vstupniho souboru
  inputFile = fopen(params.inputFile, "r");
  if (inputFile == NULL)
  { // soubor se nepodarilo otevrit
    printMsg(EFOPEN);
    return EXIT_FAILURE;
  }
  
  #ifndef NDEBUG
    printf("Nacteni: \n");
    printf("----------\n");
  #endif
    
  // nacteni polozek ze souboru
  state = loadData(&data, inputFile);
  if (state != EOK)
  { // nepodarilo se nacist data
    printMsg(state);
    fclose(inputFile);
    return EXIT_FAILURE;
  }
  
  #ifndef NDEBUG
    printf("Nacteni: OK\n\n");
  #endif  
  
  // uzavreni vstupniho souboru
  if (fclose(inputFile) == EOF)
  {}// soubor se nepodarilo uzavrit, pokracuji dale, na funkcnost to nema vliv
  
  state = sortData(&data, keyTable);
  if (state != EOK)
  { // chyba pri razeni dat
    printMsg(state);
    fclose(inputFile);
    freeData(&data);
    return EXIT_FAILURE;
  }
  
  #ifndef NDEBUG
    printf("\nRazeni: OK\n");
  #endif 
    
  // otevreni vystupniho souboru
  outputFile = fopen(params.outputFile, "w");
  if (outputFile == NULL)
  { // soubor se nepodarilo otevrit
    printMsg(EFOPEN);
    freeData(&data);
    return EXIT_FAILURE;
  }
  
  // zapsani dat do vystupniho souboru
  state = saveData(&data, outputFile);
  if (state != EOK)
  { // doslo k chybe pri zapisu do souboru
    printMsg(state);
    freeData(&data);
    return EXIT_FAILURE;
  } 
  
  #ifndef NDEBUG
    printf("Zapsani: OK\n");
  #endif 
  
  // uzavreni vystupniho souboru
  if (fclose(outputFile) == EOF)
  {}// soubor se nepodarilo uzavrit, pokracuji dale, na funkcnost to nema vliv
  
  // uvolneni pameti
  freeData(&data);
  
  #ifndef NDEBUG
    printf("Free a konec: OK\n");
  #endif 
        
  return EXIT_SUCCESS;
}  // main() 

/* konec souboru proj4.c */
