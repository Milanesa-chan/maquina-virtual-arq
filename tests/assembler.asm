MOV [1], [ES:1]
MOV [2], [ES:2]
MOV [3], [ES:3]
MOV [4], [ES:4]
MOV [5], [ES:5]
MOV [6], [ES:6]
MOV [7], [ES:7]
MOV [8], [ES:8]
MOV [9], [ES:9]
MOV [10], [ES:10]
MOV [11], [ES:11]
MOV [12], [ES:12]
MOV [13], [ES:13]
MOV [14], [ES:14]
MOV [15], [ES:15]
PUSH 1
CALL COMPA
ADD SP, 1
SYS 0
MOV [420], AX
MOV AX %0001
MOV DX 420
MOV CX 1
SYS 2
STOP

IZQ EQU 1
DER EQU 2
TRUE EQU #-1
FALSE EQU #0
NULL EQU -1
// Inicio de la sub rutina
COMPA: PUSH BP
MOV BP, SP
PUSH BX
PUSH CX
PUSH DX
// Verifico que no sea NULL
MOV CX, [BP+2]
CMP CX, NULL
JZ FIN1 //Si es null OK
// Si no es NULL recupera los valores de sus hijos
MOV BX, #0 //aux guarda la suma
// Recupera el valor del nodo izquierdo
MOV DX, [CX+IZQ]
CMP DX, NULL
JZ CDER
// Si no es NULL acumula el valor en DX
ADD BX, [DX]
// Recupera el valor del nodo derecho
CDER: MOV DX, [CX+DER]
 CMP DX, NULL
JZ VERIF
// Suma el valor del nodo izquierdo y derecho
ADD BX, [DX]
// si BX = 0 => no tiene hijos
VERIF: CMP #0, BX
JZ FIN1
// verifica si la suma es igual
// al valor del nodo
CMP [CX], BX
JNZ FIN2
// Si llegó acá entonces el padre es igual
// a la suma de los hijos.
// Entonces hay que verificar que se cumpla
// también para los hijos.
// Invoca para la rama izquierda
PUSH [CX+IZQ]
CALL COMPA
ADD SP, 1
// Si falló corta la recursividad
CMP AX,FALSE
JZ FIN
// (si sigue ok) Invoca para la rama derecha
PUSH [CX+DER]
CALL COMPA
ADD SP, 1
// el resultado queda definido por el valor
// que trae AX en la rama derecha.
JMP FIN
// Cuando devuelve OK
FIN1: MOV AX,TRUE
JMP FIN
// Cuando No es OK
FIN2: MOV AX,FALSE
// Fin de la subrutina
FIN: POP DX
POP CX
POP BX
MOV SP, BP
POP BP
RET