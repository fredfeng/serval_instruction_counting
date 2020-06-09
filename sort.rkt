#lang rosette

(require
  (except-in rackunit fail)
  rackunit/text-ui
  rosette/lib/roseunit
  serval/llvm
  serval/lib/unittest
  serval/lib/core)

(require
  (prefix-in globals: "generated/racket/test/sort.globals.rkt")
  (prefix-in map: "generated/racket/test/sort.map.rkt")
  (prefix-in source: "generated/racket/test/sort.ll.rkt"))

(parameterize ([current-machine (make-machine map:symbols globals:globals)])
  (printf "Starting\n")
  ; Define input variables
  (define-symbolic a0 a1 a2 (bitvector 32))

  ; ----------------------------
  ;  Run a function from sort.c
  ; ----------------------------
  ; (printf "The answer is: ~a\n" (source:@foo (bv 5 32)))
  (source:@test_quicksort_3 a0 a1 a2)
  ; (source:@test_insertion_sort_3 a0 a1 a2)
  ; (source:@test_hash a0 a1 a2)

  ; ---------------------------------------
  ;  Get the instruction count and min+max
  ; ---------------------------------------
  (define instr-count (current-instr-count))
  (define max-sol (optimize
    #:maximize (list instr-count)
    #:guarantee (assert #t)))
  (printf "Max solution: ~a\n" max-sol)
  (printf "Max # of instructions: ~a\n" (evaluate instr-count max-sol))
  (define min-sol (optimize
    #:minimize (list instr-count)
    #:guarantee (assert #t)))
  (printf "Min solution: ~a\n" min-sol)
  (printf "Min # of instructions: ~a\n" (evaluate instr-count min-sol)))
