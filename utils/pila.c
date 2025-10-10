#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./pila.h"

t_pila *pila;

extern FILE *archivoDebugPolaca;

void inicializarPila()
{
    pila = (t_pila *)malloc(sizeof(t_pila));

    if (pila == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para Pila\n");
        exit(1);
    }

    pila->tope = NULL;
}

int apilar(const char *elemento)
{
    if (elemento == NULL)
    {
        fprintf(stderr, "Error: Elemento NULL\n");
        return 0;
    }

    t_nodo_pila *nuevoNodo = (t_nodo_pila *)malloc(sizeof(t_nodo_pila));

    if (nuevoNodo == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo\n");
        return 0;
    }

    strncpy(nuevoNodo->contenido, elemento, TAM_CONTENIDO_PILA - 1);
    nuevoNodo->contenido[TAM_CONTENIDO_PILA - 1] = '\0';

    nuevoNodo->siguiente = pila->tope;
    pila->tope = nuevoNodo;

    // Debug
    char debugMsg[100];
    snprintf(debugMsg, 100, "apilar(\"%s\")", elemento);
    registrarEstadoPila(debugMsg);

    return 1;
}

char *desapilar()
{
    if (pila == NULL || pila->tope == NULL)
    {
        fprintf(stderr, "Error: Pila vacía\n");
        return NULL;
    }

    static char contenidoDesapilado[TAM_CONTENIDO_PILA];

    t_nodo_pila *nodoAEliminar = pila->tope;

    strcpy(contenidoDesapilado, nodoAEliminar->contenido);

    pila->tope = pila->tope->siguiente;

    free(nodoAEliminar);

    // Debug
    char debugMsg[100];
    snprintf(debugMsg, 100, "desapilar() -> \"%s\"", contenidoDesapilado);
    registrarEstadoPila(debugMsg);

    return contenidoDesapilado;
}

void eliminarPila()
{
    if (pila == NULL)
    {
        return;
    }

    t_nodo_pila *actual = pila->tope;
    t_nodo_pila *siguiente;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    free(pila);
}

/* -------------------------------------------------------------------------- */
/*                           FUNCIONES DE DEBUG                               */
/* -------------------------------------------------------------------------- */

void registrarEstadoPila(const char *operacion)
{
    if (archivoDebugPolaca == NULL || pila == NULL)
    {
        return;
    }

    fprintf(archivoDebugPolaca, "%s (tope->fondo)\n", operacion);

    t_nodo_pila *actual = pila->tope;

    if (actual == NULL)
    {
        fprintf(archivoDebugPolaca, "(vacía)");
    }
    else
    {
        fprintf(archivoDebugPolaca, "  [");
        while (actual != NULL)
        {
            fprintf(archivoDebugPolaca, "%s", actual->contenido);
            if (actual->siguiente != NULL)
            {
                fprintf(archivoDebugPolaca, " | ");
            }
            actual = actual->siguiente;
        }
        fprintf(archivoDebugPolaca, "]");
    }

    fprintf(archivoDebugPolaca, "\n");
    fflush(archivoDebugPolaca);
}
