#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tdacazorla.h"
//El codigo que incluya esta libreria esta permanentemente cursed.

char listaMnemonicos[144][5];

char vec[16][5];//aca alcanza con [15][3]//vector de registros

int esRotulo(char *string)//si no es comentario y el ultimo caracter es ':'
{
    if(!string)//si en null
        return 0;

    int len = strlen(string);
    if(string[0] != '/' && string[len-1] == ':')
        return 1;//es rotulo
    return 0;
}

void crearRegistros()
{
    for(int i=0; i<16; i++)
    {
        strcpy(vec[i], " ");
    }
    strcpy(vec[0],"PS");
    strcpy(vec[1],"CS");
    strcpy(vec[2],"DS");
    strcpy(vec[3],"ES");
    strcpy(vec[4],"IP");
    strcpy(vec[5],"SS");
    strcpy(vec[6],"SP");
    strcpy(vec[7],"BP");
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

int contieneArg(int argc, char* args[], char* busca)//busca en los argumentos si existe el que busca
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

int esMnemonico(char mnemonico[])//regresa el valor decimal del mnemonico, si no existe regresa -1
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
    char string[50] = " ";
    sscanf(linea, "%s", string);
    return (string[0] != '\t' &&
            string[0] != ' ' &&
            string != NULL &&
            string[0] != '\0' &&
            string[0] != '\n' &&
            string[0] != '/' &&
            string[0] != '\\')
            //si no es tabulacion y no es espacio en blanco y no es nulo y no es fin de string y no es salto de linea y no es comentario y no es ASM
        && (esRotulo(string) || esMnemonico(string)!=-1); //y ademas es Rotulo o nemonico entonces es valido
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

listaConst buscarConstante(listaConst listaconst,char *nombre )
{
    listaConst retorno = NULL;
    listaConst sig = listaconst;
    while (sig!=NULL){
        if (!strcmp(sig->nombre,nombre)){
            retorno = sig;
            break;
        }
        sig=sig->sig;
    }
    return retorno;
}
