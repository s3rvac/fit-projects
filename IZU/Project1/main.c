/* Misionari a kanibalove -- prvni ukol z IZU  LS2006
 * (C) Zdenek Mazal
 *  Ukolem je implementovat reseni zname hadanky o 3 kanibalech a 3 misionarich
 *  kteri maji za ukol dostat se na druhou stranu brehu za pomoci lodky o 2 mis-
 *  tech. Na zadnem brehu nesmi byt kanibalu vice nez misionaru, jinak by byli
 *  sezrani.
 */

#include <stdio.h>
#include <stdlib.h>

// konstanty -- pocet kanibalu a misionaru
#define CANNIBALS_CNT 6
#define MISSIONARIES_CNT 8
#define DEFAULT_BOAT_POSITION 1
// pomocne definice pro implementaci -- kardinalita mnoziny operatoru
#define OPERATORS_CNT 10
// fronta i zasobnik jsou pro jednoduchost implementovany staticky pomoci pole
#define MAX_STACK_SIZE 1000
#define MAX_QUEUE_SIZE 1000

// operatory ulohy, tedy vsechny mozne kombinace, ktere se daji vozit lodkou
// obemi smery
typedef enum operators {
  TWO_CANNIBALS_FORW,
  ONE_CANNIBAL_FORW,
  ONE_CANN_ONE_MISS_FORW,
  TWO_MISSIONARIES_FORW,
  ONE_MISSIONARY_FORW,
  TWO_CANNIBALS_BACK,
  ONE_CANNIBAL_BACK,
  ONE_CANN_ONE_MISS_BACK,
  TWO_MISSIONARIES_BACK,
  ONE_MISSIONARY_BACK,
} operators;

/* definice stavu ulohy -- vyjadrujeme jej v souladu s prednaskami jako trojici
 * - pocet kanibalu a misionaru na prvnim brehu
 * - poloha lodky (1 = prvni breh, 0 = cilovy breh)
 * dale je k dispozici operator, kterym jsme se do stavu dostali kvuli vypisum
 * cilovy stav tedy je (0,0,0)
 */
typedef struct status{
  int cannibals;
  int missionaries;
  int boat;
  operators appliedOperator;
} status;

/*
  pomocna struktura zasobnik - pro manipulaci pouzivejte vyhradne definovane
  funkce
*/
typedef struct stack{
  int top;
  status data[MAX_STACK_SIZE];
} stack;

/*
  pomocna struktura fronta - pro manipulaci pouzivejte vyhradne definovane
  funkce
*/
typedef struct queue{
  int front;
  int end;
  status data[MAX_STACK_SIZE];
} queue;


/*******************************************************************************
  nasleduji funkce pro praci se zasobnikem, a to:
    void stackPush(stack* s, status stat) - vlozi stav do zasobniku
    status stackPop(stack* s) - vyjme a vrati stav ze zasobniku
    void stackInit(stack* s) - inicializuje zasobnik
    int stackEmpty(stack* s) - vraci 1 pokud je zasobnik prazdny
    int stackIsPresent(stack* s, status stat) - vraci 1 pokud se stav nachazi v
                                                zasobniku
    void stackPrint(stack* s) - vypise obsah zasobniku
*******************************************************************************/

void stackPush(stack* s, status stat){
  s->top ++; // vrchol bude dalsi prvek
  if (s->top < MAX_STACK_SIZE){ // pokud neni plno
    s->data[s->top] = stat; // ulozi se
  }
  else{ // pokud je preplneno, exit
    fprintf(stderr, "ERROR: Stack full.");
    exit(1);
  }
}

status stackPop(stack* s){
  if (s->top >= 0){ // pokud neni prazdno
    s->top--; // vrchol bude predchozi prvek
    return s->data[s->top + 1]; // a puvodni vrchol se vrati
  }
  else{ // pokus o vyber z prazdneho zasobniku
    fprintf(stderr, "ERROR: Stack empty.");
    exit(2);
  }
}

