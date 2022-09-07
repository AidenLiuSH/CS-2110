.orig x3000
    LD R0, A
    LD R1, B
    ADD R0, R0, R1
    ST R0, RESULT
    HALT
.end

.orig x4000
    A .fill x1000
    B .fill x234
    RESULT .blkw 1
.end