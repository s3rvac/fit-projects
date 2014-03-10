/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		happyscanner.c
	Projekt:	IFJ06 - test

	Popis: Testovani lexikalniho analyzatoru (scanneru)
******************************************************************************/

#include <stdio.h>
#include "../src/stacks.h"

int main (void)
{
  SItemD itemD;
  SItemP itemP;
  
  stackD_init();
  stackP_init();
  
  for (int i = 0; i < 25; i++)
  {
    itemD.IntVal = itemP.BP = i;
    pushD_value (itemD);
    pushP_value (itemP);
  }
  
  printf ("Data top?: %i\n", stackD_top());
  pushD_reserve (20);
  for (int i = 25; i < 45; i++)
  {
    itemD.IntVal = 111;
    putD_variable (i, itemD);
  }
  printf ("Data top?: %i\n", stackD_top());
  
  for (int i = 44; i>= 0; i--)
  {
    itemD = getD_variable(i);
    printf ("%d: %d\n", i, itemD.IntVal);
  }
  
  popD_block (45);
    
  printf ("Data top?: %i\n", stackD_top());
  
  printf ("Ptr stack empty?: %i\n", stackP_is_empty());
  
  for (int i =0; i < 25; i++)
  {
    itemP = popP_value();
    printf ("%d: %d\n", i, itemP.BP);
  }
  
  printf ("Ptr stack empty?: %i\n", stackP_is_empty());
  
  stackD_dispose();
  stackP_dispose();


  return 0;
}
