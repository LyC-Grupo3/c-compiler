#ifndef PILA_H
#define PILA_H

/* -------------------------------------------------------------------------- */
/*                                   DEFINES                                  */
/* -------------------------------------------------------------------------- */

#define TAM_CONTENIDO_PILA 50

/* -------------------------------------------------------------------------- */
/*                                 ESTRUCTURAS                                */
/* -------------------------------------------------------------------------- */

typedef struct t_nodo_pila
{
    char contenido[TAM_CONTENIDO_PILA];
    struct t_nodo_pila *siguiente;
} t_nodo_pila;

typedef struct
{
    t_nodo_pila *tope;
} t_pila;

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

void inicializarPila();

int apilar(const char *elemento);
char *desapilar(void);

void registrarEstadoPila(const char *operacion);

void eliminarPila(void);

#endif // PILA_H
