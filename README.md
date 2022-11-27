# libguileavl

[AVL tree](https://en.wikipedia.org/wiki/AVL_tree) data structure for Guile Scheme

## Install

Required:
- [GNU Guile](https://www.gnu.org/software/guile/)
- C++11 compiler
- `make` toolchain
- Unix environment

1. Clone repository
2. Run `make install`
3. Run `make uninstall` to remove this Guile extension

## Use

```scheme
(use-modules (extra avl))

;; Define a comparison function for ordering elements
;; in the tree
(define (comp a b)
  (cond ((< a b) -1)
        ((= a b) 0)
        (else 1)))

;; Create AVL tree from a sorted array
(define foo (avl-from-array #(0 1 2 3 4) comp))
;; Manipulate the tree
(avl-insert! foo 100)
(avl-count foo) ; number of elements in tree
(avl-array foo) ; elements of tree as a sorted array
```

## Author, License

Copyright :copyright: 2022 Alan Tseng

The source files `pavl.c` and `pavl.h` are from
[GNU libavl](https://adtinfo.org/) copyright :copyright: 2022 Ben Pfaff.

GNU Lesser General Public License version 3
