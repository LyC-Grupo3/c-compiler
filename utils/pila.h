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

t_pila *crearPila();

int apilar(t_pila *pila, const char *elemento);
char *desapilar(t_pila *pila);

void registrarEstadoPila(t_pila *pila, const char *operacion);

void eliminarPila(t_pila *pila);

int pilaVacia(t_pila *pila);

void imprimirContenidoPila(t_pila *pila);

#endif // PILA_H
