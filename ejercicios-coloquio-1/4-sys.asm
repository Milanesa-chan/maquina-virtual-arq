// Entrada: B, 67, 104, 45, A


// lee un caracter en la posicion 1

        MOV CX, 5
        MOV DX, 1
        MOV AX, %1100
        SYS 1


// lee un numero en cada base: decimal, octal, hexa
// guarda en las posiciones 2 a 4

        MOV CX, 1
        MOV DX, 2
        MOV AX, 1

read:   SYS 1

        MOV BX, 1
        CMP AX, 1
        ADD BX, CC
        SHL AX, BX

        ADD DX, 1

        JG %D, read


// lee un caracter en la posicion 0

        MOV CX, 5
        MOV DX, 0
        MOV AX, %1100
        SYS 1


// imprime lo que se leyo con las 15 combinaciones posibles de formato

        MOV CX, 1
        MOV DX, 0
        MOV AX, 0

write:  ADD AX, 1
        MOV AC, AX
        AND AC, 2
        JNZ write

        SYS 2

        ADD DX, 1
        MOD DX, 5

        JG %1D, write


// Salida esperada

// B
// [0002]: 67
// [0003]: 104
// [0004]: 45
// A
// [0000]: 65
// [0001]: @0
// [0002]: @103 67
// [0003]: %44
// [0004]: %45 69
// [0000]: %41 @101
// [0001]: %0 @0 0
// [0002]: C
// [0003]: D 68
// [0004]: E @105
// [0000]: A @101 65
// [0001]: . %0
// [0002]: C %43 67
// [0003]: D %44 @104
// [0004]: E %45 @105 69