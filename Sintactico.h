#ifndef SINTACTICO
#define SINTACTICO

#include "./utils/archivos.h"
#include "./utils/utils.h"
#include "./utils/informes.h"
#include "./utils/polaca.h"
#include "./utils/pila.h"
#include "./utils/helper_triangle.h"
#include "./utils/helper_control_flow.h"
#include "./utils/helper_init_variables.h"

#include "y.tab.h"

// Utiles para el sintatico
FILE *yyin;
int yylineno;
char *yytext;

int yyerror(const char *msg);
int yylex();

void setOperadorComparacionPendientePolaca(char *operador);
char *getOperadorComparacionPendientePolaca();

void apilarNroCeldaActualPolaca(t_pila *pila);
void apilarNroCeldaActualYAvanzarPolaca(t_pila *pila);
int desapilarNroCeldaYEscribirEnEllaNroCeldaActual(t_pila *pila);
int desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(t_pila *pila);
int desapilarNroCeldaYEscribirloEnCeldaActualPolaca(t_pila *pila);

#endif // SINTACTICO