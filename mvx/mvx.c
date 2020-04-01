#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mvc/tdacazorla.h"


void mov(int t1, int t2, int par1, int par2);
void add(int t1, int t2, int par1, int par2);
void sub(int t1, int t2, int par1, int par2);
void mul(int t1, int t2, int par1, int par2);
void Div(int t1, int t2, int par1, int par2);
void mod(int t1, int t2, int par1, int par2);
void cmp(int t1, int t2, int par1, int par2);
void swap(int t1, int t2, int par1, int par2);
void rnd(int t1, int t2, int par1, int par2);
void and(int t1, int t2, int par1, int par2);
void or(int t1, int t2, int par1, int par2);
void not(int t1, int t2, int par1, int par2);
void xor(int t1, int t2, int par1, int par2);
void shl(int t1, int t2, int par1, int par2);
void shr(int t1, int t2, int par1, int par2);
void jmp(int t1, int t2, int par1, int par2);
void je(int t1, int t2, int par1, int par2);
void jg(int t1, int t2, int par1, int par2);
void jl(int t1, int t2, int par1, int par2);
void jz(int t1, int t2, int par1, int par2);
void jp(int t1, int t2, int par1, int par2);
void Jn(int t1, int t2, int par1, int par2);
void jnz(int t1, int t2, int par1, int par2);
void jnp(int t1, int t2, int par1, int par2);
void jnn(int t1, int t2, int par1, int par2);
void sys(int t1, int t2, int par1, int par2);
void stop(int t1, int t2, int par1, int par2);
void agregarFunciones(void (*funciones[])(int, int, int, int));
void cargarArchivo(char[]);
//"Jn" y "Div" empiezan con mayuscula xd

int registros[16];
int memoria[2000];

int main(int argc, char *args[])
{
    if(argc>1)
    {
        void (*funciones[144])(int, int, int, int);
        agregarFunciones(funciones);
        cargarArchivo(args[1]);
    }
    return 0;
}

void cargarArchivo(char nombre[]){
    FILE *arch = fopen(nombre, "rb");
    //for(int i=0;)
    while(!feof(arch)){
        int temp;
        for(int i=0; i<32; i++){
            fread(&temp, sizeof(int), 1, arch);
            printf("%08X\n", temp);
        }
    }
}

void agregarFunciones(void (*funciones[])(int, int, int, int)){
        funciones[1] = mov;
        funciones[2] = add;
        funciones[3] = sub;
        funciones[4] = mul;
        funciones[5] = Div;
        funciones[6] = mod;
        funciones[19] = cmp;
        funciones[23] = swap;
        funciones[25] = rnd;
        funciones[49] = and;
        funciones[50] = or;
        funciones[51] = not;
        funciones[52] = xor;
        funciones[55] = shl;
        funciones[56] = shr;
        funciones[32] = jmp;
        funciones[33] = je;
        funciones[34] = jg;
        funciones[35] = jl;
        funciones[36] = jz;
        funciones[37] = jp;
        funciones[38] = Jn;
        funciones[39] = jnz;
        funciones[40] = jnp;
        funciones[41] = jnn;
        funciones[129] = sys;
        funciones[143] = stop;
}

void mov(int t1, int t2, int par1, int par2){

}
void add(int t1, int t2, int par1, int par2){}
void sub(int t1, int t2, int par1, int par2){}
void mul(int t1, int t2, int par1, int par2){}
void Div(int t1, int t2, int par1, int par2){}
void mod(int t1, int t2, int par1, int par2){}
void cmp(int t1, int t2, int par1, int par2){}
void swap(int t1, int t2, int par1, int par2){}
void rnd(int t1, int t2, int par1, int par2){}
void and(int t1, int t2, int par1, int par2){}
void or(int t1, int t2, int par1, int par2){}
void not(int t1, int t2, int par1, int par2){}
void xor(int t1, int t2, int par1, int par2){}
void shl(int t1, int t2, int par1, int par2){}
void shr(int t1, int t2, int par1, int par2){}
void jmp(int t1, int t2, int par1, int par2){}
void je(int t1, int t2, int par1, int par2){}
void jg(int t1, int t2, int par1, int par2){}
void jl(int t1, int t2, int par1, int par2){}
void jz(int t1, int t2, int par1, int par2){}
void jp(int t1, int t2, int par1, int par2){}
void Jn(int t1, int t2, int par1, int par2){}
void jnz(int t1, int t2, int par1, int par2){
    printf("jnz funciona");
}
void jnp(int t1, int t2, int par1, int par2){}
void jnn(int t1, int t2, int par1, int par2){}
void sys(int t1, int t2, int par1, int par2){}
void stop(int t1, int t2, int par1, int par2){

}


