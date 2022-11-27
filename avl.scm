(define-module (extra avl)
  #:use-module (system foreign)
  #:use-module (system foreign-library)
  #:export (avl
            avl-compare ; get comparison function
	    avl-copy
	    avl-count
	    avl-find
	    avl-insert!
	    avl-delete!
	    avl-replace!
	    avlt
	    avlt-valid?
	    avlt-set-first!
	    avlt-set-last!
	    avlt-copy
	    avlt-find
	    avlt-insert!
	    avlt-next!
	    avlt-prev!
	    avlt-get
	    avlt-set!
	    avl-from-array
	    avl-array))

(load-extension "libguileavl" "init_guile_trees")

(define (avlt-valid? it)
  "Whether iterator refers to an actual Scheme value."
  (not (equal? (avlt-get it) %null-pointer)))

(define (avl-array tree)
  "Returns array of elements in the tree in sorted order."
  (let* ((n (avl-count tree))
         (out (make-array #f n))
         (it (avlt tree)))
    (avlt-set-first! it tree)
    ;; For indices i from 0 below n do
    (do ((i 0 (+ i 1)))
	((= i n))
      (begin
	(vector-set! out i (avlt-get it))
	(avlt-next! it))) ; advance iterator
    out))