void stackInit(stack* s){
  s->top = -1; // na pocatku je index mimo pole
}

int stackEmpty(stack* s){
  return (s->top < 0); // pokud je index mimo pole...
}

int stackIsPresent(stack* s, status stat){
  int i;
  for (i = 0; i <= s->top; i++){ // pro vsechny aktualni zaznamy
    if (stat.cannibals == s->data[i].cannibals &&  // se provede porovnani stavu
        stat.missionaries == s->data[i].missionaries &&
        stat.boat == s->data[i].boat
    ){
      return 1; // pokud nastane shoda, vraci se uspech
    }
  }
  return 0; // jinak neuspech
}

void stackPrint(stack* s){
  int i;
  for (i = 0; i <= s->top; i++){ // pro vsechny aktualni zaznamy
    printf("(%d %d %d) ", s->data[i].cannibals, s->data[i].missionaries,
    s->data[i].boat); // provedeme tisk
  }
}

/*******************************************************************************
  nasleduji funkce pro praci s frontou, a to:
    void queueInsert(queue* q, status stat) - vlozi stav do fronty
    status queueRemove(queue* q) - vyjme a vrati stav z konce fronty
    void queueInit(queue* q) - inicializuje frontu
    int queueEmpty(queue* q) - vraci 1 pokud je fronta prazdna
    int queueIsPresent(queue* q, status stat) - vraci 1 pokud se stav nachazi ve
                                                fronte
    void queuePrint(queue* q) - vypise obsah fronty
*******************************************************************************/

void queueInsert(queue* q, status stat){
  q->end ++; // posune se konec
  q->end %= MAX_QUEUE_SIZE; // provede se pripadna rotace
  if (q->end != q->front){ // pokud neni plno
    q->data[q->end] = stat; // vlozi se stav
  }
  else{ // pokud je preplneno, exit
    fprintf(stderr, "ERROR: Queue full.");
    exit(3);
  }
}

status queueRemove(queue* q){
  if (q->end != q->front){ // pokud neni fronta prazdna
    q->front ++; // posune se zacatek
    q->front %= MAX_QUEUE_SIZE; // provede se pripadna rotace
    return q->data[q->front]; // a vrati se prislusny prvek
  }
  else{ // pokud je fronta prazdna
    fprintf(stderr, "ERROR: Queue empty.");
    exit(4);
  }
}

void queueInit(queue* q){
  q->front = -1; // inicializace mimo pole
  q->end = -1;
}

int queueEmpty(queue* q){
  return (q->front == q->end); // prazdno je pri rovnosti indexu
}

int queueIsPresent(queue* q, status stat){
  int i = q->front + 1; // zacne se od prvniho prvku
  i %= MAX_QUEUE_SIZE; // provede se pripadna rotace
  while (i != q->end + 1){ // dokud nejsou projite vsechny platne zaznamy
    if (stat.cannibals == q->data[i].cannibals && // porovnava se
        stat.missionaries == q->data[i].missionaries &&
        stat.boat == q->data[i].boat
    ){
      return 1; // pripadne se vraci uspech
    }
    i++; // zvyseni citace
    i %= MAX_QUEUE_SIZE;
  }
  return 0; // nenalezeno
}

void queuePrint(queue* q){
  int i = q->front + 1; // obdoba predchozi fce, jen se misto porovnani tiskne
  i %= MAX_QUEUE_SIZE;
  while (i != q->end + 1){
    printf("(%d %d %d) ", q->data[i].cannibals, q->data[i].missionaries,
                          q->data[i].boat);
    i++;
    i %= MAX_QUEUE_SIZE;
  }
}

/******************************************************************************/

