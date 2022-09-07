;;=============================================================
;; CS 2110 - Fall 2021
;; Homework 5 - Array Modify
;;=============================================================
;; Name: (Shihui Liu)
;;=============================================================

;; Pseudocode (see PDF for explanation)
;;
;;    i = 0; // first index 
;;    len = Len(ARR_X);
;;
;;    while (i < len) { 
;;        if (i % 2 == 0) {
;;            //if we are at an even index, subtract 10 and save it to the array at that index 
;;    	    ARR_Y[i] = ARR_X[i] - 10];
;;        } else {
;;            //if we are at odd index, multiply by 2 and save it to the array at that index 
;;    	    ARR_Y[i] = ARR_X[i] * 2;
;;        }
;;        	i++;
;;    }

.orig x3000

AND R1, R1, #0              ;;i = 0, i --> R1
LD R2, LENGTH               ;;len --> R2

W1                          ;;while loop
    NOT R3, R2              ;;~len
    ADD R3, R3, R1          ;;i - len
    BRZP TERMINATE          ;;End loop if i - len < 0

    AND R4, R1, #1          ;;Get last digit, even --> R4 = 0, odd --> R4 = 1

    LD R5, ARR_X            ;;R5 = ARR_X address
    ADD R5, R5, R1          ;;Address of ARR_X[i]

    LD R6, ARR_Y            ;;R6 = ARR_Y Address
    ADD R6, R6, R1          ;;Address of ARR_Y[i]

    ADD R4, R4, #0       
    BRNP ELSE1              ;;else if (i % 2 != 0)

    LDR R7, R5, #0          ;;Load ARR_X memory to R6
    ADD R7, R7, #-10        ;;ARR_X[i] - 10
    STR R7, R6, #0          ;;ARR_Y[i] = ARR_X[i] - 10];
    BR ENDIF1               ;;End if statement

    ELSE1
        LDR R7, R5, #0      ;;Load R5 memory to R6
        ADD R7, R7, R7      ;;ARR_X[i] * 2;
        STR R7, R6, #0      ;;ARR_Y[i] = ARR_X[i] * 2;
    
    ENDIF1                  ;;End if statement
    ADD R1, R1, #1          ;;i++
    BR W1                   ;;Continue while loop from beginning

TERMINATE                   ;;End of while loop

HALT

ARR_X       .fill x4000
ARR_Y       .fill x4100
LENGTH      .fill 4
.end

.orig x4000
.fill 1
.fill 5
.fill 10
.fill 11
.end

.orig x4100
.blkw 4
.end
