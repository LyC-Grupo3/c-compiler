#ifndef UTILS_H
#define UTILS_H



#include "tabla_simbolos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIPO_TOKEN_ID "ID"
#define TIPO_TOKEN_CONST_INT "CONST_INT"
#define TIPO_TOKEN_CONST_FLOAT "CONST_FLOAT"
#define TIPO_TOKEN_CONST_STR "CONST_STR"

// Programa
extern t_tabla_simbolos tabla_simbolos;

static void formatearSimbolo(const char* lexema, const char* tipo_token, 
                            char* nombre_simbolo, char* tipo_simbolo, 
                            char* valor_simbolo, int* longitud_simbolo,
                            const char** tipo_para_informe);

void procesarSimbolo(const char* lexema, const char* tipo_token);

void crearTablaSimbolos(void);

void crearTablaSimbolos(void);

void abrirArchivoSalidaLexico(const char* nombre_archivo);

void cerrarArchivoSalidaLexico(void);

int exportarTablaSimbolos(const char* nombre_archivo);



#endif // UTILS_Hs
