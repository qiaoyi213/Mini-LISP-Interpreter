(define f
(fun (x) (if (= x 1)
    1
        (* x (f (- x 1))))))
(print-num (f 4))
