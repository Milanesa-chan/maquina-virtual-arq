typedef struct rotulo{
    char rot[50];
    int pos;
    struct rotulo* sig;
} rotulo;

typedef rotulo *listaRotulos;

//lista de constantes
typedef struct nodoConst{
    char dato[100];
    char nombre[11];
    int valor;
    int esDirecto;
    struct nodoConst* sig;
} nodoConst;

typedef nodoConst *listaConst;

listaConst buscarConstante(listaConst listaconst,char *nombre); //retorna el struct constante si lo encuentra y null si no

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int);

int esRotulo(char*);

void crearRegistros();

void getReg(int i,char String[5]);

int contieneArg(int, char* [], char*);

void mostrarCelda(int dato);

void crearListaMnemonicos();

int esMnemonico(char[]);

void getMnemonico(int, char[]);

int esValido(char[]);

int buscarRotulo(listaRotulos rotulos, char* rot);
