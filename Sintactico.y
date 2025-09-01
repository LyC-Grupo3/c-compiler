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

void informarMatchSintactico(const char* mensaje);

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

programa: conjunto_sentencias { 
    informarMatchSintactico("SINTAXIS OK");
}
        ;

conjunto_sentencias: sentencia {
    informarMatchSintactico("\"sentencia\" -> \"conjunto_sentencias\"");
}
          | conjunto_sentencias sentencia {
    informarMatchSintactico("\"conjunto_sentencias sentencia\" -> \"conjunto_sentencias\"");
}
          ;

sentencia: asignacion {
    informarMatchSintactico("\"asignacion\" -> \"sentencia\"");
}
         ;

asignacion: ID OP_ASIG_VALOR expresion { 
    informarMatchSintactico("\"ID OP_ASIG_VALOR expresion\" -> \"asignacion\"");
}
          ;

expresion: CONST_INT { informarMatchSintactico("\"CONST_INT\" -> \"expresion\""); }
         | CONST_FLOAT { informarMatchSintactico("\"CONST_FLOAT\" -> \"expresion\""); }
         | CONST_STR { informarMatchSintactico("\"CONST_STR\" -> \"expresion\""); }
         ;


%%



/* -------------------------------------------------------------------------- */
/*                          FUNCIONES PARA EL SINTACTICO                      */
/* -------------------------------------------------------------------------- */

void informarMatchSintactico(const char* mensaje) {
    char mensaje_formateado[500];
    sprintf(mensaje_formateado, "[SINTACTICO]   %s\n", mensaje);
    
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