
/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{
#include "./utils/utils.h"
#include "./utils/informes.h"
#include "./utils/validacion.h"

// Solo  se usa cuando se usa el analizador sintactico, el script se encarga de comentarlo para que no falle
#include "y.tab.h"


int yylval;
char *yyltext;

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
CONST_FLOAT     -?({DIGITO}*"."{DIGITO}+([eE][+-]?{DIGITO}+)?|{DIGITO}+[eE][+-]?{DIGITO}+)
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


{INT}			{informarMatch(yytext, "INT"); }
{FLOAT}			{informarMatch(yytext, "FLOAT"); }
{STRING}		{informarMatch(yytext, "STRING"); }
{BOOL}			{informarMatch(yytext, "BOOL"); }


{AND}			{informarMatch(yytext, "AND"); }
{OR}			{informarMatch(yytext, "OR"); }
{NOT}			{informarMatch(yytext, "NOT"); }


{WHILE}			{informarMatch(yytext, "WHILE"); }

{IF}			{informarMatch(yytext, "IF"); }
{ELSE}			{informarMatch(yytext, "ELSE"); }

{INIT}			{informarMatch(yytext, "INIT"); }

{WRITE}			{informarMatch(yytext, "WRITE"); }
{READ}			{informarMatch(yytext, "READ"); }


{CONST_BOOL_T}	{informarMatch(yytext, "CONST_BOOL_T"); }
{CONST_BOOL_F}	{informarMatch(yytext, "CONST_BOOL_F"); }


{EQUAL_EXP}		{informarMatch(yytext, "EQUAL_EXP"); }
{TRIAN_MAX}		{informarMatch(yytext, "TRIAN_MAX"); }


{ID}			{informarMatch(yytext, "ID"); }

{CONST_INT}		{informarMatch(yytext, "CONST_INT"); validarRangoInt(yytext);}
{CONST_FLOAT}	{informarMatch(yytext, "CONST_FLOAT"); validarRangoFloat(yytext);}
{CONST_STR}		{informarMatch(yytext, "CONST_STR"); validarLongitudString(yytext);}

{OP_ASIG_VALOR}	{informarMatch(yytext, "OP_ASIG_VALOR"); }
{OP_ASIG_TIPO}	{informarMatch(yytext, "OP_ASIG_TIPO"); }

{COMENTARIO}	{informarMatch(yytext, "COMENTARIO"); }


{OP_SUM}		{informarMatch(yytext, "OP_SUM"); }
{OP_MUL}		{informarMatch(yytext, "OP_MUL"); }
{OP_RES}		{informarMatch(yytext, "OP_RES"); }
{OP_DIV}		{informarMatch(yytext, "OP_DIV"); }


{OP_MAYOR}		{informarMatch(yytext, "OP_MAYOR"); }
{OP_MAYOR_IGUAL} {informarMatch(yytext, "OP_MAYOR_IGUAL"); }

{OP_MENOR}		{informarMatch(yytext, "OP_MENOR"); }
{OP_MENOR_IGUAL} {informarMatch(yytext, "OP_MENOR_IGUAL"); }

{OP_IGUAL}		{informarMatch(yytext, "OP_IGUAL"); }
{OP_DISTINTO}	{informarMatch(yytext, "OP_DISTINTO"); }


{PAR_A}			{informarMatch(yytext, "PAR_A"); }
{PAR_C}			{informarMatch(yytext, "PAR_C"); }
{LLA_A}			{informarMatch(yytext, "LLA_A"); }
{LLA_C}			{informarMatch(yytext, "LLA_C"); }
{COR_A}			{informarMatch(yytext, "COR_A"); }
{COR_C}			{informarMatch(yytext, "COR_C"); }


{PUNTO_C}		{informarMatch(yytext, "PUNTO_C"); }
{COMA}			{informarMatch(yytext, "COMA"); }


{SUMA_UNO}		{informarMatch(yytext, "SUMA_UNO"); }
{RESTA_UNO}		{informarMatch(yytext, "RESTA_UNO"); }

{OP_MOD}		{informarMatch(yytext, "OP_MOD"); }

{COM_SIM}		{informarMatch(yytext, "COM_SIM"); }
{COM_DOB}		{informarMatch(yytext, "COM_DOB"); }


"\n"      		
"\t"
"\n\t"
" "             		
"\r\n"


.   { informarErrorNoMatch(yytext); } 


%%



/* -------------------------------------------------------------------------- */
/*                          CODIGO PARA LA EJECUCCION                         */
/* -------------------------------------------------------------------------- */

/* -------------------------------- INFORMES -------------------------------- */
void informarMatch(char* lexema, char* nombreDelToken) {
    printf("\n\nLEXEMA: %-20s - TOKEN: %-15s", lexema, nombreDelToken);
}

void informarErrorNoMatch(char* lexema)
{
    printf("\n\nLEXEMA: %-20s - LINEA: %-10d - ERROR NO-MATCH", lexema, yylineno);
    exit(1);
}

void informarErrorValidacion(char* name_token)
{
    printf("\nLINEA:  %-20d - ERROR VALIDACION: %-20s", yylineno, name_token);
}



/* ------------------------------ VALIDACIONES ------------------------------ */
int validarRangoInt(char*ptr)
{
    int valueCasted = atoi(ptr);
    if (valueCasted < INT_MIN || valueCasted > INT_MAX) {
        informarErrorValidacion("CONST_INT");
        return 1;
    }
    return 0;
}

int validarRangoFloat(char*ptr)
{
    float valueCasted = atof(ptr);
    if (valueCasted < FLOAT_MIN || valueCasted > FLOAT_MAX) {
        informarErrorValidacion("CONST_FLOAT");
        return 1;
    }
    return 0;
}

int validarLongitudString(char*ptr)
{
    int length = strlen(ptr) - 2; // -2 para no contar las comillas de apertura y cierre
    if (length > STRING_MAX_LENGTH) {
        informarErrorValidacion("CONST_STR");
        return 1;
    }
    return 0;
}


/* ---------------------------------- MAIN ---------------------------------- */
int main (int argc, char *argv[]){
    if((yyin = fopen(argv[1],"rt"))== NULL){
        return 1;
    }
    else{
        yylex();
    }
    fclose(yyin);
    return 0;
}