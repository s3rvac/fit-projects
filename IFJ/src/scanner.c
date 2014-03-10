/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		scanner.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Lexikalni analyzator - scanner - implementace
******************************************************************************/

#include <stdio.h>
#include <string.h> // strcmp
#include <stdlib.h> // malloc
#include <ctype.h>  // isalpha, isalnum, isdigit, isspace
#include <errno.h>  // detekce preteceni
#include <limits.h> // MAX_INT MIN_INT
#include "debug.h"
#include "error.h"
#include "token.h"
#include "scanner_define.h"
#include "scanner.h"
#include "strptr_stack.h" //ukladani ukazatelu na retezce

#if SCANNER_DEBUG
	#include "scanner_debug.c"
#endif

/*
 * Retezcove podoby klicovych slov z vyctu Etoken (token.h)
 */
static const char *keywords[] = {
	"int",
	"double",
	"string",
	"void",
	"var",
	"goto",
	"if",
	"return",
	"div",
	NULL
};


/*
 * Funkce rozpoznava klicova slova ("specialni identifikatory") v lexemech
 * Funkce vraci typ tokenu (tj. bud TOK_{INT|DOUBLE...} nebo TOK_IDENTIFIER)
 */
static Etoken get_keyword_or_identifier(const char *str)
{
	// Prohledame seznam klicovych slov
	for (Etoken i = 0; keywords[i] != NULL; i++) {
		if (strcmp(keywords[i], str) == 0) {
			// Pokud jsme nasli klicove slovo, vrat index
			return i;
		}
	}

	// Retezec neni klicove slovo, je identifikator
	return TOK_IDENTIFIER;
}

/*
 * [A-Za-z]
 */
