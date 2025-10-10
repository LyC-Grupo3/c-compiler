/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{

#include "./Sintactico.h"

%}

%union {
    char lexema[100];
}

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
%token <lexema> ID

/* -------------------------------- CONSTANTES ------------------------------- */
%token <lexema> CONST_INT CONST_FLOAT CONST_STR

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
     init conjunto_sentencias                           {informarMatchLexicoSintactico("programa", "init conjunto_sentencias");}
    |init                                               {informarMatchLexicoSintactico("programa", "init");}
    ;

conjunto_sentencias:
    conjunto_sentencias sentencia                       {informarMatchLexicoSintactico("conjunto_sentencias", "conjunto_sentencias sentencia");}
    |sentencia                                          {informarMatchLexicoSintactico("conjunto_sentencias", "sentencia");}
                         
    ;

sentencia:
    seleccion_con_else                                  {informarMatchLexicoSintactico("sentencia", "seleccion_con_else");}
    | seleccion_sin_else                                {informarMatchLexicoSintactico("sentencia", "seleccion_sin_else");}
    | iteracion_while                                      {informarMatchLexicoSintactico("sentencia", "iteracion_while");}
    | asignacion                                        {informarMatchLexicoSintactico("sentencia", "asignacion");}
    | funcion_read                                      {informarMatchLexicoSintactico("sentencia", "funcion_read");}
    | funcion_write                                     {informarMatchLexicoSintactico("sentencia", "funcion_write");}
    ;

/* ------------------------------- ARITHMETIC ------------------------------- */
expresion:
    expresion OP_SUM termino                            {
                                                            informarMatchLexicoSintactico("expresion", "expresion OP_SUM termino");
                                                            insertarEnPolaca("+");
                                                        }
    | expresion OP_RES termino                          {
                                                            informarMatchLexicoSintactico("expresion", "expresion OP_RES termino");
                                                            insertarEnPolaca("-");
                                                        }
    | termino                                           {informarMatchLexicoSintactico("expresion", "termino");}
    ;

termino:
    termino OP_MUL factor                               {
                                                            informarMatchLexicoSintactico("termino", "termino OP_MUL factor");
                                                            insertarEnPolaca("*");
                                                        }
    | termino OP_DIV factor                             {
                                                            informarMatchLexicoSintactico("termino", "termino OP_DIV factor");
                                                            insertarEnPolaca("/");
                                                        }
    | factor                                            {informarMatchLexicoSintactico("termino", "factor");}
    ;

factor:
    ID                                                  {
                                                            informarMatchLexicoSintactico("factor", "ID");
                                                            insertarEnPolaca($1);
                                                        }
    | CONST_INT                                         {
                                                            informarMatchLexicoSintactico("factor", "CONST_INT");
                                                            insertarEnPolaca($1);
                                                        }
    | CONST_FLOAT                                       {
                                                            informarMatchLexicoSintactico("factor", "CONST_FLOAT");
                                                            insertarEnPolaca($1);
                                                        }
    | PAR_A expresion PAR_C                             {informarMatchLexicoSintactico("factor", "PAR_A expresion PAR_C");}
    ;

/* ------------------------------- ASSIGMENTS ------------------------------- */
asignacion:
    ID OP_ASIG_VALOR expresion                          {
                                                            informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR expresion");
                                                            insertarEnPolaca($1);
                                                            insertarEnPolaca(":=");
                                                        }
    | ID OP_ASIG_VALOR CONST_STR                        {
                                                            informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR CONST_STR");
                                                            insertarEnPolaca($3);
                                                            insertarEnPolaca($1);
                                                            insertarEnPolaca(":=");
                                                        }
    | ID OP_ASIG_VALOR funciones_temas_especiales       {
                                                            informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR funciones_temas_especiales");
                                                        }
    ;

/* ---------------------------------- INIT ---------------------------------- */

