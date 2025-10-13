/* -------------------------------------------------------------------------- */
/*                           SECCION DE DEFINICIONES                          */
/* -------------------------------------------------------------------------- */

%{

#include "./Sintactico.h"

extern t_pila *pilaBase;

extern void apilarNroCeldaActualPolaca(t_pila *pila);
extern void apilarNroCeldaActualYAvanzarPolaca(t_pila *pila);
extern int desapilarNroCeldaYEscribirEnEllaNroCeldaActual(t_pila *pila);
extern int desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(t_pila *pila);
extern int desapilarNroCeldaYEscribirloEnCeldaActualPolaca(t_pila *pila);

extern void insertarEnPolacaNroCeldaActualMasTres();

extern void desapilarNroCeldaYEscribirEnEllaValor(t_pila *pila, const char *valor);

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
    | iteracion_while                                   {informarMatchLexicoSintactico("sentencia", "iteracion_while");}
    | asignacion                                        {informarMatchLexicoSintactico("sentencia", "asignacion");}
    | funcion_read                                      {informarMatchLexicoSintactico("sentencia", "funcion_read");}
    | funcion_write                                     {informarMatchLexicoSintactico("sentencia", "funcion_write");}
    ;

/* ------------------------------- ARITHMETIC ------------------------------- */
expresion:
    expresion OP_SUM termino                            {
                                                            informarMatchLexicoSintactico("expresion", "expresion OP_SUM termino");
                                                            
                                                            validarTipoDatoResolucionDeExpresion();
                                                            
                                                            insertarEnPolaca("+");
                                                        }
    | expresion OP_RES termino                          {
                                                            informarMatchLexicoSintactico("expresion", "expresion OP_RES termino");
                                                            
                                                            validarTipoDatoResolucionDeExpresion();
                                                            
                                                            insertarEnPolaca("-");
                                                        }
    | termino                                           {informarMatchLexicoSintactico("expresion", "termino");}
    ;

termino:
    termino OP_MUL factor                               {
                                                            informarMatchLexicoSintactico("termino", "termino OP_MUL factor");

                                                            validarTipoDatoResolucionDeExpresion(); 

                                                            insertarEnPolaca("*");
                                                        }
    | termino OP_DIV factor                             {
                                                            informarMatchLexicoSintactico("termino", "termino OP_DIV factor");

                                                            validarTipoDatoResolucionDeExpresion();

                                                            insertarEnPolaca("/");
                                                        }
    | factor                                            {informarMatchLexicoSintactico("termino", "factor");}
    ;

factor:
    ID                                                  {
                                                            informarMatchLexicoSintactico("factor", "ID");

                                                            insertarEnPolaca($1);

                                                            t_simbolo *simbolo = buscarSimboloIDEnTablaSimbolo($1);
                                                            apilarTipoDatoUtilizado(simbolo->tipoDato);
                                                        }
    | CONST_INT                                         {
                                                            informarMatchLexicoSintactico("factor", "CONST_INT");

                                                            insertarEnPolaca($1);

                                                            apilarTipoDatoUtilizado(TIPO_TOKEN_CONST_INT);
                                                        }
    | CONST_FLOAT                                       {
                                                            informarMatchLexicoSintactico("factor", "CONST_FLOAT");

                                                            insertarEnPolaca($1);

                                                            apilarTipoDatoUtilizado(TIPO_TOKEN_CONST_FLOAT);
                                                        }
    | PAR_A expresion PAR_C                             {informarMatchLexicoSintactico("factor", "PAR_A expresion PAR_C");}
    | funciones_temas_especiales                        {informarMatchLexicoSintactico("sentencia", "funciones_temas_especiales");}
    ;

/* ------------------------------- ASSIGMENTS ------------------------------- */
asignacion:
    ID OP_ASIG_VALOR expresion                          {
                                                            informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR expresion");
                                                            
                                                            validarIDAsignacionEsTipoDatoExpresion($1);
                                                            
                                                            insertarEnPolaca($1);
                                                            insertarEnPolaca(":=");
                                                        }
    | ID OP_ASIG_VALOR CONST_STR                        {
                                                            informarMatchLexicoSintactico("asignacion", "ID OP_ASIG_VALOR CONST_STR");
                                                            
                                                            validarIDAsignacionEsTipoDatoString($1);
                                                            
                                                            insertarEnPolaca($3);
                                                            insertarEnPolaca($1);
                                                            insertarEnPolaca(":=");
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
    conjunto_ids OP_ASIG_TIPO tipo_dato                 {
                                                            informarMatchLexicoSintactico("declaracion", "conjunto_ids OP_ASIG_TIPO tipo_dato");

                                                            setTipoDatoEnTSParaVariablesDeclaradas();
                                                        }
    ;

conjunto_ids:
    conjunto_ids COMA ID                                {
                                                            informarMatchLexicoSintactico("conjunto_ids", "conjunto_ids COMA ID");

                                                            apilarIDVariableDeclarada($3);
                                                        }
    | ID                                                {
                                                            informarMatchLexicoSintactico("conjunto_ids", "ID");

                                                            apilarIDVariableDeclarada($1);
                                                        }
    ;

tipo_dato:
    INT                                                 {
                                                            informarMatchLexicoSintactico("tipo_dato", "INT");

                                                            setTipoDatoDeclaracionVariableActual(TIPO_TOKEN_CONST_INT);
                                                        }
    | FLOAT                                             {
                                                            informarMatchLexicoSintactico("tipo_dato", "FLOAT");

                                                            setTipoDatoDeclaracionVariableActual(TIPO_TOKEN_CONST_FLOAT);
                                                        }
    | STRING                                            {
                                                            informarMatchLexicoSintactico("tipo_dato", "STRING");

                                                            setTipoDatoDeclaracionVariableActual(TIPO_TOKEN_CONST_STR);
                                                        }
    ;

/* ----------------------------------- IF ----------------------------------- */
seleccion_con_else:
    bloque_if 
                {
                    if(esCondicionalConDosExpresiones() == 1)
                    {
                        // SIRVE PARA EL AND y OR
                        insertarEnPolaca("BI");
                        apilarNroCeldaActualYAvanzarPolaca(pilaBase);
                    }
                    else
                    {
                        generarCodigoFinBloqueVerdaderoIfConElse();
                    }
                }
                bloque_else                               
                                {
                                    informarMatchLexicoSintactico("seleccion_con_else", "bloque_if bloque_else");
                                    
                                    if(esCondicionalConDosExpresiones() == 1)
                                    {
                                        char operadorLogicoActual[TAM_CONTENIDO_PILA];
                                        strcpy(operadorLogicoActual, getOperadorLogicoActual());

                                        // PARA EL AND
                                        if(strcmp(operadorLogicoActual, "AND") == 0)
                                        {
                                            int nroCeldaDesapilado = desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);
                                            char nroCeldaDesapiladoMasUno[TAM_CONTENIDO_PILA];

                                            sprintf(nroCeldaDesapiladoMasUno, "%d", nroCeldaDesapilado + 1);

                                            desapilarNroCeldaYEscribirEnEllaValor(pilaBase, nroCeldaDesapiladoMasUno);

                                            desapilarNroCeldaYEscribirEnEllaValor(pilaBase, nroCeldaDesapiladoMasUno);
                                        }
                                        // PARA EL OR
                                        else
                                        {
                                            int nroCeldaDesapilado = desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);
                                            char nroCeldaDesapiladoMasUno[TAM_CONTENIDO_PILA];

                                            sprintf(nroCeldaDesapiladoMasUno, "%d", nroCeldaDesapilado + 1);

                                            desapilarNroCeldaYEscribirEnEllaValor(pilaBase, nroCeldaDesapiladoMasUno);

                                            desapilarNroCeldaYEscribirEnEllaNroCeldaAuxInicioTrueOR(pilaBase);
                                        }

                                    }
                                    else
                                    {
                                        generarCodigoFinBloqueElse();
                                    }
                                }
    ;

