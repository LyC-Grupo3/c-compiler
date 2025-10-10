#include <stdio.h>
#include <string.h>

#include "./simbolo.h"

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */
t_simbolo crearSimbolo(const char *nombre, const char *tipoDato, const char *valor, const char *longitud)
{
    t_simbolo simbolo;

    memset(&simbolo, 0, sizeof(t_simbolo));

    // TODO: revisar si es correcto ya que estaria truncando
    if (nombre != NULL)
    {
        strncpy(simbolo.nombre, nombre, MAX_LONG_NOMBRE_SIMBOLO - 1);
        simbolo.nombre[MAX_LONG_NOMBRE_SIMBOLO - 1] = '\0';
    }

    if (tipoDato != NULL)
    {
        strncpy(simbolo.tipoDato, tipoDato, MAX_LONG_TIPO_SIMBOLO - 1);
        simbolo.tipoDato[MAX_LONG_TIPO_SIMBOLO - 1] = '\0';
    }

    if (valor != NULL)
    {
        strncpy(simbolo.valor, valor, MAX_LONG_VALOR_SIMBOLO - 1);
        simbolo.valor[MAX_LONG_VALOR_SIMBOLO - 1] = '\0';
    }

    if (longitud != NULL)
    {
        strncpy(simbolo.longitud, longitud, MAX_LONG_LONGITUD_SIMBOLO - 1);
        simbolo.longitud[MAX_LONG_LONGITUD_SIMBOLO - 1] = '\0';
    }

    return simbolo;
}