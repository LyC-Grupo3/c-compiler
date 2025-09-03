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

programa: init conjunto_sentencias {informarMatchLexicoSintactico("SINTAXIS OK");}
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