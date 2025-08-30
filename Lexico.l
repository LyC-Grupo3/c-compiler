
/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{
#include "utils/utils.h"
#include "y.tab.h"


int yylval;
char *yyltext;


int informarError(char* descripcion, char* lexema);

%}



/* -------------------------------------------------------------------------- */
/*                             SECCION DE OPCIONES                            */
/* -------------------------------------------------------------------------- */

/* Indica a flex que lea solo un fichero de entrada */
%option noyywrap  
/* Permite obtener el numero de linea*/
%option yylineno 



/* -------------------------------------------------------------------------- */
/*                              SECCION DE TOKENS                             */
/* -------------------------------------------------------------------------- */

/* -------------------------------- CONJUNTOS ------------------------------- */
DIGITO			[0-9]
LETRA			[a-zA-Z]


/* ----------------------------- TIPOS DE DATOS ----------------------------- */
INT               "int"
FLOAT             "float"
STRING            "string"
BOOL              "bool"


/* --------------------------- CONECTORES LOGICOS --------------------------- */
AND                "&&"
OR                 "||"
NOT                "!"


/* --------------------------- PALABRAS RESERVADAS -------------------------- */
WHILE           "while"

IF              "if"
ELSE            "else"

INIT            "init"

WRITE           "write"
READ            "read"

CONST_BOOL_T      "true"
CONST_BOOL_F      "false"

/* ---------------------------- NOMBRES FUNCIONES --------------------------- */
EQUAL_EXP       "equalExpressions"
TRIAN_MAX       "triangleAreaMaximum"


/* --------------------------------- BASICOS -------------------------------- */
ID			    {LETRA}({LETRA}|{DIGITO})*

CONST_INT       -?{DIGITO}+
CONST_FLOAT     -?{DIGITO}*"."{DIGITO}+
CONST_STR       \"[^\n"]*\"

OP_ASIG_VALOR   ":="
OP_ASIG_TIPO    ":"

COMENTARIO      "#+"([^\n]|\n)*"+#"


/* ------------------------- OPERADORES ARITMETICOS ------------------------- */
OP_SUM		     "+"
OP_MUL           "*"
OP_RES		     "-"
OP_DIV           "/"


/* --------------------------- OPERADORES LOGICOS --------------------------- */
OP_MAYOR         ">"
OP_MAYOR_IGUAL   ">="

OP_MENOR         "<"
OP_MENOR_IGUAL   "<="

OP_IGUAL         "=="
OP_DISTINTO      "!="


/* --------------------------------- BLOQUES -------------------------------- */
PAR_A			 "("
PAR_C			 ")"
LLA_A            "{"
LLA_C            "}"
COR_A            "["
COR_C            "]"


/* ------------------------------- PUNTUACION ------------------------------- */
PUNTO_C          ";"
COMA             ","


/* ----------------------------- OTROS A REVISAR ---------------------------- */
SUMA_UNO         "++"
RESTA_UNO        "--"

OP_MOD          "%"

COM_SIM          "'"
COM_DOB          "\""




/* -------------------------------------------------------------------------- */
/*                              SECCION DE REGLAS                             */
/* -------------------------------------------------------------------------- */

/* Cuando se produce un match lo informa, luego para bizon agregar el return */
%%


{INT}			{printf("\nTipo INT: %s\n", yytext); }
{FLOAT}			{printf("\nTipo FLOAT: %s\n", yytext); }
{STRING}		{printf("\nTipo STRING: %s\n", yytext); }
{BOOL}			{printf("\nTipo BOOL: %s\n", yytext); }


{AND}			{printf("\nConector AND: %s\n", yytext); }
{OR}			{printf("\nConector OR: %s\n", yytext); }
{NOT}			{printf("\nConector NOT: %s\n", yytext); }


{WHILE}			{printf("\nPalabra reservada WHILE: %s\n", yytext); }

{IF}			{printf("\nPalabra reservada IF: %s\n", yytext); }
{ELSE}			{printf("\nPalabra reservada ELSE: %s\n", yytext); }

{INIT}			{printf("\nPalabra reservada INIT: %s\n", yytext); }

