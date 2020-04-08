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
    //printf("♥%d\n", carg);

    if (carg > 1) {//si al menos tiene 2  argumentos traduce
        int mostrar,traducir;
        FILE *arch, *archImg;
        crearListaMnemonicos();//matriz de caracteres ooo vector de strings
        arch = fopen(args[1], "rt");

        mostrar = !contieneArg(carg, args, "-o");//boolean mostrar es verdade si exite el argumentos "-o"
        rotulos = NULL;
        buscaRotulos(arch, &rotulos, mostrar);
        traducir=traduce(arch, mostrar);//traducir es verdadero si hay algun error
        if (!traducir) {//si no hay errores
            archImg = fopen(args[2], "wb");
            generaImg(archImg);//se genera el archivo binario
            fclose(archImg);
        }
        fclose(arch);
    }
    return 0;
}

int traduce(FILE *arch, int muestra) {//ya tenemos la lista de rotulos creada
    rewind(arch);
    int linea = 0;                  //linea actual en el programa assembler
    int pos;                        //se usa para la posicion del mnemonico -1 si no existe
    char nextLinea[200];            //cada linea sin comentarios de assembler
    char *palabra;                  //cada palabra
    int arg;                        //argumento actual (0,1,2)
    int errorsin = 0;               //boolean
    int errorrot = 0;               //boolean
    char lineaSinFiltrar[200];      //cada linea completa de assembler
    int mostrarLinea;               //boolean para no mostrar el numero de linea de los rotulos con dos puntos
    int rot;                        //
    // INICIO A LEER ARCHIVO----------------------------------------------------------------------
    while (!feof(arch)) {
        mostrarLinea = 1;
        arg = 0;
        strcpy(nextLinea, " ");
        fgets(nextLinea, sizeof(nextLinea), arch);      //nextLinea="toda una linea"
        strcpy(lineaSinFiltrar, nextLinea);             //lineaSinFiltrar=nextLinea
        strcpy(nextLinea, strtok(nextLinea, "/"));      //nextLinea="quita comentarios"

        if (esValido(lineaSinFiltrar)) {                //
            palabra = strtok(nextLinea, " ,\n");        //palabra= "la primer palabra" de nextLinea
            // EMPIEZA A DIFERENCIAR PALABRAS-------------------------------------------------------------------------
            while (palabra != NULL) {
                // SI LA PALABRA NO ES LA DE DEFINICION DE UN ROTULO----------------------------------------------------------
                if (!esRotulo(palabra)) {               //
                    char *s = palabra;
                    while (*s) {
                        *s = toupper((unsigned char)*s);        //palabra pasa "a mayuscula"
                        s++;
                    }
                    pos = esMnemonico(palabra);                 //pos = codigo mnemonico ó -1
                    // SI LA PALABRA ES MNEMONICO----------------------------------------------------------------------
                    if (pos != -1) {
                        memoria[linea * 3] = pos << 16;         //corre el codigo del Mnemonico y lo guarda en memoria
                    } else {
                        // SI LA PALABRA ES OPERANDO DIRECTO---------------------------------------------------------------------
                        if (*palabra == '[') {
                            memoria[linea * 3] |= (2 << ((2 - arg) * 8));       //agrega a la memoria que el operando 1 o 2 es un operando directo
                            char aux[50];
                            strcpy(aux, palabra);
                            char *p = aux;
                            aux[strlen(aux) - 1] = '\0';                        //a aux le quito el ultimo caracter se espera que sea ']'

                            if (palabra[1] >= '0' && palabra[1] <= '9') {//SI PALABRA ES [UnNumero
                                p++;                                            //si palabra es[numero]entonces *p=numero
                                memoria[linea * 3 + arg] = atoi(p);
                                memoria[linea * 3 + arg] |= (2 << 28);          //codigo de registro base... si es numero se presupone[DS:numero]
                            } else {
                                if (palabra[1] == 'D') {                 //SI PALABRA ES [D tambien se asume que es [DS:numero]
                                    p += 4;
                                    memoria[linea * 3 + arg] = atoi(p);
                                    memoria[linea * 3 + arg] |= (2 << 28);      //codigo de registro base DS
                                } else {                                 //SI NO ES [D se asume que es [ES:numero]
                                    p += 4;
                                    memoria[linea * 3 + arg] = atoi(p);
                                    memoria[linea * 3 + arg] |= (3 << 28);      //codigo de registro base ES
                                }
                            }
                        } // SI LA PALABRA ES OPERANDO INMEDIATO----------------------------------------------------------------------
                           else if (*palabra == '\'' ||
                                   *palabra == '%' ||
                                   *palabra == '@' ||
                                   *palabra == '#' ||
                                   ('0' <= *palabra && '9' >= *palabra)) {
                            char aux[1000];                                     //en la memoria ya hay un cero asi que no necesita aclararlo
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
                        } // SI LA PALABRA ES OPERANDO DE REGISTRO----------------------------------------------------------------------
                            else if (strlen(palabra) == 2) {
                            memoria[linea * 3] |= (1 << ((2 - arg) * 8));           //deja en memoria la informacion de que el operando 1 o 2 es un operando de registro
                            if(!strcmp(palabra, "AC")) {
                                memoria[linea * 3 + arg] = 8;
                            } else {
                                memoria[linea * 3 + arg] = strtol(&palabra[0], NULL, 16);
                            }
                        } // SI LA PALABRA ES LA INVOCACION A UN ROTULO----------------------------------------------------------------------
                          else if ((rot = buscarRotulo(rotulos, palabra)) != -1) {
                            memoria[linea * 3 + arg] = rot*3;
                        } else {
                            // SI LA PALABRA NO ES NINGUNA DE LAS ANTERIOR SE INTERPRETA COMO ERROR-----------------------------------------------------------                                                   //para este else palabra!=Mnemonico
                            if (arg == 0) {                                         //arg==0 error de Mnemonico
                                errorsin = 1;
                                for (int i = 0; i < 3; i++) {
                                    memoria[linea * 3 + i] = 0xFFFFFFFF;
                                }
                            } else {
                                errorrot = 1;                                       //error de rotulo
                                memoria[linea * 3 + arg] = 0xFFFFFFFF;
                            }
                        }
                    }//
                    arg++;
                } else// SI LA PALABRA ES LA DEFINICION DE UN ROTULO, osea rotulo:---------------------------------------------------------------
                    mostrarLinea = 0;                                               //los rotulos dos puntos no muestran el numero de linea

                palabra = strtok(NULL, " ,\n");                                     //avanzo a la siguiente palabra que exista en NextLinea
            }
            // FIN LINEA ----------------------------------------------------------------------

            if (muestra) {              //[00000003]:
                printf("[");
                mostrarCelda(linea * 3);
                printf("]: ");
                for (int i = 0; i < 3; i++) {
                    mostrarCelda(memoria[linea * 3 + i]);
                    printf(" ");
                }                       //0001 0100 0000 000A 0000 0001
                if (mostrarLinea) {
                    printf(" %4d:", linea); //     1:
                }
                printf(" %s", lineaSinFiltrar);     // MOV AX, %0001
            }
            linea++;
        }else{//SI LINEASINFILTRAR NO ES VALIDA
            if(muestra){
                printf("%47s%s", "", lineaSinFiltrar);
            }
        }
    }
    // FIN ARCHIVO----------------------------------------------------------------------
    if (errorsin) {
        printf("\nError de sintaxis.");
    }
    if (errorrot) {
        printf("\nError: no se encontro el rotulo.");
    }
    registros[2] = linea*3;     //registro DS=posicion de memoria donde termina el programa
    registros[3] = 1000;        //registro ES=1000

    return (errorsin || errorrot);
}


