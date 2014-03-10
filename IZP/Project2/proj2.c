/*
 * Soubor:  proj2.c
 * Datum:   23.11.2005
 * Autor:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Projekt: IZP c. 2 - Iteracni vypocty
 * Popis:   Program pocita funkce pomoci zakladnich operaci (+,-,*,/)
 *          se zadanou presnosti.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/* Maximalni mozne cislo, kterym lze umocnit e,
 * aby vysledek nepresahl maximalni velikost double. */
#define EX_MAX 709.78271464
#define EX_MIN -709.78271464

/** Minimalni hodnota presnosti */
const double MIN_PRESNOST = 2.22e-308;

/** Konstanty. */
const double IZP_E = 2.7182818284590452354;        // e
const double IZP_PI = 3.14159265358979323846;      // pi
const double IZP_2PI = 6.28318530717958647692;     // 2*pi
const double IZP_PI_2 = 1.57079632679489661923;    // pi/2
const double IZP_PI_4 = 0.78539816339744830962;    // pi/4

/**
 * Struktura obsahujici hodnoty parametru prikazove radky.
 * 1. polozka je ukazatel na funkci, kterou budeme chtit provadet
 * 2. polozka je presnost, se kterou budeme pocitat
 */
typedef struct params
{
  double (*funkce)(double, double); /** Funkce */
  double epsilon;                   /** Presnost */
} TParams;

/**
 * Vytiskne na standardni vystup text s napovedou.
 */
void printHelp(void)
{
  printf("******************************************************************\n"
         "*    Program Iteracni vypocty.                                   *\n"
         "*    Autor: Petr Zemek                                           *\n"
         "*    Program pocita funkce pomoci zakladnich operaci (+,-,*,/)   *\n"
         "*    se zadanou presnosti.                                       *\n"
         "*    Argument funkce se nacita ze standardniho vstupu.           *\n"
         "*    Pouziti: proj2 -h                                           *\n"
         "*             proj2 -funkce epsilon                              *\n"
         "*    Popis parametru:                                            *\n"
         "*      -h       Vypise tuto obrazovku s napovedou.               *\n"
         "*      -funkce  Jakou funkci bude program pocitat.               *\n"
         "*      epsilon  S jakou presnosti bude program pocitat.          *\n"
         "*    Minimalni hodnota epsilon je 0.0                            *\n"
         "*    Moznosti parametru -funkce jsou:                            *\n"
         "*      -sqrt  Vypocet druha odmocnina(x)                         *\n"
         "*      -cos   Vypocet cos(x)                                     *\n"
         "*      -ex    Vypocet e^x                                        *\n"
         "*      -ln    Vypocet ln(x)                                      *\n"
         "******************************************************************\n"
         );
}  // printHelp()

/**
 * Prototypy matematickych funkci, ktere jsou v tomto programu k dispozici
 * Blizsi popis viz. cast Implementace
 */