seleccion_sin_else:
    bloque_if   
                {
                    informarMatchLexicoSintactico("seleccion_sin_else", "bloque_if");
                        
                    if(esCondicionalConDosExpresiones() == 1)
                    {
                        char operadorLogicoActual[TAM_CONTENIDO_PILA];
                        strcpy(operadorLogicoActual, getOperadorLogicoActual());

                        // PARA EL AND
                        if(strcmp(operadorLogicoActual, "AND") == 0)
                        {
                            desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);
                            desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);
                        }
                        // PARA EL OR
                        else
                        {   
                            desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);
                            desapilarNroCeldaYEscribirEnEllaNroCeldaAuxInicioTrueOR(pilaBase);
                        }
                    }
                    else
                    {
                        generarCodigoFinBloqueVerdaderoIfSinElse();
                    }

                    reiniciarPilaOperadoresLogicosUtilizados();
                }
    ;


bloque_if:
    IF PAR_A condicional 
                        {
                            if(esCondicionalConDosExpresiones() == 1)
                            {
                                char operadorLogicoActual[TAM_CONTENIDO_PILA];
                                strcpy(operadorLogicoActual, getOperadorLogicoActual());

                                // Si es un OR tengo que guardar el inicio de la parte TRUE
                                if(strcmp(operadorLogicoActual, "OR") == 0)
                                {
                                    setearNroCeldaActualPolacaAuxInicioTrueSoloParaOR();
                                }
                            }
                            else
                            {
                                generarCodigoFinCondicionIf();
                            }
                        }
                        PAR_C LLA_A conjunto_sentencias LLA_C      { informarMatchLexicoSintactico("bloque_if", "IF PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C");}
    ;

