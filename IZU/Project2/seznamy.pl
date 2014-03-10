% Delka linearniho seznamu
delka([],0). % Delka prazdneho seznamu je 0
delka([_|T],S) :- delka(T,SS), S is SS + 1.

% Je prvek clenem linearniho seznamu?
jePrvek([X|_],X). % Prvek nalezen jako hlavicka
jePrvek([_|T],X) :- jePrvek(T,X). % Proverim telo seznamu

% Spojeni dvou linearnich seznamu
spoj([],L,L). % Spojeni prazdneho seznamu a seznamu L je dany druhy seznam
spoj([H|T],L,[H|TT]) :- spoj(T,L,TT).


% Doplnte nasledujici predikaty

% Otoceni linearniho seznamu
otoc([],[]). % Otocit prazdny seznam je jednoduche
otoc([H|T],L) :-
        otoc(T,P),spoj(P,[H],L). % Nejdrive se dostan na konec a pak postupne
                                 % pridavej 1-prvkove seznamy (jejich prvkem je hlavicka)

% Je prvek clenem obecneho seznamu?
jePrvekOb(P,[P|_]). % Prvek nalezen jako hlavicka
jePrvekOb(P,[H|T]) :-
        jePrvekOb(P,H); % Prozkoumame hlavicku
        jePrvekOb(P,T). % Prozkoumame telo

% Je prvek atom? (v nasem pripade je atom jenom integer)
% Tento "wrapper" vytvoren jen z duvodu mozneho budouciho rozsireni
jeAtom(X) :- integer(X).

% Delka obecneho seznamu
delkaOb([],0). % Delka prazdneho seznamu je 0
delkaOb([H|T],P) :-
        % Hlavicka je atomem, tudiz pokracujeme telem
        jeAtom(H),delkaOb(T,PVH),P is PVH + 1;
         % Hlavicka neni atomem, tudiz pokracujeme hlavickou a az pak telem
        not(jeAtom(H)),delkaOb(H,PVH),delkaOb(T,PVT),P is PVH + PVT.

% Nalezeni maxima v obecnem seznamu prvku typu integer
maximumOb([],[]). % V prazdnem seznamu neni zadne maximum
maximumOb([H|T],MAX) :-
        maximumObDva(T, H, MAX). % Rozdelime seznam na hlavicku (pocatecni maximum) a telo
maximumObDva([],A,A). % Maximum z atomu a prazdneho seznamu je atom
maximumObDva([H|T],OLDMAX,NEWMAX) :- % Mohou nastat 4 situace:
        % Prvek je atom a je vetsi nez stare maximum, otestujeme telo s novym maximem
        integer(H),H > OLDMAX,maximumObDva(T,H,NEWMAX);
        % Prvek je atom, ale neni vetsi nez stare maximum, tudiz testujeme telo se starym maximem
        integer(H),H =< OLDMAX,maximumObDva(T,OLDMAX,NEWMAX);
        % Prvek neni atom, musime jej rozdelit a pak testujeme, zda je nalezene maximum vetsi nez to stare
        % V tomto pripade je vetsi, tudiz testujeme telo s novym maximem
        not(integer(H)),maximumObDva(H,OLDMAX,NEWMAX2),NEWMAX2 > OLDMAX,maximumObDva(T,NEWMAX2,NEWMAX);
        % Prvek neni atom, musime jej rozdelit a pak testujeme, zda je nalezene maximum vetsi nez to stare
        % V tomto pripade neni vetsi, tudiz testujeme telo se starym maximem
        not(integer(H)),maximumObDva(H,OLDMAX,NEWMAX2),NEWMAX2 =< OLDMAX,maximumObDva(T,OLDMAX,NEWMAX).

% Hloubka zanoreni obecneho seznamu
hloubkaOb(L,X) :-
        hloubkaObTwo(L,1,X). % Zahajujeme hledani s hloubkou 1
hloubkaObTwo([],OLDHL,NEWHL) :-
        % Stara hloubka je vetsi, takze prepiseme novou hloubku starou hloubkou
        integer(NEWHL),OLDHL > NEWHL,NEWHL is OLDHL;
        % Stara hloubka je mensi, nebo rovna, takze vse nechame tak, jak je
        integer(NEWHL),OLDHL =< NEWHL;
        % Nemame zadne maximum, takze pouze nastavime novou hloubku
        not(integer(NEWHL)),NEWHL is OLDHL.
hloubkaObTwo([H|T],OLDHL,NEWHL) :-
        % Hlavicka je cislo, takze testujeme telo a hloubku nemenime
        integer(H),hloubkaObTwo(T,OLDHL,NEWHL);
        % Hlavicka je seznam, projdi ho (a zvys hloubku) a pak projdi zbytek seznamu
        not(integer(H)),hloubkaObTwo(H,OLDHL + 1,NEWHL),hloubkaObTwo(T,OLDHL,NEWHL).
