//Prueba aritmética
Mov [DS:1],'4   //guarda caracter
mov [DS:2],%4E  //guarda hexa
MOV [DS:3],@4   //guarda octal 
mov [DS:4],#390 //guarda decimal
mov AX,%A
mov BX,7
shr AX,1
shl BX,1
mov cx,69
mod cx,bx
swap ax,BX
add [DS:1],AX
sub [DS:2],BX
mul [DS:3],AX
add [DS:3],cx
div [DS:4],BX
mov AX,%10
mov CX,4
mov DX,1
SYS 2
//Espero que salga BIEN ;)