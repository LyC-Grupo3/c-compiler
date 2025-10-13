#ifndef HELPER_EQUAL_EXPRESSIONS_H
#define HELPER_EQUAL_EXPRESSIONS_H

#include "./lista_expresiones.h"

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Inicializa la lista global de expresiones para equalExpressions
 */
void inicializarListaExpresiones();

/**
 * @brief Obtiene la lista global de expresiones
 * @return Puntero a la lista global de expresiones
 */
t_lista_expresiones *obtenerListaExpresiones();

/**
 * @brief Libera la lista global de expresiones
 */
void liberarListaExpresiones();

/**
 * @brief Procesa una expresión cuando se completa en el parser
 * Debe llamarse en la acción semántica después de cada expresion en parametros_equal_expressions
 */
void procesarExpresionCompleta();

/**
 * @brief Genera el código en la polaca para evaluar equalExpressions (Versión 1)
 * Compara todas las expresiones usando variables @exp0, @exp1, @exp2, etc.
 * Resultado en @equal (0 o 1)
 */
void generarCodigoEqualExpressions_1();

/**
 * @brief Genera el código en la polaca para evaluar equalExpressions (Versión 2)
 * Solo usa dos variables: @expActual y @expAux
 * Resultado en @equal (0 o 1)
 */
void generarCodigoEqualExpressions_2();

#endif // HELPER_EQUAL_EXPRESSIONS_H
