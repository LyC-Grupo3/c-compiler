#ifndef POLACA_H
#define POLACA_H

/* -------------------------------------------------------------------------- */
/*                                   DEFINES                                  */
/* -------------------------------------------------------------------------- */

#define TAM_CONTENIDO 50

#define VALOR_POLACA_COMPARADOR         "CMP"

#define VALOR_POLACA_OP_IGUAL           "BNE"
#define VALOR_POLACA_OP_DISTINTO        "BEQ"
#define VALOR_POLACA_OP_MAYOR           "BLE"
#define VALOR_POLACA_OP_MAYOR_IGUAL     "BLT"
#define VALOR_POLACA_OP_MENOR           "BGE"
#define VALOR_POLACA_OP_MENOR_IGUAL     "BGT"

/* -------------------------------------------------------------------------- */
/*                                 ESTRUCTURAS                                */
/* -------------------------------------------------------------------------- */

typedef struct t_nodo_polaca
{
    int indice;
    char contenido[TAM_CONTENIDO];
    struct t_nodo_polaca *siguiente;
} t_nodo_polaca;

typedef struct
{
    t_nodo_polaca *inicio;
    t_nodo_polaca *actual;
    int contador;
} t_polaca;

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

void inicializarPolaca();

int insertarEnPolaca(const char *elemento);
int insertarEnPolacaIndice(const char *elemento, int indice);
void avanzarPolaca(void);

void exportarPolaca(const char *nombreArchivo);
void eliminarPolaca(void);

char* getIndiceActualPolaca();

#endif // POLACA_H