// vypise stav jako trojici (pocet kanibalu na brehu 1, pocet misionaru, lodka)
// + pouzity operator
void printStatus(status stat){
  printf("(st: %d %d %d, op: ", stat.cannibals, stat.missionaries, stat.boat);
  switch (stat.appliedOperator){
    case TWO_CANNIBALS_FORW: printf("2CAN->"); break;
    case ONE_CANNIBAL_FORW: printf("1CAN->");  break;
    case ONE_CANN_ONE_MISS_FORW: printf("1CAN+1MIS->");  break;
    case TWO_MISSIONARIES_FORW: printf("2MIS->");  break;
    case ONE_MISSIONARY_FORW: printf("1MIS->");  break;
    case TWO_CANNIBALS_BACK: printf("2CAN<-");  break;
    case ONE_CANNIBAL_BACK: printf("1CAN<-");  break;
    case ONE_CANN_ONE_MISS_BACK: printf("1CAN+1MIS<-");  break;
    case TWO_MISSIONARIES_BACK: printf("2MIS<-");  break;
    case ONE_MISSIONARY_BACK: printf("1MIS<-");  break;
  }
  printf(") ");
}

// otestuje, zda se operator da pouzit ve stavu zadanem parametrem
int isApplicable(operators oper, status stat){
	switch (oper) {
		case TWO_CANNIBALS_FORW:
			if (stat.cannibals >= 2 &&
				stat.boat == 1) {
				if ((MISSIONARIES_CNT - stat.missionaries) > 0) {
					if ((CANNIBALS_CNT - stat.cannibals + 2) <= (MISSIONARIES_CNT - stat.missionaries)) {
						return 1;
					} else {
						return 0;
					}
				} else {
					return 1;
				}
			}
		break;
		case ONE_CANNIBAL_FORW:
			if (stat.cannibals >= 1 &&
				stat.boat == 1) {
				if ((MISSIONARIES_CNT - stat.missionaries) > 0) {
					if ((CANNIBALS_CNT - stat.cannibals + 1) <= (MISSIONARIES_CNT - stat.missionaries)) {
						return 1;
					} else {
						return 0;
					}
				} else {
					return 1;
				}
			}
		break;
		case ONE_CANN_ONE_MISS_FORW:
			if (stat.cannibals >= 1 &&
				stat.missionaries >= 1 &&
				stat.boat == 1) {
				if ((CANNIBALS_CNT - stat.cannibals + 1) <= (MISSIONARIES_CNT - stat.missionaries + 1)) {
					return 1;
				} else {
					return 0;
				}
			}
		break;
		case TWO_MISSIONARIES_FORW:
			if (stat.missionaries >= 2 &&
				stat.boat == 1) {
				if ((stat.missionaries - 2) > 0) {
					if (stat.cannibals <= (stat.missionaries - 2) &&
						(CANNIBALS_CNT - stat.cannibals) <= (MISSIONARIES_CNT - stat.missionaries + 2)) {
						return 1;
					}
				} else {
					if ((CANNIBALS_CNT - stat.cannibals) <= (MISSIONARIES_CNT - stat.missionaries + 2)) {
						return 1;
					}
				}
			}
		break;
		case ONE_MISSIONARY_FORW:
			if (stat.missionaries >= 1 &&
				stat.boat == 1) {
				if ((stat.missionaries - 1) > 0) {
					if (stat.cannibals <= (stat.missionaries - 1) &&
						(CANNIBALS_CNT - stat.cannibals) <= (MISSIONARIES_CNT - stat.missionaries + 1)) {
						return 1;
					}
				} else {
					if ((CANNIBALS_CNT - stat.cannibals) <= (MISSIONARIES_CNT - stat.missionaries + 1)) {
						return 1;
					}
				}
			}
		break;
		case TWO_CANNIBALS_BACK:
			if ((stat.cannibals + 2) <= CANNIBALS_CNT &&
				stat.boat == 0) {
				if (stat.missionaries > 0) {
					if ((stat.cannibals + 2) <= stat.missionaries) {
						return 1;
					} else {
						return 0;
					}
				} else {
					return 1;
				}
			}
		break;
		case ONE_CANNIBAL_BACK:
			if ((stat.cannibals + 1) <= CANNIBALS_CNT &&
				stat.boat == 0) {
				if (stat.missionaries > 0) {
					if ((stat.cannibals + 1) <= stat.missionaries) {
						return 1;
					} else {
						return 0;
					}
				} else {
					return 1;
				}
			}
		break;
		case ONE_CANN_ONE_MISS_BACK:
			if ((stat.cannibals + 1) <= CANNIBALS_CNT &&
				(stat.missionaries + 1) <= MISSIONARIES_CNT &&
				stat.boat == 0) {
				if ((stat.cannibals + 1) <= (stat.missionaries + 1)) {
					return 1;
				} else {
					return 0;
				}
			}
		break;
		case TWO_MISSIONARIES_BACK:
			if (stat.missionaries <= (MISSIONARIES_CNT - 2) &&
				stat.boat == 0) {
				if ((MISSIONARIES_CNT - stat.missionaries - 2) > 0) {
					if ((CANNIBALS_CNT - stat.cannibals) <= (MISSIONARIES_CNT - stat.missionaries - 2) &&
						stat.cannibals <= (stat.missionaries + 2)) {
						return 1;
					} else {
						return 0;
					}
				} else {
					if (stat.cannibals <= (stat.missionaries + 2)) {
						return 1;
					} else {
						return 0;
					}
				}
			}
		break;
		case ONE_MISSIONARY_BACK:
			if (stat.missionaries <= (MISSIONARIES_CNT - 1) &&
				stat.boat == 0) {
				if ((MISSIONARIES_CNT - stat.missionaries - 1) > 0) {
					if ((CANNIBALS_CNT - stat.cannibals) <= (MISSIONARIES_CNT - stat.missionaries - 1) &&
						stat.cannibals <= (stat.missionaries + 1)) {
						return 1;
					} else {
						return 0;
					}
				} else {
					if (stat.cannibals <= (stat.missionaries + 1)) {
						return 1;
					} else {
						return 0;
					}
				}
			}
		break;
	}

	return 0;
}

