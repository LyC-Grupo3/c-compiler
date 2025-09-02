#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./tabla_simbolos.h"


void inicializarTablaSimbolos(t_tabla_simbolos* tabla) {
    if (tabla == NULL) {
        return;
    }
    tabla->cantidad = 0;
}

int insertarSimbolo(t_tabla_simbolos* tabla, const t_simbolo* simbolo) {
    if (tabla == NULL || simbolo == NULL) {
        printf("Error: Lista o símbolo NULL en insertarSimbolo\n");
        return 0;
    }
    
    if (tablaSimbolosLlena(tabla)) {
        printf("Error: Lista llena, no se puede insertar\n");
        return 0;
    }
    
    tabla->elementos[tabla->cantidad] = *simbolo;
    tabla->cantidad++;
    
    return 1;
}

int existeSimboloPorNombre(const t_tabla_simbolos* tabla, const char* nombre) {
    if (tabla == NULL || nombre == NULL) {
        return 0;
    }
    
    for (int i = 0; i < tabla->cantidad; i++) {
        // TODO: preguntar el criterio para saber si un simbolo ya existe o no 
        if (strcmp(tabla->elementos[i].nombre, nombre) == 0) {
            return 1;
        }
    }
    
    return 0;
}

int existeSimboloPorTipoValor(const t_tabla_simbolos* tabla, const char* tipo_dato, const char* valor) {
    if (tabla == NULL || valor == NULL) {
        return 0;
    }

    for (int i = 0; i < tabla->cantidad; i++) {
        if (strcmp(tabla->elementos[i].tipoDato, tipo_dato) == 0 && strcmp(tabla->elementos[i].valor, valor) == 0) {
            return 1;
        }
    }

    return 0;
}

int tablaSimbolosLlena(const t_tabla_simbolos* tabla) {
    if (tabla == NULL) {
        return 1;
    }
    return tabla->cantidad == MAX_LISTA;
}
