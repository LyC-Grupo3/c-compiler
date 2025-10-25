#ifndef SINTACTICO
#define SINTACTICO

#include <stdio.h>
#include "./archivos.h"
#include "./utils.h"
#include "./informes.h"
#include "./polaca.h"
#include "./pila.h"
#include "./helper_triangle.h"
#include "./helper_control_flow.h"
#include "./helper_init_variables.h"
#include "./helper_tipos_datos.h"
#include "./helper_operador_comparacion.h"
#include "./helper_operador_logico.h"
#include "./helper_equal_expressions.h"
#include "./assembler.h"

#include "../y.tab.h"

// Utiles para el sintatico
FILE *yyin;
int yylineno;
char *yytext;

int yyerror(const char *msg);
int yylex();

void setearNroCeldaActualPolacaAuxInicioTrueSoloParaOR();
void desapilarNroCeldaYEscribirEnEllaNroCeldaAuxInicioTrueOR(t_pila *pila);

#endif // SINTACTICO