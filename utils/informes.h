#ifndef INFORMES_H
#define INFORMES_H



/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

void informarMatchLexico(char* lexema, char* nombreDelToken);
void informarErrorNoMatch(char* lexema);
void informarErrorValidacion(char* name_token);
void informarExitoInsertarSimbolo(const char* nombre, const char* lexema, const char* tipo_agregado);
void informarDuplicadoSimbolo(const char* nombre);



#endif // INFORMES_H
