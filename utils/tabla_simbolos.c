#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./tabla_simbolos.h"
#include "./informes.h"


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
    
    if (existeSimbolo(tabla, simbolo->nombre)) {
        informarDuplicadoSimbolo(simbolo->nombre);
        return 0;
    }
    
    tabla->elementos[tabla->cantidad] = *simbolo;

    tabla->cantidad++;
    
    return 1;
}

int existeSimbolo(const t_tabla_simbolos* tabla, const char* nombre) {
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

int tablaSimbolosLlena(const t_tabla_simbolos* tabla) {
    if (tabla == NULL) {
        return 1;
    }
    return tabla->cantidad == MAX_LISTA;
}



// HANDLERS SEGUN TIPO LEXEMA
int procesarLexemaTablaID(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaID\n");
        return 0;
    }

    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;

    snprintf(nombre_simbolo, sizeof(nombre_simbolo), "%s", lexema);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", "");
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", "");
    longitud_simbolo = 0;

    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);

    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        informarExitoInsertarSimbolo(nombre_simbolo, lexema, "ID");
    }
    
    return resultado;
}

int procesarLexemaTablaConstanteInt(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaConstanteInt\n");
        return 0;
    }
    
    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;
    
    snprintf(nombre_simbolo, sizeof(nombre_simbolo), "_%s", lexema);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", TIPO_INT);
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", lexema);
    longitud_simbolo = strlen(lexema);
    
    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);
    
    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        informarExitoInsertarSimbolo(nombre_simbolo, lexema, TIPO_INT);
    }
    
    return resultado;
}

int procesarLexemaTablaConstanteFloat(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaConstanteFloat\n");
        return 0;
    }
    
    // NOMBRE: Generar un nombre que no sea tal cual el valor ya que puedo tener "." o "-"
    char valor_normalizado[MAX_LONG_NOMBRE_SIMBOLO];
    strcpy(valor_normalizado, lexema);
    
    // Reemplazar caracteres especiales para hacer un nombre válido
    for (int i = 0; valor_normalizado[i]; i++) {
        if (valor_normalizado[i] == '.') {
            valor_normalizado[i] = '_';
        }
        // TODO: revisar si nos va afectar ponerle N
        else if (valor_normalizado[i] == '-') {
            valor_normalizado[i] = 'N';
        }
    }
    
    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;

    snprintf(nombre_simbolo, sizeof(nombre_simbolo) + 1, "_%s", valor_normalizado);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", TIPO_FLOAT);
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", lexema);
    longitud_simbolo = strlen(lexema);

    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);
    
    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        informarExitoInsertarSimbolo(nombre_simbolo, lexema, TIPO_FLOAT);
    }
    
    return resultado;
}

int procesarLexemaTablaConstanteString(t_tabla_simbolos* tabla, const char* lexema) {
    // 1. Validar parametros
    if (tabla == NULL || lexema == NULL) {
        printf("Error: Parámetros NULL en procesarLexemaTablaConstanteString\n");
        return 0;
    }

    // NOMBRE: Generar uno único usando un contador ya que sino  me queda el valor y nombre de simbolo iguales
    static int contador_string = 0;
    char nombre_simbolo_incremental[MAX_LONG_NOMBRE_SIMBOLO];
    snprintf(nombre_simbolo_incremental, sizeof(nombre_simbolo_incremental), "_CTE_STRING_%d", ++contador_string);

    // VALOR: Quedarme solo con el contenido del string
    char valor_sin_comillas[MAX_LONG_VALOR_SIMBOLO];
    int len = strlen(lexema);
    if (len >= 2 && lexema[0] == '"' && lexema[len-1] == '"') {
        strncpy(valor_sin_comillas, lexema + 1, len - 2);
        valor_sin_comillas[len - 2] = '\0';
    } else {
        // Para mayra: sirve para la cadena vacia
        strcpy(valor_sin_comillas, lexema);
    }
    
    // 2. Formatear el simbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;
    
    snprintf(nombre_simbolo, sizeof(nombre_simbolo), "%s", nombre_simbolo_incremental);
    snprintf(tipo_simbolo, sizeof(tipo_simbolo), "%s", TIPO_STRING);
    snprintf(valor_simbolo, sizeof(valor_simbolo), "%s", valor_sin_comillas);
    longitud_simbolo = strlen(valor_sin_comillas);

    // 3. Crear simbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);

    // 4. Insertar Simbolo
    int resultado = insertarSimbolo(tabla, &simbolo);
    if (resultado) {
        informarExitoInsertarSimbolo(nombre_simbolo, lexema, TIPO_STRING);
    }
    
    return resultado;
}