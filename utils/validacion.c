
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./validacion.h"


/* -------------------------------------------------------------------------- */
/*                             VARIABLES EXTERNAS                             */
/* -------------------------------------------------------------------------- */
extern void informarErrorValidacion(char* name_token);



/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */
int validarRangoInt(char*ptr)
{
    int valueCasted = atoi(ptr);
    if (valueCasted < INT_MIN || valueCasted > INT_MAX) {
        informarErrorValidacion("CONST_INT");
        return 1;
    }
    return 0;
}

int validarRangoFloat(char*ptr)
{
    float valueCasted = atof(ptr);
    if (valueCasted < FLOAT_MIN || valueCasted > FLOAT_MAX) {
        informarErrorValidacion("CONST_FLOAT");
        return 1;
    }
    return 0;
}

int validarLongitudString(char*ptr)
{
    int length = strlen(ptr) - 2; // -2 para no contar las comillas de apertura y cierre
    if (length > STRING_MAX_LENGTH) {
        informarErrorValidacion("CONST_STR");
        return 1;
    }
    return 0;
}