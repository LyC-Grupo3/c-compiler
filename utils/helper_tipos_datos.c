#include "./helper_tipos_datos.h"

t_pila *pilaTiposDatosUtilizados;

/* -------------------------------------------------------------------------- */
/*                          FUNCIONES DE INICIALIZACIÓN                       */
/* -------------------------------------------------------------------------- */
void inicializarTiposDatosHelper()
{
    pilaTiposDatosUtilizados = crearPila();
}

void limpiarTiposDatosHelper()
{
    eliminarPila(pilaTiposDatosUtilizados);
}

/* -------------------------------------------------------------------------- */
/*                          FUNCIONES TIPOS DE DATOS                          */
/* -------------------------------------------------------------------------- */
void apilarTipoDatoUtilizado(const char *tipoDato)
{
    apilar(pilaTiposDatosUtilizados, tipoDato);
}

// Para resolver expresiones
void validarTipoDatoResolucionDeExpresion()
{
    char tipoDatoExpresion1[TAM_CONTENIDO_PILA];
    strcpy(tipoDatoExpresion1, desapilar(pilaTiposDatosUtilizados));
    
    char tipoDatoExpresion2[TAM_CONTENIDO_PILA];
    strcpy(tipoDatoExpresion2, desapilar(pilaTiposDatosUtilizados));

    /*
        CASO PRUEBA:
        init {
                variableINT : int
                variableSTRING : string
        }

        if ((variableINT + variableSTRING) > 4.0)
        {
            write("Hola")
        }
    */
    if (strcmp(tipoDatoExpresion1, TIPO_TOKEN_CONST_STR) == 0 || strcmp(tipoDatoExpresion2, TIPO_TOKEN_CONST_STR) == 0)
    {
        printf("[SINTACTICO ] 200. Error resolucion expresion: No se pueden realizar operaciones aritméticas con strings.\n");
        exit(1);
    }

    /*
        init {
                variableINT : int
                variableFLOAT : float
        }

        if ((variableINT + variableFLOAT) > 4)
        {
            write("Hola")
        }
     */
    if (strcmp(tipoDatoExpresion1, tipoDatoExpresion2) != 0)
    {
        printf("[SINTACTICO]  201. Error resolucion expresion: No se pueden realizar operacion arimenticas entre '%s' y '%s'.\n", tipoDatoExpresion1, tipoDatoExpresion2);
        exit(1);
    }

    apilarTipoDatoUtilizado(tipoDatoExpresion1);
}

// Para asignacion
void validarIDAsignacionEsTipoDatoExpresion(const char *id)
{
    char tipoDatoExpresion[TAM_CONTENIDO_PILA];
    strcpy(tipoDatoExpresion, desapilar(pilaTiposDatosUtilizados));

    t_simbolo *simbolo = buscarSimboloIDEnTablaSimbolo(id);

    if (strcmp(simbolo->tipoDato, TIPO_TOKEN_CONST_STR) == 0)
    {
        /*
            init {
                varString, variable: string
            }
            varString := "Hola como estas"
            variable := varString
        */
        printf("300. Error asignacion expresion: La variable '%s' es de tipo '%s', no se le puede asignar una expresion que es string.\n", id, simbolo->tipoDato);
        exit(1);
    }

    if (strcmp(simbolo->tipoDato, tipoDatoExpresion) != 0)
    {
        /*
            CASO PRUEBA:
            init {
                    variableINT : int
            }

            variableINT := (4.5 + 4.5)
        */
        printf("[SINTACTICO] 301. Error asignacion expresion: La variable '%s' es de tipo '%s', no se le puede asignar una expresion de tipo '%s'.\n", id, simbolo->tipoDato, tipoDatoExpresion);
        exit(1);
    }
}

void validarIDAsignacionEsTipoDatoString(const char *id)
{
    t_simbolo *simbolo = buscarSimboloIDEnTablaSimbolo(id);

    if (strcmp(simbolo->tipoDato, TIPO_TOKEN_CONST_STR) != 0)
    {
        /*
            CASO PRUEBA:
            init {
                    variableINT : int
            }

            variableINT := "cadena"
        */
        printf("[SINTACTICO] 301. Error asignacion string: La variable '%s' es de tipo '%s', no se le puede asignar un string'.\n", id, simbolo->tipoDato);
        exit(1);
    }
}

// Para operacion comparacion
void validarTipoDatoExpresionesAlComparar()
{
    char tipoDatoExpresion1[TAM_CONTENIDO_PILA];
    strcpy(tipoDatoExpresion1, desapilar(pilaTiposDatosUtilizados));
    char tipoDatoExpresion2[TAM_CONTENIDO_PILA];
    strcpy(tipoDatoExpresion2, desapilar(pilaTiposDatosUtilizados));

    if (strcmp(tipoDatoExpresion1, TIPO_TOKEN_CONST_STR) == 0 || strcmp(tipoDatoExpresion2, TIPO_TOKEN_CONST_STR) == 0)
    {
        /*
            CASO PRUEBA:
            init {
                    variableINT : int
                    variableSTRING : string
            }

            if ( variableINT > variableSTRING )
            {
                write("Hola")
            }
        */
        printf("[SINTACTICO] 400. Error comparacion: No se pueden realizar comparaciones con strings.\n");
        exit(1);
    }

    if (strcmp(tipoDatoExpresion1, tipoDatoExpresion2) != 0)
    {
        /*
            CASO PRUEBA:
            init {
                    variableINT : int
                    variableFLOAT : float
            }

            if ( variableINT > variableFLOAT )
            {
                write("Hola")
            }
        */
        printf("[SINTACTICO] 401. Error comparacion: No se pueden realizar comparaciones entre: '%s' y '%s'.\n", tipoDatoExpresion1, tipoDatoExpresion2);
        exit(1);
    }
}
