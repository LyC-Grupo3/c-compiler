#include <stdio.h>
#include <string.h>

#include "./informes.h"

/* -------------------------------------------------------------------------- */
/*                             VARIABLES EXTERNAS                             */
/* -------------------------------------------------------------------------- */
extern FILE *archivo_salida_lexico;
extern int yylineno;



/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */
void informarMatchLexico(char* lexema, char* nombreDelToken) {
    char mensaje[500];
    sprintf(mensaje, "[LEXICO]       LEXEMA: %-20s - TOKEN: %-15s\n", lexema, nombreDelToken);
    printf("%s", mensaje);

    if (archivo_salida_lexico != NULL) {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}

void informarErrorNoMatch(char* lexema)
{
    char mensaje[500];
    sprintf(mensaje, "[LEXICO]       LEXEMA: %-20s - LINEA: %-10d - ERROR NO-MATCH\n", lexema, yylineno);
    printf("%s", mensaje);

    if (archivo_salida_lexico != NULL) {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}

void informarErrorValidacion(char* name_token)
{
    char mensaje[500];
    sprintf(mensaje, "[LEXICO]       LINEA:  %-20d - ERROR VALIDACION: %-20s\n", yylineno, name_token);
    printf("%s", mensaje);

    if (archivo_salida_lexico != NULL) {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}

void informarExitoInsertarSimbolo(const char* nombre, const char* lexema, const char* tipo_agregado)
{
    char mensaje[500];
    sprintf(mensaje, "[SIMBOLO]      NOMBRE: %-20s - TIPO: %-15s\n", nombre, tipo_agregado);
    printf("%s", mensaje);
    
    if (archivo_salida_lexico != NULL) {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}

void informarDuplicadoSimbolo(const char* nombre)
{
    char mensaje[500];
    sprintf(mensaje, "[SIMBOLO]      Warning: El símbolo '%s' ya existe en la tabla de simbolos\n", nombre);
    printf("%s", mensaje);
    
    if (archivo_salida_lexico != NULL) {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}