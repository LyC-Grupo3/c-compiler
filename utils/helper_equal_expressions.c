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
 * @brief Genera código en la polaca para comparar todas las expresiones
 * y determinar si hay dos iguales
 * Resultado se guarda en @equal (1 si hay iguales, 0 si no)
 */
void generarCodigoEqualExpressions_1()
{
    if (listaExpresionesGlobal == NULL || listaExpresionesGlobal->cantidad < 2)
    {
        fprintf(stderr, "Error: Se necesitan al menos 2 expresiones para equalExpressions\n");
        // Por defecto, asignar 0 (no hay iguales)
        insertarEnPolaca("0");
        insertarEnPolaca("@equal");
        insertarEnPolaca(":=");
        return;
    }

    int cantidadExpresiones = listaExpresionesGlobal->cantidad;
    
    printf("[DEBUG] Generando código para comparar %d expresiones\n", cantidadExpresiones);

    // Paso 1: Evaluar todas las expresiones y guardarlas en variables temporales
    for (int i = 0; i < cantidadExpresiones; i++)
    {
        const char *exp = obtenerExpresion(listaExpresionesGlobal, i);
        if (exp != NULL)
        {
            // Dividir la expresión en tokens y agregarlos a la polaca
            char expCopia[TAM_CONTENIDO_PILA];
            strncpy(expCopia, exp, TAM_CONTENIDO_PILA - 1);
            expCopia[TAM_CONTENIDO_PILA - 1] = '\0';
            
            // Tokenizar la expresión (separada por espacios)
            char *token = strtok(expCopia, " ");
            while (token != NULL)
            {
                insertarEnPolaca(token);
                token = strtok(NULL, " ");
            }
            
            // Guardar resultado en variable temporal
            char varTemp[20];
            snprintf(varTemp, 20, "@exp%d", i);
            insertarEnPolaca(varTemp);
            insertarEnPolaca(":=");
        }
    }

    // Paso 2: Comparar todas las expresiones entre sí
    // Guardamos los índices de los saltos BEQ para actualizarlos después
    int saltosBEQ[100]; // Array para guardar índices de saltos cuando son iguales
    int numSaltos = 0;
    
    for (int i = 0; i < cantidadExpresiones - 1; i++)
    {
        for (int j = i + 1; j < cantidadExpresiones; j++)
        {
            // Comparar @expi con @expj
            char varTemp1[20], varTemp2[20];
            snprintf(varTemp1, 20, "@exp%d", i);
            snprintf(varTemp2, 20, "@exp%d", j);
            
            insertarEnPolaca(varTemp1);
            insertarEnPolaca(varTemp2);
            insertarEnPolaca("CMP");
            
            // BEQ = Branch if Equal - si son iguales, salta a poner @equal=1
            insertarEnPolaca("BEQ");
            
            // Guardar el índice actual para actualizar después
            char *indiceActual = getIndiceActualPolaca();
            saltosBEQ[numSaltos++] = atoi(indiceActual);
            avanzarPolaca(); // Reservar espacio para el número de salto
        }
    }
    
    // Paso 3: Si llegamos aquí, no hay expresiones iguales
    insertarEnPolaca("0");
    insertarEnPolaca("@equal");
    insertarEnPolaca(":=");
    
    // Saltar al final (después de @equal=1)
    insertarEnPolaca("BI");
    char *indiceSaltoFinal = getIndiceActualPolaca();
    int idxSaltoFinal = atoi(indiceSaltoFinal);
    avanzarPolaca();
    
    // Paso 4: Etiqueta para cuando SÍ son iguales
    char *indiceIguales = getIndiceActualPolaca();
    int idxIguales = atoi(indiceIguales);
    
    insertarEnPolaca("1");
    insertarEnPolaca("@equal");
    insertarEnPolaca(":=");
    
    // Paso 5: Etiqueta final
    char *indiceFinal = getIndiceActualPolaca();
    int idxFinal = atoi(indiceFinal);
    
    // Paso 6: Actualizar todos los saltos
    // Actualizar los BEQ para que salten a la etiqueta de iguales
    for (int i = 0; i < numSaltos; i++)
    {
        char valorSalto[20];
        snprintf(valorSalto, 20, "%d", idxIguales);
        insertarEnPolacaIndice(saltosBEQ[i], valorSalto);
    }
    
    // Actualizar el BI para que salte al final
    char valorSaltoFinal[20];
    snprintf(valorSaltoFinal, 20, "%d", idxFinal);
    insertarEnPolacaIndice(idxSaltoFinal, valorSaltoFinal);
    
    // Paso 7: Insertar @equal como valor de retorno de la función
    insertarEnPolaca("@equal");
    
    printf("[DEBUG] Código equalExpressions generado con %d comparaciones\n", numSaltos);
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


