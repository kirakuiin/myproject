#lang planet neil/sicp

(define (memo-proc procedure)
  (let ((has-res? false)
        (result false))
    (lambda ()
      (if has-res?
          result
          (begin (set! has-res? true)
                 (set! result (procedure))
                 result)))))

(define-syntax delay
  (syntax-rules ()
    ((_ exp) (memo-proc (lambda () exp)))))

(define (force procedure)
  (procedure))

(define-syntax stream-cons
  (syntax-rules ()
    ((_ expr1 expr2) (cons expr1 (delay expr2)))))

(define stream-nil '())

(define (stream-car stream)
  (car stream))

(define (stream-cdr stream)
  (force (cdr stream)))

(define (stream-null? stream)
  (if (eq? stream-nil stream)
      true
      false))

(define (stream-list . args)
  (define (iter args)
    (if (stream-null? args)
        stream-nil
        (stream-cons (car args) (iter (cdr args)))))
  (iter args))

(define (stream-length stream)
  (if (stream-null? stream)
      0
      (+ 1 (stream-length (stream-cdr stream)))))

(define (stream-ref stream n)
  (if (stream-null? stream)
      (error "exceed list length -- stream-ref")
      (if (= n 0)
          (stream-car stream)
          (stream-ref (stream-cdr stream) (- n 1)))))

(define (stream-enum bottom top)
  (if (= bottom top)
      stream-nil
      (stream-cons bottom (stream-enum (+ bottom 1) top))))

(define (stream-map procedure . args)
  (if (stream-null? (car args))
      stream-nil
      (stream-cons
       (apply procedure (map stream-car args))
       (apply stream-map
              (cons procedure (map stream-cdr args))))))

(define (stream-filter predict? stream)
  (if (stream-null? stream)
      stream-nil
      (if (predict? (stream-car stream))
          (stream-cons (stream-car stream) (stream-filter predict? (stream-cdr stream)))
          (stream-filter predict? (stream-cdr stream)))))

(define (stream-foreach procedure stream)
  (if (not (stream-null? stream))
      (begin (procedure (stream-car stream))
             (stream-foreach procedure (stream-cdr stream)))))

(define (stream-range stream bottom top . step)
  (let ((i-step
         (if (null? step)
             1
             (car step))))
    (if (>= bottom top)
        stream-nil
        (stream-cons
         (stream-ref stream bottom)
         (stream-range stream (+ i-step bottom) top i-step)))))

(define (stream-enum-n n)
  (stream-cons n
               (stream-enum-n n)))


(define (stream-merge s1 s2)
  (cond ((stream-null? s1)
         s2)
        ((stream-null? s2)
         s1)
        ((> (stream-car s1) (stream-car s2))
         (stream-cons (stream-car s2)
                      (stream-merge (stream-cdr s2) s1)))
        ((> (stream-car s2) (stream-car s1))
         (stream-cons (stream-car s1)
                      (stream-merge s2 (stream-cdr s1))))
        (else
         (stream-cons (stream-car s1)
                      (stream-merge (stream-cdr s1) (stream-cdr s2))))))

(define (stream-display stream)
  (stream-foreach (lambda (x) (begin
                                (display x)
                                (newline))) stream))

(define (sd stream)
  (stream-display (stream-range stream 0 20)))

(define (integer-from-n n)
  (stream-cons n (integer-from-n (+ n 1))))

(define (divisible? x y) (= (remainder x y) 0))

(define (sieve stream)
  (if (stream-null? stream)
      stream-nil
      (stream-cons
       (stream-car stream)
       (sieve
        (stream-filter
         (lambda (x) (not (divisible? x (stream-car stream))))
         (stream-cdr stream))))))

(define (stream-add a1 a2)
  (stream-map + a1 a2))

(define (stream-mul m1 m2)
  (stream-map * m1 m2))

(define integers (integer-from-n 1))

(define fib (stream-cons
             0
             (stream-cons
              1
              (stream-add (stream-cdr fib)
                          fib))))

(define fact (stream-cons 1 (stream-mul (integer-from-n 2) fact)))

(define (partial-sums stream)
  (if (stream-null? stream)
      stream-nil
      (stream-cons
       (stream-car stream)
       (partial-sums (stream-add (stream-cdr stream)
                                 (stream-cons (stream-car stream)
                                              (stream-enum-n 0)))))))

(define s235 (stream-cons 1
                       (stream-merge
                        (stream-merge
                         (stream-mul s235 (stream-enum-n 2))
                         (stream-mul s235 (stream-enum-n 3)))
                        (stream-mul s235 (stream-enum-n 5)))))

(define (integrate-series stream)
  (define (iter n)
    (stream-cons
     (* (/ 1 n) (stream-ref stream (- n 1)))
     (iter (+ n 1))))
  (iter 1))
        
(define exp-series
  (stream-cons 1 (integrate-series exp-series)))

(define sine-series
  (stream-cons 0 (integrate-series cosine-series)))

(define cosine-series
  (stream-cons 1 (stream-mul (integrate-series sine-series) (stream-enum-n -1))))

(define (series-mul a b)
  (stream-cons
   (* (stream-car a) (stream-car b))
   (stream-add
    (stream-mul (stream-enum-n (stream-car a)) (stream-cdr b))
    (series-mul (stream-cdr a) b))))

(define (one-series stream)
  (stream-cons
   1
   (series-mul (stream-mul (stream-enum-n -1) (stream-cdr stream))
               (one-series stream))))

(define (series-divide a b)
  (if (= (stream-car b) 0)
      (error "Invalid argument -- series-divide" b)
      (series-mul a (one-series b))))

(define (pi-summands n)
  (stream-cons (/ 1.0 n)
               (stream-map - (pi-summands (+ n 2)))))

(define stream-pi
  (stream-mul (stream-enum-n 4) (partial-sums (pi-summands 1))))

(define (euler-transform s)
  (let ((s0 (stream-ref s 0))
        (s1 (stream-ref s 1))
        (s2 (stream-ref s 2)))
    (stream-cons (- s2 (/ (* (- s2 s1) (- s2 s1))
                          (+ s0 (* -2 s1) s2)))
                 (euler-transform (stream-cdr s)))))

(define (make-tableau transform s)
  (stream-cons s
               (make-tableau transform
                             (transform s))))

(define (accelerated-sequence transform s)
  (stream-map stream-car
              (make-tableau transform s)))

(define (stream-limit stream tolerance)
  (if (stream-null? (stream-car (stream-cdr stream)))
      stream-nil
      (if (<= (abs (- (stream-car stream)
                   (stream-car (stream-cdr stream))))
             tolerance)
          (stream-car (stream-cdr stream))
          (stream-limit (stream-cdr stream) tolerance))))

(define (ln2-summands n)
  (stream-cons
   (/ 1.0 n)
   (stream-map - (ln2-summands (+ n 1)))))

(define stream-ln2
  (partial-sums (ln2-summands 1)))