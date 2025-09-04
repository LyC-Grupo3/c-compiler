#include "archivos.h"

/* --------------------------------- LEXICO --------------------------------- */
FILE *archivo_salida_lexico;

void abrirArchivoSalidaLexico(const char *nombre_archivo)
{
    archivo_salida_lexico = fopen(nombre_archivo, "w");
    if (archivo_salida_lexico == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
    }
    else
    {
        // Escribir cabecera del archivo
        fprintf(archivo_salida_lexico, "=== ANÁLISIS LÉXICO ===\n");
        fflush(archivo_salida_lexico);
    }
}

void cerrarArchivoSalidaLexico(void)
{
    if (archivo_salida_lexico != NULL)
    {
        fclose(archivo_salida_lexico);
        archivo_salida_lexico = NULL;
    }
}

/* ------------------------------- SINTACTICO ------------------------------- */
FILE *archivo_salida_sintactico;

void abrirArchivoSalidaSintactico(const char *nombre_archivo)
{
    archivo_salida_sintactico = fopen(nombre_archivo, "w");
    if (archivo_salida_sintactico == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
    }
    else
    {
        fprintf(archivo_salida_sintactico, "=== ANÁLISIS SINTÁCTICO ===\n");
        fflush(archivo_salida_sintactico);
    }
}

void cerrarArchivoSalidaLexicoSintactico(void)
{
    if (archivo_salida_sintactico != NULL)
    {
        fclose(archivo_salida_sintactico);
        archivo_salida_sintactico = NULL;
    }
}