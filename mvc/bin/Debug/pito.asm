MOV AX,%0001
MOV DX,1
MOV CX,2
SYS 1
MOV BX,[1]
MOV CX,[2]

XOR AX, AX
CMP BX, 0
JZ FINIQUITAR
CMP CX, 0         //Si cualquiera de los dos es cero, 
JZ FINIQUITAR     //dejo cero en AX y me marcho xd

FILTROBX: XOR EX, EX        //EX es boolean "BX es negativo"
CMP BX, 0         //Si BX es positiva voy a filtrar CX
JNN FILTROCX      //Si es negativa la convierto en positiva
NOT BX
ADD BX, 1
ADD EX, 1         //Cambio EX a "true"

FILTROCX: XOR FX, FX        //FX es boolean "CX es negativo"
CMP CX, 0         //Exactamente lo mismo que con BX
JNN OPTIMIZA      //Continua a OPTIMIZA
NOT CX
ADD CX, 1
ADD FX, 1

OPTIMIZA: CMP BX, CX      //Si CX es mas grande que BX hace 
JP LOOP         //el loop inverso (va a LOOPINV)
SWAP BX, CX     //Si no, continua al LOOP comun

LOOP: ADD AX, BX           //Suma BX en AX "CX veces"
SUB CX, 1
JZ FIN
JMP LOOP

FIN: XOR EX, FX      //Si los signos son iguales (XOR), me marcho xd
JZ FINIQUITAR
NOT AX          //Si no son iguales doy vuelta el resultado
ADD AX, 1

FINIQUITAR: MOV [1],AX
MOV AX,%0001
MOV DX,1
MOV CX,1
SYS 2

MOV DX,0
MOV CX,0
MOV AX,%0008
SYS 3
STOP    //Finiquitar deberia volver a lo