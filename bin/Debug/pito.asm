XOR FX,FX //inicializo en 0 contador
MOV [2],[23]  //inicializo en 0 acumulador
OTRO: MOV AX,%0001  //leer decimal

MOV DX,1
JE 0, OTRO
JE 1, CACA
JMP CACA
JMP OTRO
MOV CX,1  //uso una sola celda para leer
SYS 1
CMP [1],[DS:4]  //se fija si es negativo
JNP SIGUE  //si es negativo salta
ADD [2],[1]  //acumulador
ADD FX,1  //contador
JMP OTRO
TPS

ADD AX, 'F'
ADD AX, %A

ADD AX, @1001010


SIGUE: CMP 0,FX  //me fijo si el contador esta en 0

JZ fin  //si es 0 salta a fin
DIV [2],FX
FIN: MOV DX,[ES:24]
SYS 2
STOP