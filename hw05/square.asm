;;=============================================================
;; CS 2110 - Fall 2021
;; Homework 5 - Square
;;=============================================================
;; Name: (Shihui Liu)
;;=============================================================

;; Pseudocode (see PDF for explanation)
;; a = (argument 1);
;; ANSWER = 0;
;; for (int i = 0; i < a; i++) {
;;		ANSWER += a;
;; }
;; return ANSWER;

.orig x3000

AND R1, R1, 0       ;;ANSWER = 0
LD R2, A            ;;Load argument value into R2

AND R3, R3, 0       ;;Use R3 as i, clear R3, i = 0
ADD R3, R3, R2      ;;set i = a

W1                  ;;while Loop
    ADD R3, R3, 0   ;;Set BRnz to refer to R1
    BRnz TERMINATE  ;;End while loop if i = 0

    ADD R3, R3, #-1 ;;Decrement R3

    ADD R1, R2, R1  ;;Increment R2

    BR W1           ;;Continue while loop from beginning
TERMINATE           ;;End of while loop

ST R1, ANSWER       ;;Store the value of R1 into ANSWER

HALT

A .fill 9

ANSWER .blkw 1

.end
