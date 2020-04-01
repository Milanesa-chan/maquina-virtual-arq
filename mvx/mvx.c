#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
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

int main(int argc, char *args[]) {
    if(argc>1) {
        srand(time(NULL));
        agregarFunciones(funciones);
        cargarArchivo(args[1]);
        ejecutar();
    }
    return 0;
}

void cargarArchivo(char nombre[]) {
    FILE *arch = fopen(nombre, "rb");
    int32_t temp;
    for(int i=0; i<16; i++) {
        fread(&temp, sizeof(temp), 1, arch);
        registros[i] = temp;
    }
    for(int i=0; i<registros[2]; i++) {
        fread(&temp, sizeof(temp), 1, arch);
        memoria[i] = temp;
    }
}

void ejecutar() {
    int celdainst, param1, param2, tipo1, tipo2;
    int maskarg1 = 0x0000FF00, maskarg2 = 0x000000FF;
    int shiftinst = 16, shift1 = 8;

    while(ejecutando) {
        jump = 0;
        celdainst = memoria[registros[4]];
        param1 = memoria[registros[4]+1];
        param2 = memoria[registros[4]+2];
        tipo1 = (celdainst & maskarg1)>>shift1;
        tipo2 = (celdainst & maskarg2);
        funciones[celdainst>>shiftinst](tipo1, tipo2, param1, param2);

        if(!jump)
            registros[4]+=3; //Si no hubo un salto, aumenta el IP
        ejecutando = (registros[4] < registros[2])&&ejecutando; //Si IP < DS sigue ejecutando
    }
}

void agregarFunciones(void (*funciones[])(int, int, int, int)) {
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

void mov(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] = b;
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] = b;
        break;
    }
    printf("Funciono el MOV");
}
void add(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] += b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] += b;
        res = memoria[registros[basea]+(par1 & ~mask)];

        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("ADD: %d, cc: %d\n", res, registros[9]);
}
void sub(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] -= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] -= b;
        res = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("SUB: %d, cc: %d\n", res, registros[9]);

}
void mul(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] *= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] *= b;
        res = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("MUL: %d, cc: %d\n", res, registros[9]);
}
void Div(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] /= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] /= b;
        res = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("res: %d, cc: %d\n", res, registros[9]);

}
void mod(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;
    int division;
    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }
    switch(t1) {
    case 1: //Registro
        division =  registros[par1]/b;
        registros[par1] %= b;
        res = registros[par1];
        break;
    case 2: //Directo
        division = memoria[registros[basea]+(par1 & ~mask)]/b;
        memoria[registros[basea]+(par1 & ~mask)] %= b;
        res = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }
    registros[9] = 0;
    if(division==0)
        registros[9] += 1;
    else if(division<0)
        registros[9] |= (1<<31);

    printf("MOD: %d, cc: %d\n", res, registros[9]);
}

void cmp(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;

    }

    switch(t1) {
    case 1: //Registro
        res = registros[par1] - b;
        break;
    case 2: //Directo
        res = memoria[registros[basea]+(par1 & ~mask)] - b;
        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("CMP: %d, cc: %d\n", res, registros[9]);

}
void swap(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int b,a, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        a = registros[par1];
        break;
    case 2: //Directo
        a = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }
    switch(t1+(t2<<2)) {
    case 5: //registro registro 0101
        registros[par2]=a;
        registros[par1]=b;
        break;
    case 6://memoria registro 0110
        registros[par2]=a;
        memoria[registros[basea]+(par1 & ~mask)]=b;
        break;
    case 9://registro memoria 1001
        memoria[registros[baseb]+(par2 & ~mask)]=a;
        registros[par1]=b;
        break;
    case 10://memoria memoria 1010
        memoria[registros[basea]+(par1 & ~mask)]=b;
        memoria[registros[baseb]+(par2 & ~mask)]=a;
        break;
    }

    printf("SWAP: a.%d, b.%d\n", a,b);
}

