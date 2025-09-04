/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{
#include <stdio.h>
#include <stdlib.h>

#include "./utils/utils.h"
#include "./utils/informes.h"
#include "./utils/validacion.h"
#include "./utils/tabla_simbolos.h"

#include "y.tab.h"

int yystopparser=0;
FILE  *yyin;

// Lexico.l
extern t_tabla_simbolos tabla_simbolos;
extern FILE *archivo_salida_lexico;

// Sintactico.y
FILE *archivo_salida_sintactico;

// Utiles para el sintatico
//int yyerror();
int yyerror(const char *s);
 int yylex();

 //  Nuestras
void abrirArchivoSalidaLexico(const char* nombre_archivo);
void cerrarArchivoSalidaLexico(void);

void informarMatchLexicoSintactico(const char* mensaje);

void abrirArchivoSalidaSintactico(const char* nombre_archivo);
void cerrarArchivoSalidaLexicoSintactico(void);

%}




/* -------------------------------------------------------------------------- */
/*                              SECCION DE TOKENS                             */
/* -------------------------------------------------------------------------- */

/* ---------------------------- TIPOS DE DATOS ----------------------------- */
%token INT FLOAT STRING BOOL

/* --------------------------- CONECTORES LOGICOS --------------------------- */
%token AND OR NOT

/* --------------------------- PALABRAS RESERVADAS -------------------------- */
%token WHILE IF ELSE INIT WRITE READ

/* ---------------------------- CONSTANTES BOOLEANAS ------------------------ */
%token CONST_BOOL_T CONST_BOOL_F

/* ---------------------------- NOMBRES FUNCIONES --------------------------- */
%token EQUAL_EXP TRIAN_MAX

/* --------------------------------- BASICOS -------------------------------- */
%token ID

/* -------------------------------- CONSTANTES ------------------------------- */
%token CONST_INT CONST_FLOAT CONST_STR

/* ---------------------------- OPERADORES ASIGNACION ----------------------- */
%token OP_ASIG_VALOR OP_ASIG_TIPO

/* --------------------------- COMENTARIOS ---------------------------------- */
%token COMENTARIO

/* ------------------------- OPERADORES ARITMETICOS ------------------------- */
%token OP_SUM OP_MUL OP_RES OP_DIV

/* --------------------------- OPERADORES LOGICOS --------------------------- */
%token OP_MAYOR OP_MAYOR_IGUAL OP_MENOR OP_MENOR_IGUAL OP_IGUAL OP_DISTINTO

/* --------------------------------- BLOQUES -------------------------------- */
%token PAR_A PAR_C LLA_A LLA_C COR_A COR_C

/* ------------------------------- PUNTUACION ------------------------------- */
%token PUNTO_C COMA




/* -------------------------------------------------------------------------- */
/*                              SECCION DE REGLAS                             */
/* -------------------------------------------------------------------------- */

%%

start: programa {informarMatchLexicoSintactico("\"programa\" -> \"start\"");}
    ;

programa:   init
            | init conjunto_sentencias {informarMatchLexicoSintactico("SINTAXIS OK");}
    ;

conjunto_sentencias:    sentencia {informarMatchLexicoSintactico("\"sentencia\" -> \"conjunto_sentencias\"");}
                        | conjunto_sentencias sentencia {informarMatchLexicoSintactico("\"conjunto_sentencias sentencia\" -> \"conjunto_sentencias\"");}
    ;

sentencia:   asignacion {informarMatchLexicoSintactico("\"asignacion\" -> \"sentencia\"");}
            | bloque_if bloque_else {informarMatchLexicoSintactico("\"bloque_if bloque_else\" -> \"sentencia\"");}
            | bloque_if {informarMatchLexicoSintactico("\"bloque_if\" -> \"sentencia\"");}
            | bloque_while {informarMatchLexicoSintactico("\"bloque_while\" -> \"sentencia\"");}
            | funcion_read {informarMatchLexicoSintactico("\"funcion_read\" -> \"sentencia\"");}
            | funcion_write {informarMatchLexicoSintactico("\"funcion_write\" -> \"sentencia\"");}
    ;

/* ------------------------------- ARITHMETIC ------------------------------- */
expresion:   expresion OP_SUM termino {informarMatchLexicoSintactico("\"expresion OP_SUM termino\" -> \"expresion\"");}
            | expresion OP_RES termino {informarMatchLexicoSintactico("\"expresion OP_RES termino\" -> \"expresion\"");}
            | termino {informarMatchLexicoSintactico("\"termino\" -> \"expresion\"");}
    ;

termino:    termino OP_MUL factor {informarMatchLexicoSintactico("\"termino OP_MUL factor\" -> \"termino\"");}
            | termino OP_DIV factor {informarMatchLexicoSintactico("\"termino OP_DIV factor\" -> \"termino\"");}
            | factor {informarMatchLexicoSintactico("\"factor\" -> \"termino\"");}
    ;