{WRITE}			{printf("\nPalabra reservada WRITE: %s\n", yytext); }
{READ}			{printf("\nPalabra reservada READ: %s\n", yytext); }


{CONST_BOOL_T}	{printf("\nConstante Booleana True: %s\n", yytext); }
{CONST_BOOL_F}	{printf("\nConstante Booleana False: %s\n", yytext); }


{EQUAL_EXP}		{printf("\nFuncion equalExpressions: %s\n", yytext); }
{TRIAN_MAX}		{printf("\nFuncion triangleAreaMaximum: %s\n", yytext); }


{ID}			{printf("\nIdentificador: %s\n", yytext); }

{CONST_INT}		{printf("\nConstante Entera: %s\n", yytext); }
{CONST_FLOAT}	{printf("\nConstante Real: %s\n", yytext); }
{CONST_STR}		{printf("\nConstante Cadena: %s\n", yytext); }

{OP_ASIG_VALOR}	{printf("\nAsignacion de valor: %s\n", yytext); }
{OP_ASIG_TIPO}	{printf("\nAsignacion de tipo: %s\n", yytext); }

{COMENTARIO}	{printf("\nComentario: %s\n", yytext); }


{OP_SUM}		{printf("\nSuma: %s\n", yytext); }
{OP_MUL}		{printf("\nMultiplicacion: %s\n", yytext); }
{OP_RES}		{printf("\nResta: %s\n", yytext); }
{OP_DIV}		{printf("\nDivision: %s\n", yytext); }


{OP_MAYOR}		{printf("\nMayor: %s\n", yytext); }
{OP_MAYOR_IGUAL} {printf("\nMayor igual: %s\n", yytext); }

{OP_MENOR}		{printf("\nMenor: %s\n", yytext); }
{OP_MENOR_IGUAL} {printf("\nMenor igual: %s\n", yytext); }

{OP_IGUAL}		{printf("\nIgual: %s\n", yytext); }
{OP_DISTINTO}	{printf("\nDistinto: %s\n", yytext); }


{PAR_A}			{printf("\nParentesis abre: %s\n", yytext); }
{PAR_C}			{printf("\nParentesis cierra: %s\n", yytext); }
{LLA_A}			{printf("\nLlave abre: %s\n", yytext); }
{LLA_C}			{printf("\nLlave cierra: %s\n", yytext); }
{COR_A}			{printf("\nCorchete abre: %s\n", yytext); }
{COR_C}			{printf("\nCorchete cierra: %s\n", yytext); }


{PUNTO_C}		{printf("\nPunto y coma: %s\n", yytext); }
{COMA}			{printf("\nComa: %s\n", yytext); }


{SUMA_UNO}		{printf("\nIncremento: %s\n", yytext); }
{RESTA_UNO}		{printf("\nDecremento: %s\n", yytext); }

{OP_MOD}		{printf("\nModulo: %s\n", yytext); }

{COM_SIM}		{printf("\nComilla simple: %s\n", yytext); }
{COM_DOB}		{printf("\nComilla doble: %s\n", yytext); }


"\n"      		
"\t"
"\n\t"
" "             		
"\r\n"


.   {
        informarError("Caracter no reconocido", yytext);
    } 


%%



/* -------------------------------------------------------------------------- */
/*                          CODIGO PARA LA EJECUCCION                         */
/* -------------------------------------------------------------------------- */

int informarError(char* descripcion, char* lexema)
{
    /* descripcion es el error léxico */
    /* lexema es la cadena inválida */
 
    printf("\n");
    printf("ERROR LEXICO - %s: %s\n", descripcion, lexema);
    printf("LINEA: %d\n", yylineno);
    printf("\n");
 
    exit(1);
}

int main (int argc, char *argv[]){
    /*abro archivo en modo lectura y dejo apuntando el puntero yyvin que nos pide flex alli*/
    if((yyin = fopen(argv[1],"rt"))== NULL){
        printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
        return 1;
    }
    else{
        printf("\nEl archivo %s se abrió correctamente\n",argv[1]);
        yylex();
    }
    printf("\nflex finalizó la lectura del archivo %s \n",argv[1]);
    fclose(yyin);
    return 0;
}