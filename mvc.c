#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <ctype.h>
#include "tdacazorla.h"

int32_t memoria[2000];
int32_t registros[16]= {0};
listaRotulos rotulos;

void traduce(FILE* arch, int muestra);

void mostrarCelda(int dato);

int main(int carg, char *args[])
{
    printf("%d\n", carg);

    if(carg > 1)
    {
        int mostrar;
        FILE* arch;
        crearListaMnemonicos();

        arch = fopen(args[1], "rt");
        mostrar = !contieneArg(carg, args, "-o");
        rotulos = NULL;
        buscaRotulos(arch, &rotulos, mostrar);
        traduce(arch,mostrar);

    }
    return 0;
}

void traduce(FILE* arch, int muestra)
{
    rewind(arch);
    int linea=0;
    int pos;
    char nextLinea[200];
    char *palabra;
    int arg;
    int errorsin = 0;
    int errorrot = 0;
    char lineaSinFiltrar[200];
    int mostrarLinea;

    while(!feof(arch))
    {
        mostrarLinea = 1;
        arg = 0;
        fgets(nextLinea, sizeof(nextLinea), arch);
        strcpy(lineaSinFiltrar, nextLinea);
        strcpy(nextLinea,strtok(nextLinea,"/"));

        if(esValido(nextLinea))
        {
            palabra=strtok(nextLinea," ,\n");
            while(palabra!=NULL)
            {
                if(!esRotulo(palabra))
                {
                    char *s = palabra;
                    while(*s)
                    {
                        *s = toupper((unsigned char) *s);
                        s++;
                    }
                    pos = esMnemonico(palabra);
                    if(pos!=-1)
                    {
                        memoria[linea*3]=pos<<16;
                    }
                    else
                    {
                        if(*palabra=='[')
                        {
                            //Es directo
                        }
                        else if(*palabra == '\''||
                                *palabra == '%' ||
                                *palabra == '@' ||
                                *palabra == '#' ||
                                ('0'<=*palabra && '9'>=*palabra))
                        {
                            //Es inmediato
                        }
                        else if(strlen(palabra)==2)
                        {
                            //Es registro
                        }
                        else if(buscarRotulo(rotulos, palabra)!=-1)
                        {
                            //Es rotulo
                        }
                        else
                        {
                            if(arg == 0)
                            {
                                errorsin = 1;
                                for(int i=0; i<3; i++)
                                {
                                    memoria[linea*3 + i] = 0xFFFFFFFF;
                                }
                            }
                            else
                            {
                                errorrot = 1;
                                memoria[linea*3 + arg] = 0xFFFFFFFF;
                            }
                        }
                    }
                    arg++;
                }
                else
                    mostrarLinea = 0;
                palabra=strtok(NULL," ,\n");
            }

            if(muestra)
            {
                printf("[");
                mostrarCelda(linea*3);
                printf("]: ");
                for(int i=0; i<3; i++)
                {
                    mostrarCelda(memoria[linea*3+i]);
                    printf(" ");
                }
                if(mostrarLinea)
                {
                    printf(" %d : ", linea);
                }
                printf(" %s", lineaSinFiltrar);
            }
            linea++;
        }
    }

    if(muestra && errorsin)
    {
        printf("\nError de sintaxis.");
    }
    if(muestra && errorrot)
    {
        printf("\nError: no se encontro el rotulo.");
    }
}

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int mostrar)
{
    if(mostrar)
        printf("\nLista de Rotulos:\n-----------------\n");

    int linea = 0;
    char nextLinea[200];
    char next[50];
    rotulo* ult = NULL;

    while(!feof(arch))
    {
        fgets(nextLinea, sizeof(nextLinea), arch);
        if(esValido(nextLinea))
        {
            sscanf(nextLinea, "%s", next);
            if(esRotulo(next))
            {
                rotulo *nextRotulo = (rotulo*)malloc(sizeof(rotulo));
                nextRotulo->sig = NULL;
                next[strlen(next)-1] = '\0';
                char *s = next;
                while(*s)
                {
                    *s = toupper((unsigned char) *s);
                    s++;
                }
                strcpy(nextRotulo->rot, next);
                nextRotulo->pos = linea;

                if(*rotulos == NULL)
                {
                    *rotulos = nextRotulo;
                }
                else
                {
                    ult->sig = nextRotulo;
                }
                ult = nextRotulo;

                if(mostrar)
                    printf("%d: %s\n", linea, next);
            }
            linea++;
        }
    }
}

void mostrarCelda(int dato)
{
    int a=dato;
    int b=0;
    b=a>>16;
    b=b&0x0000FFFF;
    a=a&0x0000FFFF;
    printf("%04X %04X",b,a);
}