double funkceSQRT(double x, double presnost);
double funkceCOS(double x, double presnost);
double funkceEX(double x, double presnost);
double funkceLN(double x, double presnost);

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
    .funkce = NULL,
    .epsilon = 0.0
  };

  // byl zadan pouze 1 parametr a byl to parametr -h
  if (argc == 2 && strcmp("-h", argv[1]) == 0)
  { // tisk napovedy
    printHelp();
    exit(EXIT_SUCCESS);
  }
  // test, zda nebylo zadano malo/moc parametru
  if (argc != 3)
  { // vypise na standardni chybovy vystup chybovou hlasku.
    fprintf(stderr, "Program byl spusten s chybnymi parametry!\n"
                    "Spustte program s parametrem -h pro napovedu.\n");
    exit(EXIT_FAILURE);
  }
  // byly zadany 2 parametry
  else if (argc == 3)
  { // ulozi parametry do struktury

    // konverze epsilon
    char *pom; // pomocna promenna ke zjisteni, co bylo zadano jako epsilon
    result.epsilon = strtod(argv[2], &pom); // pokus o prevedeni epsilon

    // epsilon je 0.0 -> do eps vlozim MIN_PRESNOST (mezi 0.0 a 2.22e-308 neni
    // rozpoznatelny rozdil a vypocty probihaji bez problemu)
    if (result.epsilon == 0.0)
    {
      result.epsilon = MIN_PRESNOST;
    }

    // spatny format presnosti
    if (*pom != '\0')
    { // vypise na standardni chybovy vystup chybovou hlasku.
      fprintf(stderr, "Zadana hodnota presnosti je ve spatnem formatu!\n"
                      "Spustte program s parametrem -h pro napovedu.\n");
      exit(EXIT_FAILURE);
    }

    // epsilon mimo platny rozsah
    if ((result.epsilon < 0.0) || (result.epsilon == +INFINITY))
    { // vypise na standardni chybovy vystup chybovou hlasku.
      fprintf(stderr, "Zadana hodnota presnosti je mimo platny rozsah!\n"
                      "Spustte program s parametrem -h pro napovedu.\n");
      exit(EXIT_FAILURE);
    }

    // test, zda nebyl zadany neexistujici parametr -funkce
    if (!strcmp("-sqrt", argv[1]))
    {  // funkce Sqrt(x)
      result.funkce = &funkceSQRT;
    }
    else if (!strcmp("-cos", argv[1]))
    {  // funkce Cos(x)
      result.funkce = &funkceCOS;
    }
    else if (!strcmp("-ex", argv[1]))
    {  // funkce e^x
      result.funkce = &funkceEX;
    }
    else if (!strcmp("-ln", argv[1]))
    {  // funkce Ln(x)
      result.funkce = &funkceLN;
    }
    // zadana funkce neexistuje
    else
      { // vypise na standardni chybovy vystup chybovou hlasku.
        fprintf(stderr, "Tato funkce neexistuje!\n"
                        "Spustte program s parametrem -h pro napovedu.\n");
        exit(EXIT_FAILURE);
      }
  }

  return result;
}  // getParams()

/**
 * Vypise vysledek operace na standartni vystup
 *
 * @param vysledneCislo Vysledek, ktery chceme vypsat
 */
void vypisVysledek(double vysledneCislo)
{
  printf("%.10le\n", vysledneCislo);
}

/**
 * Hlavni program
 */
int main(int argc, char *argv[])
{
  TParams params = getParams(argc, argv); // vyhodnotim parametry

  int stav;        // pomocna promenna - stav nacteni cisla
  double cislo;    // zde budu ukladat nactena cisla
  double vysledek; // vysledek dane operace

  /**
   * Budeme nacitat cisla ze standartniho vstupu.
   * Po kazdem spravne nactenem cisle provedeme danou operaci.
   * Nakonec vypiseme vysledek.
   * V pripade spatne zadaneho cisla (spatny format, atd.)
   * vypise na standartni chybovy vystup chybovou hlasku.
   */
   do
   {
    // nacte cislo a do promenne stav ulozi navratovou hodnotu fscanf
    stav = fscanf(stdin, "%le", &cislo);
    if (stav == 1) // cislo bylo nacteno v poradku
    {
      // vypocitame vysledek dane operace s danym cislem
      vysledek = params.funkce(cislo, params.epsilon);
      // vypiseme vysledek
      vypisVysledek(vysledek);
    }
    // spatne zadane cislo - vypise chybovou hlasku a pokracuje dal
    else if (stav != EOF)
    { // vypise na standardni chybovy vystup chybovou hlasku.
      fprintf(stderr, "Zadano neplatne cislo.\n");
      scanf("%*s"); // precte a zahodi vsechny znaky az do bileho znaku
    }
   } while (stav != EOF);

  return EXIT_SUCCESS; // ukonceni programu
}  // main()

////////////////////// Implementace matematicych funkci ///////////////////////

/**
 * Funkce SQRT.
 * Vypocita druhou odmocninu z danych cisel.
 *
 * @param x Double cislo, se kterym budeme pocitat
 * @param presnost Presnost zadana jako parametr programu
 * @return Vraci vysledek operace
 */
