MOV AX,%1001
MOV DX,10
MOV CX,1
SYS 1
PUSH [10]
CALL FACT
ADD SP,1
MOV [10],AX
MOV AX,%1101
SYS 2
STOP
// SUBRUTINA
FACT: PUSH BP //guarda BP
MOV BP,SP
SUB SP,1 //Espacio var local
PUSH BX
PUSH CX
CMP [BP+2],1 //if (n<=1)
JP SIGO
MOV AX,1 //retorna 1
JMP FIN
SIGO: MOV BX,[BP+2]
SUB BX,1 //calcula n-1
PUSH BX
CALL FACT //invocación recursiva
ADD SP,1 //compensa el PUSH
MOV CX,[BP+2]
MUL CX,AX //res = n * (n-1)!
MOV [BP-1],CX
MOV AX,[BP-1] //retorna res
FIN: POP CX
POP BX
ADD SP,1 //libera memoria
MOV SP,BP
POP BP
RET