#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "tdacazorla.h"

int32_t memoria[2000];
int32_t registros[16] = {0};
listaRotulos rotulos;

int traduce(FILE *arch, int muestra);

void generaImg(FILE*);

int main(int carg, char *args[]) {
    printf("%d\n", carg);

    if (carg > 1) {
        int mostrar,traducir;
        FILE *arch, *archImg;
        crearListaMnemonicos();
        arch = fopen(args[1], "rt");

        mostrar = !contieneArg(carg, args, "-o");
        rotulos = NULL;
        buscaRotulos(arch, &rotulos, mostrar);
        traducir=traduce(arch, mostrar);
        if (!traducir) {
            archImg = fopen(args[2], "wb");
            generaImg(archImg);
            fclose(archImg);
        }
        fclose(arch);
    }
    return 0;
}

int traduce(FILE *arch, int muestra) {
    rewind(arch);
    int linea = 0;
    int pos;
    char nextLinea[200];
    char *palabra;
    int arg;
    int errorsin = 0;
    int errorrot = 0;
    char lineaSinFiltrar[200];
    int mostrarLinea;
    int rot;

    while (!feof(arch)) {
        mostrarLinea = 1;
        arg = 0;
        strcpy(nextLinea, " ");
        fgets(nextLinea, sizeof(nextLinea), arch);
        strcpy(lineaSinFiltrar, nextLinea);
        strcpy(nextLinea, strtok(nextLinea, "/"));

        if (esValido(lineaSinFiltrar)) {
            palabra = strtok(nextLinea, " ,\n");
            while (palabra != NULL) {
                if (!esRotulo(palabra)) {
                    char *s = palabra;
                    while (*s) {
                        *s = toupper((unsigned char)*s);
                        s++;
                    }
                    pos = esMnemonico(palabra);
                    if (pos != -1) {
                        memoria[linea * 3] = pos << 16;
                    } else {
                        if (*palabra == '[') {
                            memoria[linea * 3] |= (2 << ((2 - arg) * 8));
                            char aux[50];
                            strcpy(aux, palabra);
                            char *p = aux;
                            aux[strlen(aux) - 1] = '\0';

                            if (palabra[1] >= '0' && palabra[1] <= '9') {
                                p++;
                                memoria[linea * 3 + arg] = atoi(p);
                                memoria[linea * 3 + arg] |= (2 << 28);
                            } else { //es un registro
                                if (palabra[1] == 'D') {
                                    p += 4;
                                    memoria[linea * 3 + arg] = atoi(p);
                                    memoria[linea * 3 + arg] |= (2 << 28);
                                } else {
                                    p += 4;
                                    memoria[linea * 3 + arg] = atoi(p);
                                    memoria[linea * 3 + arg] |= (3 << 28);
                                }
                            }
                        } else if (*palabra == '\'' ||
                                   *palabra == '%' ||
                                   *palabra == '@' ||
                                   *palabra == '#' ||
                                   ('0' <= *palabra && '9' >= *palabra)) {
                            char aux[1000];
                            strcpy(aux, palabra);
                            char *p = aux;

                            switch (*palabra) {
                            case '\'':
                                p++;
                                memoria[linea * 3 + arg] = aux[1];
                                break;
                            case '%':
                                p++;
                                memoria[linea * 3 + arg] = strtol(p, NULL, 16);
                                break;
                            case '@':
                                p++;
                                memoria[linea * 3 + arg] = strtol(p, NULL, 8);
                                break;
                            case '#':
                                p++;
                            default:
                                memoria[linea * 3 + arg] = atoi(p);
                                break;
                            }
                        } else if (strlen(palabra) == 2) {
                            memoria[linea * 3] |= (1 << ((2 - arg) * 8));
                            if(!strcmp(palabra, "AC")) {
                                memoria[linea * 3 + arg] = 8;
                            } else {
                                memoria[linea * 3 + arg] = strtol(&palabra[0], NULL, 16);
                            }
                        } else if ((rot = buscarRotulo(rotulos, palabra)) != -1) {
                            memoria[linea * 3 + arg] = rot*3;
                        } else {
                            if (arg == 0) {
                                errorsin = 1;
                                for (int i = 0; i < 3; i++) {
                                    memoria[linea * 3 + i] = 0xFFFFFFFF;
                                }
                            } else {
                                errorrot = 1;
                                memoria[linea * 3 + arg] = 0xFFFFFFFF;
                            }
                        }
                    }
                    arg++;
                } else
                    mostrarLinea = 0;
                palabra = strtok(NULL, " ,\n");
            }

            if (muestra) {
                printf("[");
                mostrarCelda(linea * 3);
                printf("]: ");
                for (int i = 0; i < 3; i++) {
                    mostrarCelda(memoria[linea * 3 + i]);
                    printf(" ");
                }
                if (mostrarLinea) {
                    printf(" %4d:", linea);
                }
                printf(" %s", lineaSinFiltrar);
            }
            linea++;
        }else{
            if(muestra){
                printf("%47s%s", "", lineaSinFiltrar);
            }
        }
    }

    if (errorsin) {
        printf("\nError de sintaxis.");
    }
    if (errorrot) {
        printf("\nError: no se encontro el rotulo.");
    }
    registros[2] = linea*3;
    registros[3] = 1000;
    return (errorsin || errorrot);
}

void buscaRotulos(FILE *arch, listaRotulos *rotulos, int mostrar) {
    if (mostrar)
        printf("\nLista de Rotulos:\n-----------------\n");

    int linea = 0;
    char nextLinea[200];
    char next[50];
    rotulo *ult = NULL;

    while (!feof(arch)) {
        fgets(nextLinea, sizeof(nextLinea), arch);
        if (esValido(nextLinea)) {
            sscanf(nextLinea, "%s", next);
            if (esRotulo(next)) {
                rotulo *nextRotulo = (rotulo *)malloc(sizeof(rotulo));
                nextRotulo->sig = NULL;
                next[strlen(next) - 1] = '\0';
                char *s = next;
                while (*s) {
                    *s = toupper((unsigned char)*s);
                    s++;
                }
                strcpy(nextRotulo->rot, next);
                nextRotulo->pos = linea;

                if (*rotulos == NULL) {
                    *rotulos = nextRotulo;
                } else {
                    ult->sig = nextRotulo;
                }
                ult = nextRotulo;

                if (mostrar)
                    printf("%d: %s\n", linea, next);
            }
            linea++;
        }
    }
}

void generaImg(FILE* archImg) {
    for(int i=0; i<16; i++) {
        fwrite(&registros[i], sizeof(registros[i]), 1, archImg);
    }
    for(int i=0; i<2000; i++) {
        fwrite(&memoria[i], sizeof(memoria[i]), 1, archImg);
    }

    /*
    char string[50];
    char num[50];
    for (int i=0;i<16;i++){
        itoa(registros[i],string,2);
        for(int j=0;j<32-strlen(string);j++){
            num[j] = '0';
        }
        num[32-strlen(string)]='\0';
        strcat(num, string);
        fprintf(archImg, "%s\n", num);
    }
    for(int i=0;i<2000;i++){
        itoa(memoria[i], string, 2);
        for(int j=0;j<32-strlen(string);j++){
            num[j] = '0';
        }
        num[32-strlen(string)]='\0';
        strcat(num, string);
        fprintf(archImg, "%s\n", num);
    }
    */
}
