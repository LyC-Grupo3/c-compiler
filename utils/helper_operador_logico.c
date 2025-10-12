#include "./helper_operador_logico.h"

t_pila *pilaOperadoresLogicosUtilizados;

/* -------------------------------------------------------------------------- */
/*                          FUNCIONES DE INICIALIZACIÓN                       */
/* -------------------------------------------------------------------------- */
void inicializarCondicionalHelper()
{
    pilaOperadoresLogicosUtilizados = crearPila();
}

void limpiarCondicionalHelper()
{
    eliminarPila(pilaOperadoresLogicosUtilizados);
}

/* -------------------------------------------------------------------------- */
/*                          FUNCIONES CONDICIONALES                           */
/* -------------------------------------------------------------------------- */
// Para regla condicional y operador_logico
void apilarOperadorLogicoUtilizado(const char *operadorLogico)
{
    apilar(pilaOperadoresLogicosUtilizados, operadorLogico);
}

char* getOperadorLogicoActual()
{
    return verTope(pilaOperadoresLogicosUtilizados);
}


int esCondicionalConDosExpresiones()
{
    // 1 - si esta vacia
    // 0 - si tiene algo

    // Si esta vacia es condicion simple
    if(pilaVacia(pilaOperadoresLogicosUtilizados))
    {
        return 0;
    }
    // Si no esta vacia es condicion con dos expresiones
    else
    {
        return 1;
    }
}

void reiniciarPilaOperadoresLogicosUtilizados()
{
    if(pilaVacia(pilaOperadoresLogicosUtilizados) == 0)
    {
        desapilar(pilaOperadoresLogicosUtilizados);
    }
}