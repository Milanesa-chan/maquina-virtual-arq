\\ASM DATA=1 EXTRA=0 STACK=70
buffer  equ     0
    mov     dx, buffer
    mov     cx, 1
    mov     ax, %1    
    push    1
    push    1
    push    10
    call    fibo
    add     sp, 3
    mov     [buffer], 1
    sys     2
    sys     2
    sys     0
    pop     ax
    stop
fibo:   push    bp
        mov     bp, sp         
        sub     sp, 1
        cmp     [bp+2], 0 
        jz      finf
        sub     [bp+2], 1
        mov     [bp-1],[bp+3]
        add     [bp-1],[bp+4]
        push    [bp-1]
        push    [bp+4]        
        push    [bp+2]
        call    fibo
        add     sp, 3
        mov     [buffer],[bp-1]
        sys     2        
finf:   mov     sp, bp 
        pop     bp
        ret