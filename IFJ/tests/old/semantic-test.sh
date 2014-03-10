#!/bin/bash
# Testovani semantiky
# Format:
#    - co se testuje (napr. Nemohou byt definovany 2 funkce se stejnym jmenem)
#    - vstupni kod (napr. ./tests/parser <<< "int f() {} int f() {}")
#    - ocekavany vystup (napr. Semanticka chyba - funkce f jiz byla definovana)
#    - skutecny vystup parseru (napr. parser: ./src/symbol_table.c:236: symtable_start_insert_func:
#                                     Assertion `global->act == ((void *)0)' failed. Aborted)
#    - info, zda byl test v minulosti uspesny nebo stale nefunguje (napr. NE)
#    @ pripadne poznamky atd.
#


# Parametry funkce mohou mit ve vice funkcich stejne nazvy
./tests/parser <<< "int f(int a){} int g(int a) {}"
# Ocekavany vystup: OK
# Skutecny vystup:  OK
# ANO

# Jmeno navesti nemuze zastinit lokalni promennou
./tests/parser <<< "int f(int lab){lab: return 1;}"
# Ocekavany vystup: semanticka analyza: Identifikator lab byl jiz na stejne urovni pouzit!
# Skutecny vystup:  semanticka analyza: Identifikator lab byl jiz na stejne urovni pouzit!
# ANO

# Volana funkce musi byt deklarovana
./tests/parser <<< "int f() {g();}"
# Ocekavany vystup: Semanticka analyza: Funkce g nebyla deklarovana.
# Skutecny vystup:  Semanticka analyza: Funkce g nebyla deklarovana.
# ANO

# Kontrola, jestli sedi deklarace s naslednou definici
./tests/parser <<< "int f(int); int f(string a) {}"
# Ocekavany vystup: neco ve stylu "neshoduje se deklarace s definici"
# Skutecny vystup:  OK
# NE

# Na stejne urovni nemuzou byt 2 identifikatory se stejnym nazevem (globalni)
./tests/parser <<< "var int id, int id;"
# Ocekavany vystup: neco ve stylu: "Identifikator id byl jiz na stejne urovni pouzit!"
# Skutecny vystup:  semanticka analyza: Identifikator id byl jiz na stejne urovni pouzit!
# ANO

# Na stejne urovni nemuzou byt 2 identifikatory se stejnym nazevem (globalni a funkce)
./tests/parser <<< "var int f; void f();"
# Ocekavany vystup: neco ve stylu: "Identifikator f byl jiz na stejne urovni pouzit!"
# Skutecny vystup:  semanticka analyza: identifikator f jiz nemuze byt pouzit jako nazev funkce, protoze je jiz pouzivan!
# ANO

# Na stejne urovni nemuzou byt 2 identifikatory se stejnym nazevem (lokalni)
./tests/parser <<< "int f(int id) var int id;"
# Ocekavany vystup: semanticka analyza: Identifikator id byl jiz na stejne urovni pouzit!
# Skutecny vystup:  semanticka analyza: Identifikator id byl jiz na stejne urovni pouzit!
# ANO

# Pouzita promenna musi byt deklarovana (zde globalni)
./tests/parser <<< "var int a; int f(int b) {return a;}"
# Ocekavany vystup: OK
# Skutecny vystup:  OK
# ANO

# Pouzita promenna musi byt deklarovana (zde lokalni)
./tests/parser <<< "var int a; int f(int b) {return b;}"
# Ocekavany vystup: OK
# Skutecny vystup:  OK
# ANO

# Pouzita promenna musi byt deklarovana (zde chyba)
./tests/parser <<< "var int a; int f(int b) {return c;}"
# Ocekavany vystup: neco jako "Promenna c nebyla deklarovana"
# Skutecny vystup:  parser: ./src/parser/precedence_SA.c:435: get_term_data_type: Assertion `0' failed.
#Neúspěšně ukončen (SIGABRT)
# NE

# Neurceny datovy typ parametru funkce
./tests/parser <<< "int f(id);"
# Ocekavany vystup: Semanticka analyza: Parametr id nema urceny datovy typ.
# Skutecny vystup:  Semanticka analyza: Parametr id nema urceny datovy typ.
# ANO

# Neurceny datovy typ globalni promenne
./tests/parser <<< "var id;"
# Ocekavany vystup: Semanticka analyza: Globalni promenna id nema urceny datovy typ.
# Skutecny vystup:  Semanticka analyza: Globalni promenna id nema urceny datovy typ.
# ANO

# Neureceny datovy typ lokalni promenne
./tests/parser <<< "void func(int i) var abc {}"
# Ocekavany vystup: Semanticka analyza: Lokalni promenna abc nema urceny datovy typ.
# Skutecny vystup:  Semanticka analyza: Lokalni promenna abc nema urceny datovy typ.
# ANO

# Datovy typ parametru/globalni promenne nemuze byt void (testovan jen parametr)
./tests/parser <<< "string g(int i) var void null {}"
# Ocekavany vystup: chyba v parseru
# Skutecny vystup:  chyba v parseru
# ANO

# NOVE TESTY B.P. 9.12.2006
# Vicenasobna deklarace 
./tests/parser <<< "int f(int);int f(int);int f(int a) {} int f(int);"
# Ocekavany vystup: OK
# Skutecny vystup:  OK
# ANO

# Rekurzivni volani
./tests/parser <<< "int f(int); int g(int a) {f(a);} int f(int a) {}"
# Ocekavany vystup: OK
# Skutecny vystup:  OK
