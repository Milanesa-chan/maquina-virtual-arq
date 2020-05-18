\\ASM DATA=10 EXTRA=-1 STACK=0
dmin    EQU 0
dmax    EQU 1
line    EQU "-----------------------------------------------------------------------"
mmin    EQU "El minimo es: "
mmax    EQU "El maximo es: "
        SYS 0
        MOV     AX, %1000
        MOV     BX, 1
        MOV     CX, -1
        MOV     DX, line
        SYS     20 
        MOV     AX, %1100
        MOV     BX, 1
        MOV     CX, -1
        MOV     DX, mmin
        SYS     20
        MOV     [dmin],[ES:dmin]
        MOV     [ES:dmin],0
        MOV     AX, %1001
        MOV     BX, 2
        MOV     CX, 1
        MOV     DX, dmin
        SYS     2
        MOV     AX, %1000
        MOV     BX, 1
        MOV     CX, -1
        MOV     DX, line
        SYS     20 
        MOV     AX, %1100
        MOV     BX, 1
        MOV     CX, -1
        MOV     DX, mmax
        SYS     20
        MOV     [dmax],[ES:dmax]
        MOV     [ES:dmax],0
        MOV     AX, %1001
        MOV     BX, 2
        MOV     CX, 1
        MOV     DX, dmax
        SYS     2
        MOV     AX, %1000
        MOV     BX, 1
        MOV     CX, -1
        MOV     DX, line
        SYS     20 
        STOP
        
        