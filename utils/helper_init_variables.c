#include "./helper_init_variables.h"

char tipoDatoDeclaracionVariableActual[MAX_LONG_TIPO_SIMBOLO];
t_pila *pilaVariablesDeclaradas;

/* -------------------------------------------------------------------------- */
/*                          FUNCIONES DE INICIALIZACIÓN                       */
/* -------------------------------------------------------------------------- */

void inicializarInitVariablesHelper()
{
    pilaVariablesDeclaradas = crearPila();
}

void limpiarInitVariablesHelper()
{
    eliminarPila(pilaVariablesDeclaradas);
}

/* -------------------------------------------------------------------------- */
/*                              FUNCIONES DE INIT                             */
/* -------------------------------------------------------------------------- */
void apilarIDVariableDeclarada(const char *id)
{
    apilar(pilaVariablesDeclaradas, id);
}

void setTipoDatoEnTSParaVariablesDeclaradas()
{
    char idVariable[TAM_CONTENIDO_PILA];
    t_simbolo *simboloEncontrado;

    while (!pilaVacia(pilaVariablesDeclaradas))
    {
        strcpy(idVariable, desapilar(pilaVariablesDeclaradas));
        simboloEncontrado = buscarSimboloPorNombre(idVariable);

        if (strcmp(simboloEncontrado->tipoDato, "") != 0)
        {
            /*
                CASO PRUEBA: 
                init { 
                    variableFLOAT : float 
                    variableFLOAT : int 
                }
            */ 
            printf("[SINTACTICO] 100. Error declaracion variable: La variable '%s' ya fue declarada previamente como: '%s'\n", idVariable, simboloEncontrado->tipoDato);
            exit(1);
        }

        // El símbolo existe pero no tiene tipo asignado - ACTUALIZAR EN LA TABLA
        strncpy(simboloEncontrado->tipoDato, tipoDatoDeclaracionVariableActual, MAX_LONG_TIPO_SIMBOLO - 1);
        simboloEncontrado->tipoDato[MAX_LONG_TIPO_SIMBOLO - 1] = '\0';
    }
}

t_simbolo *buscarSimboloIDEnTablaSimbolo(const char *nombre)
{
    t_simbolo *simbolo = buscarSimboloPorNombre(nombre);

    if (simbolo == NULL || strcmp(simbolo->tipoDato, "") == 0)
    {
        /*
            CASO PRUEBA: init { variableDECLARADA : float } 
            variableNODECLARADA := 4
        */ 
        printf("[SINTACTICO] 101. Error uso de variable: La variable '%s' no fue declarada previamente.\n", nombre);
        exit(1);
    }

    return simbolo;
}

void setTipoDatoDeclaracionVariableActual(const char *tipoDato)
{
    strncpy(tipoDatoDeclaracionVariableActual, tipoDato, MAX_LONG_TIPO_SIMBOLO - 1);
    tipoDatoDeclaracionVariableActual[MAX_LONG_TIPO_SIMBOLO - 1] = '\0';
}