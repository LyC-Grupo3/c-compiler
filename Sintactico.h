#ifndef SINTACTICO
#define SINTACTICO

#include "./utils/archivos.h"
#include "./utils/utils.h"
#include "./utils/informes.h"

#include "y.tab.h"

// Utiles para el sintatico
FILE *yyin;
int yylineno;
char *yytext;

int yyerror(const char *msg);
int yylex();

#endif // SINTACTICO