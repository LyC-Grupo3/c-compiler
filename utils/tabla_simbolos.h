#ifndef LISTA_H
#define LISTA_H



#include "simbolo.h"

/* -------------------------------------------------------------------------- */
/*                                   DEFINES                                  */
/* -------------------------------------------------------------------------- */

#define MAX_LISTA 1000


/* -------------------------------------------------------------------------- */
/*                                 ESTRUCTURAS                                */
/* -------------------------------------------------------------------------- */

typedef struct {
    t_simbolo elementos[MAX_LISTA];
    int cantidad;
} t_tabla_simbolos;


/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

// MANEJO DE TABLA
void inicializarTablaSimbolos(t_tabla_simbolos* tabla);

int insertarSimbolo(t_tabla_simbolos* tabla, const t_simbolo* simbolo);

int existeSimboloPorNombre(const t_tabla_simbolos* tabla, const char* nombre);
int existeSimboloPorTipoValor(const t_tabla_simbolos* tabla, const char* tipo_dato, const char* valor);

int tablaSimbolosLlena(const t_tabla_simbolos* tabla);



#endif // LISTA_H
