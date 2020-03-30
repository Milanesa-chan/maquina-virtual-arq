typedef struct rotulo{
    char rot[50];
    int pos;
    struct rotulo* sig;
} rotulo;

typedef rotulo *listaRotulos;

void buscaRotulos(FILE* arch, listaRotulos *rotulos, int);

int esRotulo(char*);

int contieneArg(int, char* [], char*);

void mostrarCelda(int dato);

void crearListaMnemonicos();

int esMnemonico(char[]);

int esValido(char[]);

int buscarRotulo(listaRotulos rotulos, char* rot);
