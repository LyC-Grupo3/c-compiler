
/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{

#include "./Lexico.h"

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


/* --------------------------- OPERADORES LOGICOS --------------------------- */
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
CONST_FLOAT     -?({DIGITO}*"."{DIGITO}*([eE][+-]?{DIGITO}+)?|{DIGITO}+[eE][+-]?{DIGITO}+|{DIGITO}+".") 
CONST_STR       \"[^\n"]*\"

OP_ASIG_VALOR   ":="
OP_ASIG_TIPO    ":"

COMENTARIO      "#+"[ \t\n\"\:\=\$\+\*\(\)\{\}\<\>\.\,\;\!\?\@\.0-9a-zA-ZáéíóúÁÉÍÓÚñÑüÜ_-]*"+#"


/* ------------------------- OPERADORES ARITMETICOS ------------------------- */
OP_SUM		     "+"
OP_MUL           "*"
OP_RES		     "-"
OP_DIV           "/"


/* --------------------------- OPERADORES COMPARACION --------------------------- */
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



/* -------------------------------------------------------------------------- */
/*                              SECCION DE REGLAS                             */
/* -------------------------------------------------------------------------- */

/* Cuando se produce un match lo informa, luego para bizon agregar el return */
%%


{INT}			{informarMatchLexico(yytext, "INT"); return INT;}
{FLOAT}			{informarMatchLexico(yytext, "FLOAT"); return FLOAT;}
{STRING}		{informarMatchLexico(yytext, "STRING"); return STRING;}
{BOOL}			{informarMatchLexico(yytext, "BOOL"); return BOOL;}


{AND}			{informarMatchLexico(yytext, "AND"); return AND;}
{OR}			{informarMatchLexico(yytext, "OR"); return OR;}
{NOT}			{informarMatchLexico(yytext, "NOT"); return NOT;}


{WHILE}			{informarMatchLexico(yytext, "WHILE"); return WHILE;}

{IF}			{informarMatchLexico(yytext, "IF"); return IF;}
{ELSE}			{informarMatchLexico(yytext, "ELSE"); return ELSE;}

{INIT}			{informarMatchLexico(yytext, "INIT"); return INIT;}

{WRITE}			{informarMatchLexico(yytext, "WRITE"); return WRITE;}
{READ}			{informarMatchLexico(yytext, "READ"); return READ;}


{CONST_BOOL_T}	{informarMatchLexico(yytext, "CONST_BOOL_T"); return CONST_BOOL_T;}
{CONST_BOOL_F}	{informarMatchLexico(yytext, "CONST_BOOL_F"); return CONST_BOOL_F;}


{EQUAL_EXP}		{informarMatchLexico(yytext, "EQUAL_EXP"); return EQUAL_EXP;}
{TRIAN_MAX}		{informarMatchLexico(yytext, "TRIAN_MAX"); return TRIAN_MAX;}


{ID}			{informarMatchLexico(yytext, "ID"); procesarSimbolo(yytext, TIPO_TOKEN_ID); strcpy(yylval.lexema, yytext); return ID;}

{CONST_INT}		{
    informarMatchLexico(yytext, TIPO_TOKEN_CONST_INT); 
    if (validarRangoInt(yytext) == 0) {
        procesarSimbolo(yytext, TIPO_TOKEN_CONST_INT);
    } else {
        informarErrorValidacion(yytext, TIPO_TOKEN_CONST_INT, "El valor excede el rango permitido", yylineno);
        exit(1);
    }
    strcpy(yylval.lexema, yytext);
    return CONST_INT;
}
{CONST_FLOAT}	{
    informarMatchLexico(yytext, TIPO_TOKEN_CONST_FLOAT); 
    if (validarRangoFloat(yytext) == 0) {
        procesarSimbolo(yytext, TIPO_TOKEN_CONST_FLOAT);
    } else {
        informarErrorValidacion(yytext, TIPO_TOKEN_CONST_FLOAT, "El valor excede el rango permitido", yylineno);
        exit(1);
    }
    strcpy(yylval.lexema, yytext);
    return CONST_FLOAT;
}
{CONST_STR}		{
    informarMatchLexico(yytext, TIPO_TOKEN_CONST_STR); 
    if (validarLongitudString(yytext) == 0) {
        procesarSimbolo(yytext, TIPO_TOKEN_CONST_STR);
    } else {
        informarErrorValidacion(yytext, TIPO_TOKEN_CONST_STR, "La longitud del string excede el maximo permitido", yylineno);
        exit(1);
    }
    strcpy(yylval.lexema, yytext);
    return CONST_STR;
}

{OP_ASIG_VALOR}	{informarMatchLexico(yytext, "OP_ASIG_VALOR"); return OP_ASIG_VALOR;}
{OP_ASIG_TIPO}	{informarMatchLexico(yytext, "OP_ASIG_TIPO"); return OP_ASIG_TIPO;}

{COMENTARIO}	{informarMatchLexico(yytext, "COMENTARIO");}


{OP_SUM}		{informarMatchLexico(yytext, "OP_SUM"); return OP_SUM;}
{OP_MUL}		{informarMatchLexico(yytext, "OP_MUL"); return OP_MUL;}
{OP_RES}		{informarMatchLexico(yytext, "OP_RES"); return OP_RES;}
{OP_DIV}		{informarMatchLexico(yytext, "OP_DIV"); return OP_DIV;}


{OP_MAYOR}		{informarMatchLexico(yytext, "OP_MAYOR"); return OP_MAYOR;}
{OP_MAYOR_IGUAL} {informarMatchLexico(yytext, "OP_MAYOR_IGUAL"); return OP_MAYOR_IGUAL;}

{OP_MENOR}		{informarMatchLexico(yytext, "OP_MENOR"); return OP_MENOR;}
{OP_MENOR_IGUAL} {informarMatchLexico(yytext, "OP_MENOR_IGUAL"); return OP_MENOR_IGUAL;}

{OP_IGUAL}		{informarMatchLexico(yytext, "OP_IGUAL"); return OP_IGUAL;}
{OP_DISTINTO}	{informarMatchLexico(yytext, "OP_DISTINTO"); return OP_DISTINTO;}


{PAR_A}			{informarMatchLexico(yytext, "PAR_A"); return PAR_A;}
{PAR_C}			{informarMatchLexico(yytext, "PAR_C"); return PAR_C;}
{LLA_A}			{informarMatchLexico(yytext, "LLA_A"); return LLA_A;}
{LLA_C}			{informarMatchLexico(yytext, "LLA_C"); return LLA_C;}
{COR_A}			{informarMatchLexico(yytext, "COR_A"); return COR_A;}
{COR_C}			{informarMatchLexico(yytext, "COR_C"); return COR_C;}


{PUNTO_C}		{informarMatchLexico(yytext, "PUNTO_C"); return PUNTO_C;}
{COMA}			{informarMatchLexico(yytext, "COMA"); return COMA;}



"\n"      		
"\t"
"\n\t"
" "             		
"\r\n"


.   { 
        informarErrorNoMatchLexico(yytext, yylineno); 
        exit(1);
    } 


%%