MOV EX, 1001
MOV BX, 100
ADD EX, BX
MOV AX, 3
MOV [DS:AX+2], EX
SYS 0
MOV AX, %1101
MOV DX, 5
MOV CX, 1
SYS 2