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
    
    if (strcmp(tipo_token, TIPO_TOKEN_ID) == 0) {
        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "%s", lexema);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", "");
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", "");
        *longitud_simbolo = 0;
        *tipo_para_informe = TIPO_TOKEN_ID;
        
    } else if (strcmp(tipo_token, TIPO_TOKEN_CONST_INT) == 0) {
        /* --------------------------- NOMBRE NORMALIZADO --------------------------- */
        char nombre_normalizado[MAX_LONG_NOMBRE_SIMBOLO - 1];

        strcpy(nombre_normalizado, lexema);
        for (int i = 0; nombre_normalizado[i]; i++) {
            if (nombre_normalizado[i] == '-') {
                nombre_normalizado[i] = 'n';
            }
        }

        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%s", nombre_normalizado);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_TOKEN_CONST_INT);
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", lexema);
        *longitud_simbolo = strlen(lexema);
        *tipo_para_informe = TIPO_TOKEN_CONST_INT;
        
    } else if (strcmp(tipo_token, TIPO_TOKEN_CONST_FLOAT) == 0) {
        /* ---------------------------- VALOR NORMALIZADO --------------------------- */
        char valor_normalizado[MAX_LONG_VALOR_SIMBOLO];

        // Copiar el lexema original
        strcpy(valor_normalizado, lexema);
        int len = strlen(valor_normalizado);
        int punto_pos = -1;
        int inicio = 0;
        
        // Manejar signo negativo
        if (valor_normalizado[0] == '-') {
            inicio = 1;
        }
        
        // Caso especial: número que comienza con punto (.555)
        if (valor_normalizado[inicio] == '.') {
            // No eliminar nada, el número comienza con punto
        } else {
            // Eliminar ceros no significativos del principio
            while (inicio < len && valor_normalizado[inicio] == '0') {
                inicio++;
            }
            
            // Si eliminamos todos los dígitos antes del punto, mantener al menos un 0
            if (inicio < len && valor_normalizado[inicio] == '.') {
                inicio--;
            }
        }
        
        // Encontrar la posición del punto decimal
        for (int i = inicio; i < len; i++) {
            if (valor_normalizado[i] == '.') {
                punto_pos = i;
                break;
            }
        }
        
        // Construir la cadena normalizada
        char temp[MAX_LONG_VALOR_SIMBOLO];
        int temp_idx = 0;
        
        // Copiar el signo si es negativo
        if (valor_normalizado[0] == '-') {
            temp[temp_idx++] = '-';
        }
        
        // Caso especial: si comienza con punto, agregar 0
        if ((valor_normalizado[0] == '.' || (valor_normalizado[0] == '-' && valor_normalizado[1] == '.'))) {
            temp[temp_idx++] = '0';
        }
        
        // Copiar desde el primer dígito significativo
        for (int i = inicio; i < len; i++) {
            temp[temp_idx++] = valor_normalizado[i];
        }
        temp[temp_idx] = '\0';
        
        // Caso especial: si termina con punto, agregar 0
        int temp_len = strlen(temp);
        if (temp_len > 0 && temp[temp_len - 1] == '.') {
            temp[temp_idx++] = '0';
            temp[temp_idx] = '\0';
            temp_len++;
        }
        
        // Eliminar ceros del final si hay punto decimal
        if (punto_pos != -1) {
            // Eliminar ceros del final, pero mantener al menos un decimal
            while (temp_len > 2 && temp[temp_len - 1] == '0' && temp[temp_len - 2] != '.') {
                temp_len--;
            }
            temp[temp_len] = '\0';
        }
        
        strcpy(valor_normalizado, temp);

        /* --------------------------- NOMBRE NORMALIZADO --------------------------- */
        char nombre_normalizado[MAX_LONG_NOMBRE_SIMBOLO];

        strcpy(nombre_normalizado, valor_normalizado);
        for (int i = 0; nombre_normalizado[i]; i++) {
            if (nombre_normalizado[i] == '.') {
                nombre_normalizado[i] = '_';
            }
            else if (nombre_normalizado[i] == '-') {
                nombre_normalizado[i] = 'n';
            }
        }

        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%.*s", MAX_LONG_NOMBRE_SIMBOLO - 2, nombre_normalizado);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_TOKEN_CONST_FLOAT);
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", valor_normalizado);
        *longitud_simbolo = strlen(valor_normalizado);
        *tipo_para_informe = TIPO_TOKEN_CONST_FLOAT;
        
    } else if (strcmp(tipo_token, TIPO_TOKEN_CONST_STR) == 0) {
        /* --------------------- NOMBRE SIMBOLO AUTOINCREMENTAL --------------------- */
        static int contadorStr = 0;
        contadorStr = (contadorStr + 1);

        /* --------------------------- VALOR SIN COMILLAS --------------------------- */
        int len = strlen(lexema);
        if (len >= 2 && lexema[0] == '"' && lexema[len-1] == '"') {
            strncpy(valor_simbolo, lexema + 1, len - 2);
            valor_simbolo[len - 2] = '\0';
        } else {
            strcpy(valor_simbolo, lexema);
            
        }
        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%s_%d", TIPO_TOKEN_CONST_STR, contadorStr);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_TOKEN_CONST_STR);
        *longitud_simbolo = strlen(valor_simbolo);
        *tipo_para_informe = TIPO_TOKEN_CONST_STR;
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
    if (strcmp(tipo_token, TIPO_TOKEN_ID) == 0) {
        // Para IDs, verificar solo por nombre
        if (existeSimboloPorNombre(&tabla_simbolos, nombre_simbolo) == 1) {
            informarDuplicadoSimbolo(nombre_simbolo);
            return;
        }
    } else {
        // Para constantes, verificar solo por valor
        if (existeSimboloPorTipoValor(&tabla_simbolos, tipo_token, valor_simbolo) == 1) {
            informarDuplicadoSimbolo(nombre_simbolo);
            return;
        }
    }

    // 3. Verificar si la tabla está llena
    if (tablaSimbolosLlena(&tabla_simbolos)) {
        printf("Error: Tabla de símbolos llena, no se puede insertar el símbolo %s '%s'\n", tipo_token, lexema);
        return;
    }

    // 4. Crear e insertar símbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);
    int resultado = insertarSimbolo(&tabla_simbolos, &simbolo);
    
    if (resultado == 1) {
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