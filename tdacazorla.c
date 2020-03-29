#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tdacazorla.h"

char listaMnemonicos[144][5];

int esRotulo(char *string){
    if(!string) return 0;

    int len = strlen(string);
    if(string[0]  != '/' && string[len-1] == ':') return 1;
    return 0;
}

int buscarRotulo(listaRotulos rotulos, char* rot){
    listaRotulos aux = rotulos;
    while(aux != NULL){
        if(!strcmp(aux->rot, rot)){
            return aux->pos;
        }
        aux = aux->sig;
    }
    printf("No se encontro el rotulo %s", rot);
    exit(1);
}

int contieneArg(int argc, char* args[], char* busca){
    for(int i=0; i<argc; i++){
        if(!strcmp(args[i], busca)){
            return 1;
        }
    }
    return 0;
}

int esMnemonico(char mnemonico[]){
    for(int i=0; i<144; i++){
        if(!strcmp(listaMnemonicos[i], mnemonico)) return i;
    }
    return -1;
}

int esValido(char *linea){
    char string[50];
    sscanf(linea, "%s", string);
    return esRotulo(string) || esMnemonico(string);
}
