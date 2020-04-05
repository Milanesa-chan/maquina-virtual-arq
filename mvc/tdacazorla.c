#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tdacazorla.h"
//El codigo que incluya esta libreria esta permanentemente cursed.

char listaMnemonicos[144][5];

char vec[16][5];

int esRotulo(char *string)
{
    if(!string)
        return 0;

    int len = strlen(string);
    if(string[0] != '/' && string[len-1] == ':')
        return 1;
    return 0;
}

void crearRegistros()
{
    for(int i=0; i<16; i++)
    {
        strcpy(vec[i], " ");
    }
    strcpy(vec[2],"DS");
    strcpy(vec[3],"ES");
    strcpy(vec[4],"IP");
    strcpy(vec[8],"AC");
    strcpy(vec[9],"CC");
    strcpy(vec[10],"AX");
    strcpy(vec[11],"BX");
    strcpy(vec[12],"CX");
    strcpy(vec[13],"DX");
    strcpy(vec[14],"EX");
    strcpy(vec[15],"FX");
}
void getReg(int i ,char String[5])
{
    strcpy(String,vec[i]);
}
void crearListaMnemonicos()
{
    for(int i=0; i<144; i++)
    {
        strcpy(listaMnemonicos[i], "");
    }

    strcpy(listaMnemonicos[1], "MOV");
    strcpy(listaMnemonicos[2], "ADD");
    strcpy(listaMnemonicos[3], "SUB");
    strcpy(listaMnemonicos[4], "MUL");
    strcpy(listaMnemonicos[5], "DIV");
    strcpy(listaMnemonicos[6], "MOD");
    strcpy(listaMnemonicos[19], "CMP");
    strcpy(listaMnemonicos[23], "SWAP");
    strcpy(listaMnemonicos[25], "RND");
    strcpy(listaMnemonicos[49], "AND");
    strcpy(listaMnemonicos[50], "OR");
    strcpy(listaMnemonicos[51], "NOT");
    strcpy(listaMnemonicos[52], "XOR");
    strcpy(listaMnemonicos[55], "SHL");
    strcpy(listaMnemonicos[56], "SHR");
    strcpy(listaMnemonicos[32], "JMP");
    strcpy(listaMnemonicos[33], "JE");
    strcpy(listaMnemonicos[34], "JG");
    strcpy(listaMnemonicos[35], "JL");
    strcpy(listaMnemonicos[36], "JZ");
    strcpy(listaMnemonicos[37], "JP");
    strcpy(listaMnemonicos[38], "JN");
    strcpy(listaMnemonicos[39], "JNZ");
    strcpy(listaMnemonicos[40], "JNP");
    strcpy(listaMnemonicos[41], "JNN");
    strcpy(listaMnemonicos[129], "SYS");
    strcpy(listaMnemonicos[143], "STOP");
}

int buscarRotulo(listaRotulos rotulos, char* rot)
{
    listaRotulos aux = rotulos;
    while(aux != NULL)
    {
        if(!strcmp(aux->rot, rot))
        {
            return aux->pos;
        }
        aux = aux->sig;
    }
    return -1;
}

int contieneArg(int argc, char* args[], char* busca)
{
    for(int i=0; i<argc; i++)
    {
        if(!strcmp(args[i], busca))
        {
            return 1;
        }
    }
    return 0;
}

int esMnemonico(char mnemonico[])
{
    for(int i=0; i<144; i++)
    {
        if(!strcmp(listaMnemonicos[i], mnemonico))
            return i;
    }
    return -1;
}

int esValido(char *linea)
{
    char string[50];
    sscanf(linea, "%s", string);
    return (linea[0] != '\t' && linea[0] != ' ' && linea != NULL && linea[0] != '\0' && linea[0] != '\n' && linea[0] != '/')
        && (esRotulo(string) || esMnemonico(string));
}

void getMnemonico(int cod, char retorna[10]){
    strcpy(retorna, listaMnemonicos[cod]);
    //printf("GET MNEMONICO: %s %d", listaMnemonicos[cod], cod);
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