factor:     ID {informarMatchLexicoSintactico("\"ID\" -> \"factor\"");}
            | CONST_INT {informarMatchLexicoSintactico("\"CONST_INT\" -> \"factor\"");}
            | CONST_FLOAT {informarMatchLexicoSintactico("\"CONST_FLOAT\" -> \"factor\"");}
            | PAR_A expresion PAR_C {informarMatchLexicoSintactico("\"PAR_A expresion PAR_C\" -> \"factor\"");}
    ;

/* ------------------------------- ASSIGMENTS ------------------------------- */
asignacion: ID OP_ASIG_VALOR expresion {informarMatchLexicoSintactico("\"ID OP_ASIG_VALOR expresion\" -> \"asignacion\""); }
            | ID OP_ASIG_VALOR CONST_STR {informarMatchLexicoSintactico("\"ID OP_ASIG_VALOR CONST_STR\" -> \"asignacion\""); }
            | ID OP_ASIG_VALOR funcion_equal_expressions {informarMatchLexicoSintactico("\"ID OP_ASIG_VALOR funcion_equal_expressions\" -> \"asignacion\""); }
            | ID OP_ASIG_VALOR funcion_triangle_area_maximum {informarMatchLexicoSintactico("\"ID OP_ASIG_VALOR funcion_triangle_area_maximum\" -> \"asignacion\""); }
    ;

/* ---------------------------------- INIT ---------------------------------- */

init: INIT LLA_A conjunto_declaraciones LLA_C {informarMatchLexicoSintactico("\"INIT LLAVE_A conjunto_declaraciones LLAVE_C\" -> \"init\"");}
    ;

conjunto_declaraciones:  conjunto_declaraciones declaracion {informarMatchLexicoSintactico("\"conjunto_declaraciones declaracion\" -> \"conjunto_declaraciones\"");}
                        | declaracion {informarMatchLexicoSintactico("\"declaracion\" -> \"conjunto_declaraciones\"");}
    ;

declaracion: variables OP_ASIG_TIPO tipo_dato {informarMatchLexicoSintactico("\"variables OP_ASIG_TIPO tipo_dato\" -> \"declaracion\"");}
    ;

variables: variables COMA ID {informarMatchLexicoSintactico("\"variables COMA ID\" -> \"variables\"");}
            | ID {informarMatchLexicoSintactico("\"ID\" -> \"variables\"");}
    ;

tipo_dato:  INT {informarMatchLexicoSintactico("\"INT\" -> \"tipo_dato\"");}
            | FLOAT {informarMatchLexicoSintactico("\"FLOAT\" -> \"tipo_dato\"");}
            | STRING {informarMatchLexicoSintactico("\"STRING\" -> \"tipo_dato\"");}
    ;

/* ----------------------------------- IF ----------------------------------- */
bloque_if: IF PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C {informarMatchLexicoSintactico("\"IF PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C\" -> \"bloque_if\"");}
    ;

condicional:    condicion {informarMatchLexicoSintactico("\"condicion\" -> \"condicional\"");}
                | condicion operador_logico condicion {informarMatchLexicoSintactico("\"condicion operador_logico condicion\" -> \"condicional\"");}
                | NOT condicion {informarMatchLexicoSintactico("\"NOT condicion\" -> \"condicional\"");}
    ;

condicion: expresion comparador_operacion expresion {informarMatchLexicoSintactico("\"expresion comparador_operacion expresion\" -> \"condicion\"");}
    ;

comparador_operacion:   OP_IGUAL {informarMatchLexicoSintactico("\"OP_IGUAL\" -> \"comparador_operacion\"");}
                        | OP_DISTINTO {informarMatchLexicoSintactico("\"OP_DISTINTO\" -> \"comparador_operacion\"");}
                        | OP_MAYOR {informarMatchLexicoSintactico("\"OP_MAYOR\" -> \"comparador_operacion\"");}
                        | OP_MAYOR_IGUAL {informarMatchLexicoSintactico("\"OP_MAYOR_IGUAL\" -> \"comparador_operacion\"");}
                        | OP_MENOR {informarMatchLexicoSintactico("\"OP_MENOR\" -> \"comparador_operacion\"");}
                        | OP_MENOR_IGUAL {informarMatchLexicoSintactico("\"OP_MENOR_IGUAL\" -> \"comparador_operacion\"");}
    ;

operador_logico: AND {informarMatchLexicoSintactico("\"AND\" -> \"operador_logico\"");}
                 | OR {informarMatchLexicoSintactico("\"OR\" -> \"operador_logico\"");}
    ;

/* ---------------------------------- ELSE ---------------------------------- */
bloque_else: ELSE LLA_A conjunto_sentencias LLA_C {informarMatchLexicoSintactico("\"ELSE LLA_A conjunto_sentencias LLA_C\" -> \"bloque_else\"");}
    ;
    
/* ---------------------------------- WHILE --------------------------------- */
bloque_while: WHILE PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C {informarMatchLexicoSintactico("\"WHILE PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C\" -> \"bloque_while\"");}
    ;

/* ---------------------------------- READ ---------------------------------- */
funcion_read: READ PAR_A ID PAR_C {informarMatchLexicoSintactico("\"READ PAR_A ID PAR_C\" -> \"funcion_read\"");}
    ;

