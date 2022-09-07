;;=============================================================
;; CS 2110 - Fall 2021
;; Homework 6 - Selection Sort
;;=============================================================
;; Name: Shihui Liu
;;=============================================================

;; In this file, you must implement the 'SORT' subroutine.

;; Little reminder from your friendly neighborhood 2110 TA staff: don't run
;; this directly by pressing 'RUN' in complx, since there is nothing put at
;; address x3000. Instead, load it and use 'Debug' -> 'Simulate
;; Subroutine Call' and choose the 'sort' label.

.orig x3000
HALT

;;Pseudocode (see PDF for explanation)
;;  arr: memory address of the first element in the array
;;  n: integer value of the number of elements in the array
;;
;;  sort(array, len, function compare) {
;;      i = 0;
;;      while (i < len - 1) {
;;          j = i;
;;          k = i + 1;
;;          while (k < len) {
;;              // update j if ARRAY[j] > ARRAY[k]
;;              if (compare(ARRAY[j], ARRAY[k]) > 0) {
;;                  j = k;
;;              }
;;              k++;
;;          }
;;          temp = ARRAY[i];
;;          ARRAY[i] = ARRAY[j];
;;          ARRAY[j] = temp;
;;          i++;
;;      }
;;  }

sort

;;YOUR CODE HERE
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

AND R1, R1, #0          ;; R3 --> i, i = 0
W1
    ;;while (i < len - 1)
    LDR R2, R5, #5      ;; R2 --> len
    NOT R2, R2          ;; ~len
    ADD R2, R2, R1      ;; i - (len - 1)
    BRZP TERMINATE1      ;; if (i >= len - 1) end while

    ;;j = i;
    ;;k = i + 1;
    AND R3, R3, #0      ;; j --> R3, j = 0
    ADD R3, R3, R1      ;; j = i
    AND R4, R4, #0      ;; k --> R4
    ADD R4, R4, R1      ;; k = i
    ADD R4, R4, #1      ;; k = i + 1
    W2
        while (k < len) 
        LDR R2, R5, #5  ;; R2 --> len
        NOT R2, R2      ;; ~len
        ADD R2, R2, #1  ;; ~len + 1
        ADD R2, R2, R4  ;; k - len
        BRZP TERMINATE2 ;; if (k >= len) end inner while

        ADD R6, R6, #-2 ;; Allocate space

        ;;ARRAY[j]
        LDR R0, R5, #4  ;; Load ARR
        ADD R0, R0, R3  ;; Load ARR[j] address
        LDR R0, R0, #0  ;; ARR[j]
        STR R0, R6, #0  ;; Push

        ;;ARRAY[k]
        LDR R0, R5, #4  ;; Load ARR
        ADD R0, R0, R4  ;; Load ARR[k] address
        LDR R0, R0, #0  ;; ARR[k]
        STR R0, R6, #1  ;; Push
        
        ;;compare(ARRAY[j], ARRAY[k])
        LDR R0, R5, #6  ;; Load compare
        JSRR R0         ;; Call compare
        LDR R0, R6, #0  ;; compare(ARRAY[j], ARRAY[k]) --> R0
        BRNZ ELSE       ;; if compare(ARRAY[j], ARRAY[k]) <= 0 --> ELSE

        ;;j = k;
        AND R3, R3, #0  ;; R3 = 0
        ADD R3, R3, R4  ;; R3 = R4 or j = k

        ;;k++;
        ELSE            ;; end of if
        ADD R4, R4, #1  ;; k++

        ADD R6, R6, #3  ;; Pop three spots on stack
        BR W2           ;; loop
    
    TERMINATE2
    ;;temp = ARRAY[i];
    ;;ARRAY[i] = ARRAY[j];
    ;;ARRAY[j] = temp;

    ;;temp = ARRAY[i];
    LDR R0, R5, #4      ;; Load ARR
    ADD R0, R0, R1      ;; ARR[i] address
    LDR R0, R0, #0      ;; ARR[i](temp)

    ;;ARRAY[i] = ARRAY[j];
    LDR R4, R5, #4      ;; Load ARR
    ADD R4, R4, R3      ;; ARR[j] address
    LDR R4, R4, #0      ;; ARR[j]
    LDR R2, R5, #4      ;; Load ARR
    ADD R2, R2, R1      ;; ARR[i] address
    STR R4, R2, #0      ;; ARR[i] = ARR[j]

    ;;ARRAY[j] = temp;
    LDR R2, R5, #4      ;; Load ARR
    ADD R2, R2, R3      ;; ARR[j] address
    STR R0, R2, #0      ;; ARRAY[j] = ARRAY[i](temp)

    ADD R1, R1, #1      ;; i++
    BR W1               ;; loop
TERMINATE1

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

;; USE FOR DEBUGGING IN COMPLX
;; load array at x4000 and put the length as 7

;; ARRAY
.orig x4000
    .fill 4
    .fill -9
    .fill 0
    .fill -2
    .fill 9
    .fill 3
    .fill -10
.end

;; The following subroutine is the compare function that is passed  
;; as the function address parameter into the sorting function. It   
;; will work perfectly fine as long as you follow the   
;; convention on the caller's side. 
;; DO NOT edit the code below; it will be used by the autograder.   
.orig x5000 
    ;;greater than  
CMPGT   
    .fill x1DBD
    .fill x7180
    .fill x7381
    .fill x6183
    .fill x6384
    .fill x927F
    .fill x1261
    .fill x1201
    .fill x0C03
    .fill x5020
    .fill x1021
    .fill x0E01
    .fill x5020
    .fill x7182
    .fill x6180
    .fill x6381
    .fill x1DA2
    .fill xC1C0
    .fill x9241
.end

