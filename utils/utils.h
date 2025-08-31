#ifndef UTILS_H
#define UTILS_H

#include "tabla_simbolos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Programa
extern t_tabla_simbolos tabla_simbolos;
void crearTablaSimbolos(void);
int exportarTablaSimbolos(const char* nombre_archivo);

void procesarMatchID(const char* lexema);
void procesarMatchConstante(const char* lexema, const char* tipo_token);



#endif // UTILS_H
