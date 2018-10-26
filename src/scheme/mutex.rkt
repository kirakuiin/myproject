#lang planet neil/sicp

;; 互斥

(define (make-serializer)
  (let ((mutex (make-mutex)))
    (lambda (p)
      (define (serialized-p . args)
        (mutex 'acquire)
        (let ((val (apply p args)))
          (mutex 'release)
          val))
      serialized-p)))

(define (make-mutex)
  (let ((cell (list false)))
    (define (the-mutex m)
      (cond ((eq? m 'acquire)
             (if (test-and-set! cell)
                 (the-mutex 'acquire)
                 'get)) ; retry
            ((eq? m 'release)
             (clear! cell))))
    the-mutex))

(define (clear! cell)
  (set-car! cell false))

(define (test-and-set! cell)
  (without-interrupts
   (lambda ()
     (if (car cell)
         true
         (begin (set-car! true)
                false)))))

;; 如果test-and-set! 不是原子过程，则会出现如下情况
;; 两个进程同时acquire，此时cell为false，而后这两个
;; 进程得以同时运行

(define (make-signal num)
  (let ((process 0)
        (in-mutex (make-mutex))
        (out-mutex (make-mutex)))
    (define (the-signal request)
      (display "process num = ")
      (display process)
      (newline)
      (cond ((eq? request 'p)
             (in-mutex 'acquire)
             (if (not (= process num))
                 (begin (set! process (+ process 1))
                        (in-mutex 'release))
                 (begin (in-mutex 'release)
                        (out-mutex 'require))))
            ((eq? request 'v)
             (in-mutex 'acquire)
             (cond ((eq? (= process num))
                    (set! process (- process 1))
                    (out-mutex 'release))
                   ((eq? (= process 0)))
                   (else
                    (set! process (- process 1))))
             (in-mutex 'release))
            (else
             (error "Invalid argument -- signal" request))))
    the-signal))

(define (parallel-execute . thunk)
  (for-each thread thunk))

(define pv (make-signal 3))

(define (test-pv)
  (let ((test-num 10)
        (execute-num 10))
    (define (addone)
      (display "test-num = ")
      (display test-num)
      (newline)
      (if (< test-num 10)
          (set! test-num (+ test-num 1))))
        (parallel-execute addone addone addone)))

    