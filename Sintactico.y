// Usa Lexico_ClasePractica
//Solo expresiones sin ()
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

%token CTE
%token CONST_INT CONST_FLOAT CONST_STR
%token WHILE IF ELSE INIT WRITE
%token ID
%token OP_SUM OP_RES OP_MUL OP_DIV OP_MOD
%token OP_AS
%token OP_MAYOR OP_MENOR OP_MAYOR_IGUAL OP_MENOR_IGUAL OP_COMPARACION
%token SUMA_UNO RESTA_UNO
%token PA PC LA LC CA CC
%token PyC COMA COM_SIM COM_DOB

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