double funkceSQRT(double x, double presnost)
{
 /**
  * Podminka: lze vypocitat pouze pro hodnoty x >= 0
  *           Jestlize je x < 0, vraci NAN
  * Pro x = +INFINITY funkce vraci +INFINITY
  */
  if (x < 0.0)
    {
      return NAN;
    }
  if (x == +INFINITY)
    {
      return +INFINITY;
    } // konec podminky

  /**
   * Promenne pro vypocet/upravy
   */
  double stareC, noveC; // ukladani vysledku

  /**
   * Vlastni vypocet
   */
   stareC = 1.0;
   noveC = 0.5*(x/stareC + stareC);
   while (fabs(noveC - stareC) > presnost)
     {
       stareC = noveC;
       noveC = 0.5*(x/stareC + stareC);
     }

  return noveC;
} // funkceSQRT()

/**
 * Funkce COS.
 * Vypocita kosinus z daneho cisla.
 *
 * @param x Double cislo, se kterym budeme pocitat
 * @param presnost Presnost zadana jako parametr programu
 * @return Vraci vysledek operace
 */
double funkceCOS(double x, double presnost)
{
  /**
  * Podminky: nejsou
  *
  * Pokud x = +/-INFINITY, funkce vraci NaN
  */
  if ((x == +INFINITY) || (x == -INFINITY))
    {
      return NAN;
    } // konec podminky

  /**
   * Promenne pro vypocet/upravy
   */
   int k;
   double mocnina;       // pomocna promenna pro vypocet
   double stareC, noveC; // ukladani vysledku
   double soucetRady;    // ukladani celkoveho vysledku

  /**
   * Upravy pro zefektivneni algoritmu:
   *
   * Pro vetsi efektivitu a presnejsi vysledek je treba
   * upravit vstupni cislo.
   * Plati: cos(2kPI+x) = cos(x)
   * Tzn. pred samotnym vypoctem odstranime z cisla periody k*2PI
   */
   x = remainder(x, IZP_2PI);

  /**
   * Vlastni vypocet
   *
   * Nejdrive provedu heuristiku a odstranim prebytecne vypocty
   */
   k = 0;
   noveC = 1.0;
   soucetRady = noveC;
   mocnina = x*x;

   while (fabs(noveC) > presnost)
   {
     k += 2;
     stareC = noveC;
     noveC = ((-1.0)*stareC*mocnina)/(k*(k - 1));
     soucetRady += noveC;
   }

  return soucetRady;
} // funkceCOS()

/**
 * Funkce EX.
 * Vypocita e^x z daneho cisla.
 *
 * @param x Double cislo, se kterym budeme pocitat
 * @param presnost Presnost zadana jako parametr programu
 * @return Vraci vysledek operace
 */
