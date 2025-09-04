/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{

#include "./Sintactico.h"

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