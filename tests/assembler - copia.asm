\\ASM DATA=500 EXTRA=500 STACK=500
MOV AX,#99
MOV BX,98
MOV CX,'A'
MOV DX,%FF
MOV EX @70
MOV FX 'a
MOV [DS:11] EX
MOV [ES:11] EX
MOV [11] EX
MOV [DS:EX-1] [CS:CONTS]
MOV [AX] CX
MOV [BX+1] CX
MOV [CX+3] CX
MOV [DS:DX] CX
MOV [EX] CX
MOV [FX] CX
MOV [AX+1] [AX]
MOV [BP] 1
MOV [SS:BP] 2
CONTS EQU #2
STOP