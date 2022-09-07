;;=======================================
;; CS 2110 - Fall 2021
;; HW6 - Collatz Conjecture
;;=======================================
;; Name: Shihui Liu
;;=======================================

;; In this file, you must implement the 'collatz' subroutine.

;; Little reminder from your friendly neighborhood 2110 TA staff:
;; don't run this directly by pressing 'RUN' in Complx, since there is nothing
;; put at address x3000. Instead, load it and use 'Debug' -> 'Simulate
;; Subroutine Call' and choose the 'collatz' label.


.orig x3000
HALT

collatz
;; See the PDF for more information on what this subroutine should do.
;;
;; Arguments of collatz: postive integer n
;;
;; Pseudocode:
;; collatz(int n) {
;;     if (n == 1) {
;;         return 0;
;;     }
;;     if (n % 2 == 0) {
;;         return collatz(n/2) + 1;
;;     } else {
;;         return collatz(3 * n + 1) + 1;
;;     }
;; }


;; YOUR CODE HERE
;;Stack Set up
ADD R6, R6, -4          ;; Make room for RV, RA, old FP, local var
STR R7, R6, 2           ;; Save r7 in RA
STR R5, R6, 1           ;; Save R5 in old FP
ADD R5, R6, 0           ;; Set R5 to the new FP
ADD R6, R6, -5          ;; Make room to save registers
;; Save registers R0-R5
STR R0, R6, 4
STR R1, R6, 3
STR R2, R6, 2
STR R3, R6, 1
STR R4, R6, 0

LDR R0, R5, #4          ;; R0 --> n
ADD R1, R0, #-1         ;; n = n - 1
BRZ IF1                 ;; if n == 1

AND R1, R0, #1          ;; Take last digit of n
BRZ IF2                 ;; if (n % 2 == 0) --> IF2

AND R1, R0, #1          ;; Take last digit of n
BRP ELSE                ;; if (n % 2 != 0) --> ELSE 

IF1                     ;; if (n == 1) return 0;
    AND R0, R0, #0      ;; R0 = 0
    STR R0, R5, #0      ;; Store 0 in RV
    BR ENDIF            ;; End if statement

IF2                     ;; if (n % 2 == 0) return collatz(n/2) + 1;
    ADD R6, R6, #-1     ;; allocate space
    STR R0, R6, #0      ;; Push n
    JSR divideBy2       ;; n / 2
    LDR R0, R6, #0      ;; R0 --> n / 2
    ADD R6, R6, #2      ;; Pop
    BR RECUR            ;; collatz(arg) + 1

ELSE                    ;; if (n % 2 != 0) return collatz(3 * n + 1) + 1;
    ADD R1, R1, R0      ;; 1 * n
    ADD R1, R1, R0      ;; 2 * n
    ADD R1, R1, R0      ;; 3 * n
    AND R0, R0, #0      ;; R0 = 0
    ADD R0, R0, R1      ;; R0 = 3 * N + 1
    BR RECUR            ;; collatz(arg) + 1

RECUR                   ;; collatz(arg) + 1
    ADD R6, R6, #-1     ;; allocate space
    STR R0, R6, #0      ;; Push n
    JSR collatz         ;; collatz(R0)
    LDR R0, R6, #0      ;; Load collatz(R0) into R0
    ADD R6, R6, #2      ;; Pop
    ADD R0, R0, #1      ;; R0 += 1
    STR R0, R5, #0      ;; answer = R0
    BR ENDIF            ;; End if statement


ENDIF

;;Tear down
LDR R0, R5, 0           ;; Ret val = answer
STR R0, R5, 3
LDR R4, R6, 0           ;; Restore R4
LDR R3, R6, 1           ;; Restore R3
LDR R2, R6, 2           ;; Restore R2
LDR R1, R6, 3           ;; Restore R1
LDR R0, R6, 4           ;; Restore R0
ADD R6, R5, 0           ;; Restore SP to FP
LDR R5, R6, 1           ;; Restore old FP into R5
LDR R7, R6, 2           ;; Restore old RA into R7
ADD R6, R6, 3           ;; Deallocate space for loca, oldFP, and RA


RET

;; The following is a subroutine that takes a single number n and returns n/2.
;; You may call this subroutine to help you with 'collatz'.

;; DO NOT CHANGE THIS SUBROUTINE IN ANY WAY
divideBy2
.fill x1DBC
.fill x7F82
.fill x7B81
.fill x1BA0
.fill x1DBB
.fill x7184
.fill x7383
.fill x7582
.fill x7781
.fill x7980
.fill x6144
.fill x5260
.fill x1020
.fill x0C03
.fill x103E
.fill x1261
.fill x0FFB
.fill x7343
.fill x6980
.fill x6781
.fill x6582
.fill x6383
.fill x6184
.fill x1D60
.fill x6B81
.fill x6F82
.fill x1DA3
.fill xC1C0


; Needed by Simulate Subroutine Call in Complx
STACK .fill xF000
.end