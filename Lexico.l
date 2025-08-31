
/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{
#include "./utils/utils.h"
#include "./utils/informes.h"
#include "./utils/validacion.h"
#include "./utils/tabla_simbolos.h"

// Solo  se usa cuando se usa el analizador sintactico, el script se encarga de comentarlo para que no falle
#include "y.tab.h"


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

COMENTARIO      "#+"([^+]|(\n|\+[^#]))*"+#"


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


{ID}			{informarMatch(yytext, "ID"); procesarMatchID(yytext); }

{CONST_INT}		{informarMatch(yytext, "CONST_INT"); validarRangoInt(yytext); procesarMatchConstante(yytext, "CONST_INT");}
{CONST_FLOAT}	{informarMatch(yytext, "CONST_FLOAT"); validarRangoFloat(yytext); procesarMatchConstante(yytext, "CONST_FLOAT");}
{CONST_STR}		{informarMatch(yytext, "CONST_STR"); validarLongitudString(yytext); procesarMatchConstante(yytext, "CONST_STR");}

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

/* ------------------------------- INFORMES.C ------------------------------- */
void informarMatch(char* lexema, char* nombreDelToken) {
    printf("\n\nLEXEMA: %-20s - TOKEN: %-15s", lexema, nombreDelToken);
}

void informarErrorNoMatch(char* lexema)
{
    printf("\n\nLEXEMA: %-20s - LINEA: %-10d - ERROR NO-MATCH", lexema, yylineno);
}

void informarErrorValidacion(char* name_token)
{
    printf("\nLINEA:  %-20d - ERROR VALIDACION: %-20s", yylineno, name_token);
}



/* ----------------------------- VALIDACIONES.C ----------------------------- */
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


/* ---------------------------- TABLA_SIMBOLOS.C ---------------------------- */
// MANEJO DE TABLA
void inicializarTablaSimbolos(t_tabla_simbolos* tabla) {
    if (tabla == NULL) {
        return;
    }
    tabla->cantidad = 0;
}

int insertarSimbolo(t_tabla_simbolos* tabla, const t_simbolo* simbolo) {
    if (tabla == NULL || simbolo == NULL) {
        printf("Error: Lista o símbolo NULL en insertarSimbolo\n");
        return 0;
    }
    
    if (tablaSimbolosLlena(tabla)) {
        printf("Error: Lista llena, no se puede insertar\n");
        return 0;
    }
    
    if (existeSimbolo(tabla, simbolo->nombre)) {
        printf("Warning: El símbolo '%s' ya existe en la tabla de simbolos\n", simbolo->nombre);
        return 0;
    }
    
    tabla->elementos[tabla->cantidad] = *simbolo;

    tabla->cantidad++;
    
    return 1;
}

int existeSimbolo(const t_tabla_simbolos* tabla, const char* nombre) {
    if (tabla == NULL || nombre == NULL) {
        return 0;
    }
    
    for (int i = 0; i < tabla->cantidad; i++) {
        // TODO: preguntar el criterio para saber si un simbolo ya existe o no 
        if (strcmp(tabla->elementos[i].nombre, nombre) == 0) {
            return 1;
        }
    }
    
    return 0;
}

int tablaSimbolosLlena(const t_tabla_simbolos* tabla) {
    if (tabla == NULL) {
        return 1;
    }
    return tabla->cantidad == MAX_LISTA;
}



// HANDLERS SEGUN TIPO LEXEMA
int procesarLexemaTablaID(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaID\n");
        return 0;
    }

    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;

    snprintf(nombre_simbolo, sizeof(nombre_simbolo), "%s", lexema);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", "");
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", "");
    longitud_simbolo = 0;

    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);

    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        printf(" [ID agregado a tabla simbolos: (simbolo) %s = (lexema procesado) %s]", nombre_simbolo, lexema);
    }
    
    return resultado;
}

int procesarLexemaTablaConstanteInt(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaConstanteInt\n");
        return 0;
    }
    
    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;
    
    snprintf(nombre_simbolo, sizeof(nombre_simbolo), "_%s", lexema);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", TIPO_INT);
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", lexema);
    longitud_simbolo = strlen(lexema);
    
    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);
    
    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        printf(" [Constante INT agregada a tabla simbolos: (simbolo) %s = (lexema procesado) %s]", nombre_simbolo, lexema);
    }
    
    return resultado;
}

int procesarLexemaTablaConstanteFloat(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaConstanteFloat\n");
        return 0;
    }
    
    // NOMBRE: Generar un nombre que no sea tal cual el valor ya que puedo tener "." o "-"
    char valor_normalizado[MAX_LONG_NOMBRE_SIMBOLO];
    strcpy(valor_normalizado, lexema);
    
    // Reemplazar caracteres especiales para hacer un nombre válido
    for (int i = 0; valor_normalizado[i]; i++) {
        if (valor_normalizado[i] == '.') {
            valor_normalizado[i] = '_';
        }
        // TODO: revisar si nos va afectar ponerle N
        else if (valor_normalizado[i] == '-') {
            valor_normalizado[i] = 'N';
        }
    }
    
    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;

    snprintf(nombre_simbolo, sizeof(nombre_simbolo) + 1, "_%s", valor_normalizado);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", TIPO_FLOAT);
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", lexema);
    longitud_simbolo = strlen(lexema);

    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);
    
    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        printf(" [Constante FLOAT agregada a tabla simbolos: (simbolo) %s = (lexema procesado) %s]", nombre_simbolo, lexema);
    }
    
    return resultado;
}

