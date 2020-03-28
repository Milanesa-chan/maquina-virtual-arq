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

int esMnemonico(char[]);

int esValido(char[]);
