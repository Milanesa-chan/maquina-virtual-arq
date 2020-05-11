#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "tdacazorla.h"

int32_t memoria[8192];
int32_t registros[16] = {0};
int32_t segmentos[3] = {500, 500, 500}; //0 = DATA, 1 = EXTRA, 2 = STACK//por defecto en 500
int errorsin = 0;               //boolean
int errorrot = 0;               //boolean
int errorconst = 0;
listaRotulos rotulos;
listaConst constantes;


int traduce(FILE *arch, int muestra);

void generaImg(FILE*);

int main(int carg, char *args[])
{
    //printf("♥%d\n", carg);

    if (carg > 1)  //si al menos tiene 2  argumentos traduce
    {
        int mostrar,traducir;
        FILE *arch, *archImg;
        crearListaMnemonicos();     //CREA MNEMONICOS-------------------------
        crearRegistros();           //CREA REGISTROS-------------------------
        arch = fopen(args[1], "rt");

        mostrar = !contieneArg(carg, args, "-o");//boolean mostrar es verdade si exite el argumentos "-o"
        rotulos = NULL;
        constantes = NULL;
        buscaRotulos(arch, &rotulos, mostrar);  //CREA ROTULOS, CONSTANTES Y COMANDO \\ASM-------------------------
        constantesAMemoria();                   //GUARDA EN MEMORIA LAS CONSTANTES---------------------------------
        errorconst = !verificarConstantesYRotulos(constantes,rotulos);// 1 si son todos diferentes, 0 si hay alguno igual
        traducir=traduce(arch, mostrar);//traducir es verdadero si hay algun error
        /*while (constantes!=NULL)
        {
            printf("\n%s %s %d directo %d\n",constantes->nombre,constantes->dato,constantes->valor,constantes->esDirecto);
            constantes=constantes->sig;
        }*/
        if (!traducir)  //si no hay errores
        {
            archImg = fopen(args[2], "wb");
            generaImg(archImg);//se genera el archivo binario
            fclose(archImg);
        }
        fclose(arch);
    }
    return 0;
}

