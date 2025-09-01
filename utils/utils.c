#include <stdio.h>
#include <string.h>

#include "./utils.h"
#include "./simbolo.h"
#include "./tabla_simbolos.h"


/* -------------------------------------------------------------------------- */
/*                             VARIABLES EXTERNAS                             */
/* -------------------------------------------------------------------------- */
extern t_tabla_simbolos tabla_simbolos;
extern FILE* archivo_salida_lexico;

// Declaraciones externas de funciones de informe
extern void informarExitoInsertarSimbolo(const char* nombre, const char* lexema, const char* tipo_agregado);
extern void informarDuplicadoSimbolo(const char* nombre);



/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */
// Función interna para formatear símbolos según su tipo
static void formatearSimbolo(const char* lexema, const char* tipo_token, 
                            char* nombre_simbolo, char* tipo_simbolo, 
                            char* valor_simbolo, int* longitud_simbolo,
                            const char** tipo_para_informe) {
    
    if (strcmp(tipo_token, "ID") == 0) {
        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "%s", lexema);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", "");
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", "");
        *longitud_simbolo = 0;
        *tipo_para_informe = "ID";
        
    } else if (strcmp(tipo_token, "CONST_INT") == 0) {
        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%s", lexema);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_INT);
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", lexema);
        *longitud_simbolo = strlen(lexema);
        *tipo_para_informe = TIPO_INT;
        
    } else if (strcmp(tipo_token, "CONST_FLOAT") == 0) {
        // Aux nombre simbolo
        char valor_normalizado[MAX_LONG_NOMBRE_SIMBOLO - 1]; // -1 para dejar espacio para el prefijo "_"
        strncpy(valor_normalizado, lexema, sizeof(valor_normalizado) - 1);
        valor_normalizado[sizeof(valor_normalizado) - 1] = '\0';
        for (int i = 0; valor_normalizado[i]; i++) {
            if (valor_normalizado[i] == '.') {
                valor_normalizado[i] = '_';
            } else if (valor_normalizado[i] == '-') {
                valor_normalizado[i] = 'N';
            }
        }

        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%s", valor_normalizado);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_FLOAT);
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", lexema);
        *longitud_simbolo = strlen(lexema);
        *tipo_para_informe = TIPO_FLOAT;
        
    } else if (strcmp(tipo_token, "CONST_STR") == 0) {
        // Aux nombre simbolo
        static int contador_string = 0;
        
        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_CTE_STRING_%d", ++contador_string);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_STRING);
        int len = strlen(lexema);
        if (len >= 2 && lexema[0] == '"' && lexema[len-1] == '"') {
            strncpy(valor_simbolo, lexema + 1, len - 2);
            valor_simbolo[len - 2] = '\0';
        } else {
            strcpy(valor_simbolo, lexema);
        }
        *longitud_simbolo = strlen(valor_simbolo);
        *tipo_para_informe = TIPO_STRING;
    }
}

void procesarSimbolo(const char* lexema, const char* tipo_token) {
    // 1. Formatear el símbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    int longitud_simbolo;
    const char* tipo_para_informe = "";
    
    formatearSimbolo(lexema, tipo_token, nombre_simbolo, tipo_simbolo, 
                    valor_simbolo, &longitud_simbolo, &tipo_para_informe);
    
    // 2. Verificar si ya existe
    if (existeSimbolo(&tabla_simbolos, nombre_simbolo)) {
        informarDuplicadoSimbolo(nombre_simbolo);
        return;
    }

    // 3. Verificar si la tabla está llena
    if (tablaSimbolosLlena(&tabla_simbolos)) {
        printf("Error: Tabla de símbolos llena, no se puede insertar el símbolo %s '%s'\n", tipo_token, lexema);
        return;
    }

    // 4. Crear e insertar símbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);
    int resultado = insertarSimbolo(&tabla_simbolos, &simbolo);
    
    if (resultado) {
        informarExitoInsertarSimbolo(nombre_simbolo, lexema, tipo_para_informe);
    } else {
        printf("Error: No se pudo insertar el símbolo %s '%s'\n", tipo_token, lexema);
    }
}

void crearTablaSimbolos(void) {
    inicializarTablaSimbolos(&tabla_simbolos);
}

void abrirArchivoSalidaLexico(const char* nombre_archivo) {
    archivo_salida_lexico = fopen(nombre_archivo, "w");
    if (archivo_salida_lexico == NULL) {
        printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
    } else {
        // Escribir cabecera del archivo
        fprintf(archivo_salida_lexico, "=== ANÁLISIS LÉXICO ===\n");
        fflush(archivo_salida_lexico);
    }
}

void cerrarArchivoSalidaLexico(void) {
    if (archivo_salida_lexico != NULL) {
        fclose(archivo_salida_lexico);
        archivo_salida_lexico = NULL;
    }
}

int exportarTablaSimbolos(const char* nombre_archivo) {
    if (nombre_archivo == NULL) {
        printf("Error: Nombre de archivo NULL\n");
        return 0;
    }
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
        return 0;
    }
    
    fprintf(archivo, "%s|%s|%s|%s\n", "NOMBRE", "TIPO", "VALOR", "LONGITUD");
    
    // Iterar sobre todos los símbolos
    for (int i = 0; i < tabla_simbolos.cantidad; i++) {
        t_simbolo* simbolo = &(tabla_simbolos.elementos[i]);
        fprintf(archivo, "%s|%s|%s|%d\n", 
                simbolo->nombre, 
                simbolo->tipoDato, 
                simbolo->valor, 
                simbolo->longitud);
    }
    
    fclose(archivo);
    
    return 1;
}