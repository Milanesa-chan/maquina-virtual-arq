mov ax,1
mov [0],ax
mov bx,[0]
sub ax,1
mov [1],ax
mov ex,[1]
sub ax,1
mov [2],ax
mov fx,[2]
mul ax,%ffffffff
mov [3],ax
mov ac,[3]
or  ax,8
xor dx,dx
mov cx,4
sys 3 
stop