
/*
 *  Pøedmìt: Algoritmy (IAL) - FIT VUT v Brnì
 *  Základní testy pro pøíklad c206.c (Dvousmìrnì vázaný lineární seznam)
 *  Vytvoøil: Martin Tuèek, záøí 2005
 *  Úprava: Bohuslav Køena, øíjen 2006
 */

#include "c206.h"
                                                         /* pracovní promìnné */
tDLList TEMPLIST;		
int obsah=1;

/*******************************************************************************
 * Pomocné funkce usnadòující testování vlastní implementace.
 ******************************************************************************/

void print_elements_of_list(tDLList SEZNAM)	{
	
	tDLList temp_S=SEZNAM;	
	printf("-----------------");
	while (temp_S.First!=NULL)	{
		printf("\n \t%d",temp_S.First->data);
		if ((temp_S.First==SEZNAM.Act) && (SEZNAM.Act!=NULL))
			printf("\t <= toto je aktivní prvek ");
		temp_S.First=temp_S.First->rptr;	
	}
	printf("\n-----------------\n");
}

int test_DLInitList()	{
	solved=TRUE;	
	DLInitList(&TEMPLIST);
	if (!solved)	{
		printf("Operace InitList nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}	
}

int test_DLDisposeList()	{
	solved=TRUE;
	DLDisposeList(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLDisposeList nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}	
}

int test_DLInsertFirst()	{
	solved=TRUE;
	DLInsertFirst(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLInsertFirst nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}	

int test_DLInsertLast()	{
	solved=TRUE;
	DLInsertLast(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLInsertLast nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}	

int test_DLFirst()	{
	solved=TRUE;
	DLFirst(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLFirst nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLLast()	{
	solved=TRUE;
	DLLast(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLLast nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLCopyFirst()	{
	solved=TRUE;	
    int temp=DLCopyFirst(&TEMPLIST);	
	if (!solved)	{
		printf("Operace DLCopyFirst nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace DLCopyFirst volala funkci DLError.\n");
			errflg=FALSE;
			return(FALSE);
		}	
		else	{	 			
			printf("Operace DLCopyFirst vrací obsah %d.\n",temp);
			return(TRUE);
		}	
	}
}	

int test_DLCopyLast()	{
	solved=TRUE;
    int temp=DLCopyLast(&TEMPLIST);	
	if (!solved)	{
		printf("Operace DLCopyLast nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace DLCopyLast volala funkci DLError.\n");
			errflg=FALSE;
			return(FALSE);
		}	
		else	{	 			
			printf("Operace DLCopyLast vrací obsah %d.\n",temp);
			return(TRUE);
		}	
	}
}	

int test_DLDeleteFirst ()	{
	solved=TRUE;	
	DLDeleteFirst(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLDeleteFirst nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}	

int test_DLDeleteLast ()	{
	solved=TRUE;	
	DLDeleteLast(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLDeleteLast nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}	

int test_DLPostDelete()	{
	solved=TRUE;	
	DLPostDelete(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLPostDelete nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}	

int test_DLPreDelete()	{
	solved=TRUE;	
	DLPreDelete(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLPreDelete nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPostInsert()	{
	solved=TRUE;
	DLPostInsert(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLPostInsert nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPreInsert()	{
	solved=TRUE;
	DLPreInsert(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLPreInsert nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLCopy()	{
	solved=TRUE;
	int temp=DLCopy(&TEMPLIST);	
	
	if (!solved)	{
		printf("Operace DLCopy nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace DLCopy volala funkci DLError.\n");
			errflg=FALSE;
			return(FALSE);
		}	
		else	{	 			
			printf("Operace DLCopy vrací obsah %d.\n",temp);
			return(TRUE);
		}	
	}	
}

int test_DLActualize()	{
	solved=TRUE;
	DLActualize(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLActualize nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLSucc()	{
	solved=TRUE;
	DLSucc(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLSucc nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPred()	{
	solved=TRUE;
	DLPred(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLPred nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLActive()	{
	solved=TRUE;
	int tmp=DLActive(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLActive nebyla implementována!\n");
		return(FALSE);
	}	
	else	{
		if (tmp)
			printf("Operace DLActive vrací hodnotu true.\n");
		else
			printf("Operace DLActive vrací hodnotu false.\n");	
		return(TRUE);			
	}
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
	
    printf("Dvousmìrnì vázaný lineární seznam\n");
    printf("=================================\n");

    printf("\n[TEST01]\n");
    printf("Inicializace seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    test_DLInitList();

    printf("\n[TEST02]\n");
    printf("Zavoláme 2x operaci DLInsertFirst a 2x operaci DLInsertLast.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");	
    obsah=1;
    test_DLInsertFirst();	
    obsah=2;
    test_DLInsertFirst();	
    obsah=3;
    test_DLInsertLast();
    obsah=4;
    test_DLInsertLast();
	
    printf("\n[TEST03]\n");
    printf("Otestujeme funkci DLFirst pøi neaktivním seznamu a funkci DLActive.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    test_DLActive();

    printf("\n[TEST04]\n");
    printf("Otestujeme funkci DLLast.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLLast();
    test_DLActive();
	
    printf("\n[TEST05]\n");
    printf("Test funkce DLCopy pøi aktivitì na posledním prvku\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopy();

    printf("\n[TEST06]\n");
    printf("Test funkce DLPred -- voláme 3x, aktivita zùstane na prvním prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPred();
    test_DLPred();
    test_DLPred();
    test_DLActive();

    printf("\n[TEST07]\n");
    printf("Aktualizujeme obsah aktivního prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");	
    obsah=5;
    test_DLActualize();
    test_DLActive();
    test_DLCopy();
	
    printf("\n[TEST08]\n");
    printf("Test funkce DLSucc -- voláme 3x, aktivita zùstane na posledním prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLSucc();
    test_DLSucc();
    test_DLSucc();
    test_DLActive();
	
    printf("\n[TEST09]\n");
    printf("Provedeme je¹tì jednou DLSucc - aktivita se ztratí.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLSucc();
    test_DLActive();
	
    printf("\n[TEST10]\n");
    printf("Nastavíme aktivitu na zaèátek a pak ji zru¹íme operací DLPred.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    test_DLPred();
    test_DLActive();
	
    printf("\n[TEST11]\n");
    printf("Pokus o DLCopy pøi neaktivním seznamu => o¹etøená chyba.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopy();
    
    printf("\n[TEST12]\n");
    printf("Pou¾ití operace DLCopyFirst pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopyFirst();

    printf("\n[TEST13]\n");
    printf("Pou¾ití operace DLCopyLast pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopyLast();

    printf("\n[TEST14]\n");
    printf("Pou¾ití operace DLDeleteFirst pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDeleteFirst();
    test_DLActive();

    printf("\n[TEST15]\n");
    printf("Pou¾ití operace DLDeleteLast pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDeleteLast();
      test_DLActive();
	
    printf("\n[TEST16]\n");
    printf("Zkusíme DLPostDelete pøi aktivitì na posledním prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLLast();
    test_DLPostDelete();
    test_DLActive();
	
    printf("\n[TEST17]\n");
    printf("Operace DLPreDelete nyní sma¾e první prvek.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPreDelete();
    test_DLActive();
    
    printf("\n[TEST18]\n"); 
    printf("Seznam nakonec zru¹íme.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDisposeList();
    test_DLActive();

    printf("\n----------------------- konec pøíkladu c206 -------------------------\n");
		
    return(0);
} 
