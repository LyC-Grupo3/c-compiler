#ifndef SIMBOLO_H
#define SIMBOLO_H

/* -------------------------------------------------------------------------- */
/*                                   DEFINES                                  */
/* -------------------------------------------------------------------------- */

#define MAX_LONG_NOMBRE_SIMBOLO 100
#define MAX_LONG_TIPO_SIMBOLO 20
#define MAX_LONG_VALOR_SIMBOLO 100

/* -------------------------------------------------------------------------- */
/*                             ESTRUCTURA SIMBOLO                             */
/* -------------------------------------------------------------------------- */

typedef struct
{
    char nombre[MAX_LONG_NOMBRE_SIMBOLO];
    char tipoDato[MAX_LONG_TIPO_SIMBOLO];
    char valor[MAX_LONG_VALOR_SIMBOLO];
    int longitud;
} t_simbolo;

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

t_simbolo crearSimbolo(const char *nombre, const char *tipoDato, const char *valor, int longitud);

#endif // SIMBOLO_H