void buscaRotulos(FILE *arch, listaRotulos *rotulos, int mostrar) {
//busca rotulos en arch y los almacena en listaRotulos y si hay que mostrar los muestra
    if (mostrar)
        printf("\nLista de Rotulos:\n-----------------\n");

    int linea = 0;                              //Linea actual del código assembler
    char nextLinea[200];
    char next[50];
    rotulo *ult = NULL;

    while (!feof(arch)) {
        fgets(nextLinea, sizeof(nextLinea), arch);      //nextLinea cada linea completa del assembler
        if (esValido(nextLinea)) {                      //si es nemonico o rotulo y nada raro
            sscanf(nextLinea, "%s", next);              //next es la primer palabra de nextLinea
            if (esRotulo(next)) {                       //si tiene ':' y no es comentario
                rotulo *nextRotulo = (rotulo *)malloc(sizeof(rotulo));
                nextRotulo->sig = NULL;
                next[strlen(next) - 1] = '\0';
                char *s = next;                         //
                while (*s) {
                    *s = toupper((unsigned char)*s);    //pasa next a mayuscula
                    s++;
                }                                       //
                strcpy(nextRotulo->rot, next);
                nextRotulo->pos = linea;                //la posicion del rotulo es la linea actual

                if (*rotulos == NULL) {                 //crea la lista de rotulos
                    *rotulos = nextRotulo;
                } else {
                    ult->sig = nextRotulo;
                }
                ult = nextRotulo;

                if (mostrar)                            //muestra el rotulo si es necesario
                    printf("%d: %s\n", linea, next);
            }
            linea++;
        }
    }
}

void generaImg(FILE* archImg) {             //genera el archivo binario en base a los registros y a la memoria
    for(int i=0; i<16; i++) {
        fwrite(&registros[i], sizeof(registros[i]), 1, archImg);
    }
    for(int i=0; i<2000; i++) {
        fwrite(&memoria[i], sizeof(memoria[i]), 1, archImg);
    }
}
