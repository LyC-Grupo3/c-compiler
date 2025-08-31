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

int existeSimbolo(const t_tabla_simbolos* tabla, const char* nombre);
int tablaSimbolosLlena(const t_tabla_simbolos* tabla);

// HANDLERS SEGUN TIPO LEXEMA
int procesarLexemaTablaID(t_tabla_simbolos* tabla, const char* lexema);
int procesarLexemaTablaConstanteInt(t_tabla_simbolos* tabla, const char* lexema);
int procesarLexemaTablaConstanteFloat(t_tabla_simbolos* tabla, const char* lexema);
int procesarLexemaTablaConstanteString(t_tabla_simbolos* tabla, const char* lexema);


#endif // LISTA_H
