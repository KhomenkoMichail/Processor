;----- get arguments
IN
POPREG Ax
IN
POPREG Bx
IN
POPREG Cx       ;9

;----a==0?
PUSHREG Ax
PUSH 0
JE 135         ;15

;----c==0?
PUSHREG Cx
PUSH 0
JE 111          ;21

;----calc discriminate
PUSHREG Bx
PUSHREG Bx
MUL
PUSH 4
PUSHREG Ax
PUSHREG Cx
MUL
MUL
SUB
POPREG Dx      ;37

;----d < 0?
PUSH 0
PUSHREG Dx
JB 86     ;43

;----d == 0?
PUSH 0
PUSHREG Dx
JE 92     ;49

;----if d > 0
PUSH 2
POPREG Ex
PUSHREG Dx
SQRT
PUSH -1
MUL
PUSHREG Bx
SUB
PUSH 2
PUSHREG Ax
MUL
DIV
POPREG Fx     ;save x1
PUSHREG Dx
SQRT
PUSHREG Bx
SUB
PUSH 2
PUSHREG Ax
MUL
DIV
POPREG Gx   ; save x2
JMP 188              ;86

;----if d < 0
PUSH 0
POPREG Ex
JMP 188            ;92

;----if d == 0;
PUSH 1
POPREG Ex
PUSH 0
PUSHREG Bx
SUB
PUSH 2
PUSHREG Ax
MUL
DIV
POPREG Fx
JMP 188  ;111


;---- if ((a!=0 ) && (c == 0))
PUSH 0
POPREG Fx    ;115
PUSH 1
POPREG Ex

PUSH 0
PUSHREG Bx
JNE 127         ;;;solve LINEAR !!!! ;;b == 0
JMP 188         ;res     ;127

;;; a!=0 c==0
PUSHREG Bx
POPREG Cx
PUSHREG Ax
POPREG Bx             ;135

;---- solve linear
PUSH 0
PUSHREG Bx
JE 170      ; a == 0 , b == 0
PUSH 1
PUSHREG Ex
ADD
POPREG Ex
PUSH 0
PUSHREG Cx
SUB
PUSHREG Bx
DIV           ;ROOT ;156

PUSHREG Ex
PUSH 2          ;;;
JE 166 ;НА 2 СТРОЧКИ
POPREG Fx
JMP 188 ; res
POPREG Gx
JMP 188 ; res ;170

;---- a == 0 , b == 0
PUSHREG Cx
PUSH 0
JE 182  ; na 3 stringi   ;176

PUSH 0
POPREG Ex
JMP 188 ; res       ;182

PUSH -1
POPREG Ex
JMP 188 ; res   ;188

;---- results
PUSHREG Ex
OUT            ;191

PUSH 0
PUSHREG Ex
JBE 209  ; HLT   ; 197

PUSHREG Fx
OUT               ;200

PUSH 1
PUSHREG Ex
JE 209           ;HLT     ;206

PUSHREG Gx
OUT       ;209

HLT        ;210







