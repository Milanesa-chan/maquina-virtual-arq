MOV EX, 1001
MOV BX, 100
ADD EX, BX
MOV [DS:1], EX
SYS 0
MOV AX, %1101
MOV DX, 1
MOV CX, 1
SYS 2