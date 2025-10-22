#include <stdio.h>
#include <string.h>

#include "./utils.h"

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

// Función interna para formatear símbolos según su tipo
void formatearSimbolo(const char *lexema, const char *tipo_token,
                      char *nombre_simbolo, char *tipo_simbolo,
                      char *valor_simbolo, char *longitud_simbolo,
                      const char **tipo_para_informe)
{

    if (strcmp(tipo_token, TIPO_TOKEN_ID) == 0)
    {
        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "%s", lexema);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", "");
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", "");
        snprintf(longitud_simbolo, MAX_LONG_LONGITUD_SIMBOLO, "%s", "");
        *tipo_para_informe = TIPO_TOKEN_ID;
    }
    else if (strcmp(tipo_token, TIPO_TOKEN_CONST_INT) == 0)
    {
        /* --------------------------- NOMBRE NORMALIZADO --------------------------- */
        char nombre_normalizado[MAX_LONG_NOMBRE_SIMBOLO - 1];

        strcpy(nombre_normalizado, lexema);
        for (int i = 0; nombre_normalizado[i]; i++)
        {
            if (nombre_normalizado[i] == '-')
            {
                nombre_normalizado[i] = 'n';
            }
        }

        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%s", nombre_normalizado);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_TOKEN_CONST_INT);
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", lexema);
        snprintf(longitud_simbolo, MAX_LONG_LONGITUD_SIMBOLO, "%s", "");
        *tipo_para_informe = TIPO_TOKEN_CONST_INT;
    }
    else if (strcmp(tipo_token, TIPO_TOKEN_CONST_FLOAT) == 0)
    {
        /* ---------------------------- VALOR NORMALIZADO --------------------------- */
        char valor_normalizado[MAX_LONG_VALOR_SIMBOLO];
        normalizarValorFloat(lexema, valor_normalizado, MAX_LONG_VALOR_SIMBOLO);

        /* --------------------------- NOMBRE NORMALIZADO --------------------------- */
        char nombre_normalizado[MAX_LONG_NOMBRE_SIMBOLO];

        strcpy(nombre_normalizado, valor_normalizado);
        for (int i = 0; nombre_normalizado[i]; i++)
        {
            if (nombre_normalizado[i] == '.')
            {
                nombre_normalizado[i] = '_';
            }
            else if (nombre_normalizado[i] == '-')
            {
                nombre_normalizado[i] = 'n';
            }
        }

        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%.*s", MAX_LONG_NOMBRE_SIMBOLO - 2, nombre_normalizado);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_TOKEN_CONST_FLOAT);
        snprintf(valor_simbolo, MAX_LONG_VALOR_SIMBOLO, "%s", valor_normalizado);
        snprintf(longitud_simbolo, MAX_LONG_LONGITUD_SIMBOLO, "%s", "");
        *tipo_para_informe = TIPO_TOKEN_CONST_FLOAT;
    }
    else if (strcmp(tipo_token, TIPO_TOKEN_CONST_STR) == 0)
    {
        /* --------------------------- VALOR SIN COMILLAS --------------------------- */
        int len = strlen(lexema);
        if (len >= 2 && lexema[0] == '"' && lexema[len - 1] == '"')
        {
            strncpy(valor_simbolo, lexema + 1, len - 2);
            valor_simbolo[len - 2] = '\0';
        }
        else
        {
            strcpy(valor_simbolo, lexema);
        }

        /* --------------------- NOMBRE SIMBOLO AUTOINCREMENTAL --------------------- */
        char nombre_formateado[MAX_LONG_NOMBRE_SIMBOLO];
        static int contadorStr = 0;

        snprintf(nombre_formateado, MAX_LONG_NOMBRE_SIMBOLO, "cte_%s_%d", "str", contadorStr);
        contadorStr = (contadorStr + 1);

        /* -------------------- NOMBRE ESPACIOS POR GUIONES BAJOS ------------------- */
        // int idx = 0;
        // for (int i = 0; valor_simbolo[i] && idx < MAX_LONG_NOMBRE_SIMBOLO - 2; i++)
        // {
        //     if (valor_simbolo[i] == ' ')
        //     {
        //         nombre_sin_espacios[idx++] = '_';
        //     }
        //     else
        //     {
        //         nombre_sin_espacios[idx++] = valor_simbolo[i];
        //     }
        // }
        // nombre_formateado[idx] = '\0';

        snprintf(nombre_simbolo, MAX_LONG_NOMBRE_SIMBOLO, "_%.*s", MAX_LONG_NOMBRE_SIMBOLO - 2, nombre_formateado);
        snprintf(tipo_simbolo, MAX_LONG_TIPO_SIMBOLO, "%s", TIPO_TOKEN_CONST_STR);
        snprintf(longitud_simbolo, MAX_LONG_LONGITUD_SIMBOLO, "%d", (int)strlen(valor_simbolo));
        *tipo_para_informe = TIPO_TOKEN_CONST_STR;
    }
}

void normalizarValorFloat(const char *lexema, char *valor_normalizado, size_t max_len)
{
    // Copiar el lexema original
    strncpy(valor_normalizado, lexema, max_len - 1);
    valor_normalizado[max_len - 1] = '\0';

    int len = strlen(valor_normalizado);
    int punto_pos = -1;
    int inicio = 0;

    // Manejar signo negativo
    if (valor_normalizado[0] == '-')
    {
        inicio = 1;
    }

    // Caso especial: número que comienza con punto (.555)
    if (valor_normalizado[inicio] == '.')
    {
        // No eliminar nada, el número comienza con punto
    }
    else
    {
        // Eliminar ceros no significativos del principio
        while (inicio < len && valor_normalizado[inicio] == '0')
        {
            inicio++;
        }

        // Si eliminamos todos los dígitos antes del punto, mantener al menos un 0
        if (inicio < len && valor_normalizado[inicio] == '.')
        {
            inicio--;
        }
    }

    // Encontrar la posición del punto decimal
    for (int i = inicio; i < len; i++)
    {
        if (valor_normalizado[i] == '.')
        {
            punto_pos = i;
            break;
        }
    }

    // Construir la cadena normalizada
    char temp[MAX_LONG_VALOR_SIMBOLO];
    int temp_idx = 0;

    // Copiar el signo si es negativo
    if (valor_normalizado[0] == '-')
    {
        temp[temp_idx++] = '-';
    }

    // Caso especial: si comienza con punto, agregar 0
    if ((valor_normalizado[0] == '.' || (valor_normalizado[0] == '-' && valor_normalizado[1] == '.')))
    {
        temp[temp_idx++] = '0';
    }

    // Copiar desde el primer dígito significativo
    for (int i = inicio; i < len; i++)
    {
        temp[temp_idx++] = valor_normalizado[i];
    }
    temp[temp_idx] = '\0';

    // Caso especial: si termina con punto, agregar 0
    int temp_len = strlen(temp);
    if (temp_len > 0 && temp[temp_len - 1] == '.')
    {
        temp[temp_idx++] = '0';
        temp[temp_idx] = '\0';
        temp_len++;
    }

    // Eliminar ceros del final si hay punto decimal
    if (punto_pos != -1)
    {
        // Eliminar ceros del final, pero mantener al menos un decimal
        while (temp_len > 2 && temp[temp_len - 1] == '0' && temp[temp_len - 2] != '.')
        {
            temp_len--;
        }
        temp[temp_len] = '\0';
    }

    strcpy(valor_normalizado, temp);
}