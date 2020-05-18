#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
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

int getValReg(char String[5])
{
    for (int i=0;i<16;i++)
        if (!strcmp(vec[i],String))
            return i;
    return -1;
}

//Asume que base es un string de 2 caracteres
int registroBase(int linea, int32_t memoria[], int arg, char* base){
    int pos = getValReg(base),retornar=0;
    if(pos==1 || pos==2 ||pos==3 ||pos==5)
    memoria[linea*3 + arg] |= pos << 28;
    else
    {
        memoria[linea*3 + arg] = 0xffffffff;
        retornar=1;
    }
    return retornar;//1 error 0 todo en orden :)
}

void argumentoIndirecto(int linea, int32_t memoria[], listaConst constantes, int arg, char* palabra){
    //printf("\nARG IND: %s\n", palabra);
    memoria[linea*3] |= (3 << (2 - arg) * 8);
    char *strTemp = (char*) malloc(50);
    strcpy(strTemp, palabra);
    *(strTemp+2) = '\0';
    memoria[linea*3+arg] |= getValReg(strTemp);
    nodoConst *nodoTemp;
    if((strTemp = strchr(palabra, '+')) != NULL){
        if((nodoTemp = buscarConstante(constantes, strTemp+1))!=NULL){
            memoria[linea*3+arg] |= (nodoTemp->valor & 0x00FFFFFF) << 4;
        }else{
            memoria[linea*3+arg] |= atoi(strTemp+1) << 4;
        }
    }else if((strTemp = strchr(palabra, '-')) != NULL){
        int valor;

        if((nodoTemp = buscarConstante(constantes, strTemp+1))!=NULL){
            valor = nodoTemp->valor;
        }else{
            valor = atoi(strTemp+1);
        }

        int compl = (~valor)+1;
        compl &= 0x00FFFFFF;
        memoria[linea*3+arg] |= compl << 4;
    }
}

//[DS:32] - > 32
//[CS:32] - > 32
//[DS:BASE] - > BASE
//[BASE] - > BASE
//[32] - > 32
void argumentoDirecto(int linea, int32_t memoria[], listaConst constantes, int arg, char* palabra){
    memoria[linea*3] |= (2 << (2 - arg) * 8);

    nodoConst *nodoTemp;
    if((nodoTemp = buscarConstante(constantes, palabra)) != NULL){
        memoria[linea*3+arg] |= nodoTemp->valor;
    }else{
        memoria[linea*3+arg] |= atoi(palabra);
    }
}

void determinarBase(int linea, int32_t memoria[], int arg, char* palabra){
    int numBase = 0, numReg = 0;
    if((numReg = getValReg(palabra)) != -1){// && (numReg== 0 || numReg== 1 ||numReg== 2 || numReg== 0)
        if(numReg >= 10) numBase = 2;
        else if(numReg == 6 || numReg == 7) numBase = 5;
        else numBase = 0xF;
    }else numBase = 2;

    memoria[linea*3+arg] |= numBase << 28;
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
    strcpy(listaMnemonicos[68], "PUSH");//44
    strcpy(listaMnemonicos[69], "POP");//45
    strcpy(listaMnemonicos[64], "CALL");//40
    strcpy(listaMnemonicos[72], "RET");//48
    strcpy(listaMnemonicos[80], "SLEN");//50
    strcpy(listaMnemonicos[81], "SMOV");//51
    strcpy(listaMnemonicos[83], "SCMP");//53
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
        if(!strcasecmp(listaMnemonicos[i], mnemonico))
            return i;
    }
    return -1;
}

int esValido(char *linea)
{
    char string[50] = " ";
    sscanf(linea, "%s", string);
    return (string[0] != '\t' &&        //si no es tabulacion
            string[0] != ' ' &&         //y no es espacio en blanco
            string != NULL &&           //y no es nulo
            string[0] != '\0' &&        //y no es fin de string
            string[0] != '\n' &&        //y no es salto de linea
            string[0] != '/' &&         //y no es comentario
            string[0] != '\\')          //y no es ASM
        && (esRotulo(string) || esMnemonico(string)!=-1); //y ademas es Rotulo o nemonico entonces
                                        //entonces es valido
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
        if (!strcasecmp(sig->nombre,nombre)){
            retorno = sig;
            break;
        }
        sig=sig->sig;
    }
    return retorno;
}

int verificarConstantesYRotulos(listaConst constantes,listaRotulos rotulos) // 1 si son todos diferentes, 0 si hay alguno igual
{
    listaConst auxConst = constantes;
    listaRotulos auxRot;
    while (auxConst!=NULL)// si al menos hay una constante
    {
        auxRot = rotulos;
        while (auxRot!=NULL)        //si al menos hay un rotulo
        {
            if (!strcmp(auxConst->nombre,auxRot->rot))  //si son iguales los nombres
                return 0;
            auxRot = auxRot->sig;                       //recorre ambas listas

        }
        auxConst = auxConst->sig;
    }
    return 1;
}

int int24Bits(int subInt){
    if(subInt>>23){
        subInt |= 0xFF000000;
        return subInt;
    }else{
        return subInt;
    }
}