int traduce(FILE *arch, int muestra)  //ya tenemos la lista de rotulos creada
{
    rewind(arch);
    int linea = 0;                  //linea actual en el programa assembler
    int pos;                        //se usa para la posicion del mnemonico -1 si no existe
    char nextLinea[200];            //cada linea sin comentarios de assembler
    char *palabra;                  //cada palabra
    int arg;                        //argumento actual (0,1,2)

    char lineaSinFiltrar[200];      //cada linea completa de assembler
    int mostrarLinea;               //boolean para no mostrar el numero de linea de los rotulos con dos puntos
    int rot;
    listaConst nodoconst;                   //
    // INICIO A LEER ARCHIVO----------------------------------------------------------------------
    while (!feof(arch))
    {
        mostrarLinea = 1;
        arg = 0;
        strcpy(nextLinea, " ");
        fgets(nextLinea, sizeof(nextLinea), arch);      //nextLinea="toda una linea"
        strcpy(lineaSinFiltrar, nextLinea);             //lineaSinFiltrar=nextLinea
        strcpy(nextLinea, strtok(nextLinea, "/"));      //nextLinea="quita comentarios"

        if (esValido(lineaSinFiltrar))                  //
        {
            palabra = strtok(nextLinea, " ,\n\t");        //palabra= "la primer palabra" de nextLinea
            // EMPIEZA A DIFERENCIAR PALABRAS-------------------------------------------------------------------------
            while (palabra != NULL)
            {
                // SI LA PALABRA NO ES LA DE DEFINICION DE UN ROTULO----------------------------------------------------------
                if (!esRotulo(palabra))                 //
                {
                    char *s = palabra;
                    while (*s)
                    {
                        *s = toupper((unsigned char)*s);        //palabra pasa "a mayuscula"
                        s++;
                    }
                    pos = esMnemonico(palabra);                 //pos = codigo mnemonico ó -1
                    // SI LA PALABRA ES MNEMONICO----------------------------------------------------------------------
                    if (pos != -1)
                    {
                        memoria[linea * 3] = pos << 16;         //corre el codigo del Mnemonico y lo guarda en memoria
                    }
                    else
                    {
                        // SI LA PALABRA ES OPERANDO DIRECTO O INDIRECTO---------------------------------------------------------------------
                        if (*palabra == '[') //
                        {   //palabra = [DX:AX]
                            palabra[strlen(palabra)-1] = '\0';
                            strcpy(palabra, palabra+1);             //quita corchetes
                            char *dospuntos;
                            char *regTemp = (char*) malloc(50);
                            if((dospuntos = strchr(palabra, ':')) != NULL){     //dospuntos= despues de dos puntos
                                //palabra = DX:AX       //dospuntos=
                                strcpy(regTemp, palabra);
                                //regTemp = DX:AX
                                *(regTemp+2) = '\0';
                                //regTemp = DX
                                            //12,          , 2 ,dx
                                registroBase(linea, memoria, arg, regTemp);

                                strcpy(regTemp, dospuntos+1);
                                if(regTemp[2] == '+' || regTemp[2] == '-')
                                    regTemp[2] = '\0';

                                if(getValReg(regTemp) != -1){
                                    argumentoIndirecto(linea, memoria, constantes, arg, dospuntos+1);
                                }else{
                                    argumentoDirecto(linea, memoria, constantes, arg, dospuntos+1);
                                }
                            }else{
                                strcpy(regTemp, palabra);
                                if(regTemp[2] == '+' || regTemp[2] == '-')
                                    regTemp[2] = '\0';

                                determinarBase(linea, memoria, arg, regTemp);

                                if(getValReg(regTemp) != -1){
                                    argumentoIndirecto(linea, memoria, constantes, arg, palabra);
                                }else{
                                    argumentoDirecto(linea, memoria, constantes, arg, palabra);
                                }
                            }

                            /*
                            memoria[linea * 3] |= (2 << ((2 - arg) * 8));       //agrega a la memoria que el operando 1 o 2 es un operando directo
                            char aux[50];
                            strcpy(aux, palabra);
                            char *p = aux;
                            aux[strlen(aux) - 1] = '\0';                        //a aux le quito el ultimo caracter se espera que sea ']'

                            if (palabra[1] >= '0' && palabra[1] <= '9')  //SI PALABRA ES [UnNumero
                            {
                                p++;                                            //si palabra es[numero]entonces *p=numero
                                memoria[linea * 3 + arg] = atoi(p);
                                memoria[linea * 3 + arg] |= (2 << 28);          //codigo de registro base...
                            }
                            else
                            {
                                if (palabra[1] == 'D' && palabra[1] == 'S')                   //SI PALABRA ES [D tambien se asume que es [DS:numero]
                                {
                                    p += 4;
                                    if (atoi(p)!=0){
                                        memoria[linea * 3 + arg] = atoi(p);
                                        memoria[linea * 3 + arg] |= (2 << 28);
                                    }

                                }
                                else                                     //SI NO ES [D se asume que es [ES:numero]
                                {
                                    p += 4;
                                    memoria[linea * 3 + arg] = atoi(p);
                                    memoria[linea * 3 + arg] |= (3 << 28);      //codigo de registro base ES
                                }
                            }
                        */
                        } // SI LA PALABRA ES OPERANDO INMEDIATO----------------------------------------------------------------------
                        else if (*palabra == '\'' ||
                                 *palabra == '%' ||
                                 *palabra == '@' ||
                                 *palabra == '#' ||
                                 ('0' <= *palabra && '9' >= *palabra))
                        {
                            char aux[1000];                                     //en la memoria ya hay un cero asi que no necesita aclararlo
                            strcpy(aux, palabra);
                            char *p = aux;

                            switch (*palabra)
                            {
                            case '\'':
                                p++;
                                memoria[linea * 3 + arg] = aux[1];
                                break;
                            case '%':
                                p++;
                                memoria[linea * 3 + arg] = strtoul(p, NULL, 16);
                                break;
                            case '@':
                                p++;
                                memoria[linea * 3 + arg] = strtoul(p, NULL, 8);
                                break;
                            case '#':
                                p++;
                            default:
                                memoria[linea * 3 + arg] = atoi(p);
                                break;
                            }
                        } // SI LA PALABRA ES OPERANDO DE REGISTRO----------------------------------------------------------------------
                        else if (strlen(palabra) == 2)
                        {
                            memoria[linea * 3] |= (1 << ((2 - arg) * 8));           //deja en memoria la informacion de que el operando 1 o 2 es un operando de registro
                            if(!strcmp(palabra, "AC"))
                            {
                                memoria[linea * 3 + arg] = 8;
                            }
                            else if(!strcmp(palabra, "CC"))
                            {
                                memoria[linea * 3 + arg] = 9;
                            }
                            else
                            {
                                memoria[linea * 3 + arg] = strtol(&palabra[0], NULL, 16);
                            }
                        } // SI LA PALABRA ES LA INVOCACION A UN ROTULO----------------------------------------------------------------------
                        else if ((rot = buscarRotulo(rotulos, palabra)) != -1)
                        {
                            memoria[linea * 3 + arg] = rot*3;
                        }
                        else if (arg!=0 && (nodoconst = buscarConstante(constantes,palabra))!=NULL)
                        {
                            memoria[linea*3+arg] = nodoconst->valor;
                            if (nodoconst->esDirecto)
                            {
                                memoria[linea * 3] |= (2 << ((2 - arg) * 8));
                                memoria[linea*3+arg] |= (1 << 28);
                            }
                        }
                        /*(nodoConst = buscarconstante) != NULL*/
                        //se toman dos caminos de acuerdo al tipo de nodo
                        //si es inmediato se pone el valor directamente en la celda de memoria
                        //si es directo se pone el valor como una direccion de memoria basada en cs

                        else
                        //
                        {
                            // SI LA PALABRA NO ES NINGUNA DE LAS ANTERIOR SE INTERPRETA COMO ERROR-----------------------------------------------------------                                                   //para este else palabra!=Mnemonico
                            if (arg == 0)                                           //arg==0 error de Mnemonico
                            {
                                errorsin = 1;
                                for (int i = 0; i < 3; i++)
                                {
                                    memoria[linea * 3 + i] = 0xFFFFFFFF;
                                }
                            }
                            else
                            {
                                errorrot = 1;                                       //error de rotulo
                                memoria[linea * 3 + arg] = 0xFFFFFFFF;
                            }
                        }
                    }//
                    arg++;
                }
                else  // SI LA PALABRA ES LA DEFINICION DE UN ROTULO, osea rotulo:---------------------------------------------------------------
                    mostrarLinea = 0;                                               //los rotulos dos puntos no muestran el numero de linea

                palabra = strtok(NULL, " ,\n");                                     //avanzo a la siguiente palabra que exista en NextLinea
            }
            // FIN LINEA ----------------------------------------------------------------------

            if (muestra)                //[00000003]:
            {
                printf("[");
                mostrarCelda(linea * 3);
                printf("]: ");
                for (int i = 0; i < 3; i++)
                {
                    mostrarCelda(memoria[linea * 3 + i]);
                    printf(" ");
                }                       //0001 0100 0000 000A 0000 0001
                if (mostrarLinea)
                {
                    printf(" %4d:", linea); //     1:
                }
                printf(" %s", lineaSinFiltrar);     // MOV AX, %0001
            }
            linea++;
        }
        else     //SI LINEASINFILTRAR NO ES VALIDA
        {
            if(muestra)
            {
                if(lineaSinFiltrar[0] != '\n')
                    printf("%47s%s", "", lineaSinFiltrar);
            }
        }
    }
    // FIN ARCHIVO----------------------------------------------------------------------
    if (errorsin)
        printf("\nError de sintaxis.");
    if (errorrot)
        printf("\nError: no se encontro el rotulo o la constante.");
    if (errorconst)
        printf("\nError de constante, ya existe.");

    //registros[3] = 1000;        //registro ES=1000
    return (errorsin || errorrot || errorconst);
}




