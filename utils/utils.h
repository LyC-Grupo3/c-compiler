#ifndef UTILS_H
#define UTILS_H

#include "tabla_simbolos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/*                                   DEFINES                                  */
/* -------------------------------------------------------------------------- */

#define TIPO_TOKEN_ID "ID"
#define TIPO_TOKEN_CONST_INT "CONST_INT"
#define TIPO_TOKEN_CONST_FLOAT "CONST_FLOAT"
#define TIPO_TOKEN_CONST_STR "CONST_STR"

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

void formatearSimbolo(const char *lexema, const char *tipo_token,
                      char *nombre_simbolo, char *tipo_simbolo,
                      char *valor_simbolo, char *longitud_simbolo,
                      const char **tipo_para_informe);

#endif // UTILS_Hs