/* ---------------------------------- ELSE ---------------------------------- */
bloque_else:
    ELSE LLA_A conjunto_sentencias LLA_C                {informarMatchLexicoSintactico("bloque_else", "ELSE LLA_A conjunto_sentencias LLA_C");}
    ;


condicional:
    condicion                                           {
                                                            informarMatchLexicoSintactico("condicional", "condicion");
                                                        }
    | condicion operador_logico { 
                                    char operadorLogicoActual[TAM_CONTENIDO_PILA];
                                    strcpy(operadorLogicoActual, getOperadorLogicoActual());

                                    // PARA EL AND
                                    if(strcmp(operadorLogicoActual, "AND") == 0)
                                    {
                                        apilarNroCeldaActualYAvanzarPolaca(pilaBase);
                                    }
                                    // PARA EL OR
                                    else
                                    {
                                        insertarEnPolacaNroCeldaActualMasTres();
                                        insertarEnPolaca("BI");
                                        apilarNroCeldaActualYAvanzarPolaca(pilaBase);
                                    }
                                }
                                condicion               
                                                                                            {
                                                                                                informarMatchLexicoSintactico("condicional", "condicion operador_logico condicion");

                                                                                                // SIRVE PARA EL AND y OR
                                                                                                apilarNroCeldaActualYAvanzarPolaca(pilaBase);
                                                                                            }
    | NOT { setNegacionPendienteOperadorComparacion(1); } condicion    {
                                                                            informarMatchLexicoSintactico("condicional", "NOT condicion");

                                                                            setNegacionPendienteOperadorComparacion(0);
                                                                        }
    ;



condicion:
    expresion operador_comparacion expresion           {
                                                            informarMatchLexicoSintactico("condicion", "expresion operador_comparacion expresion");

                                                            validarTipoDatoExpresionesAlComparar();

                                                            insertarEnPolaca(VALOR_POLACA_COMPARADOR);
                                                            
                                                            if(hayQueNegarOperadorComparacionActual() == 1)
                                                            {
                                                                negarOperadorOperacionActual();
                                                            }
                                                            insertarEnPolaca(getOperadorComparacionPendienteActual());
                                                        }
    ;

