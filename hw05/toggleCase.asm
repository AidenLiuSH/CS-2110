;;=============================================================
;; CS 2110 - Fall 2021
;; Homework 5 - Toggle Case
;;=============================================================
;; Name: (Shihui Liu)
;;=============================================================

;; Pseudocode (see PDF for explanation):
;;
;;	string = "Assembly is interesting"; // given string
;;	Array[string.len()] answer; // array to store the result string
;;	i = 0;
;;
;;	while (string[i] != '\0') {
;;	    if (string[i] == " ") {
;;	        answer[i] = " ";
;;	    } else if (string[i] >= "A" && string[i] <= "Z") {
;;	        answer[i] = string[i].lowerCase();
;;	    } else {
;;	        answer[i] = string[i].upperCase();
;;	    }
;;	    i++;
;;	}


.orig x3000


AND R1, R1, #0              ;;i = 0, i --> R
LD R2, STRING               ;;Load STRING to R2

W1                          ;;while loop
    ADD R3, R2, R1          ;;R3 = R2 + R1
    LDR R4, R3, #0          ;;Load string[i] to R4
    BRZ TERMINATE           ;;TERMINATE if 0

    ;;Check A
    LD R6, A                ;;Load A to R6
    NOT R6, R6              ;;~A
    ADD R6, R6, #1          ;;~A + 1
    ADD R6, R6, R4          ;;string[i] - A
    BRN FINAL               ;;NOT A

    ;;Check Z
    LD R6, Z                ;;Load z to R6
    NOT R6, R6              ;;~Z
    ADD R6, R6, #1          ;;~Z + 1
    ADD R6, R6, R4          ;;string[i] - Z
    BRP LOWER               ;;Check for lowercase

    LD R7, SHIFT        
    ADD R4, R4, R7      
    BR FINAL                ;;else statement

    LOWER                   ;;Check Lowercase
        ;;Check a
        LD R6, a            ;;Load a to R6
        NOT R6, R6          ;;~a
        ADD R6, R6, #1      ;;~a + 1
        ADD R6, R6, R4      ;;string[i] - a
        BRN FINAL           ;;NOT a

        ;;Check z
        LD R6, z            ;;R6 = addr[z]
        NOT R6, R6          ;;R6 = !R6
        ADD R6, R6, #1      ;;R6++
        ADD R6, R6, R4      ;;R6 = R4 - 'z'
        BRP FINAL           ;;else statement

        LD R7, SHIFT        
        NOT R7, R7          
        ADD R7, R7, #1      
        ADD R4, R4, R7      
    
    FINAL
        LD R5, ANSWER       ;;Load ANSWER to R5
        ADD R5, R5, R1      ;;R5 = R5 + R1
        STR R4, R5, #0      ;;Store ANSWER to R4

    ADD R1, R1, #1          ;:i++
    BR W1                   ;;Continue while loop from beginning

TERMINATE               ;;End loop
LD R5, ANSWER           ;;Load ANSWER to R5
ADD R5, R5, R1          ;;R5 = R5 + R1
AND R4, R4, #0          ;;R4 = 0
STR R4, R5, #0          ;;Store ANSWER to R4

END

HALT


;; ASCII table: https://www.asciitable.com/


;; FILL OUT THESE ASCII CHARACTER VALUE FIRST TO USE IT IN YOUR CODE
SPACE	.fill x20
A		.fill x41
Z		.fill x5a
a       .fill x61
z       .fill x7a
SHIFT   .fill x20
BREAK	.fill x5D

STRING .fill x4000
ANSWER .fill x4100
.end

.orig x4000
.stringz "Assembly is INTERESTING"
.end

.orig x4100
.blkw 23
.end