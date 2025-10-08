;----- get arguments
IN
POPREG Ax
IN
POPREG Bx
IN
POPREG Cx


PUSHREG Ax
PUSH 0
JE 112


PUSHREG Bx          ; вычисление дискриминанта
PUSHREG Bx
MUL
PUSH 4
PUSHREG Ax
PUSHREG Cx
MUL
MUL
SUB
POPREG Dx


PUSH 0
PUSHREG Dx    ;35
JB 84           ; if d < 0
PUSH 0
PUSHREG Dx
JE 92                  ; if d = 0


PUSH 2               ; if d > 0
POPREG Ex
PUSHREG Dx
SQRT
PUSH -1
MUL                ;53
PUSHREG Bx
SUB
PUSH 2
PUSHREG Ax
MUL
DIV
POPREG Fx    ;save x1
PUSHREG Dx
SQRT
PUSHREG Bx
SUB
PUSH 2
PUSHREG Ax
MUL                 ;75
DIV
PUSHREG Fx
PUSHREG Ex     ;
OUT             ;
OUT
OUT
HLT


PUSH 0          ;if d < 0
POPREG Ex
PUSHREG Ex
OUT
HLT


PUSH 1              ;if d == 0
POPREG Ex
PUSH 0
PUSHREG Bx
SUB
PUSH 2
PUSHREG Ax
MUL
DIV
PUSHREG Ex
OUT
OUT
HLT



PUSH -1
OUT
HLT             ;116



