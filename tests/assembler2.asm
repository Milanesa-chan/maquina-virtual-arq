\\ASM DATA=0 EXTRA=-1 STACK=0
MIN     EQU %80000000
MAX     EQU %7FFFFFFF
UNO     EQU 1
dmin    EQU 0
dmax    EQU 1
start   EQU 2
        mov Dx, start
        mov Cx, [ES:Dx]
        add Cx, Dx
        mov [ES:dmin], MAX
        mov [ES:dmax], MIN
INI:    add Dx, UNO
        cmp Dx, Cx
        jp  fin
        cmp [ES:dmin], [ES:Dx]
        jn  sig
        mov [ES:dmin], [ES:Dx]
SIG:    cmp [ES:dmax], [ES:Dx]
        jp  ini
        mov [ES:dmax], [ES:Dx]
        jmp ini
FIN:    Stop 