// aplikuje operator, vraci novy stav
status applyOperator(operators oper, status stat){
	status newState;
	newState.appliedOperator = oper;

	switch (oper) {
		case TWO_CANNIBALS_FORW:
			newState.cannibals = stat.cannibals - 2;
			newState.missionaries = stat.missionaries;
			newState.boat = 0;
		break;
		case ONE_CANNIBAL_FORW:
			newState.cannibals = stat.cannibals - 1;
			newState.missionaries = stat.missionaries;
			newState.boat = 0;
		break;
		case ONE_CANN_ONE_MISS_FORW:
			newState.cannibals = stat.cannibals - 1;
			newState.missionaries = stat.missionaries - 1;
			newState.boat = 0;
		break;
		case TWO_MISSIONARIES_FORW:
			newState.cannibals = stat.cannibals;
			newState.missionaries = stat.missionaries - 2;
			newState.boat = 0;
		break;
		case ONE_MISSIONARY_FORW:
			newState.cannibals = stat.cannibals;
			newState.missionaries = stat.missionaries - 1;
			newState.boat = 0;
		break;
		case TWO_CANNIBALS_BACK:
			newState.cannibals = stat.cannibals + 2;
			newState.missionaries = stat.missionaries;
			newState.boat = 1;
		break;
		case ONE_CANNIBAL_BACK:
			newState.cannibals = stat.cannibals + 1;
			newState.missionaries = stat.missionaries;
			newState.boat = 1;
		break;
		case ONE_CANN_ONE_MISS_BACK:
			newState.cannibals = stat.cannibals + 1;
			newState.missionaries = stat.missionaries + 1;
			newState.boat = 1;
		break;
		case TWO_MISSIONARIES_BACK:
			newState.cannibals = stat.cannibals;
			newState.missionaries = stat.missionaries + 2;
			newState.boat = 1;
		break;
		case ONE_MISSIONARY_BACK:
			newState.cannibals = stat.cannibals;
			newState.missionaries = stat.missionaries + 1;
			newState.boat = 1;
		break;
	}

	return newState;
}

