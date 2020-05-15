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
//
void push (int t1, int t2, int par1, int par2);
void pop (int t1, int t2, int par1, int par2);
void call (int t1, int t2, int par1, int par2);
void ret (int t1, int t2, int par1, int par2);
void slen (int t1, int t2, int par1, int par2);
void smov (int t1, int t2, int par1, int par2);
void scmp (int t1, int t2, int par1, int par2);
//
void sys(int t1, int t2, int par1, int par2);
void stop(int t1, int t2, int par1, int par2);

void agregarFunciones(void (*funciones[])(int, int, int, int));
void cargarArchivo(char[]);
//"Jn" y "Div" empiezan con mayuscula xd

void (*funciones[144])(int, int, int, int);
void ejecutar();
void dump();
void escribir();
void leer();
void getBuffer(int, int, int, int, char[], char[]);
int buscaFlags(int argc,char *args[]);

int preparacion(int cantImagenes,char *args[]);
int32_t registros[16];
int32_t memoria[8192];
int ejecutando = 1, jump = 0;
int mostrar = 0;
int flagA=0,flagB=0,flagC=0,flagD=0;
int ejecutarProcesos=0;


int main(int argc, char *args[]) {
    if(argc>1) {
        crearListaMnemonicos();
        int cantImagenes=argc-buscaFlags(argc,args)-1;
        //mostrar = contieneArg(argc, args, "-d");
        crearRegistros();
        srand(time(NULL));
        agregarFunciones(funciones);
        ejecutarProcesos=preparacion(cantImagenes,args); //Si da 0 es que ejecuta
        /*for(int i=1;i<=cantImagenes;i++)
        {
            cargarArchivo(args[i]);
            ejecutar();
        }*/

        if (!ejecutarProcesos)
        {
            while (memoria[1]<memoria[0])
            {
                int i;
                for (i=0;i<16;i++)
                    registros[i]=memoria[16*memoria[1]+2+i];
                ejecutar();
                for (i=0;i<16;i++)
                    memoria[16*memoria[1]+2+i] = registros[i];
                memoria[1]++;
                for(int i=0; i<=100;i++)
                    printf("memoria[%d]=%d\n",i,memoria[i]);
            }
            printf("Ejecucion exitosa de todos los procesos");
        }
            printf("No alcanza la memoria para ejecutar todos los procesos");
    }
    return 0;
}

int buscaFlags(int argc,char *args[])//cantidad de flag papa
{
    int cont=0;
    for (int i=2; i<argc;i++)//if you say, si bo deci
    {
        if(*args[i]=='-')
        {
            cont++;
            switch (*(args[i]+1))
            {
            case 'a':
                flagA=1;
                break;
            case 'b':
                flagB=1;
                break;
            case 'c':
                flagC=1;
                break;
            case 'd':
                flagD=1;
                break;
            default:
                //forrazo
                break;
            }
        }

    }
    return cont;
}