void buscaRotulos(FILE *arch, listaRotulos *rotulos, int mostrar)
{
//Esta funcion
//busca rotulos en arch y los almacena en listaRotulos y si hay que mostrar los muestra
//trabaja el comando "\\ASM"
//y busca constantes y las almacena en listaConst
    if (mostrar)
        printf("\nLista de Rotulos:\n-----------------\n");

    int linea = 0;                              //Linea actual del código assembler
    char nextLinea[200];
    char next[50];
    rotulo *ult = NULL;
    while (!feof(arch))
    {
        fgets(nextLinea, sizeof(nextLinea), arch);      //nextLinea cada linea completa del assembler
        strcpy(next, " ");
        sscanf(nextLinea, "%s", next);                  //next es la primer palabra de nextLinea

        if(!strcmp(next, "\\\\ASM"))                    //si next es igual a "\\ASM"
        {
        //INTERPRETA EL COMANDO \\ASM-------------------------------------------------------------------
            strcpy(nextLinea, strupr(nextLinea));       //
            char nextSub[30];
            strcat(nextLinea, " END");                  //
            strcpy(nextSub, strtok(nextLinea, " =\t")); //
            int ind = 0;
            while(ind != -1)
            {
                strcpy(nextSub, " ");                   //nextSub=" "
                strcpy(nextSub, strtok(NULL, " =\t"));  //nextSub= la siguiente palabra de nextLinea separada por " =\t"
                ind = -1;

                if(!strcmp(nextSub, "DATA"))            //si nextSub="DATA"
                    ind = 0;
                else if(!strcmp(nextSub, "EXTRA"))      //si nextSub="EXTRA"
                    ind = 1;
                else if(!strcmp(nextSub, "STACK"))      //si nextSub="STACK"
                    ind = 2;

                if(ind!=-1)                             //si no es ninguno
                    segmentos[ind] = atoi(strtok(NULL, " =\t\n"));     //guarda en segmentos

            }
            //SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR SACAR
            printf("\nDATA: %d EXTRA: %d STACK: %d\n", segmentos[0], segmentos[1], segmentos[2]);//♥
        }
        else
        if (esValido(nextLinea))                        //si es nemonico o rotulo y nada raro
        {
        //CREA LA LISTA ROTULOS Y CUENTA LINEAS-------------------------------------------------------------------
            if (esRotulo(next))                         //si tiene ':' y no es comentario
            {
                rotulo *nextRotulo = (rotulo *)malloc(sizeof(rotulo));
                nextRotulo->sig = NULL;
                next[strlen(next) - 1] = '\0';
                char *s = next;                         //
                while (*s)
                {
                    *s = toupper((unsigned char)*s);    //pasa next a mayuscula
                    s++;
                }                                       //
                strcpy(nextRotulo->rot, next);
                nextRotulo->pos = linea;                //la posicion del rotulo es la linea actual

                if (*rotulos == NULL)                   //crea la lista de rotulos
                {
                    *rotulos = nextRotulo;
                }
                else
                {
                    ult->sig = nextRotulo;
                }
                ult = nextRotulo;

                if (mostrar)                            //muestra el rotulo si es necesario
                    printf("%d: %s\n", linea, next);
            }
            linea++;
        }
        else    //si no es valido, puede ser ('\t') (' ') ('\0') ('\n') ('/') menos asm porque ya esta arriba
        {       //o puede ser cualquier palabra que no sea rotulo o mnemonico
        //CREA LA LISTA CONSTANTES-------------------------------------------------------------------
            char *palabra1,*palabra2,*palabra3;                 //nextLinea=="//BASE EQU 64"
            palabra1 = strtok(nextLinea," \t\n");               //palabra1=="//BASE"
            palabra2 = strtok(NULL," \t\n");                    //palabra2=="EQU"
            palabra3 = strtok(NULL," \t\n");                    //palabra3=="64"
            if (palabra1!=NULL && palabra1[0]!='/'&& palabra2!=NULL && palabra3!=NULL)
            {
                strupr(palabra2);
                listaConst nodo,ult;
                if (!strcmp(palabra2,"EQU"))                    //palabra2=="EQU"
                {
                    nodo = (listaConst)malloc(sizeof(nodoConst));
                    strcpy(nodo->nombre,palabra1);              //nodo->nombre="//BASE"
                    nodo->sig=NULL;
                    if (palabra3[0]=='"')
                    {
                        memcpy(palabra3,palabra3+1,strlen(palabra3));
                        palabra3[strlen(palabra3)-1] = '\0';
                        nodo->esDirecto=1;
                    }
                    else
                    {
                        nodo->esDirecto=0;
                        nodo->valor = stringConSimboloAInt(palabra3);   //nodo->valor=64
                    }

                    strcpy(nodo->dato,palabra3);
                    if (constantes==NULL)
                        constantes = nodo;                      //crea en constantes una lista de constante xd
                    else
                        ult->sig = nodo;
                    ult = nodo;
                }
            }
            //Se buscan 3 palabras y se ve si la segunda es EQU
            //Depende de el primer simbolo de la ultima palabra que tipo es
            //Si la primera palabra esta en buscarconstantes da error
            //to be continued en constantesAMemoria
        }
    }
    registros[2] = linea*3;     //registro DS=posicion de memoria donde termina el programa
    printf("TOTAL DE LINEAS: %d\n", linea);
}

