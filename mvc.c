#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct rotulo{
    char rot[50];
    int pos;
    struct rotulo* sig;
} rotulo;

typedef rotulo *listaRotulos;

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int);

int esRotulo(char*);

int contieneArg(int, char* [], char*);

int main(int carg, char *args[]){
    printf("%d\n", carg);

    if(carg > 1){
        FILE* arch = fopen(args[1], "rt");
        int mostrar = !contieneArg(carg, args, "-o");
        listaRotulos rotulos = NULL;
        buscaRotulos(arch, &rotulos, mostrar);

    }
    return 0;
}

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int mostrar){
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
            strcpy(nextRotulo->rot, next);
            nextRotulo->pos = linea;

            if(*rotulos == NULL){
                *rotulos = nextRotulo;
            }else{
                ult->sig = nextRotulo;
            }
            ult = nextRotulo;

            if(mostrar) printf("%d, %s\n", linea, next);
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

int contieneArg(int argc, char* args[], char* busca){
    for(int i=0; i<argc; i++){
        if(!strcmp(args[i], busca)){
            return 1;
        }
    }
    return 0;
}
