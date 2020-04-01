#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
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

int32_t registros[16];
int32_t memoria[2000];
int ejecutando = 1, jump = 0;
void (*funciones[144])(int, int, int, int);
void ejecutar();

int main(int argc, char *args[])
{
    if(argc>1)
    {
        agregarFunciones(funciones);
        cargarArchivo(args[1]);
        ejecutar();
    }
    return 0;
}

void cargarArchivo(char nombre[])
{
    FILE *arch = fopen(nombre, "rb");
    int32_t temp;
    for(int i=0; i<16; i++)
    {
        fread(&temp, sizeof(temp), 1, arch);
        registros[i] = temp;
    }
    for(int i=0; i<registros[2]; i++)
    {
        fread(&temp, sizeof(temp), 1, arch);
        memoria[i] = temp;
    }
}

void ejecutar(){
    int celdainst, param1, param2, tipo1, tipo2;
    int maskarg1 = 0x0000FF00, maskarg2 = 0x000000FF;
    int shiftinst = 16, shift1 = 8;

    while(ejecutando){
        jump = 0;
        celdainst = memoria[registros[4]];
        param1 = memoria[registros[4]+1];
        param2 = memoria[registros[4]+2];
        tipo1 = (celdainst & maskarg1)>>shift1;
        tipo2 = (celdainst & maskarg2);
        funciones[celdainst>>shiftinst](tipo1, tipo2, param1, param2);

        if(!jump) registros[4]+=3; //Si no hubo un salto, aumenta el IP
        ejecutando = registros[4] < registros[2]; //Si IP < DS sigue ejecutando
    }
}

void agregarFunciones(void (*funciones[])(int, int, int, int))
{
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

void mov(int t1, int t2, int par1, int par2)
{

}
void add(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2){
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        if(baseb){
            b = memoria[registros[baseb]+(par2 & ~mask)];
        }else{
            b = memoria[registros[2]+(par2 & ~mask)];
        }
    }

    switch(t1){
    case 1: //Registro
        registros[par1] += b;
        res = registros[par1];
        break;
    case 2: //Directo
        if(basea){
            memoria[registros[basea]+(par1 & ~mask)] += b;
            res = memoria[registros[basea]+(par1 & ~mask)];
        }
        break;
    }

    registros[9] = 0;
    if(res==0) registros[9] += 1;
    else if(res<0) registros[9] |= (1<<31);

    printf("ADD: %d, cc: %d", res, registros[9]);
}
void sub(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2){
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        if(baseb){
            b = memoria[registros[baseb]+(par2 & ~mask)];
        }else{
            b = memoria[registros[2]+(par2 & ~mask)];
        }
    }

    switch(t1){
    case 1: //Registro
        registros[par1] -= b;
        res = registros[par1];
        break;
    case 2: //Directo
        if(basea){
            memoria[registros[basea]+(par1 & ~mask)] -= b;
            res = memoria[registros[basea]+(par1 & ~mask)];
        }
        break;
    }

    registros[9] = 0;
    if(res==0) registros[9] += 1;
    else if(res<0) registros[9] |= (1<<31);

    printf("SUB: %d, cc: %d", res, registros[9]);

}
void mul(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2){
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        if(baseb){
            b = memoria[registros[baseb]+(par2 & ~mask)];
        }else{
            b = memoria[registros[2]+(par2 & ~mask)];
        }
    }

    switch(t1){
    case 1: //Registro
        registros[par1] *= b;
        res = registros[par1];
        break;
    case 2: //Directo
        if(basea){
            memoria[registros[basea]+(par1 & ~mask)] *= b;
            res = memoria[registros[basea]+(par1 & ~mask)];
        }
        break;
    }

    registros[9] = 0;
    if(res==0) registros[9] += 1;
    else if(res<0) registros[9] |= (1<<31);

    printf("MUL: %d, cc: %d", res, registros[9]);
}
void Div(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2){
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        if(baseb){
            b = memoria[registros[baseb]+(par2 & ~mask)];
        }else{
            b = memoria[registros[2]+(par2 & ~mask)];
        }
    }

    switch(t1){
    case 1: //Registro
        registros[par1] /= b;
        res = registros[par1];
        break;
    case 2: //Directo
        if(basea){
            memoria[registros[basea]+(par1 & ~mask)] /= b;
            res = memoria[registros[basea]+(par1 & ~mask)];
        }
        break;
    }

    registros[9] = 0;
    if(res==0) registros[9] += 1;
    else if(res<0) registros[9] |= (1<<31);

    printf("res: %d, cc: %d", res, registros[9]);

}
void mod(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;
    int division;
    switch(t2){
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        if(baseb){
            b = memoria[registros[baseb]+(par2 & ~mask)];
        }else{
            b = memoria[registros[2]+(par2 & ~mask)];
        }
    }
    switch(t1){
    case 1: //Registro
        division =  registros[par1]/b;
        registros[par1] %= b;
        res = registros[par1];
        break;
    case 2: //Directo
        if(basea){
            division = memoria[registros[basea]+(par1 & ~mask)]/b;
            memoria[registros[basea]+(par1 & ~mask)] %= b;
            res = memoria[registros[basea]+(par1 & ~mask)];
        }
        break;
    }

    registros[9] = 0;
    if(division==0) registros[9] += 1;
    else if(division<0) registros[9] |= (1<<31);

    printf("MOD: %d, cc: %d", res, registros[9]);

}

void cmp(int t1, int t2, int par1, int par2) {}
void swap(int t1, int t2, int par1, int par2) {}
void rnd(int t1, int t2, int par1, int par2) {}
void and(int t1, int t2, int par1, int par2) {}
void or(int t1, int t2, int par1, int par2) {}
void not(int t1, int t2, int par1, int par2) {}
void xor(int t1, int t2, int par1, int par2) {}
void shl(int t1, int t2, int par1, int par2) {}
void shr(int t1, int t2, int par1, int par2) {}
void jmp(int t1, int t2, int par1, int par2) {
    jump = 1;
    registros[4] = par1;
}
void je(int t1, int t2, int par1, int par2) {}
void jg(int t1, int t2, int par1, int par2) {}
void jl(int t1, int t2, int par1, int par2) {}
void jz(int t1, int t2, int par1, int par2) {}
void jp(int t1, int t2, int par1, int par2) {}
void Jn(int t1, int t2, int par1, int par2) {}
void jnz(int t1, int t2, int par1, int par2)
{
    printf("jnz funciona");
}
void jnp(int t1, int t2, int par1, int par2) {}
void jnn(int t1, int t2, int par1, int par2) {}
void sys(int t1, int t2, int par1, int par2) {}
void stop(int t1, int t2, int par1, int par2)
{
    ejecutando = 0;
}