init:                       
    INIT LLA_A conjunto_declaraciones LLA_C             {informarMatchLexicoSintactico("init", "INIT LLA_A conjunto_declaraciones LLA_C");}
    ;

conjunto_declaraciones:
    conjunto_declaraciones declaracion                  {informarMatchLexicoSintactico("conjunto_declaraciones", "conjunto_declaraciones declaracion");}
    | declaracion                                       {informarMatchLexicoSintactico("conjunto_declaraciones", "declaracion");}
    ;

declaracion:
    conjunto_ids OP_ASIG_TIPO tipo_dato                 {informarMatchLexicoSintactico("declaracion", "conjunto_ids OP_ASIG_TIPO tipo_dato");}
    ;

conjunto_ids:
    conjunto_ids COMA ID                                {informarMatchLexicoSintactico("conjunto_ids", "conjunto_ids COMA ID");}
    | ID                                                {informarMatchLexicoSintactico("conjunto_ids", "ID");}
    ;

tipo_dato:
    INT                                                 {informarMatchLexicoSintactico("tipo_dato", "INT");}
    | FLOAT                                             {informarMatchLexicoSintactico("tipo_dato", "FLOAT");}
    | STRING                                            {informarMatchLexicoSintactico("tipo_dato", "STRING");}
    ;

/* ----------------------------------- IF ----------------------------------- */
seleccion_con_else:
    bloque_if 
                {
                    // Sentencia de Selección con else - Fin del bloque verdadero
                    insertarEnPolaca("BI");
                    // 1. Desapilar X (tope de la pila)
                    char* indicePolacaChar = desapilar();
                    int nroCeldaDesapilada = atoi(indicePolacaChar);
                    // 2. Escribir en la celda X, el nº de celda actual + 1
                    char* nroCeldaActual = getIndiceActualPolaca();
                    char nroCeldaActualMasUno[12];
                    snprintf(nroCeldaActualMasUno, 12, "%d", atoi(nroCeldaActual) + 1);
                    insertarEnPolacaIndice(nroCeldaDesapilada, nroCeldaActualMasUno);
                    // 3. Apilar el nº de celda actual
                    apilar(nroCeldaActual);
                    avanzarPolaca();
                }
                bloque_else                               
                                {
                                    informarMatchLexicoSintactico("seleccion_con_else", "bloque_if bloque_else");
                                    // Sentencia de Selección con else - Fin del bloque falso
                                    // 1. Desapilar X (tope de la pila)
                                    char* indicePolacaChar = desapilar();
                                    int nroCeldaDesapilada = atoi(indicePolacaChar);
                                    // 2. Escribir en la celda X, el nº de celda actual + 1
                                    char* nroCeldaActual = getIndiceActualPolaca();
                                    char nroCeldaActualMasUno[12];
                                    snprintf(nroCeldaActualMasUno, 12, "%d", atoi(nroCeldaActual));
                                    insertarEnPolacaIndice(nroCeldaDesapilada, nroCeldaActualMasUno);
                                }
    ;

seleccion_sin_else:
    bloque_if   
                {
                    informarMatchLexicoSintactico("seleccion_sin_else", "bloque_if");
                    // Sentencia de Selección (sin else) - Fin del bloque verdadero
                    // 1. Desapilar X (tope de la pila)
                    char* indicePolacaChar = desapilar();
                    int nroCeldaDesapilada = atoi(indicePolacaChar);
                    // 2. Escribir en la celda X, el nº de celda actual
                    char* nroCeldaActual = getIndiceActualPolaca();
                    insertarEnPolacaIndice(nroCeldaDesapilada, nroCeldaActual);
                }
    ;


bloque_if:
    IF PAR_A condicional 
                        {
                            // Sentencia de Selección con else - Fin de Condición
                            // Sentencia de Selección (sin else) - Fin de Condición
                            // 1. Apilar el nº de celda actual
                            char* nroCeldaActual = getIndiceActualPolaca();
                            apilar(nroCeldaActual);
                            avanzarPolaca();
                        }
                        PAR_C LLA_A conjunto_sentencias LLA_C      { informarMatchLexicoSintactico("bloque_if", "IF PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C");}
    ;

