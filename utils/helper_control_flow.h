#ifndef HELPER_CONTROL_FLOW_H
#define HELPER_CONTROL_FLOW_H

#include "./pila.h"

/* -------------------------------------------------------------------------- */
/*                   FUNCIONES PARA ESTRUCTURAS DE CONTROL                    */
/* -------------------------------------------------------------------------- */

// IF - Funciones para selección con y sin else
void generarCodigoFinCondicionIf();
void generarCodigoFinBloqueVerdaderoIfConElse();
void generarCodigoFinBloqueVerdaderoIfSinElse();
void generarCodigoFinBloqueElse();

// Para compuesto
void generarCodigoFinCondicionIfCOMPUESTO();

// WHILE - Funciones para iteración
void generarCodigoInicioWhile();
void generarCodigoFinCondicionWhile();
void generarCodigoFinWhile();

#endif // HELPER_CONTROL_FLOW_H
