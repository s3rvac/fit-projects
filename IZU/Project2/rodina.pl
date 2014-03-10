muz(jan).
muz(pavel).
muz(karel).

zena(marie).
zena(jana).
zena(linda).

otec(jan,karel).
otec(jan,jana).
otec(pavel,jan).

matka(marie,karel).
matka(linda,jana).

rodic(X,Y) :- otec(X,Y).
rodic(X,Y) :- matka(X,Y).

jeMatka(X) :- matka(X,_).
jeOtec(X) :- otec(X,_).

%tyto pravidla doplnte

sourozenec(X,Y) :- rodic(Z,X),rodic(Z,Y),X \= Y.
sestra(X,Y) :- zena(X),sourozenec(X,Y).
teta(X,Y) :- matka(Z,Y),sestra(X,Z);otec(Z,Y),sestra(X,Z).
deda(X,Y) :- muz(X),rodic(X,Z),rodic(Z,Y).