// vygeneruje stavy pro DFS, ktere jsou dosazitelne z daneho stavu
// a vlozi je do open
void generateDFS(status stat, stack* open, stack* closed){
	status nextState;
	operators oper;

	for (oper = TWO_CANNIBALS_FORW; oper <= ONE_MISSIONARY_BACK; oper++) {
		if (isApplicable(oper, stat)) {
			nextState = applyOperator(oper, stat);
			if (!stackIsPresent(open, nextState) && !stackIsPresent(closed, nextState)) {
				stackPush(open, nextState);
				printStatus(nextState);
			}
		}
	}

	printf("\n");
}

// vygeneruje stavy pro BFS, ktere jsou dosazitelne z daneho stavu
// a vlozi je do open
void generateBFS(status stat, queue* open, queue* closed){
	status nextState;
	operators oper;

	for (oper = TWO_CANNIBALS_FORW; oper <= ONE_MISSIONARY_BACK; oper++) {
		if (isApplicable(oper, stat)) {
			nextState = applyOperator(oper, stat);
			if (!queueIsPresent(open, nextState) && !queueIsPresent(closed, nextState)) {
				queueInsert(open, nextState);
				printStatus(nextState);
			}
		}
	}

	printf("\n");
}

// returns true if the selected state is the final one
int isFinalState(status state) {
	return (state.cannibals == 0 &&
		state.missionaries == 0 &&
		state.boat == 0);
}

int main(int argc, char *argv[]){

  status s; // pocatecni stav
  stack openDFS; // zasobnik open pro DFS
  stack closedDFS; // zasobnik closed pro DFS
  queue openBFS; // fronta open pro BFS
  queue closedBFS; // fronta closed pro BFS
  status currentStatus; // pomocny stav
  int i; // citac

  s.cannibals = CANNIBALS_CNT; // nastaveni pocatecniho stavu
  s.missionaries = MISSIONARIES_CNT;
  s.boat = DEFAULT_BOAT_POSITION;

  /********************* DFS ********************/
  printf("DFS:\n===\n");

  stackInit(&openDFS); // inicializace zasobniku
  stackInit(&closedDFS);
  stackPush(&openDFS,s); // vlozeni startovaciho stavu

	// DFS loop
	for (;;) {
		if (stackEmpty(&openDFS)) {
			// DFS failed
			printf("DFS: Solution wasn't found.\n");
			break;
		}

		currentStatus = stackPop(&openDFS);
		// Check whether DFS was successfull
 		if (isFinalState(currentStatus)) {
 			// Yes, it was
 			break;
 		}

		stackPush(&closedDFS, currentStatus);
		generateDFS(currentStatus, &openDFS, &closedDFS);

		printf("OPEN:");
		stackPrint(&openDFS);
		printf("\n");
		printf("CLOSED:");
		stackPrint(&closedDFS);
		printf("\n");
	}

  /*****************************************/
  printf("\n\n\nBFS:\n===\n");

  queueInit(&openBFS); // inicilizace fronty
  queueInit(&closedBFS);
  queueInsert(&openBFS,s); // vlozeni pocatecniho stavu

	// BFS loop
	for (;;) {
		if (queueEmpty(&openBFS)) {
			// BFS failed
			printf("BFS: Solution wasn't found.\n");
			break;
		}

		currentStatus = queueRemove(&openBFS);
		// Check whether BFS was successfull
 		if (isFinalState(currentStatus)) {
			// Yes, it was
			break;
 		}

		queueInsert(&closedBFS, currentStatus);
		generateBFS(currentStatus, &openBFS, &closedBFS);

		printf("OPEN:");
		queuePrint(&openBFS);
		printf("\n");
		printf("CLOSED:");
		queuePrint(&closedBFS);
		printf("\n");
	}

  return 0;
}
