; spojeni seznamu
(defun spoj (L1 L2)
    (cond ((null L1) L2)
          (T (cons (car L1) (spoj (cdr L1) L2)))
    )
)

; vytvorte
; obraceni lineraniho seznamu obrat(seznam)
(defun obrat (L)
    (cond ((null L) nil)
          (T (spoj (obrat (cdr L)) (list (car L))))
    )
)

; obraceni obecneho seznamu obratOb(seznam)
(defun obratOb (L)
    (cond ((null L) nil)
          ((not (atom (car L))) (spoj (obratOb (cdr L)) (list (spoj (obratOb (cdar L)) (list (caar L)))))) ; "vchazime" do "podseznamu"
          (T (spoj (obratOb (cdr L)) (list (car L)))) ; linerani seznam
    )
)

; test zda je prvek obsazen v obecnem seznamu jePrvekOb(seznam, prvek)
(defun jePrvekOb (L p)
    (cond ((null L) nil) ; seznam je prazdny - nenalezen
          ((atom L) (= L p)) ; mame atom, testujeme rovnost
          ((or (jePrvekOb (car L) p) (jePrvekOb (cdr L) p)) T) ; otestujeme hlavicku a telo seznamu
    )
)

; vypocita pocet prvku v seznamu
(defun pocetPrvkuOb (L)
    (cond ((null L) 0) ; kde nic neni...
          ((atom L) 1) ; prvek, +1
          ((+ (pocetPrvkuOb (car L)) (pocetPrvkuOb (cdr L)))) ; secteme pocet prvku v hlavicce a v tele
    )
)

; soucet prvku obecneho seznamu soucetPrvkuOb(seznam)
(defun soucetPrvkuOb (L)
    (cond ((null L) 0) ; kde nic neni...
          ((atom L) L) ; prvek, vracime jeho hodnotu
          ((+ (soucetPrvkuOb (car L)) (soucetPrvkuOb (cdr L)))) ; secteme pocet prvku v hlavicce a v tele
    )
)

; prumer prvku obecneho seznamu prumerPrvkuOb(seznam)
(defun prumerPrvkuOb (L)
    (/ (soucetPrvkuOb L) (pocetPrvkuOb L)) ; jednoduchy vypocet prumeru
)

; zjisti, zda prvky v linearnim seznamu tvori neklesajici posloupnost
; L - seznam, p - prvni prvek
(defun neklesLin (L p)
    (cond ((null L) T) ; prvky prazdeho seznamu jiste tvori nekles. posl.
          ((atom L) (>= L p)) ; porovname prvky
          ((and (>= (car L) p) (neklesLin (cdr L) (car L)))) ; porovname hlavicku a prvek a prohledame telo
    )
)

; zjisti, zda prvky v linearnim seznamu tvori nerostouci posloupnost
; L - seznam, p - prvni prvek
(defun nerostLin (L p)
    (cond ((null L) T) ; prvky prazdeho seznamu jiste tvori nerost. posl.
          ((atom L) (<= L p)) ; porovname prvky
          ((and (<= (car L) p) (nerostLin (cdr L) (car L)))) ; porovname hlavicku a prvek a prohledame telo
    )
)

; overeni monotonnosti linearniho seznamu monotonnostLin(seznam)
(defun monotonnostLin (L)
    (cond ((null L) T) ; prazdny seznam je monotonni
          ((atom L) T) ; seznam s 1 prvkem je monotonni
          ((or (neklesLin L (car L)) (nerostLin L (car L)))) ; bud prvky seznamu tvori neklesajici nebo nerostouci posloupnost
    )
)