int preparacion(int cantImagenes,char *args[])
{
    memoria[0]=cantImagenes;
    memoria[1]=0;
    //int alcanzaMemoria=1;
    int memoriaDisponible=8190 - 16*cantImagenes;
    int cantImagenesLeidas=0;
    while(cantImagenesLeidas<memoria[0] && memoriaDisponible>=0)
    {
        //ps=0 , Cs=1 DS=2
        FILE *arch = fopen(args[cantImagenesLeidas+1], "rb");
        //rewind(arch);
        int32_t temp;
        fread(&temp, sizeof(temp), 1, arch);//temp=PS

        memoriaDisponible-=temp;//memoria disponible - PS

        if(memoriaDisponible>=0){
            memoria[2+cantImagenesLeidas*16]=temp;

            for(int i=1; i<16; i++) {
                fread(&temp, sizeof(temp), 1, arch);
                memoria[2+cantImagenesLeidas*16+i]=temp; //CS 1,,, FX 15
            }
            if(cantImagenesLeidas>0)
            {

                //CS                              =         ps anterior                 +       cs anterior
                memoria[2+16*cantImagenesLeidas+1]=memoria[2+16*(cantImagenesLeidas-1)]+memoria[2+16*(cantImagenesLeidas-1)+1];
            }
            else
            {   //CS este es el primer caso
                memoria[2+16*cantImagenesLeidas+1]=2+16*cantImagenes;
            }
            //DS
            memoria[2+16*cantImagenesLeidas+2]+=memoria[2+16*cantImagenesLeidas+1];
            if(memoria[2+16*cantImagenesLeidas+3]==-1)
            {       //ES                          =     ES anterior
                memoria[2+16*cantImagenesLeidas+3]=memoria[2+16*(cantImagenesLeidas-1)+3];
            }
            else
            {           //ES                    +=      CS
                memoria[2+16*cantImagenesLeidas+3]+=memoria[2+16*cantImagenesLeidas+1];
            }
            //SS                              +=        CS
            memoria[2+16*cantImagenesLeidas+5]+=memoria[2+16*cantImagenesLeidas+1];
        }
        fclose(arch);
        cantImagenesLeidas++;
        //memoria[1]++;
    }
    //se hace el calculo si no alcanza la memoria tirar mensaje y no ejecutar nadita nadal
    return (memoriaDisponible<0);
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
    char mnemonico[10], buffer1[10], buffer2[10];
    int offset;
    if(mostrar) {
        printf("\nCode Segment del proceso %d:\n\n",memoria[0]);
        for(int i=0; i<registros[2]; i+=3) {
            jump = 0;

            celdainst = memoria[registros[4]];
            param1 = memoria[registros[4]+1];
            param2 = memoria[registros[4]+2];
            tipo1 = (celdainst & maskarg1)>>shift1;
            tipo2 = (celdainst & maskarg2);


            getMnemonico(celdainst>>shiftinst, mnemonico);

            getBuffer(tipo1, tipo2, param1, param2, buffer1, buffer2);
            printf("%s %s , %s\n", mnemonico, buffer1, buffer2);
            registros[4]+=3;
        }
        registros[4] = 0;
        printf("\n-------------------------\n\n");
    }
    offset=registros[1];
    while(ejecutando) {

        jump = 0;
        celdainst = memoria[registros[4]+offset];
        param1 = memoria[registros[4]+offset+1];
        param2 = memoria[registros[4]+offset+2];
        tipo1 = (celdainst & maskarg1)>>shift1;
        tipo2 = (celdainst & maskarg2);

        funciones[celdainst>>shiftinst](tipo1, tipo2, param1, param2);
        printf("TENTRO MASMASMASMSA %d", offset);
        if(!jump)
            registros[4]+=3; //Si no hubo un salto, aumenta el IP
        ejecutando = (registros[4]+offset < registros[2])&&ejecutando; //Si IP < DS sigue ejecutando
    }
}

