#ifndef SINTACTICO
#define SINTACTICO

#include <stdio.h>
#include <stdlib.h>

#include "./utils/utils.h"
#include "./utils/informes.h"
#include "./utils/validacion.h"
#include "./utils/tabla_simbolos.h"

#include "y.tab.h"

// Lexico.l
extern t_tabla_simbolos tabla_simbolos;
extern FILE *archivo_salida_lexico;

// Sintactico.y
FILE *archivo_salida_sintactico;

// Utiles para el sintatico
FILE *yyin;
int yylineno;
char *yytext;

int yyerror(const char *msg);
int yylex();

//  Nuestras
void informarMatchLexicoSintactico(const char *mensaje1, const char *mensaje2);

void abrirArchivoSalidaSintactico(const char *nombre_archivo);
void cerrarArchivoSalidaLexicoSintactico(void);

#endif // SINTACTICO