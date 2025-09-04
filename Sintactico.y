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

void informarMatchLexicoSintactico(const char* mensaje1, const char* mensaje2);

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

start:
    programa                                            {informarMatchLexicoSintactico("SINTAXIS OK", "SINTAXIS OK");}
    ;

programa:
     init conjunto_sentencias
    |init
    ;

conjunto_sentencias:
    conjunto_sentencias sentencia 
    |sentencia
                         
    ;

sentencia:
    bloque_if_else
    | bloque_if
    | bloque_while
    | asignacion
    | funcion_read
    | funcion_write
    ;

/* ------------------------------- ARITHMETIC ------------------------------- */
expresion:
    expresion OP_SUM termino                            {informarMatchLexicoSintactico("expresion", "expresion OP_SUM termino");}
    | expresion OP_RES termino                          {informarMatchLexicoSintactico("expresion", "expresion OP_RES termino");}
    | termino                                           {informarMatchLexicoSintactico("expresion", "termino");}
    ;

termino:
    termino OP_MUL factor
    | termino OP_DIV factor
    | factor
    ;

factor:
    ID
    | CONST_INT
    | CONST_FLOAT
    | PAR_A expresion PAR_C
    ;

/* ------------------------------- ASSIGMENTS ------------------------------- */
asignacion:
    ID OP_ASIG_VALOR expresion                          {informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR expresion");}
    | ID OP_ASIG_VALOR CONST_STR                        {informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR CONST_STR");}
    | ID OP_ASIG_VALOR funciones_temas_especiales       {informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR funciones_temas_especiales");}
    ;

/* ---------------------------------- INIT ---------------------------------- */

init:                       
    INIT LLA_A conjunto_declaraciones LLA_C             {informarMatchLexicoSintactico("init", "INIT LLA_A conjunto_declaraciones LLA_C");}
    ;

conjunto_declaraciones:
    conjunto_declaraciones declaracion
    | declaracion
    ;

declaracion:
    conjunto_ids OP_ASIG_TIPO tipo_dato
    ;

conjunto_ids:
    conjunto_ids COMA ID
    | ID
    ;

tipo_dato:
    INT
    | FLOAT
    | STRING
    ;

/* ----------------------------------- IF ----------------------------------- */
bloque_if:
    IF PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C      {informarMatchLexicoSintactico("bloque_if", "IF PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C");}
    ;

condicional:
    condicion
    | condicion operador_logico condicion
    | NOT condicion
    ;

condicion:
    expresion operador_comparacion expresion
    ;

operador_comparacion:
    OP_IGUAL
    | OP_DISTINTO
    | OP_MAYOR
    | OP_MAYOR_IGUAL
    | OP_MENOR
    | OP_MENOR_IGUAL
    ;

operador_logico:
    AND
    | OR
    ;

/* ---------------------------------- ELSE ---------------------------------- */
bloque_else:
    ELSE LLA_A conjunto_sentencias LLA_C                            {informarMatchLexicoSintactico("bloque_else", "ELSE LLA_A conjunto_sentencias LLA_C");}
    ;

/* --------------------------------- IF ELSE -------------------------------- */
bloque_if_else:
    bloque_if bloque_else                                           {informarMatchLexicoSintactico("bloque_if_else", "bloque_if bloque_else");}
    ;
    
/* ---------------------------------- WHILE --------------------------------- */
bloque_while:
    WHILE PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C   {informarMatchLexicoSintactico("bloque_while", "WHILE PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C");}
    ;

/* ---------------------------------- READ ---------------------------------- */
funcion_read:
    READ PAR_A ID PAR_C                                             {informarMatchLexicoSintactico("funcion_read", "READ PAR_A ID PAR_C");}
    ;

/* ---------------------------------- WRITE --------------------------------- */
funcion_write:
    WRITE PAR_A CONST_STR PAR_C                                     {informarMatchLexicoSintactico("funcion_write", "WRITE PAR_A CONST_STR PAR_C");}
    | WRITE PAR_A ID PAR_C
    ;

/* ---------------------------- TEMAS ESPECIALES ---------------------------- */
funciones_temas_especiales:
    funcion_equal_expressions
    | funcion_triangle_area_maximum
    ;

/* ---------------------------- EQUAL EXPRESSIONS --------------------------- */
funcion_equal_expressions:
    EQUAL_EXP PAR_A parametros_equal_expressions PAR_C              {informarMatchLexicoSintactico("funcion_equal_expressions", "EQUAL_EXP PAR_A parametros_equal_expressions PAR_C");}
    ;

parametros_equal_expressions:
    expresion COMA expresion
    | parametros_equal_expressions COMA expresion
    ;


/* ---------------------- FUNCION TRIANGLE AREA MAXIMUM --------------------- */
funcion_triangle_area_maximum:
    TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C          {informarMatchLexicoSintactico("funcion_triangle_area_maximum", "TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C");}
    ;

parametros_triangle_area_maximum:
    coordenadas_triangulo PUNTO_C coordenadas_triangulo
    ;

coordenadas_triangulo:
    COR_A conjunto_puntos_triangulo COR_C
    ;

conjunto_puntos_triangulo:
    coordenada_triangulo PUNTO_C coordenada_triangulo PUNTO_C coordenada_triangulo
    ;

coordenada_triangulo:
    tipo_parametro_triangle_area COMA tipo_parametro_triangle_area
    ;

tipo_parametro_triangle_area:
    CONST_INT
    | CONST_FLOAT
    | ID
    ;


%%


/* -------------------------------------------------------------------------- */
/*                          FUNCIONES PARA EL SINTACTICO                      */
/* ------------------------------------------------------------------------- */

void informarMatchLexicoSintactico(const char* mensaje1, const char* mensaje2) {
    char mensaje_formateado[500];
    sprintf(mensaje_formateado, "[SINTACTICO]   %30s -> %-30s\n", mensaje1, mensaje2);

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