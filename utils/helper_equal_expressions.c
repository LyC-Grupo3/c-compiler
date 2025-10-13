#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./helper_equal_expressions.h"
#include "./polaca.h"

// Declaración externa de la polaca (definida en polaca.c)
extern t_polaca *polaca;

// Lista global de expresiones para equalExpressions
static t_lista_expresiones *listaExpresionesGlobal = NULL;

/**
 * @brief Busca hacia atrás el último nodo con INICIO_EXP en la polaca
 * @return Puntero al nodo con INICIO_EXP o NULL si no se encuentra
 */
static t_nodo_polaca *buscarUltimoInicioExp()
{
    if (polaca == NULL || polaca->inicio == NULL)
    {
        return NULL;
    }

    t_nodo_polaca *actual = polaca->inicio;
    t_nodo_polaca *ultimoInicioExp = NULL;

    // Buscar el último INICIO_EXP
    while (actual != NULL)
    {
        if (strcmp(actual->contenido, "INICIO_EXP") == 0)
        {
            ultimoInicioExp = actual;
        }
        actual = actual->siguiente;
    }

    return ultimoInicioExp;
}

/**
 * @brief Extrae y construye una expresión desde INICIO_EXP hasta el final de la polaca
 * @param nodoInicio Nodo donde está INICIO_EXP
 * @param expresion Buffer donde se guardará la expresión
 * @param tamBuffer Tamaño del buffer
 */
static void extraerExpresionDesdeInicio(t_nodo_polaca *nodoInicio, char *expresion, int tamBuffer)
{
    if (nodoInicio == NULL || expresion == NULL)
    {
        return;
    }

    expresion[0] = '\0';
    int longitudActual = 0;

    // Empezar desde el siguiente nodo después de INICIO_EXP
    t_nodo_polaca *actual = nodoInicio->siguiente;

    while (actual != NULL)
    {
        int longitudNodo = strlen(actual->contenido);

        // Verificar espacio disponible
        if (longitudActual + longitudNodo + 2 < tamBuffer)
        {
            if (longitudActual > 0)
            {
                strcat(expresion, " ");
                longitudActual++;
            }

            strcat(expresion, actual->contenido);
            longitudActual += longitudNodo;
        }
        else
        {
            fprintf(stderr, "Warning: Expresión demasiado larga, truncada\n");
            break;
        }

        actual = actual->siguiente;
    }
}

/**
 * @brief Elimina nodos desde un nodo específico hasta el final de la polaca
 * @param nodoInicio Nodo desde donde empezar a eliminar (inclusive)
 */
static void eliminarDesdeNodoHastaFinal(t_nodo_polaca *nodoInicio)
{
    if (polaca == NULL || nodoInicio == NULL)
    {
        return;
    }

    // Buscar el nodo anterior a nodoInicio
    t_nodo_polaca *anterior = NULL;
    t_nodo_polaca *actual = polaca->inicio;

    while (actual != NULL && actual != nodoInicio)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    // Si no encontramos el nodo, salir
    if (actual == NULL)
    {
        return;
    }

    // Eliminar todos los nodos desde nodoInicio hasta el final
    while (actual != NULL)
    {
        t_nodo_polaca *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    // Actualizar el puntero de la polaca
    if (anterior == NULL)
    {
        // nodoInicio era el primer nodo
        polaca->inicio = NULL;
        polaca->actual = NULL;
        polaca->contador = 0;
    }
    else
    {
        // Actualizar el nodo anterior para que sea el último
        anterior->siguiente = NULL;
        polaca->contador = anterior->indice + 1;
    }
}

/**
 * @brief Inicializa la lista global de expresiones
 */
void inicializarListaExpresiones()
{
    if (listaExpresionesGlobal != NULL)
    {
        eliminarListaExpresiones(listaExpresionesGlobal);
    }

    listaExpresionesGlobal = crearListaExpresiones();

    if (listaExpresionesGlobal == NULL)
    {
        fprintf(stderr, "Error: No se pudo crear la lista de expresiones\n");
    }
}

/**
 * @brief Obtiene la lista global de expresiones
 * @return Puntero a la lista global
 */
t_lista_expresiones *obtenerListaExpresiones()
{
    return listaExpresionesGlobal;
}

/**
 * @brief Libera la lista global de expresiones
 */
void liberarListaExpresiones()
{
    if (listaExpresionesGlobal != NULL)
    {
        eliminarListaExpresiones(listaExpresionesGlobal);
        listaExpresionesGlobal = NULL;
    }
}

/**
 * @brief Procesa una expresión cuando se completa en el parser
 * Extrae desde el último INICIO_EXP hasta el final, guarda en lista y elimina de polaca
 */
void procesarExpresionCompleta()
{
    // Inicializar la lista si es la primera vez
    if (listaExpresionesGlobal == NULL)
    {
        inicializarListaExpresiones();
    }

    // Buscar el último INICIO_EXP
    t_nodo_polaca *nodoInicioExp = buscarUltimoInicioExp();

    if (nodoInicioExp == NULL)
    {
        printf("Error: No se encontró INICIO_EXP en la polaca\n");
        exit(1);
    }

    // Extraer la expresión
    char expresion[TAM_CONTENIDO_PILA];
    extraerExpresionDesdeInicio(nodoInicioExp, expresion, TAM_CONTENIDO_PILA);

    // Agregar a la lista
    if (strlen(expresion) > 0)
    {
        agregarExpresion(listaExpresionesGlobal, expresion);
        printf("[DEBUG] Expresión agregada a la lista: '%s'\n", expresion);
    }

    // Eliminar desde INICIO_EXP hasta el final de la polaca
    eliminarDesdeNodoHastaFinal(nodoInicioExp);
}


