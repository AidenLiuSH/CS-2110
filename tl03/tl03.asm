;; Timed Lab 3
;; Student Name: Shihui Liu

;; Please read the PDF for full directions.
;; The pseudocode for the program you must implement is listed below; it is also listed in the PDF.
;; If there are any discrepancies between the PDF's pseudocode and the pseudocode below, notify a TA immediately.
;; However, in the end, the pseudocode is just an example of a program that would fulfill the requirements specified in the PDF.

;; Pseudocode:
;; // (checkpoint 1)
;; int MAX(int a, int b) {
;;   if (a > b) {
;;       return 0;
;;   } else {
;;       return 1;
;;   }
;; }
;;
;;
;;
;;
;; DIV(x, y) {
;;	   // (checkpoint 2) - Base Case
;;     if (y > x) {
;;         return 0;
;;     // (checkpoint 3) - Recursion
;;     } else {
;;         return 1 + DIV(x - y, y);
;;     }
;; }
;;
;;
;;
;; // (checkpoint 4)
;; void MAP(array, length) {
;;   int i = 0;
;;   while (i < length) {
;;      int firstElem = arr[i];
;;      int secondElem = arr[i + 1];
;;      int div = DIV(firstElem, secondElem);
;;      int offset = MAX(firstElem, secondElem);
;;      arr[i + offset] = div;
;;      i += 2;
;;   }
;; }


.orig x3000
HALT

STACK .fill xF000

; DO NOT MODIFY ABOVE


; START MAX SUBROUTINE
MAX



;;stack build up
ADD R6, R6, -4      
STR R7, R6, 2       
STR R5, R6, 1       
ADD R5, R6, 0       
ADD R6, R6, -5      
STR R0, R5, -1
STR R1, R5, -2
STR R2, R5, -3
STR R3, R5, -4
STR R4, R5, -5


LDR R0, R5, #4    ; Load a into R1
LDR R1, R5, #5    ; Load b into R2
NOT R2, R1
ADD R2, R2, #1
ADD R2, R2, R0
BRP MAXELSE
AND R0, R0, #0
ADD R0, R0, #1 
STR R0, R5, #0
BR ENDMAX
MAXELSE
AND R0, R0, #0 
STR R0, R5, #0

ENDMAX
LDR R0, R5, 0    
STR R0, R5, 3
LDR R4, R5, -5  
LDR R3, R5, -4   
LDR R2, R5, -3    
LDR R1, R5, -2    
LDR R0, R5, -1   
ADD R6, R5, 0     
LDR R5, R6, 1     
LDR R7, R6, 2     
ADD R6, R6, 3     



RET
; END MAX SUBROUTINE




; START DIV SUBROUTINE
DIV



;;Stack buildup
ADD R6, R6, -4    
STR R7, R6, 2  
STR R5, R6, 1   
ADD R5, R6, 0     
ADD R6, R6, -5     
STR R0, R5, -1
STR R1, R5, -2
STR R2, R5, -3
STR R3, R5, -4
STR R4, R5, -5


LDR R0, R5, #4    ; Load x into R1
LDR R1, R5, #5    ; Load y into R2
NOT R2, R1
ADD R2, R2, #1
ADD R2, R2, R0
BRN DIVELSE
ADD R6, R6, #-1 
  STR R1, R6, #0
ADD R6, R6, #-1 
  STR R2, R6, #0  
  JSR DIV    
  LDR R0, R6, #0  
  ADD R6, R6, #3 
  ADD R0, R0, #1 
  STR R0, R5, #0  
BR ENDDIV
DIVELSE
AND R0, R0, #0
STR R0, R5, #0

ENDDIV
LDR R0, R5, 0    
STR R0, R5, 3
LDR R4, R5, -5  
LDR R3, R5, -4   
LDR R2, R5, -3    
LDR R1, R5, -2    
LDR R0, R5, -1   
ADD R6, R5, 0     
LDR R5, R6, 1     
LDR R7, R6, 2     
ADD R6, R6, 3     



RET
; END DIV SUBROUTINE



; START MAP SUBROUTINE
MAP


ADD R6, R6, -4      ; Allocate space
STR R7, R6, 2       ; Save Ret Addr
STR R5, R6, 1       ; Save Old FP
ADD R5, R6, 0       ; Copy SP to FP
ADD R6, R6, -5      ; Room for 5 regs
STR R0, R5, -1
STR R1, R5, -2
STR R2, R5, -3
STR R3, R5, -4
STR R4, R5, -5


AND R0, R0, #0     
WHILE1
  LDR R1, R5, #5    
  NOT R1, R1        
  ADD R1, R1, #1
  ADD R1, R1, R0    
  BRZP ENDMAP    
  LDR R1, R5, #4  
  ADD R1, R1, R0 
  LDR R1, R1, #0 
  LDR R2, R5, #4  
  ADD R2, R2, R0  
  ADD R2, R2, #1
  LDR R2, R2, #0 

  ADD R6, R6, #-1 
  STR R2, R6, #0  
  ADD R6, R6, #-1 
  STR R1, R6, #0  
  JSR DIV     
  LDR R3, R6, #0  
  ADD R6, R6, #3  

  ADD R6, R6, #-1 
  STR R2, R6, #0  
  ADD R6, R6, #-1 
  STR R1, R6, #0 
  JSR MAX     
  LDR R4, R6, #0  
  ADD R6, R6, #3  

  LDR R2, R5, #4  
  ADD R4, R4, R2
  ADD R4, R4, R0 
  STR R3, R4, #0    

  ADD R0, R0, #2
  BR WHILE1

ENDMAP
LDR R0, R5, 0    
STR R0, R5, 3
LDR R4, R5, -5  
LDR R3, R5, -4   
LDR R2, R5, -3    
LDR R1, R5, -2    
LDR R0, R5, -1   
ADD R6, R5, 0     
LDR R5, R6, 1     
LDR R7, R6, 2     
ADD R6, R6, 3  


RET
; END MAP SUBROUTINE


; LENGTH FOR TESTING

LENGTH .fill x12

; ARRAY FOR TESTING
ARRAY .fill x4000

.end

.orig x4000
.fill 12
.fill 3
.fill 5
.fill 7
.fill 16
.fill 2
.fill 5
.fill 5
.fill 25
.fill 7
.fill 48
.fill 60
.end