static inline int ismyalpha(int c)
{
	/* return isalpha(c); */
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

/*
 * [0-9]
 */
inline int ismydigit(int c)
{
	return isdigit(c);
}

/*
 * [A-Za-z0-9]
 */
static inline int ismyalnum(int c)
{
	/* return isalnum(c); */
	return (ismyalpha(c) || ismydigit(c));
}

/*
 * Bile znaky/oddelovace
 */
inline int ismyspace(int c)
{
	return isspace(c);
}

/*
 * Bezpecna fce atoi, kontroluje preteceni.
 */
static inline void myatoi(Ttoken *token, const char *lexem)
{
	long l;

	errno = 0;
	l = strtol(lexem, NULL, 10);

	// Test na preteceni
	if (errno == ERANGE || l > INT_MAX || l < INT_MIN) {
		error(ROWLEX, "scanner: Preteceni [%s]\n", lexem);
		token->type = TOK_SCANNERERROR;
		token->attribute.ptr = NULL;
	}
	else {
		token->type = TOK_CONSTINT;
		token->attribute.i = (int) l;
	}
}

/*
 * Bezpecna fce atof, kontroluje preteceni.
 */
static inline void myatof(Ttoken *token, const char *lexem)
{
  double d;

	errno = 0;
	d = strtod(lexem, (char **)NULL);

	// Test na preteceni
	if (errno == ERANGE) {
		error(ROWLEX, "scanner: Preteceni [%s]\n", lexem);
		token->type = TOK_SCANNERERROR;
		token->attribute.ptr = NULL;
	}
	else {
		token->type = TOK_CONSTDOUBLE;
		token->attribute.d = d;
	}
}

/*
 * Citac pozice scanneru v souboru.
 * GET - vraci strukturu popisujici pozici.
 * INC_ROW - inkrementuj radek, nastavuje lexem, vraci pozici.
 * INC_LEXEME - inkrementuje lexem, vraci pozici.
 */
Tgp_info get_position(Tgp_flag flag)
{
	static Tgp_info info = {.row = 1, .lexeme = 0};

	switch(flag)
	{
		case INC_ROW:
			info.row++;
			// Viz. INC_LEXEME pri zavolani get_net_token()
			info.lexeme = 1;
			break;
		case INC_LEXEME:
			info.lexeme++;
			break;
		case GET:
		default:
			break;
	}

	return info;
}

/*
 * Pomocne makro pro tokeny bez atributu
 */
#define UNGETCFREERET(TOK_TYPE) \
	ungetc(c, input); \
	token.type = TOK_TYPE; \
	token.attribute.ptr = NULL; \
	free(lexem); \
	return token;

/*
 * Konecny automat pro lexikalni analyzu
 * input - vstupni soubor/stream
 * Automat vzdy vraci token
 * Na konci souboru vraci TOK_EOF
 * V pripade jakekoliv chyby vypise informace a vraci TOK_SCANNERERROR
 * Typy chyb: malloc(), neznamy znak v FSM_S, chybny lexem, preteceni
 *            (neznamy stav, vyskoceni z for(;;))
 * Alokovanou pamet predava (pres ukazatel Token.attribute.ptr) pouze
 * u tokenu typu TOK_IDENTIFIER a TOK_CONSTSRING
 */
static Ttoken fsm(FILE* input)
{
	Efsm_state state = FSM_S;   // Aktualni stav automatu
	char *lexem;                // Nacteny lexem
	unsigned int lexem_i = 0;   // Delka lexemu
	unsigned int lexem_block = BLOCK_INCREMENT; // Delka bloku pro lexem
	int c = 0;                  // Nacitany znak
	Ttoken token;               // Vraceny token
	unsigned int extension_level = 0; //Rozsireni - uroven zanoreni /* */

	// Alokace bloku zakladni velikosti pro lexem
	lexem = (char *)malloc(lexem_block * sizeof(char));
	if (lexem == NULL) {
		error(ROWLEX, "scanner: Nedostatek pameti pro lexem!\n");
		token.type = TOK_SCANNERERROR;
		return token;
	}
	// Lexem zakoncime z duvodu pripadu "" - prazdny string
	lexem[0] = '\0';

	// Nekonecny cyklus automatu (vyskoceni znamena chybny break)
	for (;;) {

		// Vzdy nacteme znak pro rozhodnuti, do ktereho stavu se ma automat
		// presunout. Kdyz vracime token, znak vracime pomoci ungetc().
		// (Plati i pro EOF, i kdyz je to zbytecne)
		c = getc(input);

		#if SCANNER_DEBUG
		fprintf(stderr, "debug: scanner: [%s|%s|%c]\n", Efsm_state_name[state], lexem, c);
		#endif

		switch(state) {
			case FSM_S:
				switch(c) {
					case ',': state = FSM_F_COMMA;break;
					case ';': state = FSM_F_SEMICOLON;break;
					case ':': state = FSM_F_LABEL;break;
					case '^': state = FSM_F_POWER;break;
					case '+': state = FSM_F_PLUS;break;
					case '-': state = FSM_F_MINUS;break;
					case '*': state = FSM_F_MULTIPLE;break;
					case '/': state = FSM_F_DIVDOUBLE;break;
					case '<': state = FSM_F_LESS;break;
					case '>': state = FSM_F_GREATER;break;
					case '=': state = FSM_Q_EQUAL;break;
					case '!': state = FSM_Q_NOTEQUAL;break;
					case '(': state = FSM_F_LBRACKET;break;
					case ')': state = FSM_F_RBRACKET;break;
					case '{': state = FSM_F_LVINCULUM;break;
					case '}': state = FSM_F_RVINCULUM;break;

					case '\n':
						state = FSM_S;
						get_position(INC_ROW); //zvedni citac radku
						continue; //<--znak nechci uchovavat
					case '"': state = FSM_Q1_STRING;continue; //<--znak nechci uchovavat
					case EOF: //Konec souboru = konec scanneru
						UNGETCFREERET(TOK_EOF);

					default : //intervaly
						if (ismyspace(c)) {
							state = FSM_S;
							continue; //<--znak nechci uchovavat
						}
						else if (ismyalpha(c)) {
							state = FSM_F_IDENTIFIERORKEYWORD;
						}
						else if (ismydigit(c)) {
							state = FSM_F_CONSTINT;
						}
						else { //nerozpoznany znak ve stavu FSM_S
							error(ROWLEX, "scanner: Chybny znak [%c]\n",c);
							UNGETCFREERET(TOK_SCANNERERROR);
						}
						break;
				}
				break;
			// operatory, vzdy jedine a koncove stavy (az na :=,==,!=,//)
			case FSM_F_COMMA: //[,]
				UNGETCFREERET(TOK_COMMA);
			case FSM_F_SEMICOLON: //[;]
				UNGETCFREERET(TOK_SEMICOLON);
			case FSM_F_LABEL: //[:]
				if (c == '=') {
					state = FSM_F_ASSIGNMENT;
				}
				else {
					UNGETCFREERET(TOK_LABEL);
				}
				break;
			case FSM_F_ASSIGNMENT: //[:=]
				UNGETCFREERET(TOK_ASSIGNMENT);
			case FSM_F_POWER: //[^]
				UNGETCFREERET(TOK_POWER);
			case FSM_F_PLUS: //[+]
				UNGETCFREERET(TOK_PLUS);
			case FSM_F_MINUS: //[-]
				UNGETCFREERET(TOK_MINUS);
			case FSM_F_MULTIPLE: //[*]
				UNGETCFREERET(TOK_MULTIPLE);
			case FSM_F_DIVDOUBLE: //[/]
				if (c == '/') {
					state = FSM_COMMENT;
				}
				else if (c == '*') {
					state = FSM_Q_EXTENSION_IN;
					extension_level++;
				}
				else {
					UNGETCFREERET(TOK_DIVDOUBLE);
				}
				break;
			case FSM_Q_EXTENSION_IN: //[/][*] + extension_level = level zanoreni
				//zahazuj
				while (c != '*' && c!= '/' && c!= EOF) {
					c = getc(input);
				}
				//poznej dalsi akci
				if (c == '/')
					state = FSM_Q_EXTENSION_DEEPER; //mozna zanor (podle *),mozna kom(/)
				else if (c == '*')
					state = FSM_Q_EXTENSION_LOWER; //mozna vynor (podle /)
				else { //konec souboru a jsme zanoreni, je chyba
					error(RAW ,"scanner: Rozsireni - blokovy komentar jeste nebyl vynoren z %d urovni!\n", extension_level);
					UNGETCFREERET(TOK_SCANNERERROR);
				}
				continue; //<--znak neukladej
			case FSM_Q_EXTENSION_DEEPER: //[/]
				if (c == '/') // je-li dalsi znak '/', komentar do konce radku
					state = FSM_Q_EXTENSION_COMMENT;
				else if (c == '*') { //zanorime se
					extension_level++;
					state = FSM_Q_EXTENSION_IN;
				} else { //Zde vratime znak a varcime se do FSM_Q_EXTENSION_IN
					ungetc(c, input);
					state = FSM_Q_EXTENSION_IN;
				}
				continue; //<--znak neukladej
			case FSM_Q_EXTENSION_LOWER: //[*]
				if (c == '*') //je-li dalsi znak '*' zustan
					state = FSM_Q_EXTENSION_LOWER;
				else if (c == '/') { //vynor se
					extension_level--;
					//zkontroluj, zda jiz nejsi vynoren zcela
					if (extension_level == 0) { //ano jsi, bez do poc. stavu automatu
						//Zde je take nutne vyresit jiz ulozeny znaky v lexem [/][*]
						lexem_i -= 2; //smazeme [/][*]
						lexem[lexem_i] = '\0';
						state = FSM_S;
					}
					else //nejsi
						state = FSM_Q_EXTENSION_IN;
				} else { //Zde vratime znak a vracime se do FSM_Q_EXTENSION_IN
					ungetc(c, input);
					state = FSM_Q_EXTENSION_IN;
				}
				continue; //<--znak neukladej
			case FSM_Q_EXTENSION_COMMENT: // [/][*][/][/]
				//zahazuj do konce radky
				while (c != '\n' && c != EOF) {
					c = getc(input);
				}
				//navrat + vraceni znaku (se bude resit v FSM_Q_EXTENSION_IN)
				state = FSM_Q_EXTENSION_IN;
				ungetc(c, input);
				continue; //<--znak neukladej
			case FSM_COMMENT: //[/][/]
				//zahazuj
				while (c != '\n' && c != EOF) {
					c = getc(input);
				}
				state = FSM_S;
				//vratime '\n' ci EOF a resime v FSM_S
				ungetc(c, input);
				lexem_i -= 2; //smazeme [/][/]
				lexem[lexem_i] = '\0';
				continue;
			case FSM_F_LESS: //[<]
				if (c == '=') {
					state = FSM_F_LESSEQUAL;
				}
				else {
					UNGETCFREERET(TOK_LESS);
				}
				break;
			case FSM_F_LESSEQUAL: //[<][=]
				UNGETCFREERET(TOK_LESSEQUAL);
			case FSM_F_GREATER: //[>]
				if (c == '=') {
					state = FSM_F_GREATEREQUAL;
				}
				else {
					UNGETCFREERET(TOK_GREATER);
				}
				break;
			case FSM_F_GREATEREQUAL: //[>][=]
				UNGETCFREERET(TOK_GREATEREQUAL);
			case FSM_Q_EQUAL: //[=]
				if (c == '=') {
					state = FSM_F_EQUAL;
				}
				else { //neni koncovy stav
					error(ROWLEX ,"scanner: Chybny lexem [%s%c]\n", lexem, c);
					UNGETCFREERET(TOK_SCANNERERROR);
				}
			break;
			case FSM_F_EQUAL: //[=][=]
				UNGETCFREERET(TOK_EQUAL);
			case FSM_Q_NOTEQUAL: //[!]
				if (c == '=') {
					state = FSM_F_NOTEQUAL;
				}
				else { //neni koncovy stav
					error(ROWLEX, "scanner: Chybny lexem [%s%c]\n", lexem, c);
					UNGETCFREERET(TOK_SCANNERERROR);
				}
				break;
			case FSM_F_NOTEQUAL: //[!][=]
				UNGETCFREERET(TOK_NOTEQUAL);
			case FSM_F_LBRACKET: //[(]
				UNGETCFREERET(TOK_LBRACKET);
			case FSM_F_RBRACKET: //[)]
				UNGETCFREERET(TOK_RBRACKET);
			case FSM_F_LVINCULUM: //[{]
				UNGETCFREERET(TOK_LVINCULUM);
			case FSM_F_RVINCULUM: //[}]
				UNGETCFREERET(TOK_RVINCULUM);

			// identifikator
			case FSM_F_IDENTIFIERORKEYWORD:
				if (ismyalnum(c) || c == '_') { //[a-zA-Z0-9 || _]
					state = FSM_F_IDENTIFIERORKEYWORD;
				}
				else {
					ungetc(c, input); //vracime znak
					token.type = get_keyword_or_identifier(lexem);
					if (token.type == TOK_IDENTIFIER) {
						token.attribute.ptr = lexem;
					}
					else {
						free(lexem);
						token.attribute.ptr = NULL; //bezpecnost
					}
					return token;
				}
				break;
			//konstanty int a double
			case FSM_F_CONSTINT: //[0-9]+
				if (ismydigit(c)) { //[0-9]
					state = FSM_F_CONSTINT; //zbytecne
				}
				else if (c == '.') { //.
					state = FSM_F1_CONSTDOUBLE;
				}
				else {
					ungetc(c, input); //vracime znak
					myatoi(&token, lexem);
					free(lexem);
					return token;
				}
				break;
			case FSM_F1_CONSTDOUBLE: //[0-9]+\.
				if (ismydigit(c)) { //[0-9]
					state = FSM_F2_CONSTDOUBLE;
				}
				else {
					ungetc(c, input); //vracime znak
					myatof(&token, lexem);
					free(lexem);
					return token;
				}
				break;
			case FSM_F2_CONSTDOUBLE: //[0-9]+\.[0-9]+
				if (ismydigit(c)) { //[0-9]
					state = FSM_F2_CONSTDOUBLE;
				}
				else if (c == 'e') { //e
					state = FSM_Q1_CONSTDOUBLE;
				}
				else {
					ungetc(c, input); //vracime znak
					myatof(&token, lexem);
					free(lexem);
					return token;
				}
				break;
			case FSM_Q1_CONSTDOUBLE: //[0-9]+\.[0-9]+e !!
				if (ismydigit(c)) { //[0-9]
					state = FSM_F3_CONSTDOUBLE;
				}
				else if (c == '+' || c == '-') {
					state = FSM_Q2_CONSTDOUBLE;
				}
				else { //neni koncovy stav
					error(ROWLEX, "scanner: Chybny lexem [%s%c]\n", lexem, c);
					UNGETCFREERET(TOK_SCANNERERROR);
				}
				break;
			case FSM_Q2_CONSTDOUBLE: //[0-9]+\.[0-9]+e[+-] !!
				if (ismydigit(c)) { //[0-9]
					state = FSM_F3_CONSTDOUBLE;
				}
				else { //neni koncovy stav
					error(ROWLEX, "scanner: Chybny lexem [%s%c]\n", lexem, c);
					UNGETCFREERET(TOK_SCANNERERROR);
				}
				break;
			case FSM_F3_CONSTDOUBLE: //[0-9]+\.[0-9]+e[+-]?[0-9]
				if (ismydigit(c)) { //[0-9]
					state = FSM_F3_CONSTDOUBLE;
				}
				else {
					ungetc(c, input); //vracime znak
					myatof(&token, lexem);
					free(lexem);
					return token;
				}
				break;
			//string
			case FSM_Q1_STRING: //["]
				if (c > ASCIINUM) {
					if (c == '\\')
						state = FSM_Q2_STRING;
					else if (c == '"') {
						state = FSM_F_STRING;
						continue; //<-- znak neuchovavame
					}
					else
						state = FSM_Q1_STRING;
				}
				else { //neni koncovy stav
					error(ROWLEX, "scanner: Chybny lexem [%s%c]\n", lexem, c);
					UNGETCFREERET(TOK_SCANNERERROR);
				}
				break;
			case FSM_Q2_STRING: //["][>ASCIINUM+][\]
				/* nyni musi prijit nektery ze symbolu: */
				/* 'n' '\' '"' '$'  */
				if (c == 'n' || c == '\\' || c == '\"' || c == '$') {
					state = FSM_Q1_STRING;
				}
				else { //neni koncovy stav
					error(ROWLEX, "scanner: Chybny lexem [%s%c] - neznama "
						"escape sekvence v retezci\n", lexem, c);
					UNGETCFREERET(TOK_SCANNERERROR);
				}
				break;
			case FSM_F_STRING:
				//v tomto stavu vzdy koncovy stav
				ungetc(c, input); //vracime znak
				token.type = TOK_CONSTSTRING;
				token.attribute.ptr = lexem;
				return token;
			default:
				error(ROWLEX, "scanner: Neznamy stav automatu! [%d]\n", state);
				UNGETCFREERET(TOK_SCANNERERROR);
		}


		// Zarazeni znaku do lexemu
		lexem[lexem_i] = c;
		lexem_i++;

		// Pokud jsme na konci bloku pro lexem, blok rozsirime
		if ((lexem_i % lexem_block) == 0) {
			lexem_block += BLOCK_INCREMENT;
			lexem = (char *)realloc(lexem, lexem_block * sizeof(char));
			if (lexem == NULL) {
				error(RAW ,"scanner: Nedostatek pameti pro lexem\n");
				token.type = TOK_SCANNERERROR;
				return token;
			}
		}

		// Zarucuje v lexemu vzdy retezec
		lexem[lexem_i] = '\0';

	} //for(;;)

	//Teoreticky nedostupne misto (chybny break v automatu)
	error(RAW, "scanner: Vyskoceni z for(;;)! [%d|%s|%c]\n", state, lexem, c);
	UNGETCFREERET(TOK_SCANNERERROR);
}

/*
 * Vrat nasledujici token ze vstupniho souboru (wrapper fsm())
 * input - vstupni soubor/stream
 * Vzdy vraci token
 * Na konci souboru vraci TOK_EOF
 * V pripade jakekoliv chyby vypise informace a vraci TOK_SCANNERERROR
 * Typy chyb: malloc(), neznamy znak v FSM_S, chybny lexem, preteceni
 *            (neznamy stav, vyskoceni z for(;;))
 * Alokovanou pamet, kterou predava (pres ukazatel Token.attribute.ptr)
 * u tokenu typu TOK_IDENTIFIER a TOK_CONSTSTRING, _automaticky_ uklada
 * do zasobniku ukazatelu na retezce.
 */
Ttoken get_next_token(FILE *input)
{
	Ttoken token;

	get_position(INC_LEXEME);
	token = fsm(input);

	/* Uloz ukazatel na predavanou pamet do zasobniku ukazatelu na retezce. */
	if (token.type == TOK_IDENTIFIER || token.type == TOK_CONSTSTRING) {
		if (strptr_stack_push(token.attribute.ptr) == NULL) {
			error(RAW,"scanner: Nedostatek pameti pro zasobnik retezcu\n");
			token.type = TOK_SCANNERERROR;
			free(token.attribute.ptr);
			token.attribute.ptr = NULL;
		}
	}

	return token;
}

/* Konec souboru scanner.c */
