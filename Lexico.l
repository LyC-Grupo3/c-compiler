%{
#include "utils/utils.h"
#include "y.tab.h"


int yylval;
char *yyltext;


int informarError(char* descripcion, char* lexema);

%}


%option noyywrap  
%option yylineno 


/* DEFINICION PARA TOKENS */
/* -- BASICOS -- */
ID			    {LETRA}({LETRA}|{DIGITO})*
CONST_INT       -?{DIGITO}+
CONST_FLOAT     -?{DIGITO}*"."{DIGITO}+
CONST_STR       \"[^\n"]*\"
CONST_BOOL      "true"|"false"
OP_ASIG_VALOR   ":="
OP_ASIG_TIPO    ":"


/* -- COMENTARIOS -- */
COMENTARIO      "#+"([^\n]|\n)*"+#"


/* -- CONJUNTOS -- */
DIGITO			[0-9]
LETRA			[a-zA-Z]


/* -- TIPOS DE DATOS -- */
INT               "int"
FLOAT             "float"
STRING            "string"
BOOL              "bool"


/* -- CONECTORES LOGICOS -- */
AND                "&&"
OR                 "||"
NOT                "!"


/* -- OPERADORES ARITMETICOS -- */
OP_SUM		     "+"
OP_MUL           "*"
OP_RES		     "-"
OP_DIV           "/"


/* -- OPERADORES LOGICOS -- */
OP_MAYOR         ">"
OP_MAYOR_IGUAL   ">="

OP_MENOR         "<"
OP_MENOR_IGUAL   "<="

OP_IGUAL         "=="
OP_DISTINTO      "!="


/* -- PALABRAS RESERVADAS -- */
WHILE           "while"

IF              "if"
ELSE            "else"

INIT            "init"

WRITE           "write"
READ            "read"


/* -- BLOQUES -- */
PAR_A			 "("
PAR_C			 ")"
LLA_A            "{"
LLA_C            "}"
COR_A            "["
COR_C            "]"


/* -- PUNTUACION -- */
PUNTO_C          ";"
COMA             ","


/* -- OTROS A REVISAR -- */
SUMA_UNO         "++"
RESTA_UNO        "--"

OP_MOD          "%"

COM_SIM          "'"
COM_DOB          "\""


/* -- NOMBRES FUNCIONES -- */
EQUAL_EXP       "equalExpressions"

%%


{CTE}			{printf("\nConstante: %s\n", yytext);return CTE;}
{CONST_INT}		{printf("\nConstante Entera: %s\n", yytext);return CONST_INT;}
{CONST_FLOAT}	{printf("\nConstante Real: %s\n", yytext);return CONST_FLOAT;}
{CONST_STR}		{printf("\nConstante Cadena: %s\n", yytext);return CONST_STR;}


{WHILE}			{printf("\nPalabra reservada WHILE: %s\n", yytext);return WHILE;}
{IF}			{printf("\nPalabra reservada IF: %s\n", yytext);return IF;}
{ELSE}			{printf("\nPalabra reservada ELSE: %s\n", yytext);return ELSE;}
{INIT}			{printf("\nPalabra reservada INIT: %s\n", yytext);return INIT;}
{WRITE}			{printf("\nPalabra reservada WRITE: %s\n", yytext);return WRITE;}

{ID}			{printf("\nIdentificador: %s\n", yytext);return ID;}


{OP_SUM}		{printf("\nSuma: %s\n", yytext);return OP_SUM;}
{OP_ASIG}		{printf("\nAsignacion: %s\n", yytext);return OP_ASIG;}
{OP_MUL}		{printf("\nMultiplicacion: %s\n", yytext);return OP_MUL;}
{OP_RES}		{printf("\nResta: %s\n", yytext);return OP_RES;}
{OP_DIV}		{printf("\nDivision: %s\n", yytext);return OP_DIV;}
{OP_MOD}		{printf("\nModulo: %s\n", yytext);return OP_MOD;}
{OP_MAYOR}		{printf("\nMayor: %s\n", yytext);return OP_MAYOR;}
{OP_MENOR}		{printf("\nMenor: %s\n", yytext);return OP_MENOR;}


{SUMA_UNO}		{printf("\nIncremento: %s\n", yytext);return SUMA_UNO;}
{RESTA_UNO}		{printf("\nDecremento: %s\n", yytext);return RESTA_UNO;}

{PA}			{printf("\nParAbre: %s\n", yytext);return PA;}
{PC}			{printf("\nParCierra: %s\n", yytext);return PC;}
{LA}			{printf("\nLlaveAbre: %s\n", yytext);return LA;}
{LC}			{printf("\nLlaveCierra: %s\n", yytext);return LC;}
{CA}			{printf("\nCorcheteAbre: %s\n", yytext);return CA;}
{CC}			{printf("\nCorcheteCierra: %s\n", yytext);return CC;}
{PyC}			{printf("\nPuntoComa: %s\n", yytext);return PyC;}
{COMA}			{printf("\nComa: %s\n", yytext);return COMA;}
{COM_SIM}		{printf("\nComilla simple: %s\n", yytext);return COM_SIM;}
{COM_DOB}		{printf("\nComilla doble: %s\n", yytext);return COM_DOB;}
"\n"      		
"\t"
"\n\t"
" "             		
"\r\n"
.			 {informarError("Caracter no reconocido", yytext);}

%%