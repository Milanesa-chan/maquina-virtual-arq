//DIRECCIONAMIENTO INDIRECTO
\\ASM DATA=100 EXTRA=0 STACK=0
ant		EQU 	-1
sig		EQU		1
//inicializo los registros
		mov 	ax, 1
		mov		bx, 5
        mov     cx, 2        
//relleno la memoria de 1 a 10
otro:	cmp		ax, 7
		jz		sigue
		mov		[ax], ax
		add		ax, 1
		jmp 	otro
//recupero registros y multiplico x10
sigue:	mul 	[CX+ant],10
		mul 	[Cx],10
		mul 	[DS:cx+sig],10
		mul 	[bx-1],10
		mul 	[Bx],10
		mul 	[DS:BX+1],10
        mov     [DS:BX+2],[DS:cx-sig]
//imprime todo 
		mov 	ax, 1        
		mov		dx, 1
		mov		cx, 7
		sys 2
		stop