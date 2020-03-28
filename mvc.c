#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tdacazorla.h"

char listaMnemonicos[144][5];

int main(int carg, char *args[]){
    printf("%d\n", carg);

    if(carg > 1){
        int mostrar;
        listaRotulos rotulos;
        FILE* arch;
        crearListaMnemonicos(listaMnemonicos);

        arch = fopen(args[1], "rt");
        mostrar = !contieneArg(carg, args, "-o");
        rotulos = NULL;
        buscaRotulos(arch, &rotulos, mostrar);
        rewind(arch); //Vuelve el archivo al principio
    }
    return 0;
}

void crearListaMnemonicos(char lista[][5]){
    for(int i=0; i<144; i++){
        strcpy(lista[i], "");
    }

    strcpy(lista[1], "MOV");
    strcpy(lista[2], "ADD");
    strcpy(lista[3], "SUB");
    strcpy(lista[4], "MUL");
    strcpy(lista[5], "DIV");
    strcpy(lista[6], "MOD");
    strcpy(lista[19], "CMP");
    strcpy(lista[23], "SWAP");
    strcpy(lista[25], "RND");
    strcpy(lista[49], "AND");
    strcpy(lista[50], "OR");
    strcpy(lista[51], "NOT");
    strcpy(lista[52], "XOR");
    strcpy(lista[55], "SHL");
    strcpy(lista[56], "SHR");
    strcpy(lista[32], "JMP");
    strcpy(lista[33], "JE");
    strcpy(lista[34], "JG");
    strcpy(lista[35], "JL");
    strcpy(lista[36], "JZ");
    strcpy(lista[37], "JP");
    strcpy(lista[38], "JN");
    strcpy(lista[39], "JNZ");
    strcpy(lista[40], "JNP");
    strcpy(lista[41], "JNN");
    strcpy(lista[129], "SYS");
    strcpy(lista[143], "STOP");
}

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int mostrar){
    if(mostrar) printf("\nLista de Rotulos:\n-----------------\n");

    int linea = 0;
    char nextLinea[200];
    char next[50];
    rotulo* ult = NULL;

    while(!feof(arch)){
        fgets(nextLinea, sizeof(nextLinea), arch);
        if(esValido(nextLinea)){
            sscanf(nextLinea, "%s", next);
            if(esRotulo(next)){
                rotulo *nextRotulo = (rotulo*)malloc(sizeof(rotulo));
                nextRotulo->sig = NULL;
                next[strlen(next)-1] = '\0';
                char *s = next;
                while(*s){
                    *s = toupper((unsigned char) *s);
                    s++;
                }
                strcpy(nextRotulo->rot, next);
                nextRotulo->pos = linea;

                if(*rotulos == NULL){
                    *rotulos = nextRotulo;
                }else{
                    ult->sig = nextRotulo;
                }
                ult = nextRotulo;

                if(mostrar) printf("%d: %s\n", linea, next);
            }
            linea++;
        }
    }
}
