#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./helper_control_flow.h"
#include "./polaca.h"

/* -------------------------------------------------------------------------- */
/*                         DECLARACIONES EXTERNAS                             */
/* -------------------------------------------------------------------------- */

extern t_pila *pilaBase;
extern void apilarNroCeldaActualPolaca(t_pila *pila);
extern void apilarNroCeldaActualYAvanzarPolaca(t_pila *pila);
extern int desapilarNroCeldaYEscribirEnEllaNroCeldaActual(t_pila *pila);
extern int desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(t_pila *pila);
extern int desapilarNroCeldaYEscribirloEnCeldaActualPolaca(t_pila *pila);

/* -------------------------------------------------------------------------- */
/*                      FUNCIONES PARA IF (SELECCIÓN)                         */
/* -------------------------------------------------------------------------- */

void generarCodigoFinCondicionIf()
{
    // Sentencia de Selección - Fin de Condición
    // 1. Apilar el nº de celda actual
    apilarNroCeldaActualYAvanzarPolaca(pilaBase);
}

void generarCodigoFinBloqueVerdaderoIfConElse()
{
    // Sentencia de Selección con else - Fin del bloque verdadero
    insertarEnPolaca("BI");

    // 1. Desapilar X (tope de la pila)
    // 2. Escribir en la celda X, el nº de celda actual + 1
    desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(pilaBase);

    // 3. Apilar el nº de celda actual
    apilarNroCeldaActualYAvanzarPolaca(pilaBase);
}

void generarCodigoFinBloqueVerdaderoIfSinElse()
{
    // Sentencia de Selección (sin else) - Fin del bloque verdadero
    // 1. Desapilar X (tope de la pila)
    // 2. Escribir en la celda X, el nº de celda actual
    desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);
}

void generarCodigoFinBloqueElse()
{
    // Sentencia de Selección con else - Fin del bloque falso
    // 1. Desapilar X (tope de la pila)
    // 2. Escribir en la celda X, el nº de celda actual
    desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);
}

/* -------------------------------------------------------------------------- */
/*                      FUNCIONES PARA WHILE (ITERACIÓN)                      */
/* -------------------------------------------------------------------------- */

void generarCodigoInicioWhile()
{
    // Sentencias de Iteración (while) - Comienzo
    // 1. Apilar el nº celda actual
    apilarNroCeldaActualPolaca(pilaBase);

    insertarEnPolaca("ET");
}

void generarCodigoFinCondicionWhile()
{
    // Sentencias de Iteración (while) - Fin de la Condición
    // 1. Apilar el nº celda actual
    apilarNroCeldaActualYAvanzarPolaca(pilaBase);
}

void generarCodigoFinWhile()
{
    // Sentencias de Iteración (while) - Fin del ciclo
    insertarEnPolaca("BI");

    // 1. Desapilar Z (tope de la pila)
    // 2. Escribir en la celda Z, el nº de celda actual + 1
    desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(pilaBase);

    // 3. Desapilar Y (tope de la pila)
    // 4. Escribir en la celda actual el valor Y (para volver al inicio)
    desapilarNroCeldaYEscribirloEnCeldaActualPolaca(pilaBase);
}
