#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./helper_equal_expressions.h"

extern t_polaca *polaca;

static t_lista_expresiones *listaExpresionesGlobal = NULL;

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

t_lista_expresiones *obtenerListaExpresiones()
{
    return listaExpresionesGlobal;
}

void liberarListaExpresiones()
{
    if (listaExpresionesGlobal != NULL)
    {
        eliminarListaExpresiones(listaExpresionesGlobal);
        listaExpresionesGlobal = NULL;
    }
}

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
}

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
    }

    // Eliminar desde INICIO_EXP hasta el final de la polaca
    eliminarDesdeNodoHastaFinal(nodoInicioExp);
}

void generarCodigoEqualExpressions_2()
{
    if (listaExpresionesGlobal == NULL || listaExpresionesGlobal->cantidad < 2)
    {
        fprintf(stderr, "Error: Se necesitan al menos 2 expresiones para equalExpressions\n");
        // Por defecto, asignar 0 (no hay iguales)
        insertarEnPolaca("0");
        insertarEnPolaca("@equal");
        insertarEnPolaca(":=");
        insertarEnPolaca("@equal");
        return;
    }

    int cantidadExpresiones = listaExpresionesGlobal->cantidad;

    // Array para guardar los índices de los saltos BEQ
    int saltosBEQ[100];
    int numSaltos = 0;

    // Para cada posición i (desde 0 hasta n-2)
    for (int i = 0; i < cantidadExpresiones - 1; i++)
    {
        const char *expActual = obtenerExpresion(listaExpresionesGlobal, i);
        if (expActual == NULL)
            continue;

        // Evaluar la expresión i y guardarla en @expActual (se queda fija)
        char expCopia[TAM_CONTENIDO_PILA];
        strncpy(expCopia, expActual, TAM_CONTENIDO_PILA - 1);
        expCopia[TAM_CONTENIDO_PILA - 1] = '\0';

        // Insertar tokens de la expresión actual
        char *token = strtok(expCopia, " ");
        while (token != NULL)
        {
            insertarEnPolaca(token);
            token = strtok(NULL, " ");
        }

        // Guardar resultado en @expActual (se mantiene fija para todas las comparaciones)
        insertarEnPolaca("@expActual");
        insertarEnPolaca(":=");

        // Comparar con todas las expresiones siguientes (j > i)
        for (int j = i + 1; j < cantidadExpresiones; j++)
        {
            const char *expSiguiente = obtenerExpresion(listaExpresionesGlobal, j);
            if (expSiguiente == NULL)
                continue;

            // Evaluar la expresión j y guardarla en @expAux
            char expAuxCopia[TAM_CONTENIDO_PILA];
            strncpy(expAuxCopia, expSiguiente, TAM_CONTENIDO_PILA - 1);
            expAuxCopia[TAM_CONTENIDO_PILA - 1] = '\0';

            // Insertar tokens de la expresión auxiliar
            token = strtok(expAuxCopia, " ");
            while (token != NULL)
            {
                insertarEnPolaca(token);
                token = strtok(NULL, " ");
            }

            // Guardar resultado en @expAux
            insertarEnPolaca("@expAux");
            insertarEnPolaca(":=");

            // Comparar @expActual con @expAux
            insertarEnPolaca("@expActual");
            insertarEnPolaca("@expAux");
            insertarEnPolaca("CMP");

            // BEQ = si son iguales, saltar a asignar @equal=1
            insertarEnPolaca("BEQ");

            // Guardar índice del salto para actualizar después
            char *indiceActual = getIndiceActualPolaca();
            saltosBEQ[numSaltos++] = atoi(indiceActual);
            avanzarPolaca(); // Reservar espacio para el destino del salto
        }
    }

    // Si llegamos aquí, no hay expresiones iguales
    insertarEnPolaca("0");
    insertarEnPolaca("@equal");
    insertarEnPolaca(":=");

    // Saltar al final (después de @equal=1)
    insertarEnPolaca("BI");
    char *indiceSaltoFinal = getIndiceActualPolaca();
    int idxSaltoFinal = atoi(indiceSaltoFinal);
    avanzarPolaca();

    // Etiqueta para cuando SÍ son iguales
    char *indiceIguales = getIndiceActualPolaca();
    int idxIguales = atoi(indiceIguales);

    insertarEnPolaca("1");
    insertarEnPolaca("@equal");
    insertarEnPolaca(":=");

    // Etiqueta final (después de ambas asignaciones)
    char *indiceFinal = getIndiceActualPolaca();
    int idxFinal = atoi(indiceFinal);

    // Actualizar todos los saltos BEQ para que apunten a la etiqueta de iguales
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

    // Insertar @equal como valor de retorno de la función
    insertarEnPolaca("@equal");

    insertarVariableASM("@expActual", TIPO_TOKEN_CONST_FLOAT, "", "");
    insertarVariableASM("@expAux", TIPO_TOKEN_CONST_FLOAT, "", "");
    insertarVariableASM("@equal", TIPO_TOKEN_CONST_INT, "", "");

    // Las constantes usadas como retorno las tengo que agregar a la tabla de simbolos porque sino luego no las encuentro para el ASM
    procesarSimbolo("1", TIPO_TOKEN_CONST_INT);
    procesarSimbolo("0", TIPO_TOKEN_CONST_INT);
}
