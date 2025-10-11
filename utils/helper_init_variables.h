#ifndef HELPER_INIT_VARIABLES
#define HELPER_INIT_VARIABLES

#include "./pila.h"
#include "./tabla_simbolos.h"

/* -------------------------------------------------------------------------- */
/*                         FUNCIONES INIT VARIABLES                           */
/* -------------------------------------------------------------------------- */


void inicializarInitVariablesHelper();
void limpiarInitVariablesHelper();

void apilarIDVariableDeclarada(const char *id);
void setTipoDatoEnTSParaVariablesDeclaradas();
void setTipoDatoDeclaracionVariableActual(const char *tipoDato);


#endif // HELPER_INIT_VARIABLES
