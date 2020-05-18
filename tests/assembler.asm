// Inicializa HEAP
        mov     [ES:0], 1
// Escribe mensaje al usuario
msg     equ     "Escriba palabras seguidas de ENTER (en blanco para terminar)"
        mov     ax, %1000
        mov     bx, 1
        mov     cx, -1  
        mov     dx, msg
        sys     20 
        mov     cx, 1 
// Lee una palabra en DS:0
ini:    mov     ax, %1100
        mov     bx, 2
        mov     cx, -1
        mov     dx, 0
        sys     10
        slen    cx, [DS:0]
        cmp     cx, 0           //Si está vacía... 
        jz      finlee          //...termina la lectura 
        mov     dx, [ES:0]      //Sino, muevo el HEAP a DX        
        add     [ES:0], cx      //Incremento el HEAP para reservar la memoria 
        add     [ES:0], 1       //Uno más por el \0
        smov    [ES:dx], [DS:0] //Agrega la palabra en el ES 
        jmp     ini
finlee: mov dx, 1               //Inicializa 1 para comenzar el recorrido 
        mov cx, [ES:0]          
        sub cx, 1               //Posiciona a cx en el último \0
next:   cmp dx, cx              //Termina cuando DX llega al último \0
        jz  fin
may:    and [ES:dx], %DF        //Pasa a MAYUSCULAS
        cmp [ES:dx], 0          //Si encuentra un \0 ...
        jnz sig         
        mov [ES:dx], ' '        //... pone un espacio para contactenar 
sig:    add dx, 1               
        jmp next
// Muestra cadena concatenada y pasada a mayusculas
fin:    SYS 0
        mov     ax, %1000
        mov     bx, 3
        mov     cx, -1  
        mov     dx, 1
        sys     20 
        stop 


        



