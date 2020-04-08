// Leo entradas. ej: 101,87,AA,CAE

        mov     ax  ,   %0008
//      mov     ax  ,   %0001
        mov     cx  ,   1
        mov     dx  ,   0
        sys     1 

// Inicializo         
        mov     ax  ,   32
        xor     [1] ,   [1]

// Cálculos 

sigue:  mov     ex  ,   [0]
        shl     [0] ,   1
        shr     ex  ,   31
        mul     [1] ,   10
        add     [1] ,   ex
        sub     ax  ,   1
        jnz     sigeu
// Muestro resultados        

        mov     ax  ,   %0001
        mov     cx  ,   1
        mov     dx  ,   1
        sys     2
        stop
        mov     [1] ,   1 // ojo esto no. 
        sys     2
        stop