void constantesAMemoria()
{
    listaConst aux = constantes;    //hasta este momento registro[2] solo guarda la cantidad de celdas que ocupan las lineas, osea lineas*3
    while (aux!=NULL)                           //recorre las constantes
    {
        if (aux->esDirecto)                     //cuando son directas
        {
            aux->valor = registros[2];          //guarda en valor la cantidad de celdas que ocupa el programa actualmente
            for (int i=0;i<strlen(aux->dato)+1;i++)
            {
                memoria[registros[2]++]=aux->dato[i];   //guarda en la memoria caracter por caracter de la constante
            }
        }
        aux= aux->sig;
    }
    //recorre la lista de consts y si es directa
    //hace 2 cosas: pone el dato en memoria y la direccion de memoria en la lista
}

int stringConSimboloAInt(char* dato)
{
    char aux[100];                                     //en la memoria ya hay un cero asi que no necesita aclararlo
    strcpy(aux, dato);
    char *p = aux;
    int retornar;

    switch (*dato)
    {
    case '\'':
        p++;
        retornar =  aux[1];
        break;
    case '%':
        p++;
        retornar =  strtoul(p, NULL, 16);
        break;
    case '@':
        p++;
        retornar =  strtoul(p, NULL, 8);
        break;
    case '#':
        p++;
    default:
        retornar =  atoi(p);
        break;
    }

    return retornar;
}

void generaImg(FILE* archImg)               //genera el archivo binario en base a los registros y a la memoria
{
    for(int i=0; i<16; i++)
    {
        fwrite(&registros[i], sizeof(registros[i]), 1, archImg);
    }
    for(int i=0; i<registros[2]; i++)
    {
        fwrite(&memoria[i], sizeof(memoria[i]), 1, archImg);
    }
}
