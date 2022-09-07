;;=============================================================
;; CS 2110 - Fall 2021
;; Homework 5 - Remove Vowels
;;=============================================================
;; Name: (Shihui Liu)
;;=============================================================

;; Pseudocode (see PDF for explanation)
;; STRING = (argument 1);
;; ANSWER = "";
;; for (int i = 0; i < a.length; i++) {
;;       if (string[i] == '\0'){
;;          break
;;        }

;;       if (string[i] == ’A’) {
;;           continue;
;;        } else if (string[i] == ’E’) {
;;            continue;
;;        } else if (string[i] == ’I’) {
;;            continue;
;;        } else if (string[i] == ’O’) {
;;            continue;
;;        } else if (string[i] == ’U’) {
;;            continue;
;;        } else if (string[i] == ’a’) {
;;            continue;
;;        } else if (string[i] == ’e’) {
;;            continue;
;;        } else if (string[i] == ’i’) {
;;            continue;
;;        } else if (string[i] == ’o’) {
;;            continue;
;;        } else if (string[i] == 'u') {
;;            continue;
;;        }
;;
;;        ANSWER += STRING[i];
;;	}
;;  ANSWER += '\0';
;;
;;  return ANSWER;
;; }

.orig x3000

AND R1, R1, #0                      ;;R1 = 0, index for answer
AND R2, R2, #0                      ;;R2 = 0, index for string

W1                                  ;;while loop
    ADD R3, R3, #0;
    BRz TERMINATE                   ;;end loop if string[i] == '\0'

    LD R3, STRING                   ;;Load String to R3
    ADD R3, R3, R2                  ;;R3 = R3 + R2
    LDR R3, R3, #0                  ;;R3 = String[j]
    NOT R5, R3                      ;;R5 = ~R3
    ADD R5, R5, #1                  ;;R5 = -String[i]

    LD R4, A                        ;;Check for A
    ADD R4, R4, R5                  ;;A - String[i]
    BRz CONTINUE                    ;;Continue if R4 is A

    LD R4, E
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, I
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, O
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, U
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, LOWERA
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, LOWERE
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, LOWERI
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, LOWERO
    ADD R4, R4, R5
    BRz CONTINUE

    LD R4, LOWERU
    ADD R4, R4, R5
    BRz CONTINUE

    LD R5, ANSWER           ;;Load Answer to 5
    ADD R5, R5, R1          ;;R5 = Answer[i]
    STR R3, R5, #0          ;;Answer[i] = String[j]
    ADD R1, R1, #1          ;;index for answer + 1

    CONTINUE
    ADD R2, R2, #1          ;;index for string + 1

BR W1
TERMINATE


HALT

STRING .fill x4000
ANSWER .fill x4100




LOWERA .fill x61    
A .fill x41     
LOWERE .fill x65 
E .fill x45 
LOWERI .fill x69 
I .fill x49 
LOWERO .fill x6f 
O .fill x4f 
LOWERU .fill x75
U .fill x55 

.end

.orig x4000

.stringz "spongebob and SQUIDWARD"

.end
