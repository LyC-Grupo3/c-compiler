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



/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */
void procesarMatchID(const char* lexema) {
    procesarLexemaTablaID(&tabla_simbolos, lexema);
}

void procesarMatchConstante(const char* lexema, const char* tipo_token) {
    if (strcmp(tipo_token, "CONST_INT") == 0) {
        procesarLexemaTablaConstanteInt(&tabla_simbolos, lexema);
    } else if (strcmp(tipo_token, "CONST_FLOAT") == 0) {
        procesarLexemaTablaConstanteFloat(&tabla_simbolos, lexema);
    } else if (strcmp(tipo_token, "CONST_STR") == 0) {
        procesarLexemaTablaConstanteString(&tabla_simbolos, lexema);
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