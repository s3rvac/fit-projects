(defun eqlist (l1 l2)
  (cond
    ((and (null l1) (null l2)) T)
    ((and (atom (car l1)) (atom (car l2))) (and (equal (car l1) (car l2)) (eqlist (cdr l1) (cdr l2))))
    (T (and (eqlist (car l1) (car l2)) (eqlist (cdr l1) (cdr l2))))
  )
)

(defun test11 () 
  (cond
     ((eqlist (obratOb '(1 2 3)) '(3 2 1)) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)
 
(defun test12 () 
  (cond
     ( (eqlist (obratOb '(1 2 (1 2) 3 (1 2 3))) '((3 2 1) 3 (2 1) 2 1)) (and 0.25 (print 0.25)) )
     (T (and 0 (print 0)))
  )
) 

(defun test13 () 
  (cond
     ((eqlist (obratOb '()) '()) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test14 () 
  (cond
     ((eqlist (obratOb '(1)) '(1)) (and 0.25 (print 0.25))  )
     (T (and 0 (print 0)))
  )
)

(defun test21 () 
  (cond
     ((jePrvekOb '(1 2 3 4 5 6) 4) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test22 () 
  (cond
     ((jePrvekOb '(1 2 3 4 (7 8 (9)) 5 6) 6) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test23 () 
  (cond
     ((jePrvekOb '(((11) 10) 1 2 3 4 (7 8 (9)) 5 6) 11) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test24 () 
  (cond
     ((not (jePrvekOb '(((11) 10) 1 2 3 4 (7 8 (9)) 5 6) 12)) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test31 () 
  (cond
     ((eq (prumerPrvkuOb '(1 2 3 4 5)) 3) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test32 () 
  (cond
     ((eq (prumerPrvkuOb '(1 (2 3) 4 5)) 3) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test33 () 
  (cond
     ((eq (prumerPrvkuOb '(1 (2 3) ((4)) 5)) 3) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test34 () 
  (cond
     ((eq (prumerPrvkuOb '(3)) 3) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test41 () 
  (cond
     ((eq (soucetPrvkuOb '(1 2 3 4 5)) 15) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test42 () 
  (cond
     ((eq (soucetPrvkuOb '(1 (2 3) 4 5)) 15) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test43 () 
  (cond
     ((eq (soucetPrvkuOb '(1 (2 3) ((4)) 5)) 15) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test44 () 
  (cond
     ((eq (soucetPrvkuOb '(3)) 3) (and 0.25 (print 0.25)))
     (T (and 0 (print 0)))
  )
)

(defun test51 () 
  (cond
     ((monotonnostLin '(1 2 3 4)) (and 0.20 (print 0.20)))
     (T (and 0 (print 0)))
  )
)

(defun test52 () 
  (cond
     ((monotonnostLin '(4 3 2 1)) (and 0.20 (print 0.20)))
     (T (and 0 (print 0)))
  )
)

(defun test53 () 
  (cond
     ((monotonnostLin '(1 1 1 1)) (and 0.20 (print 0.20)))
     (T (and 0 (print 0)))
  )
)

(defun test54 () 
  (cond
     ((monotonnostLin '(1)) (and 0.20 (print 0.20)))
     (T (and 0 (print 0)))
  )
)

(defun test55 () 
  (cond
     ((not (monotonnostLin '(1 2 1))) (and 0.20 (print 0.20)))
     (T (and 0 (print 0)))
  )
)

(defun t ()   
  (+ (test11) (test12) (test13) (test14) 
     (test21) (test22) (test23) (test24)
     (test31) (test32) (test33) (test34)
     (test41) (test42) (test43) (test44)
     (test51) (test52) (test53) (test54) (test55)
  )
)

