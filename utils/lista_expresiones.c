#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./lista_expresiones.h"

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

t_lista_expresiones *crearListaExpresiones()
{
    t_lista_expresiones *nuevaLista = (t_lista_expresiones *)malloc(sizeof(t_lista_expresiones));

    if (nuevaLista == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para la lista de expresiones\n");
        return NULL;
    }

    nuevaLista->inicio = NULL;
    nuevaLista->fin = NULL;
    nuevaLista->cantidad = 0;

    return nuevaLista;
}

int agregarExpresion(t_lista_expresiones *lista, const char *expresion)
{
    if (lista == NULL)
    {
        fprintf(stderr, "Error: Lista de expresiones NULL\n");
        return 0;
    }

    if (expresion == NULL)
    {
        fprintf(stderr, "Error: Expresión NULL\n");
        return 0;
    }

    t_nodo_expresion *nuevoNodo = (t_nodo_expresion *)malloc(sizeof(t_nodo_expresion));

    if (nuevoNodo == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo de expresión\n");
        return 0;
    }

    // Copiar la expresión al nodo
    strncpy(nuevoNodo->expresion, expresion, TAM_CONTENIDO_PILA - 1);
    nuevoNodo->expresion[TAM_CONTENIDO_PILA - 1] = '\0';

    nuevoNodo->siguiente = NULL;

    // Si la lista está vacía
    if (lista->inicio == NULL)
    {
        lista->inicio = nuevoNodo;
        lista->fin = nuevoNodo;
    }
    else
    {
        // Agregar al final
        lista->fin->siguiente = nuevoNodo;
        lista->fin = nuevoNodo;
    }

    lista->cantidad++;

    return 1;
}

const char *obtenerExpresion(t_lista_expresiones *lista, int indice)
{
    if (lista == NULL || indice < 0 || indice >= lista->cantidad)
    {
        return NULL;
    }

    t_nodo_expresion *actual = lista->inicio;
    int contador = 0;

    while (actual != NULL && contador < indice)
    {
        actual = actual->siguiente;
        contador++;
    }

    if (actual != NULL)
    {
        return actual->expresion;
    }

    return NULL;
}

int listaExpresionesVacia(t_lista_expresiones *lista)
{
    return (lista == NULL || lista->inicio == NULL || lista->cantidad == 0);
}

void eliminarListaExpresiones(t_lista_expresiones *lista)
{
    if (lista == NULL)
    {
        return;
    }

    t_nodo_expresion *actual = lista->inicio;
    t_nodo_expresion *siguiente;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;

        free(actual);

        actual = siguiente;
    }

    free(lista);
}

void imprimirListaExpresiones(t_lista_expresiones *lista)
{
    if (lista == NULL)
    {
        printf("Lista de expresiones NULL\n");
        return;
    }

    if (listaExpresionesVacia(lista))
    {
        printf("Lista de expresiones vacía\n");
        return;
    }

    t_nodo_expresion *actual = lista->inicio;
    int indice = 0;

    while (actual != NULL)
    {
        actual = actual->siguiente;
        indice++;
    }
}

int compararExpresiones(const char *exp1, const char *exp2)
{
    if (exp1 == NULL || exp2 == NULL)
    {
        return 0;
    }

    return (strcmp(exp1, exp2) == 0);
}
