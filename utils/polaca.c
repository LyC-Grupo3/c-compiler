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

    // Debug
    char debugMsg[100];
    snprintf(debugMsg, 100, "insertarEnPolaca(\"%s\") -> índice %d", elemento, nuevoNodo->indice);
    registrarEstadoPolaca(debugMsg);

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

    t_nodo_polaca *actual = polaca->inicio;

    while (actual != NULL)
    {
        if (actual->indice == indice)
        {
            strncpy(actual->contenido, elemento, TAM_CONTENIDO - 1);
            actual->contenido[TAM_CONTENIDO - 1] = '\0';

            // Debug
            char debugMsg[100];
            snprintf(debugMsg, 100, "insertarEnPolacaIndice(%d, \"%s\") -> actualizado", indice, elemento);
            registrarEstadoPolaca(debugMsg);

            return 1;
        }
        actual = actual->siguiente;
    }

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

    // imprimirEstadoPolacaVertical(polaca->inicio, archivo);
    imprimirEstadoPolacaHorizontal(polaca->inicio, archivo);

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

char *getIndiceActualPolaca()
{
    char *indiceStr = (char *)malloc(12 * sizeof(char));

    if (indiceStr != NULL)
    {
        snprintf(indiceStr, 12, "%d", polaca->contador);
    }
    return indiceStr;
}

/* -------------------------------------------------------------------------- */
/*                           FUNCIONES DE DEBUG                               */
/* -------------------------------------------------------------------------- */

FILE *archivoDebugPolaca = NULL;
static int contadorOperaciones = 0;

void inicializarDebugPolaca(const char *nombreArchivo)
{
    archivoDebugPolaca = fopen(nombreArchivo, "w");
    if (archivoDebugPolaca == NULL)
    {
        fprintf(stderr, "Error: No se pudo crear el archivo de debug %s\n", nombreArchivo);
        return;
    }

    fprintf(archivoDebugPolaca, "=== DEBUG DE POLACA Y PILA ===\n\n");
    contadorOperaciones = 0;
}

void imprimirEstadoPolacaHorizontal(t_nodo_polaca *inicio, FILE *archivo)
{
    t_nodo_polaca *actual;

    int contador = 0;
    actual = inicio;
    while (actual != NULL)
    {
        contador++;
        actual = actual->siguiente;
    }

    int *anchos = (int *)malloc((contador + 1) * sizeof(int));

    actual = inicio;
    int i = 0;
    while (actual != NULL)
    {
        int anchoContenido = strlen(actual->contenido) + 4;

        char bufferIndice[20];
        snprintf(bufferIndice, 20, "%d", actual->indice);
        int anchoIndice = strlen(bufferIndice) + 4;

        anchos[i] = (anchoIndice > anchoContenido) ? anchoIndice : anchoContenido;

        actual = actual->siguiente;
        i++;
    }

    char bufferIndiceActual[20];
    snprintf(bufferIndiceActual, 20, "%d", polaca->contador);
    int anchoIndiceActual = strlen(bufferIndiceActual) + 4;
    anchos[contador] = anchoIndiceActual;

    // Primera fila: Índices
    actual = inicio;
    i = 0;
    while (actual != NULL)
    {
        char bufferIndice[20];
        snprintf(bufferIndice, 20, "%d", actual->indice);
        int longitudIndice = strlen(bufferIndice);

        int espacioDisponible = anchos[i] - 2; // -2 por los corchetes
        int espaciosIzq = (espacioDisponible - longitudIndice) / 2;
        int espaciosDer = espacioDisponible - longitudIndice - espaciosIzq;

        fprintf(archivo, "[%*s%d%*s] ", espaciosIzq, "", actual->indice, espaciosDer, "");
        actual = actual->siguiente;
        i++;
    }

    // Imprimir el índice actual donde estoy parado
    int longitudIndiceActual = strlen(bufferIndiceActual);
    int espacioDisponibleActual = anchos[contador] - 2;
    int espaciosIzqActual = (espacioDisponibleActual - longitudIndiceActual) / 2;
    int espaciosDerActual = espacioDisponibleActual - longitudIndiceActual - espaciosIzqActual;
    fprintf(archivo, "[%*s%d%*s] ", espaciosIzqActual, "", polaca->contador, espaciosDerActual, "");

    fprintf(archivo, "\n");

    // Segunda fila: Contenidos
    actual = inicio;
    i = 0;
    while (actual != NULL)
    {
        int longitudContenido = strlen(actual->contenido);
        int espacioDisponible = anchos[i] - 2; // -2 por los corchetes
        int espaciosIzq = (espacioDisponible - longitudContenido) / 2;
        int espaciosDer = espacioDisponible - longitudContenido - espaciosIzq;

        fprintf(archivo, "[%*s%s%*s] ", espaciosIzq, "", actual->contenido, espaciosDer, "");
        actual = actual->siguiente;
        i++;
    }

    // Imprimir el "nodo actual" vacío
    int espacioDisponibleVacio = anchos[contador] - 2;
    fprintf(archivo, "[%*s] ", espacioDisponibleVacio, "");

    fprintf(archivo, "\n");

    free(anchos);
}

void imprimirEstadoPolacaVertical(t_nodo_polaca *inicio, FILE *archivo)
{
    t_nodo_polaca *actual = inicio;

    while (actual != NULL)
    {
        fprintf(archivo, "[%3d] %s\n", actual->indice, actual->contenido);
        actual = actual->siguiente;
    }
}

void registrarEstadoPolaca(const char *operacion)
{
    if (archivoDebugPolaca == NULL || polaca == NULL)
    {
        return;
    }

    fprintf(archivoDebugPolaca, "%s\n", operacion);

    t_nodo_polaca *actual = polaca->inicio;

    if (actual == NULL)
    {
        fprintf(archivoDebugPolaca, "  POLACA: (vacía)\n");
    }
    else
    {
        imprimirEstadoPolacaHorizontal(actual, archivoDebugPolaca);
        // imprimirEstadoPolacaVertical(actual, archivoDebugPolaca);
    }

    fprintf(archivoDebugPolaca, "\n");
    fflush(archivoDebugPolaca);
}

void cerrarDebugPolaca()
{
    if (archivoDebugPolaca != NULL)
    {
        fprintf(archivoDebugPolaca, "=== FIN DEL DEBUG  ===\n");
        fclose(archivoDebugPolaca);
        archivoDebugPolaca = NULL;
    }
}
