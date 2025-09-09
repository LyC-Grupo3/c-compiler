#include <stdio.h>
#include <string.h>

#include "./informes.h"

/* -------------------------------------------------------------------------- */
/*                             VARIABLES EXTERNAS                             */
/* -------------------------------------------------------------------------- */
extern FILE *archivo_salida_lexico;
extern FILE *archivo_salida_sintactico;


/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

/* --------------------------------- LEXICO --------------------------------- */
void informarMatchLexico(char *lexema, char *tipo_token)
{
    char mensaje[500];
    sprintf(mensaje, "[LEXICO]       LEXEMA: %-20s - TOKEN: %-15s\n", lexema, tipo_token);
    // printf("%s", mensaje);

    if (archivo_salida_lexico != NULL)
    {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}

void informarErrorNoMatchLexico(char *lexema, int linea)
{
    char msg_error_linea[200];
    char msg_error_msg[200];
    char msg_error_token[500];  // Buffer más grande para lexemas largos

    snprintf(msg_error_linea, sizeof(msg_error_linea), "[LEXICO]     ERROR en línea   : %d\n", linea);
    snprintf(msg_error_msg, sizeof(msg_error_msg), "             Tipo de error    : No se reconoce el caracter\n");
    snprintf(msg_error_token, sizeof(msg_error_token), "             Detalle de error : Caracter problematico '%s'\n", lexema);

    printf("%s", msg_error_linea);
    printf("%s", msg_error_msg);
    printf("%s", msg_error_token);

    if (archivo_salida_lexico != NULL)
    {
        fprintf(archivo_salida_lexico, "%s", msg_error_linea);
        fprintf(archivo_salida_lexico, "%s", msg_error_msg);
        fprintf(archivo_salida_lexico, "%s", msg_error_token);
        fflush(archivo_salida_lexico);
    }
}

void informarErrorValidacion(char *lexema, char *tipo_token, char *mensaje, int linea)
{
    char msg_error_linea[200];
    char msg_error_msg[200];
    char msg_error_tipo[200];
    char msg_error_token[500];  // Buffer más grande para lexemas largos

    snprintf(msg_error_linea, sizeof(msg_error_linea), "[LEXICO]     ERROR en línea      : %d\n", linea);
    snprintf(msg_error_msg, sizeof(msg_error_msg), "             Tipo de error       : Error de validación %s\n", tipo_token);
    snprintf(msg_error_token, sizeof(msg_error_token), "             Detalle de error    : %s - Lexema problematico ''%s'\n", mensaje, lexema);

    printf("%s", msg_error_linea);
    printf("%s", msg_error_msg);
    printf("%s", msg_error_tipo);
    printf("%s", msg_error_token);

    if (archivo_salida_lexico != NULL)
    {
        fprintf(archivo_salida_lexico, "%s", msg_error_linea);
        fprintf(archivo_salida_lexico, "%s", msg_error_msg);
        fprintf(archivo_salida_lexico, "%s", msg_error_tipo);
        fprintf(archivo_salida_lexico, "%s", msg_error_token);
        fflush(archivo_salida_lexico);
    }
}

void informarExitoInsertarSimbolo(const char *nombre, const char *lexema, const char *tipo_agregado)
{
    char mensaje[500];
    sprintf(mensaje, "[SIMBOLO_INS]  NOMBRE: %-20s - TIPO: %-15s\n", nombre, tipo_agregado);
    // printf("%s", mensaje);

    if (archivo_salida_lexico != NULL)
    {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}

void informarDuplicadoSimbolo(const char *nombre)
{
    char mensaje[500];
    sprintf(mensaje, "[SIMBOLO_WAR]  El símbolo '%s' ya existe en la tabla de simbolos\n", nombre);
    // printf("%s", mensaje);

    if (archivo_salida_lexico != NULL)
    {
        fprintf(archivo_salida_lexico, "%s", mensaje);
        fflush(archivo_salida_lexico);
    }
}

/* ------------------------------- SINTACTICO ------------------------------- */
void informarMatchLexicoSintactico(const char *mensaje1, const char *mensaje2)
{
    char mensaje_formateado[500];
    sprintf(mensaje_formateado, "[SINTACTICO]   %30s -> %-30s\n", mensaje1, mensaje2);

    printf("%s", mensaje_formateado);
    if (archivo_salida_sintactico != NULL)
    {
        fprintf(archivo_salida_sintactico, "%s", mensaje_formateado);
        fflush(archivo_salida_sintactico);
    }
}

void informarErrorSintactico(const char *message, const char *token_problema, int linea)
{
    char msg_error_linea[200];
    char msg_error_msg[300];
    char msg_error_token[500];  // Buffer más grande para tokens largos

    snprintf(msg_error_linea, sizeof(msg_error_linea), "[SINTACTICO] ERROR en línea     : %d\n", linea);
    snprintf(msg_error_msg, sizeof(msg_error_msg), "             Tipo de error      : %s\n", message);
    snprintf(msg_error_token, sizeof(msg_error_token), "             Token problemático : '%s'\n", token_problema);

    printf("%s", msg_error_linea);
    printf("%s", msg_error_msg);
    printf("%s", msg_error_token);

    if (archivo_salida_sintactico != NULL)
    {
        fprintf(archivo_salida_sintactico, "%s", msg_error_linea);
        fprintf(archivo_salida_sintactico, "%s", msg_error_msg);
        fprintf(archivo_salida_sintactico, "%s", msg_error_token);
        fflush(archivo_salida_sintactico);
    }
}