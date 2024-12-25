(define iseven
  (fun (n)
    (= (mod n 2) 0)))

(print-bool (iseven 7))
(print-bool (iseven 4))
