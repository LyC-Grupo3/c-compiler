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