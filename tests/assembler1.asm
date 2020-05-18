        MOV AX,%0001
        MOV DX,10
        MOV CX,2
        SYS 1
        PUSH [10] //BASE
        PUSH [11] //EXPONENTE
        CALL POT
        ADD SP,2
        MOV [10],AX
        MOV CX,1
        MOV AX,%0001
        SYS 2
        STOP

POT:    PUSH BP
        MOV BP,SP
        PUSH BX //EN BX GUARDO BASE
        PUSH CX //EN CX GUARDO EXPONENTE
        CMP [BP+2],0 // me fijo si se llego al exponente 0
        JNZ SIGO 
        MOV AX,1
        JMP FIN
SIGO:   MOV BX,[BP+3]
        MOV CX,[BP+2]
        SUB CX,1
        PUSH BX
        PUSH CX
        CALL POT
        ADD SP,2
        MUL AX,[BP+3]
FIN:    POP CX
        POP BX
        MOV SP,BP
        POP BP
        RET