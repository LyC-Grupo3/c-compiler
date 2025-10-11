#ifndef HELPER_TIPOS_DATOS_H
#define HELPER_TIPOS_DATOS_H

#include "./pila.h"
#include "./tabla_simbolos.h"
#include "./helper_init_variables.h"

void inicializarTiposDatosHelper();
void limpiarTiposDatosHelper();

void apilarTipoDatoUtilizado(const char *tipoDato);

void validarTipoDatoResolucionDeExpresion();

void validarIDAsignacionEsTipoDatoExpresion(const char *id);
void validarIDAsignacionEsTipoDatoString(const char *id);

void validarTipoDatoExpresionesAlComparar();

#endif // HELPER_TIPOS_DATOS_H