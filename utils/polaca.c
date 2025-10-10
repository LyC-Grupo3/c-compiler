#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./polaca.h"

t_polaca *polaca;

void inicializarPolaca()
{
    polaca = (t_polaca *)malloc(sizeof(t_polaca));

    if (polaca == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para Polaca\n");
        exit(1);
    }

    polaca->inicio = NULL;
    polaca->actual = NULL;
    polaca->contador = 0;
}

int insertarEnPolaca(const char *elemento)
{
    if (elemento == NULL)
    {
        fprintf(stderr, "Error: Elemento NULL\n");
        return 0;
    }

    t_nodo_polaca *nuevoNodo = (t_nodo_polaca *)malloc(sizeof(t_nodo_polaca));

    if (nuevoNodo == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo\n");
        return 0;
    }

    // Asignar el índice actual
    nuevoNodo->indice = polaca->contador;
    polaca->contador++;

    strncpy(nuevoNodo->contenido, elemento, TAM_CONTENIDO - 1);
    nuevoNodo->contenido[TAM_CONTENIDO - 1] = '\0';
    nuevoNodo->siguiente = NULL;

    if (polaca->inicio == NULL)
    {
        polaca->inicio = nuevoNodo;
        polaca->actual = nuevoNodo;
    }
    else
    {
        t_nodo_polaca *temp = polaca->inicio;
        while (temp->siguiente != NULL)
        {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }

    return 1;
}


int insertarEnPolacaIndice(int indice, const char *elemento)
{
    if (elemento == NULL)
    {
        fprintf(stderr, "Error: Elemento NULL\n");
        return 0;
    }

    if (polaca == NULL || polaca->inicio == NULL)
    {
        fprintf(stderr, "Error: Polaca vacía o no inicializada\n");
        return 0;
    }

    // Buscar el nodo con el índice especificado
    t_nodo_polaca *actual = polaca->inicio;
    
    while (actual != NULL)
    {
        if (actual->indice == indice)
        {
            // Reemplazar el contenido del nodo encontrado
            strncpy(actual->contenido, elemento, TAM_CONTENIDO - 1);
            actual->contenido[TAM_CONTENIDO - 1] = '\0';
            
            return 1;
        }
        actual = actual->siguiente;
    }

    // Si no se encontró el índice
    fprintf(stderr, "Error: No se encontró el índice %d en la polaca\n", indice);
    return 0;
}


void avanzarPolaca()
{
    insertarEnPolaca("");
}

void exportarPolaca(const char *nombreArchivo)
{
    if (polaca == NULL)
    {
        fprintf(stderr, "Error: Polaca no inicializada\n");
        return;
    }

    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL)
    {
        fprintf(stderr, "Error: No se pudo crear el archivo %s\n", nombreArchivo);
        return;
    }

    fprintf(archivo, "=== CÓDIGO INTERMEDIO - NOTACIÓN POLACA INVERSA ===\n\n");

    t_nodo_polaca *actual = polaca->inicio;

    while (actual != NULL)
    {
        fprintf(archivo, "[%3d] %s\n", actual->indice, actual->contenido);
        actual = actual->siguiente;
    }

    fprintf(archivo, "\n=== Total de instrucciones: %d ===\n", polaca->contador);

    fclose(archivo);
}

void eliminarPolaca()
{
    if (polaca == NULL)
    {
        return;
    }

    t_nodo_polaca *actual = polaca->inicio;
    t_nodo_polaca *siguiente;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    free(polaca);
}

char* getIndiceActualPolaca()
{
    char *indiceStr = (char *)malloc(12 * sizeof(char)); // Suficiente para un entero

    if (indiceStr != NULL)
    {
        snprintf(indiceStr, 12, "%d", polaca->contador);
    }
    return indiceStr;
}