/* ---------------------------------- ELSE ---------------------------------- */
bloque_else:
    ELSE LLA_A conjunto_sentencias LLA_C    {informarMatchLexicoSintactico("bloque_else", "ELSE LLA_A conjunto_sentencias LLA_C");}
    ;


condicional:
    condicion                                           {informarMatchLexicoSintactico("condicional", "condicion");}
    | condicion operador_logico condicion               {informarMatchLexicoSintactico("condicional", "condicion operador_logico condicion");}
    | NOT condicion                                     {informarMatchLexicoSintactico("condicional", "NOT condicion");}
    ;



condicion:
    expresion operador_comparacion expresion           {
                                                            informarMatchLexicoSintactico("condicion", "expresion operador_comparacion expresion");
                                                            insertarEnPolaca(VALOR_POLACA_COMPARADOR);
                                                            insertarEnPolaca(getOperadorComparacionPendientePolaca());
                                                        }
    ;

operador_comparacion:
    OP_IGUAL                                            {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_IGUAL");
                                                            setOperadorComparacionPendientePolaca(VALOR_POLACA_OP_IGUAL);
                                                        }
    | OP_DISTINTO                                       {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_DISTINTO");
                                                            setOperadorComparacionPendientePolaca(VALOR_POLACA_OP_DISTINTO);
                                                        }
    | OP_MAYOR                                          {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MAYOR");
                                                            setOperadorComparacionPendientePolaca(VALOR_POLACA_OP_MAYOR);
                                                        }
    | OP_MAYOR_IGUAL                                    {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MAYOR_IGUAL");
                                                            setOperadorComparacionPendientePolaca(VALOR_POLACA_OP_MAYOR_IGUAL);
                                                        }
    | OP_MENOR                                          {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MENOR");
                                                            setOperadorComparacionPendientePolaca(VALOR_POLACA_OP_MENOR);
                                                        }
    | OP_MENOR_IGUAL                                    {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MENOR_IGUAL");
                                                            setOperadorComparacionPendientePolaca(VALOR_POLACA_OP_MENOR_IGUAL);
                                                        }
    ;

operador_logico:
    AND                                                 {informarMatchLexicoSintactico("operador_logico", "AND");}
    | OR                                                {informarMatchLexicoSintactico("operador_logico", "OR");}
    ;
    
/* ---------------------------------- WHILE --------------------------------- */
iteracion_while:
    WHILE 
        {
            // Sentencias de Iteración (while) - Comienzo
            // 1. Apilar el nº celda actual
            char* nroCeldaActual = getIndiceActualPolaca();
            apilar(nroCeldaActual);

            insertarEnPolaca("ET");
        }
        PAR_A condicional 
                            {                      
                                // Sentencias de Iteración (while) - Fin de la Condición
                                // 1. Apilar el nº celda actual
                                char* nroCeldaActual = getIndiceActualPolaca();
                                apilar(nroCeldaActual);

                                avanzarPolaca();
                            }
                            PAR_C LLA_A conjunto_sentencias LLA_C   
                                                                    {
                                                                        informarMatchLexicoSintactico("iteracion_while", "WHILE PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C");

                                                                        // Sentencias de Iteración (while) - Fin del ciclo
                                                                        insertarEnPolaca("BI");

                                                                        // 1. Desapilar Z (tope de la pila)
                                                                        char* indicePolacaChar = desapilar();
                                                                        int nroCeldaDesapilada = atoi(indicePolacaChar);
                                                                        
                                                                        // 2. Escribir en la celda Z, el nº de celda actual + 1
                                                                        char* nroCeldaActual = getIndiceActualPolaca();
                                                                        char nroCeldaActualMasUno[12];
                                                                        snprintf(nroCeldaActualMasUno, 12, "%d", atoi(nroCeldaActual) + 1);
                                                                        insertarEnPolacaIndice(nroCeldaDesapilada, nroCeldaActualMasUno);

                                                                        // 3. Desapilar Z (tope de la pila)
                                                                        indicePolacaChar = desapilar();

                                                                        // 4. Escribir Z en la celda actual
                                                                        insertarEnPolaca(indicePolacaChar);
                                                                    }
    ;

