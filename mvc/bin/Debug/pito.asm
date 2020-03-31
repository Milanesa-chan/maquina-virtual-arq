XOR FX,FX //inicializo en 0 contador
MOV [2],0  //inicializo en 0 acumulador

OTRO: MOV AX,%0001  //leer decimal

MOV DX,1
MOV CX,1  // uso una sola celda para leer
SYS 1
CMP [1],0  // se fija si es negativo
JNP sigue  // si es negativo salta
ADD [2],[1]  // acumulador
ADD FX,1  // contador
JMP otro

sigue: CMP 0,FX  //me fijo si el contador esta en 0

JZ fin  // si es 0 salta a fin
DIV [2],FX
fin: MOV DX,2
SYS 2
stop