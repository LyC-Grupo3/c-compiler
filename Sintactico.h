#ifndef SINTACTICO
#define SINTACTICO

#include <stdio.h>
#include "./utils/archivos.h"
#include "./utils/utils.h"
#include "./utils/informes.h"
#include "./utils/polaca.h"
#include "./utils/pila.h"
#include "./utils/helper_triangle.h"
#include "./utils/helper_control_flow.h"
#include "./utils/helper_init_variables.h"
#include "./utils/helper_tipos_datos.h"
#include "./utils/helper_operador_comparacion.h"
#include "./utils/helper_operador_logico.h"
#include "./utils/helper_equal_expressions.h"

#include "y.tab.h"

// Utiles para el sintatico
FILE *yyin;
int yylineno;
char *yytext;

int yyerror(const char *msg);
int yylex();

void setearNroCeldaActualPolacaAuxInicioTrueSoloParaOR();
void desapilarNroCeldaYEscribirEnEllaNroCeldaAuxInicioTrueOR(t_pila *pila);

#endif // SINTACTICO