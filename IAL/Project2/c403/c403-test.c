

/* Testy pro domácí úkol na téma Vyhledávací tabulka v neseøazeném poli
se zar. (optimalizace) (c403.c). Obsahuje procedury a funkce, které 
testují studentské verze principem jejich volání (vıpis na stdout). 
Toto je rozšíøená verze, obsahující komplexní mnoství procedur a funkcí 
na otestování. Slouí pro lektory ke kontrole pèíslušné domácí úlohy */

/* **********************SOUBOR S ST.TESTEM  *********************** */
/* **********************-----------------   *********************** */
/*  vytvoèil: Martin Tuèek
    pøedmìt: Algoritmy (IAL) - FIT (Fakulta Informacnich Technologií)
    test pro soubor: c403.c
	datum: záøí 2005												 */	
/* ***************************************************************** */

#include "c403.h"

tASTable TempTable;
int Key, ind;
int content;

/* pomocna procedura pro vypis prvku v poli */
void PrintElement(tASTable *TempTable)			{
	int i;
        printf("Aktuální stav tabulky:\n");
	for (i = 0; i < ASSize; i++)		
		printf("Pozice: %d Klíè: %d, Hodnota: %d\n",
                i,TempTable->arr[i].Key,TempTable->arr[i].Cont);
        printf("Index posledního prvku je: %d\n", TempTable->last);
        printf("=========================================================\n");

}


/* .............................. vypis jednotlivych testu .............................. */ 

int test_ASInit(tASTable *TempTable) {
	solved=TRUE;
	ASInit(TempTable);
	if (!solved)	{
		printf("Operace ASInit() nebyla implementovana \n");
		return(FALSE);
	}	
	else	{	 	
		PrintElement(TempTable);
		return(TRUE);	
	}		
}
	
int test_ASSearch (tASTable *TempTable, int Content, int *ind) {
	solved=TRUE;
	int found;
        ASCompNum = 0;
        found = ASSearch(TempTable, Content, ind);	
	if (!solved)	{
		printf("Operace ASSearch() nebyla implementovana \n");
		return(FALSE);
	}
	else	{
		PrintElement(TempTable);
                printf("Hodnota nalezena: ");
                if (found) 
                   printf("ANO\n");
                else
                   printf("NE\n");
                printf("Index vrácen: %d\n", *ind);
                printf("Poèet provedenıch porovnání: %d\n", ASCompNum);
	        printf("=========================================================\n");
                ASCompNum = 0;  
		return(TRUE);
	}	
}

int test_ASInsert(tASTable *TempTable, int Key, int Content) {
	solved=TRUE;
	ASInsert(TempTable, Key, Content);	
	if (!solved)	{
		printf("Operace ASInsert() nebyla implementovana \n");
		return(FALSE);
	}
	else	{	
		PrintElement(TempTable);
		return(TRUE);
	}	
}

/* .......................... sekce volani jednotlivych testu .............................*/ 

int main(int argc, char *argv[])			{
	
	printf("Vkládání a vyhledávání v neseøazeném poli (zaráka, optimalizace) - pokrocile testy\n");
	printf("===================================================================================\n");
	
	printf("[TEST01]\n");
	printf("Nastavíme testovací velikost tabulky na 5 a provedeme inicializaci.\n");
	ASSize=5;
	test_ASInit(&TempTable);
		
	printf("[TEST02]\n");
	printf("Pokusíme se vyhledat neexistující poloku s klíèem 1\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	Key=1;
	test_ASSearch(&TempTable, Key, &ind);
	
	printf("[TEST03]\n");
	printf("Vloíme první poloku s hodnotou 1,klíèem 1\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	content=1;
	Key=1;
	test_ASInsert(&TempTable, Key, content);
	
	printf("[TEST04]\n");
	printf("Pokusíme se znovu vyhledat poloku s klíèem 1\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_ASSearch(&TempTable, Key, &ind);
  	
	printf("[TEST05]\n");
	printf("Ještì jednou vyhledáme první poloku v tabulce (test posouvání)\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_ASSearch(&TempTable, Key, &ind);
	
	printf("[TEST06]\n");
	printf("Pokusíme se vyhledat neexistující poloku s klíèem 2\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	Key=2;
	test_ASSearch(&TempTable, Key, &ind);
  
	printf("[TEST07]\n");
	printf("Provedeme aktualizaci poloky 1 na hodnotu 9\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	Key=1;
	content=9;
	test_ASInsert(&TempTable, Key, content);	
	
	printf("[TEST08]\n");
	printf("Vloíme další poloky\n");
	printf("~~~~~~~~~~~~~~~~~~~~~\n");
	content=1;
	Key=2;
	test_ASInsert(&TempTable, Key, content);
	content=2;
	Key=3;
	test_ASInsert(&TempTable, Key, content);
	content=3;
	Key=4;
	test_ASInsert(&TempTable, Key, content);

	printf("[TEST09]\n");
	printf("Další poloka ji nesmí jít vloit, nebo by nezùstalo místo pro zaráku\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	content=4;
	Key=5;
	test_ASInsert(&TempTable, Key, content);
  
	printf("[TEST10]\n");
	printf("Provedeme aktualizaci poloky 2 na hodnotu 5. Mìl by nastat pøesun.\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	Key=2;
	content=5;
	test_ASInsert(&TempTable, Key, content);	

	printf("[TEST11]\n");
	printf("Budeme 5x vyhledávat poslední vloenou poloku 4\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	Key=4;
	int i;
	for(i = 1;i < 5;i++)
	test_ASSearch(&TempTable, Key, &ind);
	
	printf("[TEST12]\n");
	printf("Provedeme aktualizaci poloky 2 na hodnotu 8\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	Key=2;
	content=8;
	test_ASInsert(&TempTable, Key, content);	
	
	printf("[TEST13]\n");
	printf("Pokusíme se vloit další poloky, co nesmí jít\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	content=20;
	Key=30;
	test_ASInsert(&TempTable, Key, content);
	content=30;
	Key=40;
	test_ASInsert(&TempTable, Key, content);

	printf("[TEST14]\n");
	printf("A na závìr tabulku inicializujeme (ztráta všech dat)\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_ASInit(&TempTable);
	
	printf("[TEST15]\n");
	printf("A ještì jedna inicializace (ztráta všech dat) \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
	test_ASInit(&TempTable);

	printf("------------------------------ konec -------------------------------------\n");
	return(0);
}