operador_comparacion:
    OP_IGUAL                                            {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_IGUAL");

                                                            setOperadorComparacionPendienteActual(VALOR_POLACA_OP_IGUAL);
                                                        }
    | OP_DISTINTO                                       {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_DISTINTO");

                                                            setOperadorComparacionPendienteActual(VALOR_POLACA_OP_DISTINTO);
                                                        }
    | OP_MAYOR                                          {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MAYOR");

                                                            setOperadorComparacionPendienteActual(VALOR_POLACA_OP_MAYOR);
                                                        }
    | OP_MAYOR_IGUAL                                    {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MAYOR_IGUAL");

                                                            setOperadorComparacionPendienteActual(VALOR_POLACA_OP_MAYOR_IGUAL);
                                                        }
    | OP_MENOR                                          {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MENOR");

                                                            setOperadorComparacionPendienteActual(VALOR_POLACA_OP_MENOR);
                                                        }
    | OP_MENOR_IGUAL                                    {
                                                            informarMatchLexicoSintactico("operador_comparacion", "OP_MENOR_IGUAL");

                                                            setOperadorComparacionPendienteActual(VALOR_POLACA_OP_MENOR_IGUAL);
                                                        }
    ;

operador_logico:
    AND                                                 {
                                                            informarMatchLexicoSintactico("operador_logico", "AND");

                                                            apilarOperadorLogicoUtilizado("AND");
                                                        }
    | OR                                                {
                                                            informarMatchLexicoSintactico("operador_logico", "OR");

                                                            apilarOperadorLogicoUtilizado("OR");
                                                        }
    ;
    
