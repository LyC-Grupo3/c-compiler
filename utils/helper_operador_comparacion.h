#ifndef HELPER_OPERADOR_COMPARACION
#define HELPER_OPERADOR_COMPARACION

#include <string.h>
#include "./pila.h"

void inicializarCondicionalHelper();
void limpiarCondicionalHelper();

void apilarOperadorLogicoUtilizado(const char *operadorLogico);

void setOperadorComparacionPendienteActual(char *operador);
char *getOperadorComparacionPendienteActual();

void setNegacionPendienteOperadorComparacion(int valor);
int hayQueNegarOperadorComparacionActual();
void negarOperadorOperacionActual();

#endif // HELPER_OPERADOR_COMPARACION
