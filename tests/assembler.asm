MOV DX,1  // preparo la celda [1] para leer
MOV CX,1
MOV AX,%0001  // leo en decimal
SYS 1  //leo xd
MOV BX,[1]  //uso el registro BX para eficiencia
MOV EX,1  //inicializo EX en 1
CMP BX, 2
JZ PRIMO
 
	
//djksadjaskdjaksdj

LOOP: ADD EX,1 //loop para encontrar raiz de BX 
MOV FX,EX  //FX es un registro auxiliar
MUL FX,FX  //saco el cuadrado para 
CMP BX,FX  //encontrar un aproximado a la raiz de BX
JP LOOP  
SUB EX,1  //Como nos pasamos 1 restamos

LOOP2: MOV FX,BX
MOD FX,EX  //para calcular el resto de la division
CMP FX,0  //Si encontamos un num entre 2 y EX(raiz)
JZ NOPRIMO // no es primo
SUB EX,1  //voy restando EX para llegar al 2
CMP EX,1  //Cuando EX llega a 1 termina la compracion entonces 
JZ PRIMO  //Es primo
JMP LOOP2

PRIMO: MOV [1],1

JMP FIN

NOPRIMO: MOV [1],0

FIN: SYS 2
STOP
