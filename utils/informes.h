#ifndef INFORMES_H
#define INFORMES_H

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

/* --------------------------------- LEXICO --------------------------------- */
void informarMatchLexico(char *lexema, char *tipo_token);
void informarErrorNoMatchLexico(char *lexema, int linea);
void informarErrorValidacion(char *token, char *tipo_token, char *mensaje, int linea);
void informarExitoInsertarSimbolo(const char *nombre, const char *lexema, const char *tipo_agregado);
void informarDuplicadoSimbolo(const char *nombre);

/* ------------------------------- SINTACTICO ------------------------------- */
void informarMatchLexicoSintactico(const char *mensaje1, const char *mensaje2);
void informarErrorSintactico(const char *message, const char *token_problema, int linea);

#endif // INFORMES_H
