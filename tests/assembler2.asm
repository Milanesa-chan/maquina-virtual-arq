\\ASM EXTRA = 550 	 STACK =700    //Directiva

	MOV AX,5
	CON1 EQU 'A
	CON2 EQU    	"Hola como andas"	
	MOV FX ,CON1 //Guarda en FX la constante CON1

	MOV [ES:AX-1] , %0001 //Prueba si lo indirectos andan bien

	SLEN [DS:10], CON2 //Guarda la longitud del string CON2 en la celda DS:10	
	
	MOV AX,[ES:4]

	MOV CX,1
	
	MOV DX, 10

	SYS 2 //Imprime la longitud del string CON2

	SMOV [ES:7]  ,  CON2

	SMOV [DS:200],[ES:7]

	SLEN AX,[DS:200]

	MOV AX,%0000

	MOV BX,2

	MOV DX,200

	SYS 20 //Imprime la cadena copiada con un endline
	SYS 0
	SLEN [DS:10], [DS:200] //Guarda la longitud del string CON2 en la celda DS:10

	MOV AX,[ES:4]
	MOV CX,1
	
	MOV DX, 10

	SYS 2 //Imprime la longitud del string CON2

	SCMP [ES:7], CON2

	JNZ fin

loop:	MOV AX,0

	MOV BX , 2

	MOV DX,100
SYS 0
	SYS 10 //Lee un string por teclado y lo guarda a partir de la celda [DS:100]
SYS 0
	SCMP [DS:100],[ES:7]
SYS 0
	JNZ loop
SYS 0
fin:	STOP
SYS 0