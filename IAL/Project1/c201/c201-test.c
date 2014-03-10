
/*
 *  Pøedmìt: Algoritmy (IAL) - FIT VUT v Brnì
 *  Základní testy pro pøíklad c201.c (Jednosmìrnì vázaný lineární seznam)
 *  Vytvoøil: Martin Tuèek, záøí 2005
 *  Úprava: Bohuslav Køena, øíjen 2006
 */

#include "c201.h"
                                                         /* pracovní promìnné */
tList TEMPLIST;
int obsah;			

/*******************************************************************************
 * Pomocné funkce usnadòující testování vlastní implementace.
 ******************************************************************************/

void print_elements_of_list(tList SEZNAM)	{
/* Vytiskne seznam. */	
	tList temp_S=SEZNAM;	
	printf("-----------------");
	while (temp_S.frst!=NULL)	{
		printf("\n\t %d",temp_S.frst->data);
		if ((temp_S.frst==SEZNAM.act) && (SEZNAM.act!=NULL))
			printf("\t <= toto je aktivní prvek ");
		temp_S.frst=temp_S.frst->ptr;	
	}
	printf("\n-----------------\n");
}

int use_copy(tList *TL, int *obsah)	{
/* O¹etøuje pou¾ití operace Copy. */
	int tmp;	
	Copy(TL,&tmp);
	if (!solved) {
		printf("Operace Copy nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace Copy volala funkci Error.\n");
			errflg=FALSE;
			return(FALSE);
		}	
		else	{	 
			*obsah=tmp;		
			printf("Operace Copy vrací obsah %d.\n",tmp);
			return(TRUE);
		}	
	}	
}

int use_copy_first(tList *TL, int *obsah)	{
/* O¹etøuje pou¾ití operace CopyFirst. */
	int tmp;
	CopyFirst(TL,&tmp);
	
	if (!solved)	{
		printf("Operace CopyFirst nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace CopyFirst volala funkci Error.\n");
			errflg=FALSE;
			return(FALSE);
		}	
		else	{	 
			*obsah=tmp;			
			printf("Operace CopyFirst vrací obsah %d.\n",tmp);
			return(TRUE);
		}	
	}	
}

