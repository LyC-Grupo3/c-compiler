#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

/* --------------------------------- LEXICO --------------------------------- */
void abrirArchivoSalidaLexico(const char *nombre_archivo);
void cerrarArchivoSalidaLexico(void);

/* ------------------------------- SINTACTICO ------------------------------- */
void abrirArchivoSalidaSintactico(const char *nombre_archivo);
void cerrarArchivoSalidaLexicoSintactico(void);

#endif // ARCHIVOS_H