void rnd(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;
    int num;

    switch(t2) {
    case 0:
        num = par2;
        break;
    case 1:
        num = registros[par2];
        break;
    case 2:
        num = memoria[registros[baseb]+(par2 & ~mask)];
        break;

    }

    switch(t1) {
    case 1:
        registros[par1] = rand() % (num + 1);
        break;
    case 2:
        memoria[registros[basea]+(par1 & ~mask)] = rand() % (num + 1);
        break;
    }
}
void and(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] &= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] &= b;
        res = memoria[registros[basea]+(par1 & ~mask)];

        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("AND: %d, cc: %d\n", res, registros[9]);
}
void or(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] |= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] |= b;
        res = memoria[registros[basea]+(par1 & ~mask)];

        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("OR: %d, cc: %d\n", res, registros[9]);
}
void not(int t1, int t2, int par1, int par2) {}
void xor(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] ^= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] ^= b;
        res = memoria[registros[basea]+(par1 & ~mask)];

        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("XOR: %d, cc: %d\n", res, registros[9]);
}
void shl(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] <<= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] <<= b;
        res = memoria[registros[basea]+(par1 & ~mask)];

        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("SHL: %d, cc: %d\n", res, registros[9]);
}
void shr(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        b = par2;
        break;
    case 1:
        b = registros[par2];
        break;
    case 2:
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] >>= b;
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] >>= b;
        res = memoria[registros[basea]+(par1 & ~mask)];

        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

    printf("SHR: %d, cc: %d\n", res, registros[9]);
}
void jmp(int t1, int t2, int par1, int par2) {
    jump = 1;
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;

    switch(t1) {
    case 0:
        registros[4] = par1;
    case 1: //Registro
        registros[4] = registros[par1];
        break;
    case 2: //Directo
        registros[4] = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }

    printf("JMP: %d", registros[4]);
}

void je(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int salto, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        salto = par2;
        break;
    case 1:
        salto = registros[par2];
        break;
    case 2:
        salto = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 0: //Inmediato
        jump = par1 == registros[10];
        break;
    case 1: //Registro
        jump = registros[par1] == registros[10];
        break;
    case 2: //Directo
        jump = memoria[registros[basea]+(par1 & ~mask)] == registros[10];
        break;
    }

    if(jump)
        registros[4] = salto;

    printf("JE: %d", registros[4]);
}
void jg(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int salto, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        salto = par2;
        break;
    case 1:
        salto = registros[par2];
        break;
    case 2:
        salto = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 0: //Inmediato
        jump = par1 > registros[10];
        break;
    case 1: //Registro
        jump = registros[par1] > registros[10];
        break;
    case 2: //Directo
        jump = memoria[registros[basea]+(par1 & ~mask)] > registros[10];
        break;
    }

    if(jump)
        registros[4] = salto;

    printf("JG: %d", registros[4]);
}
void jl(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int salto, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;

    switch(t2) {
    case 0:
        salto = par2;
        break;
    case 1:
        salto = registros[par2];
        break;
    case 2:
        salto = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    }

    switch(t1) {
    case 0: //Inmediato
        jump = par1 < registros[10];
        break;
    case 1: //Registro
        jump = registros[par1] < registros[10];
        break;
    case 2: //Directo
        jump = memoria[registros[basea]+(par1 & ~mask)] < registros[10];
        break;
    }

    if(jump)
        registros[4] = salto;

    printf("JL: %d", registros[4]);
}

