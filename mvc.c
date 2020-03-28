#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct rotulo{
    char rot[50];
    int pos;
    struct rotulo* sig;
} rotulo;

typedef rotulo *listaRotulos;

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int);

int esRotulo(char*);

int contieneArg(int, char* [], char*);

void crearListaMnemonicos(char[][5]);

int main(int carg, char *args[]){
    printf("%d\n", carg);

    if(carg > 1){
        int mostrar;
        listaRotulos rotulos;
        FILE* arch;
        int memoria[2016];
        char listaMnemonicos[144][5];
        crearListaMnemonicos(listaMnemonicos);
        printf("%s\n", listaMnemonicos[1]);
        printf("%s\n", listaMnemonicos[0]);
        printf("%s\n", listaMnemonicos[143]);
        exit(0);
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

int esRotulo(char *string){
    if(!string) return 0;

    int len = strlen(string);
    if(string[len-1] == ':') return 1;
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

int esMnemonico(char listaMnemonicos[][5], char mnemonico[]){
    for(int i=0; i<144; i++){
        if(strcmp(listaMnemonicos[i], mnemonico))
    }
}

int esValido(char *linea){
    return *linea!='\n' && !(*linea=='/' && *(++linea)=='/');
}