int procesarLexemaTablaConstanteString(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaConstanteString\n");
        return 0;
    }

    // NOMBRE: Generar uno único usando un contador ya que sino  me queda el valor y nombre de simbolo iguales
    static int contador_string = 0;
    char nombre_simbolo_incremental[MAX_LONG_NOMBRE_SIMBOLO];
    snprintf(nombre_simbolo_incremental, sizeof(nombre_simbolo_incremental), "_CTE_STRING_%d", ++contador_string);

    // VALOR: Quedarme solo con el contenido del string
    char valor_sin_comillas[MAX_LONG_VALOR_SIMBOLO];
    int len = strlen(lexema);
    if (len >= 2 && lexema[0] == '"' && lexema[len-1] == '"') {
        strncpy(valor_sin_comillas, lexema + 1, len - 2);
        valor_sin_comillas[len - 2] = '\0';
    } else {
        // Para mayra: sirve para la cadena vacia
        strcpy(valor_sin_comillas, lexema);
    }
    
    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;
    
    snprintf(nombre_simbolo, sizeof(nombre_simbolo), "%s", nombre_simbolo_incremental);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", TIPO_STRING);
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", valor_sin_comillas);
    longitud_simbolo = strlen(valor_sin_comillas);

    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);

    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        printf(" [Constante STRING agregada a tabla simbolos: (simbolo) %s = (lexema procesado) %s]", nombre_simbolo, lexema);
    }
    
    return resultado;
}



/* -------------------------------- SIMBOLO.C ------------------------------- */
t_simbolo crearSimbolo(const char* nombre, const char* tipoDato, const char* valor, int longitud) {
    t_simbolo simbolo;
    
    memset(&simbolo, 0, sizeof(t_simbolo));
    
    // TODO: revisar si es correcto ya que estaria truncando
    if (nombre != NULL) {
        strncpy(simbolo.nombre, nombre, MAX_LONG_NOMBRE_SIMBOLO - 1);
        simbolo.nombre[MAX_LONG_NOMBRE_SIMBOLO - 1] = '\0';
    }
    
    if (tipoDato != NULL) {
        strncpy(simbolo.tipoDato, tipoDato, MAX_LONG_TIPO_SIMBOLO - 1);
        simbolo.tipoDato[MAX_LONG_TIPO_SIMBOLO - 1] = '\0';
    }
    
    if (valor != NULL) {
        strncpy(simbolo.valor, valor, MAX_LONG_VALOR_SIMBOLO - 1);
        simbolo.valor[MAX_LONG_VALOR_SIMBOLO - 1] = '\0';
    }
    
    simbolo.longitud = longitud;
    
    return simbolo;
}



/* --------------------------------- UTILS.C -------------------------------- */
void procesarMatchID(const char* lexema) {
    procesarLexemaTablaID(&tabla_simbolos, lexema);
}

void procesarMatchConstante(const char* lexema, const char* tipo_token) {
    if (strcmp(tipo_token, "CONST_INT") == 0) {
        procesarLexemaTablaConstanteInt(&tabla_simbolos, lexema);
    } else if (strcmp(tipo_token, "CONST_FLOAT") == 0) {
        procesarLexemaTablaConstanteFloat(&tabla_simbolos, lexema);
    } else if (strcmp(tipo_token, "CONST_STR") == 0) {
        procesarLexemaTablaConstanteString(&tabla_simbolos, lexema);
    }
}

void crearTablaSimbolos(void) {
    inicializarTablaSimbolos(&tabla_simbolos);
}

int exportarTablaSimbolos(const char* nombre_archivo) {
    if (nombre_archivo == NULL) {
        printf("Error: Nombre de archivo NULL\n");
        return 0;
    }
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
        return 0;
    }
    
    fprintf(archivo, "%s|%s|%s|%s\n", "NOMBRE", "TIPO", "VALOR", "LONGITUD");
    
    // Iterar sobre todos los símbolos
    for (int i = 0; i < tabla_simbolos.cantidad; i++) {
        t_simbolo* simbolo = &(tabla_simbolos.elementos[i]);
        fprintf(archivo, "%s|%s|%s|%d\n", 
                simbolo->nombre, 
                simbolo->tipoDato, 
                simbolo->valor, 
                simbolo->longitud);
    }
    
    fclose(archivo);
    printf("\n✓ Tabla de símbolos guardada en: %s\n", nombre_archivo);
    return 1;
}



/* ---------------------------------- MAIN ---------------------------------- */
t_tabla_simbolos tabla_simbolos;

int main (int argc, char *argv[]){
    if((yyin = fopen(argv[1],"rt"))== NULL){
        return 1;
    }
    else{
        crearTablaSimbolos();
        
        yylex();
        
        exportarTablaSimbolos("test_outputs/tabla_simbolos.txt");
    }
    fclose(yyin);
    return 0;
}