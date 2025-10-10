#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./pila.h"

t_pila *pila;

/**
 * Inicializa la pila
 */
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

/**
 * Apila un elemento en el tope de la pila
 * @param elemento String a apilar
 * @return 1 si fue exitoso, 0 si falló
 */
int apilar(const char *elemento)
{
    if (elemento == NULL)
    {
        fprintf(stderr, "Error: Elemento NULL\n");
        return 0;
    }

    // Crear nuevo nodo
    t_nodo_pila *nuevoNodo = (t_nodo_pila *)malloc(sizeof(t_nodo_pila));

    if (nuevoNodo == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo\n");
        return 0;
    }

    // Copiar el contenido (limitado al tamaño máximo)
    strncpy(nuevoNodo->contenido, elemento, TAM_CONTENIDO_PILA - 1);
    nuevoNodo->contenido[TAM_CONTENIDO_PILA - 1] = '\0'; // Asegurar terminación

    // Insertar al inicio (tope de la pila)
    nuevoNodo->siguiente = pila->tope;
    pila->tope = nuevoNodo;

    return 1;
}

/**
 * Desapila el elemento del tope de la pila
 * @return Puntero al contenido del elemento desapilado, NULL si la pila está vacía
 * NOTA: El contenido retornado es estático, se sobrescribe en cada llamada
 */
char *desapilar()
{
    if (pila == NULL || pila->tope == NULL)
    {
        fprintf(stderr, "Error: Pila vacía\n");
        return NULL;
    }

    // Buffer estático para retornar el contenido
    static char contenidoDesapilado[TAM_CONTENIDO_PILA];

    // Guardar referencia al nodo a eliminar
    t_nodo_pila *nodoAEliminar = pila->tope;

    // Copiar el contenido antes de eliminar
    strcpy(contenidoDesapilado, nodoAEliminar->contenido);

    // Actualizar el tope
    pila->tope = pila->tope->siguiente;

    // Liberar el nodo
    free(nodoAEliminar);

    return contenidoDesapilado;
}

/**
 * Elimina la pila y libera toda la memoria utilizada
 */
void eliminarPila()
{
    if (pila == NULL)
    {
        return;
    }

    t_nodo_pila *actual = pila->tope;
    t_nodo_pila *siguiente;

    // Recorrer y liberar todos los nodos
    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    // Liberar la estructura principal
    free(pila);
}
