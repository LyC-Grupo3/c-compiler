/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
int yystopparser=0;
FILE  *yyin;

  //int yyerror();
  int yyerror(const char *s);
  int yylex();

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

sentencia:  	   
	asignacion {printf(" FIN\n");} ;

asignacion: 
          ID OP_AS expresion {printf("    ID = Expresion es ASIGNACION\n");}
	  ;

expresion:
         termino {printf("    Termino es Expresion\n");}
	 |expresion OP_SUM termino {printf("    Expresion+Termino es Expresion\n");}
	 |expresion OP_RES termino {printf("    Expresion-Termino es Expresion\n");}
	 ;

termino: 
       factor {printf("    Factor es Termino\n");}
       |termino OP_MUL factor {printf("     Termino*Factor es Termino\n");}
       |termino OP_DIV factor {printf("     Termino/Factor es Termino\n");}
       ;

factor: 
      ID {printf("    ID es Factor \n");}
      | CTE {printf("    CTE es Factor\n");}
	| PA expresion PC {printf("    Expresion entre parentesis es Factor\n");}
     	;

%%




/* -------------------------------------------------------------------------- */
/*                          CODIGO PARA LA EJECUCCION                         */
/* -------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
    if((yyin = fopen(argv[1], "rt"))==NULL)
    {
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
       
    }
    else
    { 
        
        yyparse();
        
    }
	fclose(yyin);
        return 0;
}
/*int yyerror(void)
     {
       printf("Error Sintactico\n");
	 exit (1);
     }
     */

     int yyerror(const char *s)
{
    printf("Error Sintactico: %s\n", s);
    exit(1);
}