/* ---------------------------------- WRITE --------------------------------- */
funcion_write:  WRITE PAR_A CONST_STR PAR_C {informarMatchLexicoSintactico("\"WRITE PAR_A CTE_STRING PAR_C\" -> \"write\"");}
        | WRITE PAR_A ID PAR_C {informarMatchLexicoSintactico("\"WRITE PAR_A ID PAR_C\" -> \"write\"");}
    ;

/* ---------------------------- EQUAL EXPRESSIONS --------------------------- */
funcion_equal_expressions:  EQUAL_EXP PAR_A parametros_equal_expressions PAR_C {informarMatchLexicoSintactico("\"EQUAL_EXP PAR_A parametros_equal_expressions PAR_C\" -> \"funcion_equal_expressions\"");}

parametros_equal_expressions:   expresion COMA expresion {informarMatchLexicoSintactico("\"expresion COMA expresion\" -> \"parametros_equal_expressions\"");}
                                | parametros_equal_expressions COMA expresion {informarMatchLexicoSintactico("\"parametros_equal_expressions COMA expresion\" -> \"parametros_equal_expressions\"");}
    ;

/* ---------------------- FUNCION TRIANGLE AREA MAXIMUM --------------------- */
funcion_triangle_area_maximum: TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C {informarMatchLexicoSintactico("\"TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C\" -> \"funcion_triangle_area_maximum\"");}
    ;

parametros_triangle_area_maximum:  coordenadas_triangulo PUNTO_C coordenadas_triangulo {informarMatchLexicoSintactico("\"COR_A conjunto_puntos COR_C PUNTO_C COR_A conjunto_puntos COR_C\" -> \"parametros_triangle_area\"");}
    ;

coordenadas_triangulo: COR_A conjunto_puntos_triangulo COR_C {informarMatchLexicoSintactico("\"COR_A conjunto_puntos COR_C\" -> \"coordenadas_triangulo\"");}
    ;

conjunto_puntos_triangulo: coordenada_triangulo PUNTO_C coordenada_triangulo PUNTO_C coordenada_triangulo {informarMatchLexicoSintactico("\"coordenada_triangulo PUNTO_C coordenada_triangulo PUNTO_C coordenada_triangulo\" -> \"conjunto_puntos_triangle_area\"");}
    ;

coordenada_triangulo: tipo_parametro_triangle_area COMA tipo_parametro_triangle_area {informarMatchLexicoSintactico("\"tipo_parametro_triangle_area COMA tipo_parametro_triangle_area\" -> \"coordenada_triangulo\"");}
    ;

tipo_parametro_triangle_area:   CONST_INT {informarMatchLexicoSintactico("\"CONST_INT\" -> \"tipo_parametro_triangle_area\"");}
                                | CONST_FLOAT {informarMatchLexicoSintactico("\"CONST_FLOAT\" -> \"tipo_parametro_triangle_area\"");} 
                                | ID {informarMatchLexicoSintactico("\"ID\" -> \"tipo_parametro_triangle_area\"");}
    ;


%%


/* -------------------------------------------------------------------------- */
/*                          FUNCIONES PARA EL SINTACTICO                      */
/* ------------------------------------------------------------------------- */

void informarMatchLexicoSintactico(const char* mensaje) {
    char mensaje_formateado[500];
    sprintf(mensaje_formateado, "[SINTACTICO]   %s\n", mensaje);

    printf("%s", mensaje_formateado);
    if (archivo_salida_sintactico != NULL) {
        fprintf(archivo_salida_sintactico, "%s", mensaje_formateado);
        fflush(archivo_salida_sintactico);
    }
}

void abrirArchivoSalidaSintactico(const char* nombre_archivo) {
    archivo_salida_sintactico = fopen(nombre_archivo, "w");
    if (archivo_salida_sintactico == NULL) {-
        printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
    } else {
        fprintf(archivo_salida_sintactico, "=== ANÁLISIS SINTÁCTICO ===\n");
        fflush(archivo_salida_sintactico);
    }
}

void cerrarArchivoSalidaLexicoSintactico(void) {
    if (archivo_salida_sintactico != NULL) {
        fclose(archivo_salida_sintactico);
        archivo_salida_sintactico = NULL;
    }
}




/* -------------------------------------------------------------------------- */
/*                          CODIGO PARA LA EJECUCCION                         */
/* -------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
    if((yyin = fopen(argv[1], "rt"))==NULL)
    {
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
        return 1;
    }
    else
    { 
        crearTablaSimbolos();
        abrirArchivoSalidaLexico("test_outputs/output_lexico.txt");
        abrirArchivoSalidaSintactico("test_outputs/output_sintactico.txt");
        
        yyparse();
        
        exportarTablaSimbolos("test_outputs/tabla_simbolos.txt");
        cerrarArchivoSalidaLexico();
        cerrarArchivoSalidaLexicoSintactico();
    }
	fclose(yyin);
        return 0;
}


int yyerror(const char *s)
{
    printf("Error Sintactico: %s\n", s);
    exit(1);
}