void jz(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;


    jump = registros[9] == 1;

    if(jump) {
        switch(t1) {
        case 0:
            registros[4] = par1;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }

    printf("JZ: %d", registros[4]);
}

void jp(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;


    jump = registros[9] == 0;

    if(jump) {
        switch(t1) {
        case 0:
            registros[4] = par1;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }

    printf("JP: %d", registros[4]);
}
void Jn(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;


    jump = registros[9] < 0;

    if(jump) {
        switch(t1) {
        case 0:
            registros[4] = par1;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }

    printf("JN: %d", registros[4]);
}
void jnz(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;


    jump = registros[9] != 1;

    if(jump) {
        switch(t1) {
        case 0:
            registros[4] = par1;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }

    printf("JNZ: %d", registros[4]);
}
void jnp(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;


    jump = registros[9] != 0;

    if(jump) {
        switch(t1) {
        case 0:
            registros[4] = par1;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }

    printf("JNP: %d", registros[4]);
}
void jnn(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;


    jump = registros[9] >= 0;

    if(jump) {
        switch(t1) {
        case 0:
            registros[4] = par1;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }

    printf("JNN: %d", registros[4]);
}
void sys(int t1, int t2, int par1, int par2) {
    switch(par1) {
    case 1:
        leer();
        break;
    case 2:
        escribir();
        break;
    case 3:
        dump();
        break;
    }
}

void stop(int t1, int t2, int par1, int par2) {
    ejecutando = 0;
}


void leer() {
    int prompt = registros[10]&0x1000;
    int texto = registros[10]&0x0100;
    int modo = registros[10]&0xF; //1 = dec , 4 = octal , 8 = hexa

    int desde = registros[13];
    int cant = registros[12];
    int aux;
    char buffer[100];

    if(!texto) {
        for(int i=0; i<cant; i++) {
            if(prompt) {
                printf("[");
                mostrarCelda(desde+i);
                printf("]: ");
            }

            switch(modo) {
            case 1:
                scanf(" %d", &aux);
                memoria[desde+i] = aux;
                break;
            case 4:
                scanf(" %o", &aux);
                memoria[desde+i] = aux;
                break;
            case 8:
                scanf(" %x", &aux);
                memoria[desde+i] = aux;
                break;
            }
        }
    } else {
        if(prompt) {
            printf("[");
            mostrarCelda(desde);
            printf("]: ");
        }

        scanf(" %s", buffer);
        int max = strlen(buffer)>(cant-1) ? (cant-1) : strlen(buffer);
        for(int i=0; i<max; i++){
            memoria[desde + i] = buffer[i];
        }
        memoria[desde + max] = '\0';
    }

}

void escribir() {
    int prompt = registros[10]&0x1000;
    int endline = registros[10]&0x100;
    int car = registros[10]&0x0010;
    int modo = registros[10]&0xF; //1 = dec , 4 = octal , 8 = hexa

    int desde = registros[13];
    int cant = registros[12];
    int aux;
    char buffer[100];

    if(!car) {
        for(int i=0; i<cant; i++) {
            if(prompt) {
                printf("[");
                mostrarCelda(desde+i);
                printf("]: ");
            }

            switch(modo) {
            case 1:
                scanf(" %d", &aux);
                memoria[desde+i] = aux;
                break;
            case 4:
                scanf(" %o", &aux);
                memoria[desde+i] = aux;
                break;
            case 8:
                scanf(" %x", &aux);
                memoria[desde+i] = aux;
                break;
            }
        }
    } else {
        if(prompt) {
            printf("[");
            mostrarCelda(desde);
            printf("]: ");
        }

        scanf(" %s", buffer);
        int max = strlen(buffer)>(cant-1) ? (cant-1) : strlen(buffer);
        for(int i=0; i<max; i++){
            memoria[desde + i] = buffer[i];
        }
        memoria[desde + max] = '\0';
    }
    int prompt = registros[10]&0x1000;
    int texto = registros[10]&0x0100;
    int modo = registros[10]&0xF; //1 = dec , 4 = octal , 8 = hexa

    int desde = registros[13];
    int cant = registros[12];
    int aux;
    char buffer[100];

    if(!texto) {
        for(int i=0; i<cant; i++) {
            if(prompt) {
                printf("[");
                mostrarCelda(desde+i);
                printf("]: ");
            }

            switch(modo) {
            case 1:
                scanf(" %d", &aux);
                memoria[desde+i] = aux;
                break;
            case 4:
                scanf(" %o", &aux);
                memoria[desde+i] = aux;
                break;
            case 8:
                scanf(" %x", &aux);
                memoria[desde+i] = aux;
                break;
            }
        }
    } else {
        if(prompt) {
            printf("[");
            mostrarCelda(desde);
            printf("]: ");
        }

        scanf(" %s", buffer);
        int max = strlen(buffer)>(cant-1) ? (cant-1) : strlen(buffer);
        for(int i=0; i<max; i++){
            memoria[desde + i] = buffer[i];
        }
        memoria[desde + max] = '\0';
    }
}

void dump() {
}
