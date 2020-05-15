typedef struct rotulo{
    char rot[50];
    int pos;
    struct rotulo* sig;
} rotulo;

typedef rotulo *listaRotulos;

//lista de constantes
typedef struct nodoConst{
    char dato[100];     //solo directa
    char nombre[11];
    int valor;          //directas almacenan la direccion de memoria
    int esDirecto;      //si no es directa es inmediata
    struct nodoConst* sig;
} nodoConst;

typedef nodoConst *listaConst;

listaConst buscarConstante(listaConst listaconst,char *nombre); //retorna el struct constante si lo encuentra y null si no

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int);

int esRotulo(char*);

void crearRegistros();

void getReg(int i,char String[5]);

int getValReg(char String[5]);

int registroBase(int, int32_t[], int, char*);

void argumentoIndirecto(int linea, int32_t memoria[], listaConst constantes, int arg, char* palabra);

void argumentoDirecto(int linea, int32_t memoria[], listaConst constantes, int arg, char* palabra);

void determinarBase(int linea, int32_t memoria[], int arg, char* palabra);

int contieneArg(int, char* [], char*);

void mostrarCelda(int dato);

void crearListaMnemonicos();

int esMnemonico(char[]);

void getMnemonico(int, char[]);

int esValido(char[]);

int buscarRotulo(listaRotulos rotulos, char* rot);

int stringConSimboloAInt(char* dato);

void constantesAMemoria();

int verificarConstantesYRotulos(listaConst constantes,listaRotulos rotulos);

int int24Bits(int subInt);
