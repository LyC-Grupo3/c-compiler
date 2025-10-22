#ifndef HELPER_EQUAL_EXPRESSIONS_H
#define HELPER_EQUAL_EXPRESSIONS_H

#include "./lista_expresiones.h"
#include "./polaca.h"
#include "./tabla_simbolos.h"

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Busca hacia atrás el último nodo con INICIO_EXP en la polaca
 * @return Puntero al nodo con INICIO_EXP o NULL si no se encuentra
 */
static t_nodo_polaca *buscarUltimoInicioExp();

/**
 * @brief Extrae y construye una expresión desde INICIO_EXP hasta el final de la polaca
 * @param nodoInicio Nodo donde está INICIO_EXP
 * @param expresion Buffer donde se guardará la expresión
 * @param tamBuffer Tamaño del buffer
 */
static void extraerExpresionDesdeInicio(t_nodo_polaca *nodoInicio, char *expresion, int tamBuffer);

/**
 * @brief Elimina nodos desde un nodo específico hasta el final de la polaca
 * @param nodoInicio Nodo desde donde empezar a eliminar (inclusive)
 */
static void eliminarDesdeNodoHastaFinal(t_nodo_polaca *nodoInicio);

/**
 * @brief Inicializa la lista global de expresiones
 */
void inicializarListaExpresiones();

/**
 * @brief Obtiene la lista global de expresiones
 * @return Puntero a la lista global
 */
t_lista_expresiones *obtenerListaExpresiones();

/**
 * @brief Libera la lista global de expresiones
 */
void liberarListaExpresiones();

/**
 * @brief Genera código en la polaca para comparar todas las expresiones
 * y determinar si hay dos iguales
 * Resultado se guarda en @equal (1 si hay iguales, 0 si no)
 */
void generarCodigoEqualExpressions_1();


/**
 * @brief Procesa una expresión cuando se completa en el parser
 * Extrae desde el último INICIO_EXP hasta el final, guarda en lista y elimina de polaca
 */
void procesarExpresionCompleta();

/**
 * @brief Genera código en la polaca para comparar todas las expresiones (VERSIÓN 2 OPTIMIZADA)
 * Solo utiliza dos variables: @expActual y @expAux
 * @expActual se mantiene fija mientras @expAux recorre las expresiones siguientes
 * Resultado se guarda en @equal (1 si hay iguales, 0 si no)
 */
void generarCodigoEqualExpressions_2();

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
