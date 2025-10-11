#include "./helper_init_variables.h"

/* -------------------------------------------------------------------------- */
/*                              VARIABLES GLOBALES                            */
/* -------------------------------------------------------------------------- */
char tipoDatoDeclaracionVariableActual[MAX_LONG_TIPO_SIMBOLO];

static t_pila *pilaVariablesDeclaradas;

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
    printf("Seteando tipo dato variables\n");

    char *idVariable;
    t_simbolo* simboloEncontrado;

    while (!pilaVacia(pilaVariablesDeclaradas))
    {
        idVariable = desapilar(pilaVariablesDeclaradas);
        simboloEncontrado = buscarSimboloPorNombre(idVariable);

        if (strcmp(simboloEncontrado->tipoDato, "") == 0)
        {
            // El símbolo existe pero no tiene tipo asignado - ACTUALIZAR EN LA TABLA
            strncpy(simboloEncontrado->tipoDato, tipoDatoDeclaracionVariableActual, MAX_LONG_TIPO_SIMBOLO - 1);
            simboloEncontrado->tipoDato[MAX_LONG_TIPO_SIMBOLO - 1] = '\0';
        }
        else
        {
            printf("Error: La variable '%s' ya fue declarada previamente como: '%s'\n", idVariable, simboloEncontrado->tipoDato);
            exit(1);
        }
    }
}

void setTipoDatoDeclaracionVariableActual(const char *tipoDato)
{
    strncpy(tipoDatoDeclaracionVariableActual, tipoDato, MAX_LONG_TIPO_SIMBOLO - 1);
    tipoDatoDeclaracionVariableActual[MAX_LONG_TIPO_SIMBOLO - 1] = '\0';
}