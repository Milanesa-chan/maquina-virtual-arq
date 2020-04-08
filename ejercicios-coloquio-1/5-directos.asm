// Entradas para probar: ororo, salas, anita

// lee una palabra de 5 caracteres

    MOV AX, %0100
    MOV DX, 1
    MOV CX, 6
    SYS 1


// intercambia los caracteres en 1 y 5

    MOV [ES:2], [DS:5]
    NOV [5], [DS:1]
    MOV [1], [ES:2]


// intercambia los caracteres en 2 y 4

    MOV [ES:1], [2]
    MOV [DS:2], [4]
    MOV [DS:4], [ES:1]


// imprime los caracteres en todos los formatos

    MOV AX, %001F
    MOV DX, 1
    MOV CX, 5
    SYS 2