/* ---------------------------------- WHILE --------------------------------- */
iteracion_while:
    WHILE 
        {
            // SIRVE PARA CONDICION SIMPEL Y COMPUESTA (AND Y OR)
            // SERIA DONDE ESTA LA ETIQUETA ET Y SABER A DONDE VOLVER CUANDO SE TIENE QUE REPETIR EL WHILE
            generarCodigoInicioWhile();
        }
        PAR_A condicional 
                            {                      
                                // SIRVE EL WHILE SIMPLE Y SERIA PARA GUARDAR EL FIN DE LA CONDICION QUE TIENE QUE SALTAR A LA PARTE DE ABAJO DEL WHILE OSEA NO EJECUTARLO
                                if(esCondicionalConDosExpresiones() == 1)
                                {
                                    char operadorLogicoActual[TAM_CONTENIDO_PILA];
                                    strcpy(operadorLogicoActual, getOperadorLogicoActual());

                                    // Si es un OR tengo que guardar el inicio de la parte TRUE
                                    if(strcmp(operadorLogicoActual, "OR") == 0)
                                    {
                                        setearNroCeldaActualPolacaAuxInicioTrueSoloParaOR();
                                    }

                                    // AND NO NECESITA HACER NADA
                                }
                                else
                                {
                                    generarCodigoFinCondicionWhile();
                                }
                            }
                            PAR_C LLA_A conjunto_sentencias LLA_C   
                                                                    {
                                                                        informarMatchLexicoSintactico("iteracion_while", "WHILE PAR_A condicional PAR_C LLA_A conjunto_sentencias LLA_C");

                                                                        if(esCondicionalConDosExpresiones() == 1)
                                                                        {
                                                                            char operadorLogicoActual[TAM_CONTENIDO_PILA];
                                                                            strcpy(operadorLogicoActual, getOperadorLogicoActual());
 
                                                                            // PARA EL AND
                                                                            if(strcmp(operadorLogicoActual, "AND") == 0)
                                                                            {
                                                                                insertarEnPolaca("BI");
                                                                                desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(pilaBase);
                                                                                desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(pilaBase);
                                                                                desapilarNroCeldaYEscribirloEnCeldaActualPolaca(pilaBase);
                                                                            }
                                                                            // PARA EL OR
                                                                            else
                                                                            {   
                                                                                insertarEnPolaca("BI");
                                                                                desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(pilaBase);
                                                                                desapilarNroCeldaYEscribirEnEllaNroCeldaAuxInicioTrueOR(pilaBase);
                                                                                desapilarNroCeldaYEscribirloEnCeldaActualPolaca(pilaBase);
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            generarCodigoFinWhile();
                                                                        }
                                                                    }
    ;

/* ---------------------------------- READ ---------------------------------- */
funcion_read:
    READ PAR_A ID PAR_C                                 {
                                                            informarMatchLexicoSintactico("funcion_read", "READ PAR_A ID PAR_C");

                                                            insertarEnPolaca($3);
                                                            insertarEnPolaca("READ");
                                                        }
    ;

/* ---------------------------------- WRITE --------------------------------- */
funcion_write:
    WRITE PAR_A CONST_STR PAR_C                         {
                                                            informarMatchLexicoSintactico("funcion_write", "WRITE PAR_A CONST_STR PAR_C");

                                                            insertarEnPolaca($3);
                                                            insertarEnPolaca("WRITE");
                                                        }
    | WRITE PAR_A ID PAR_C                              {
                                                            informarMatchLexicoSintactico("funcion_write", "WRITE PAR_A ID PAR_C");

                                                            insertarEnPolaca($3);
                                                            insertarEnPolaca("WRITE");
                                                        }
    ;

/* ---------------------------- TEMAS ESPECIALES ---------------------------- */
funciones_temas_especiales:
    funcion_equal_expressions                                       {informarMatchLexicoSintactico("funciones_temas_especiales", "funcion_equal_expressions");}
    | funcion_triangle_area_maximum                                 {informarMatchLexicoSintactico("funciones_temas_especiales", "funcion_triangle_area_maximum");}
    ;

/* ---------------------------- EQUAL EXPRESSIONS --------------------------- */
funcion_equal_expressions:
    EQUAL_EXP PAR_A parametros_equal_expressions PAR_C              {informarMatchLexicoSintactico("funcion_equal_expressions", "EQUAL_EXP PAR_A parametros_equal_expressions PAR_C");}
    ;

parametros_equal_expressions:
    expresion COMA expresion                                        {informarMatchLexicoSintactico("parametros_equal_expressions", "expresion COMA expresion");}
    | parametros_equal_expressions COMA expresion                   {informarMatchLexicoSintactico("parametros_equal_expressions", "parametros_equal_expressions COMA expresion");}
    ;


/* ---------------------- FUNCION TRIANGLE AREA MAXIMUM --------------------- */
funcion_triangle_area_maximum:
    TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C          {
                                                                        informarMatchLexicoSintactico("funcion_triangle_area_maximum", "TRIAN_MAX PAR_A parametros_triangle_area_maximum PAR_C");
                                                                        generarPolacaTriangleAreaMaximun();
                                                                    }
    ;

parametros_triangle_area_maximum:
    coordenadas_triangulo PUNTO_C coordenadas_triangulo             {informarMatchLexicoSintactico("parametros_triangle_area_maximum", "coordenadas_triangulo PUNTO_C coordenadas_triangulo");}
    ;

coordenadas_triangulo:
    COR_A 
        {
            iniciarTriangulo();
        }
        conjunto_puntos_triangulo COR_C                             {
                                                                        informarMatchLexicoSintactico("coordenadas_triangulo", "COR_A conjunto_puntos_triangulo COR_C");

                                                                        generarPolacaCalculoAreaTriangulo();
                                                                    }
    ;

conjunto_puntos_triangulo:
    coordenada_triangulo PUNTO_C coordenada_triangulo PUNTO_C coordenada_triangulo {informarMatchLexicoSintactico("conjunto_puntos_triangulo", "coordenada_triangulo PUNTO_C coordenada_triangulo PUNTO_C coordenada_triangulo");}
    ;

coordenada_triangulo:
    tipo_parametro_triangle_area COMA tipo_parametro_triangle_area                  {informarMatchLexicoSintactico("coordenada_triangulo", "tipo_parametro_triangle_area COMA tipo_parametro_triangle_area");}
    ;

tipo_parametro_triangle_area:
    CONST_INT                                           {
                                                            informarMatchLexicoSintactico("tipo_parametro_triangle_area", "CONST_INT");

                                                            apilarCoordenadaTriangulo($1);
                                                        }
    | CONST_FLOAT                                       {
                                                            informarMatchLexicoSintactico("tipo_parametro_triangle_area", "CONST_FLOAT");

                                                            apilarCoordenadaTriangulo($1);
                                                        }
    | ID                                                {
                                                            informarMatchLexicoSintactico("tipo_parametro_triangle_area", "ID");
                                                            
                                                            apilarCoordenadaTriangulo($1);
                                                        }
    ;


%%