int use_active(tList TL)	{
/* O¹etøuje pou¾ití operace Active. */
	int IsActive = Active(&TL);
	if (!solved)	{
		printf("Operace Active nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		printf("Operace Active vrací %d.\n",IsActive);
		return(TRUE);
	}	
}	

/*
 * Následující funkce volané z vlastních testù uvedených ve funkci main
 * kontrolují, zda byly jednotlivé funkce implementovány,
 * pøípadnì vypisují aktuální stav pracovního seznamu TEMPLIST.
 */ 

int test_InitList()	{

	solved=TRUE;	
	InitList(&TEMPLIST);
	if (!solved)	{
		printf("Operace InitList nebyla implementovana \n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}	
	
}

int test_Copy()	{	
	
	solved=TRUE;
	return(use_copy(&TEMPLIST,&obsah));
	
}

int test_CopyFirst()	{	
	
	solved=TRUE;
	return(use_copy_first(&TEMPLIST,&obsah));
	
}

int test_InsertFirst()	{
	
	solved=TRUE;
	InsertFirst(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace InsertFirst nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}	
	
}

int test_First()	{
	
	solved=TRUE;
	First(&TEMPLIST);
	if (!solved)	{
		printf("Operace First nebyla implementována!\n");
		return(FALSE);
	}
	print_elements_of_list(TEMPLIST);
	return(TRUE);
	
}	

int test_Active()	{
	
	solved=TRUE;
	return(use_active(TEMPLIST));
	
}

int test_Succ()	{
	
	solved=TRUE;
	Succ(&TEMPLIST);
	if (!solved)	{
		printf("Operace Succ nebyla implementována!\n");
		return(FALSE);
	}
	print_elements_of_list(TEMPLIST);
	return(TRUE);
	
}

int test_Actualize()	{		
	
	solved=TRUE;
	Actualize(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace Actualize nebyla implementována!\n");
		return(FALSE);
	}
	print_elements_of_list(TEMPLIST);
	return(TRUE);
	
}

int test_DeleteFirst()	{	
	
	solved=TRUE;
	DeleteFirst(&TEMPLIST);
	if (!solved)	{
		printf("Operace DeleteFirst() nebyla implementována!\n");
		return(FALSE);
	}	
	print_elements_of_list(TEMPLIST);
	return(TRUE);	
	
}	

int test_PostDelete()	{
	
	solved=TRUE;
	PostDelete(&TEMPLIST);
	if (!solved)	{
		printf("Operace PostDelete() nebyla implementována!\n");
		return(FALSE);
	}	
	print_elements_of_list(TEMPLIST);
	return(TRUE);
	
}

int test_PostInsert()	{
	
	solved=TRUE;
	PostInsert(&TEMPLIST,obsah);
	if (!solved){
		printf("Operace PostInsert nebyla implementována!\n");
		return(FALSE);
	}
	print_elements_of_list(TEMPLIST);
	return(TRUE);
	
}

int test_DisposeList() {

    solved=TRUE;
    DisposeList(&TEMPLIST);
    if (!solved) {
        printf("Operace DisposeList() nebyla implementována!\n");
        return(FALSE);
    }	
    print_elements_of_list(TEMPLIST);
    return(TRUE);	
}

/*******************************************************************************
 * ZÁKLADNÍ TESTY
 * -------------- 
 * Za jejich úspì¹né projítí získá student první èást bodù za pøíklad.
 * Pøi hodnocení v¹ak budou pou¾ity roz¹íøené testy, které se zamìøí
 * i na okrajové situace. Doporuèujeme proto, aby si ka¾dý student
 * zkusitl tyto základní testy roz¹íøit o dal¹í zajímavé situace.
 *  
 ******************************************************************************/

int main(int argc, char *argv[])	{

    printf("Jednosmìrnì vázaný lineární seznam\n");
    printf("==================================\n");

    printf("\n[TEST01]\n");
    printf("Inicializace seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    test_InitList();
	
    printf("\n[TEST02]\n");
    printf("Pokus o volání CopyFirst nad prázdným seznamem => chyba\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_CopyFirst();
	
    printf("\n[TEST03]\n");
    printf("Zavoláme 4x operaci InsertFirst.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=1; test_InsertFirst();
    obsah=2; test_InsertFirst();
    obsah=3; test_InsertFirst();
    obsah=4; test_InsertFirst();

    printf("\n[TEST04]\n");
    printf("Seznam je neaktivní -- ovìøíme si to voláním funce Active.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_Active(); 
	
    printf("\n[TEST05]\n");
    printf("Otestujeme funkci First pøi neaktivním seznamu a funkci Active.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_First();
    test_Active();
	
    printf("\n[TEST06]\n");
    printf("Test funkce Copy pøi aktivitì na prvním prvku\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_Copy();
 
    printf("\n[TEST07]\n");
    printf("Test funkce Succ -- voláme 3x, aktivita bude na posledním prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_Succ();
    test_Succ();
    test_Succ();
    test_Active();
 
    printf("\n[TEST08]\n");
    printf("Aktualizujeme obsah aktivního prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=10;	
    test_Actualize();
    test_Active();
    test_Copy();
	
    printf("\n[TEST09]\n"); 
    printf("Provedeme je¹tì jednou Succ -- aktivita se ztratí.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_Succ();
    test_Active();
	
    printf("\n[TEST10]\n");
    printf("Operace First nastaví aktivitu na první prvek.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_First();
    test_Active();

    printf("\n[TEST11]\n");
    printf("DeleteFirst aktivního prvku povede ke ztrátì aktivity.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DeleteFirst();
    test_Active();

    printf("\n[TEST12]\n");
    printf("U¾itím operací First a Succ nastavíme aktivitu na konec seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_First();
    test_Succ();
    test_Succ();
    test_Active();

    printf("\n[TEST13]\n");
    printf("Operace PostDelete pøi aktivitì na posledním prvku nedìlá nic.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_PostDelete();
    test_Active();
    
    printf("\n[TEST14]\n");
    printf("Nastavíme aktivitu na zaèátek a dvakrát zavoláme PostDelete.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_First();
    test_Active();
    test_PostDelete();
    test_PostDelete();
    test_Active();
	
    printf("\n[TEST15]\n");
    printf("Otestujeme InsertFirst pøi seznamu s jediným prvkem, nastavíme\n");
    printf("aktivitu na novì vlo¾ený prvek a opìt posuneme aktivitu na konec\n"); 
    printf("seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=101;
    test_InsertFirst();
    test_First();
    test_Succ();
    test_Active();

    printf("\n[TEST16]\n");
    printf("Operací PostInsert vlo¾íme nový prvek za poslední prvek seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=102;
    test_PostInsert();
    test_Active();

    printf("\n[TEST17]\n");
    printf("Nastavíme aktivitu na první prvek seznamu a vyzkou¹íme PostInsert.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_First();
    obsah=103;
    test_PostInsert();
    test_Active();

    printf("\n[TEST18]\n");
    printf("Otestujeme funkènost operace DisposeList.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DisposeList();
    
    printf("\n----------------------- konec pøíkladu c201 -------------------------\n");
	
    return(0);
}	
