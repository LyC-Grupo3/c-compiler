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

programa: conjunto_sentencias {informarMatchLexicoSintactico("SINTAXIS OK");}
    ;

conjunto_sentencias:    sentencia {informarMatchLexicoSintactico("\"sentencia\" -> \"conjunto_sentencias\"");}
                        | conjunto_sentencias sentencia {informarMatchLexicoSintactico("\"conjunto_sentencias sentencia\" -> \"conjunto_sentencias\"");}
    ;

sentencia:  init {informarMatchLexicoSintactico("\"init\" -> \"sentencia\"");}
            | asignacion {informarMatchLexicoSintactico("\"asignacion\" -> \"sentencia\"");}
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

init: INIT LLA_A declaraciones_var LLA_C {informarMatchLexicoSintactico("\"INIT LLAVE_A declaraciones_var LLAVE_C\" -> \"init\"");}
    ;

declaraciones_var:  declaraciones_var declaracion {informarMatchLexicoSintactico("\"declaraciones_var declaracion\" -> \"declaraciones_var\"");}
                    | declaracion {informarMatchLexicoSintactico("\"declaracion\" -> \"declaraciones_var\"");}
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

%%



/* -------------------------------------------------------------------------- */
/*                          FUNCIONES PARA EL SINTACTICO                      */
/* -------------------------------------------------------------------------- */

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
    if (archivo_salida_sintactico == NULL) {
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