void getBuffer(int t1, int t2, int par1, int par2, char buffer1[10], char buffer2[10]) {
    char aux[10];
    int32_t reg;
    char registro[5];

    switch(t1) {
    case 0:
        itoa(par1, buffer1, 10);
        break;
    case 1:
        par1 &= 0x0FFFFFFF;
        getReg(par1, buffer1);
        break;
    case 2:
        reg = par1&0xF0000000;
        reg >>= 28;
        par1 &= 0x0FFFFFFF;
        strcpy(buffer1, "[");
        getReg(reg,registro);
        strcat(buffer1,registro);
        strcat(buffer1,":");
        itoa(par1, aux, 10);
        strcat(buffer1, aux);
        strcat(buffer1, "]");
        break;
    }

    switch(t2) {
    case 0:

        itoa(par2, buffer2, 10);
        break;
    case 1:
        par2 &= 0x0FFFFFFF;
        getReg(par2, buffer2);
        break;
    case 2:
        reg = par2&0xF0000000;
        reg >>= 28;
        par2 &= 0x0FFFFFFF;
        strcpy(buffer2, "[");
        getReg(reg,registro);
        strcat(buffer2,registro);
        strcat(buffer2,":");
        itoa(par2, aux, 10);
        strcat(buffer2, aux); //concatena
        strcat(buffer2, "]");
        break;
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
    funciones[68] = push;
    funciones[69] = pop;
    funciones[64] = call;
    funciones[72] = ret;
    funciones[80] = slen;
    funciones[81] = smov;
    funciones[83] = scmp;
    funciones[129] = sys;
    funciones[143] = stop;
}

void mov(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;
    int regBase, regIndireccion, offset;
    printf("\nTENTRO AL MOV");
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
    case 3:
        regBase = (par2 & 0xF0000000)>>28;
        regIndireccion = (par2 & 0xF);
        offset = int24Bits((par2 & 0x0FFFFFF0)>>4);
        b = memoria[registros[regBase]+registros[regIndireccion]+offset];
        break;
    }

    switch(t1) {
    case 1: //Registro
        registros[par1] = b;
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] = b;
        break;
    case 3:
        regBase = (par1 & 0xF0000000)>>28;
        regIndireccion = (par1 & 0xF);
        offset = int24Bits((par1 & 0x0FFFFFF0)>>4);
        memoria[registros[regBase]+registros[regIndireccion]+offset] = b;
        break;
    }
}
void add(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;
    int regBase, regIndireccion, offset;
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
    case 3:
        regBase = (par2 & 0xF0000000)>>28;
        regIndireccion = (par2 & 0xF);
        offset = int24Bits((par2 & 0x0FFFFFF0)>>4);
        b = memoria[registros[regBase]+registros[regIndireccion]+offset];
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
    case 3:
        regBase = (par1 & 0xF0000000)>>28;
        regIndireccion = (par1 & 0xF);
        offset = int24Bits((par1 & 0x0FFFFFF0)>>4);
        memoria[registros[regBase]+registros[regIndireccion]+offset] += b;
        res= memoria[registros[regBase]+registros[regIndireccion]+offset];
        break;
    }

    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);


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





}
void mod(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;
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
        break;
    case 2: //Directo
        division = memoria[registros[basea]+(par1 & ~mask)]/b;
        memoria[registros[basea]+(par1 & ~mask)] %= b;
        break;
    }

    registros[9] = 0;
    if(division==0)
        registros[9] += 1;
    else if(division<0)
        registros[9] |= (1<<31);



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


}
void not(int t1, int t2, int par1, int par2) {
    int mask = 0xF0000000;
    int shift = 28;
    int res, basea = (par1 & mask)>>shift;

    switch(t1) {
    case 1: //Registro
        registros[par1] = ~registros[par1];
        res = registros[par1];
        break;
    case 2: //Directo
        memoria[registros[basea]+(par1 & ~mask)] = ~memoria[registros[basea]+(par1 & ~mask)];
        res = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }
    registros[9] = 0;
    if(res==0)
        registros[9] += 1;
    else if(res<0)
        registros[9] |= (1<<31);

}
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

        //C esta mal hecho y me mete 1's al principio del numero cuando hace shr xd
        int tempMas = 0;
        for(int i=0; i<32-b; i++){
            tempMas <<= 1;
            tempMas += 1;
        }

        registros[par1] &= tempMas;

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
}
void jmp(int t1, int t2, int par1, int par2) {
    jump = 1;
    int mask = 0xF0000000;
    int shift = 28;
    int basea = (par1 & mask)>>shift;

    switch(t1) {
    case 0:
        registros[4] = par1;
        break;
    case 1: //Registro
        registros[4] = registros[par1];
        break;
    case 2: //Directo
        registros[4] = memoria[registros[basea]+(par1 & ~mask)];
        break;
    }
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
            break;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }
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
            break;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }
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
            break;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }
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
            break;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }
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
            break;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }
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
            break;
        case 1: //Registro
            registros[4] = registros[par1];
            break;
        case 2: //Directo
            registros[4] = memoria[registros[basea]+(par1 & ~mask)];
            break;
        }
    }
}
void push (int t1, int t2, int par1, int par2){

}
void pop (int t1, int t2, int par1, int par2){
    //aqui desarrolle el codigo pertinente
}
void call (int t1, int t2, int par1, int par2){
    //aqui desarrolle el codigo pertinente
}
void ret (int t1, int t2, int par1, int par2){
    //aqui desarrolle el codigo pertinente
}
void slen (int t1, int t2, int par1, int par2){
    int mask = 0xF0000000;// 0x0FFFFFFF
    int shift = 28;
    int b, basea = (par1 & mask)>>shift, baseb = (par2 & mask)>>shift;
//    int basec=par2 & 0x0000000F;//
//    int numero= (par2 & 0x0FFFFFF0)>>4;

    switch(t2) {//aca esta el string
    case 2://directo    [11] = [DS:11]  oo [ES:11] // [base][numero]=[4][28]
        b = memoria[registros[baseb]+(par2 & ~mask)];
        break;
    case 3://indirecto  //[DS:CX+23]=[DS][23][CX]=[4][24][4]
//        memoria[registros[par2 & ]]
        break;
    }

    switch(t1) {//en t1 vamos a guardar la longitud
    case 1: //Registro
        registros[par1] = b;
        break;
    case 2: //Directo [11] = [DS:11]  oo [ES:11]
        memoria[registros[basea]+(par1 & ~mask)] = b;
        break;
    case 3: //indirecto [AX]
        break;
    }
}
void smov (int t1, int t2, int par1, int par2){
    //aqui desarrolle el codigo pertinente
}
void scmp (int t1, int t2, int par1, int par2){
    //aqui desarrolle el codigo pertinente
}
void sys(int t1, int t2, int par1, int par2) {
    switch(par1) {
    case 0:
        //hace cosas
        break;
    case 1:
        leer();
        break;
    case 2:
        escribir();
        break;
    case 3:
        dump();
        break;
    case 10:
        //tambien hace cosas, pero otras cosas
        break;
    case 20:
        //igual que el 10 pero distinto
        break;
    default:
        printf("TODO MAL sr PELOTUDO");
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
            if(!prompt) {
                printf("[");
                mostrarCelda(desde+i);
                printf("]: ");
            }

            switch(modo) {
            case 1:
            case 0:
                scanf(" %d", &aux);
                memoria[registros[2]+desde+i] = aux;
                break;
            case 4:
                scanf(" %o", &aux);
                memoria[registros[2]+desde+i] = aux;
                break;
            case 8:
                scanf(" %x", &aux);
                memoria[registros[2]+desde+i] = aux;
                break;
            }
        }
    } else {
        if(!prompt) {
            printf("[");
            mostrarCelda(desde);
            printf("]: ");
        }

        scanf(" %s", buffer);
        int max = strlen(buffer)>(cant-1) ? (cant-1) : strlen(buffer);
        for(int i=0; i<max; i++) {
            memoria[registros[2]+desde + i] = buffer[i];
        }
        memoria[registros[2]+desde + max] = '\0';
    }

}
void escribir() {
    int prompt = registros[10]&0x1000;
    int endline = registros[10]&0x100;
    int car = registros[10]&0x0010;
    int modo = registros[10]&0xF; //1 = dec , 4 = octal , 8 = hexa

    int desde = registros[13];
    int cant = registros[12];

    if(!car) {
        for(int i=0; i<cant; i++) {
            if(!prompt) {
                printf("[");
                mostrarCelda(desde+i);
                printf("]: ");
            }

            if(modo & 0x8)
                printf("%c%X ", '%', memoria[registros[2]+desde+i]);
            if(modo & 0x4)
                printf("@%o ", memoria[registros[2]+desde+i]);
            if(modo & 0x1)
                printf("%d ", memoria[registros[2]+desde+i]);



            if(!endline)
                printf("\n");
            else printf(" ");
        }
    } else {

        for(int i=0; i<registros[12]; i++) {
            if(!prompt) {
                printf("[");
                mostrarCelda(desde+i);
                printf("]: ");
            }
            int letra = memoria[registros[2]+desde+i] & 0xFF;
            if(letra<=126 && letra>=33)
                printf("%c ",letra);
            else
                printf(". ");


            if(modo & 0x8)
                printf("%c%X ", '%', memoria[registros[2]+desde+i]);
            if(modo & 0x4)
                printf("@%o ", memoria[registros[2]+desde+i]);
            if(modo & 0x1)
                printf("%d ", memoria[registros[2]+desde+i]);

            if(!endline)
                printf("\n");
            else printf(" ");
        }
    }
}
void dump() {
    escribir();
    int modo=registros[10]&0xF;
    char String[5];
    switch(modo) {
    case 1:
        for(int i=0; i<16; i++) {
            getReg(i,String);
            if(String[0] != ' ') {
                printf("[%s]: %d\n",String,registros[i]);
            }
        }
        break;
    case 4:
        for(int i=0; i<16; i++) {
            getReg(i,String);
            if(String[0] != ' ') {
                printf("[%s]: %o\n",String,registros[i]);
            }
        }
        break;
    case 8:
        for(int i=0; i<16; i++) {
            getReg(i,String);
            if(String[0] != ' ') {
                printf("[%s]: %X\n",String,registros[i]);
            }
        }
        break;
    }
}