double funkceEX(double x, double presnost)
{
  /**
  * Podminky: Jsme omezeni velikosti datoveho typu double
  *           Lze vypocitat pouze pro hodnoty mensi nez EX_MAX
  *           Pokud tato situace nastane, je vysledkem nekonecno.
  *           Pokud je cislo mensi nez EX_MIN, tak by vysledek
  *           nebyl zobrazitelny - vracim min. zobraz cislo
  */
  if (x > EX_MAX)
    {
      return INFINITY; // vysledek je nekonecno
    }
  if (x < EX_MIN)
    {
      return 0.0; // vysledek neni zobrazitelny
    } // konec podminky

  /**
   * Promenne pro vypocet/upravy
   */
   int i, j;
   double celaCast, desetCast;    // ukladani vysledku pomocneho vypoctu
   double stareC, noveC;          // ukladani vysledku
   double soucetCeleCasti, soucetRady, celkovySoucet; // ukladani cel. vysledku

  /**
   * Upravy pro zefektivneni algoritmu:
   *
   * Rychlost konvergence je velka pro male hodnoty x, pro velke
   * hodnoty x je treba upravit vstupni cislo.
   * Tzn. pred samotnym vypoctem prevedeme e^x = e^(c+d) = e^c*e^d,
   * kde c je cela cast a d je desetinna cast
   * Celou cast vypocitame opakovanym nasobenim konstanty IZP_E
   * a pokud je cislo zaporne, tak provedeme 1/vysledek.
   */
   desetCast = modf(x, &celaCast); // rozlozeni na celou a deset. cast
   soucetCeleCasti = 1.0;
   for (i = 1; i <= fabs(celaCast); i++)
     {
       soucetCeleCasti *= IZP_E;
     }
   if (x < 0.0) // jestlize bylo puvodni cislo zaporne
    {
      soucetCeleCasti = 1.0/soucetCeleCasti;
    } // konec upravy

  /**
   * Vlastni vypocet
   */
   j = 0;
   noveC = 1.0;
   soucetRady = noveC;

   while (fabs(noveC) > presnost)
    {
      j++;
      stareC = noveC;
      noveC = stareC * desetCast / j;
      soucetRady += noveC;
    }
   celkovySoucet = soucetCeleCasti * soucetRady;  // celkovy vysledek

  return celkovySoucet;
} // funkceEX()

/**
 * Funkce LN.
 * Vypocita prirozeny logaritmus ln(x) z daneho cisla.
 *
 * @param x Double cislo, se kterym budeme pocitat
 * @param presnost Presnost zadana jako parametr programu
 * @return Vraci vysledek operace
 */
double funkceLN(double x, double presnost)
{
  /* Podminka: lze vypocitat pouze pro hodnoty x > 0
   *
   * Jestlize je x = 0, vraci -INFINITY
   * Jestlize je x < 0, vraci NAN
   * Pokud je x = +INFINITY, funkce vraci +INFINITY
   */
     if (x == 0.0)
     {
       return -INFINITY;
     }
     if (x < 0.0)
     {
       return NAN;
     }
     if (x == +INFINITY)
     {
       return +INFINITY;
     } // konec podminky

  /**
   * Promenne pro vypocet/upravy
   */
   int k, znamenko;
   int pocetAkci; // citac pro pocet vykonanych akci * a /
   double stareC, noveC, soucetRady, vyslednySoucet, prirustek; // vysledky

  /**
   * Upravy pro zefektivneni algoritmu:
   *
   * Musime upravit vychozi cislo, pokud je prilis velke, resp. male.
   * Prirozeny log. lze upravit tak, ze cislo postupne delime, resp. nasobime
   * konstantou IZP_E a do promenne pocetAkci nacitame,
   * kolikrat akce probehla.
   * Z vysledneho cisla algoritmem spocitame vysledek a pricteme, resp. odecteme
   * pocet deleni, resp. nasobeni a vyjde nam vysledek.
   */
   pocetAkci = 0;

   if (x >= IZP_E)
   {
     while (x > IZP_E)
       {
         x = x/IZP_E;
         pocetAkci++;
       }
     znamenko = 1; // znamenko bude kladne
   }
   else if (x < IZP_E)
   {
     while (x < IZP_E)
       {
         x = x*IZP_E;
         pocetAkci++;
       }
     znamenko = -1; // znamenko bude zaporne
   } // konec upravy

  /**
   * Vlastni vypocet
   *
   * Nejdrive provedu heuristiku a odstranim prebytecne vypocty
   */
   k = 1;
   noveC = (x-1.0)/(x+1.0);
   soucetRady = noveC;
   prirustek = ((x-1.0)*(x-1.0))/((x+1.0)*(x+1.0));

   while (fabs(noveC) > presnost)
     {
       k += 2;
       stareC = noveC;
       noveC = stareC*prirustek;
       soucetRady = soucetRady + (1.0/k)*noveC;
     }
   vyslednySoucet = 2.0*soucetRady+znamenko*pocetAkci;

  return vyslednySoucet;
} // funkceLN()

/* konec souboru proj2.c */
