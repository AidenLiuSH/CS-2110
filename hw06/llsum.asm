;;=============================================================
;; CS 2110 - Fall 2021
;; Homework 6 - Linked List Sum
;;=============================================================
;; Name: Shihui Liu
;;============================================================

;; In this file, you must implement the 'llsum' subroutine.

;; Little reminder from your friendly neighborhood 2110 TA staff: don't run
;; this directly by pressing 'RUN' in complx, since there is nothing put at
;; address x3000. Instead, load it and use 'Debug' -> 'Simulate
;; Subroutine Call' and choose the 'reverseCopy' label.

.orig x3000
HALT

;; Pseudocode (see PDF for explanation):
;;
;; Arguments of llsum: Node head
;;
;; llsum(Node head) {
;;     // note that a NULL address is the same thing as the value 0
;;     if (head == NULL) {
;;         return 0;
;;     }
;;     Node next = head.next;
;;     sum = head.data + llsum(next);
;;     return sum;
;; }

llsum

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

LDR R0, R5, #4          ;; R0 --> Node
ADD R0, R0, #0          
BRnp ELSE1              ;; if (head != NULL) --> ELSE
AND R0, R0, #0          ;; R0 = 0
STR R0, R5, #0          ;; Store 0 to RV
BR ENDIF                ;; End if statement

ELSE1
    ADD R6, R6, #-1     ;; Allocate space
    LDR R1, R0, #1      ;; R1 --> head.data
    LDR R2, R0, #0      ;; R2 --> head.next
    STR R2, R6, #0      ;; Push head.next
    JSR llsum           ;; llsum(head.next)
    LDR R0, R6, #0      ;; R0 --> llsum(head.next)
    ADD R6, R6, #2      ;; pop
    ADD R0, R0, R1      ;; sum = head.data + llsum(next), R0 --> sum
    STR R0, R5, #0      ;; store sum to answer
    

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

;; used by the autograder
STACK .fill xF000
.end

;; The following is an example of a small linked list that starts at x4000.
;;
;; The first number (offset 0) contains the address of the next node in the
;; linked list, or zero if this is the final node.
;;
;; The second number (offset 1) contains the data of this node.
.orig x4000
.fill x4008
.fill 5
.end

.orig x4008
.fill x4010
.fill 12
.end

.orig x4010
.fill 0
.fill -7
.end