/* ---------------------------------- READ ---------------------------------- */
funcion_read:
    READ PAR_A ID PAR_C                                             {informarMatchLexicoSintactico("funcion_read", "READ PAR_A ID PAR_C");}
    ;

/* ---------------------------------- WRITE --------------------------------- */
funcion_write:
    WRITE PAR_A CONST_STR PAR_C                                     {informarMatchLexicoSintactico("funcion_write", "WRITE PAR_A CONST_STR PAR_C");}
    | WRITE PAR_A ID PAR_C                              {informarMatchLexicoSintactico("funcion_write", "WRITE PAR_A ID PAR_C");}
    ;

/* ---------------------------- TEMAS ESPECIALES ---------------------------- */
funciones_temas_especiales:
    funcion_equal_expressions                           {informarMatchLexicoSintactico("funciones_temas_especiales", "funcion_equal_expressions");}
    | funcion_triangle_area_maximum                     {informarMatchLexicoSintactico("funciones_temas_especiales", "funcion_triangle_area_maximum");}
    ;

/* ---------------------------- EQUAL EXPRESSIONS --------------------------- */
funcion_equal_expressions:
    EQUAL_EXP PAR_A parametros_equal_expressions PAR_C              {informarMatchLexicoSintactico("funcion_equal_expressions", "EQUAL_EXP PAR_A parametros_equal_expressions PAR_C");}
    ;

parametros_equal_expressions:
    expresion COMA expresion                            {informarMatchLexicoSintactico("parametros_equal_expressions", "expresion COMA expresion");}
    | parametros_equal_expressions COMA expresion       {informarMatchLexicoSintactico("parametros_equal_expressions", "parametros_equal_expressions COMA expresion");}
    ;


/* ---------------------- FUNCION TRIANGLE AREA MAXIMUM --------------------- */
funcion_triangle_area_maximum:
    TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C          {informarMatchLexicoSintactico("funcion_triangle_area_maximum", "TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C");}
    ;

parametros_triangle_area_maximum:
    coordenadas_triangulo PUNTO_C coordenadas_triangulo {informarMatchLexicoSintactico("parametros_triangle_area_maximum", "coordenadas_triangulo PUNTO_C coordenadas_triangulo");}
    ;

coordenadas_triangulo:
    COR_A conjunto_puntos_triangulo COR_C               {informarMatchLexicoSintactico("coordenadas_triangulo", "COR_A conjunto_puntos_triangulo COR_C");}
    ;

conjunto_puntos_triangulo:
    coordenada_triangulo PUNTO_C coordenada_triangulo PUNTO_C coordenada_triangulo {informarMatchLexicoSintactico("conjunto_puntos_triangulo", "coordenada_triangulo PUNTO_C coordenada_triangulo PUNTO_C coordenada_triangulo");}
    ;

coordenada_triangulo:
    tipo_parametro_triangle_area COMA tipo_parametro_triangle_area {informarMatchLexicoSintactico("coordenada_triangulo", "tipo_parametro_triangle_area COMA tipo_parametro_triangle_area");}
    ;

tipo_parametro_triangle_area:
    CONST_INT                                           {informarMatchLexicoSintactico("tipo_parametro_triangle_area", "CONST_INT");}
    | CONST_FLOAT                                       {informarMatchLexicoSintactico("tipo_parametro_triangle_area", "CONST_FLOAT");}
    | ID                                                {informarMatchLexicoSintactico("tipo_parametro_triangle_area", "ID");}
    ;


%%