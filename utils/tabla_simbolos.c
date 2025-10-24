#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./tabla_simbolos.h"

t_tabla_simbolos tabla_simbolos;

void inicializarTablaSimbolos(t_tabla_simbolos *tabla)
{
    if (tabla == NULL)
    {
        return;
    }
    tabla->cantidad = 0;
}

int insertarSimbolo(t_tabla_simbolos *tabla, const t_simbolo *simbolo)
{
    if (tabla == NULL || simbolo == NULL)
    {
        printf("Error: Lista o símbolo NULL en insertarSimbolo\n");
        return 0;
    }

    if (tablaSimbolosLlena(tabla))
    {
        printf("Error: Lista llena, no se puede insertar\n");
        return 0;
    }

    tabla->elementos[tabla->cantidad] = *simbolo;
    tabla->cantidad++;

    return 1;
}

int existeSimboloPorNombre(const t_tabla_simbolos *tabla, const char *nombre)
{
    if (tabla == NULL || nombre == NULL)
    {
        return 0;
    }

    for (int i = 0; i < tabla->cantidad; i++)
    {
        // TODO: preguntar el criterio para saber si un simbolo ya existe o no
        if (strcmp(tabla->elementos[i].nombre, nombre) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int existeSimboloPorTipoValor(const t_tabla_simbolos *tabla, const char *tipo_dato, const char *valor)
{
    if (tabla == NULL || valor == NULL)
    {
        return 0;
    }

    for (int i = 0; i < tabla->cantidad; i++)
    {
        if (strcmp(tabla->elementos[i].tipoDato, tipo_dato) == 0 && strcmp(tabla->elementos[i].valor, valor) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int tablaSimbolosLlena(const t_tabla_simbolos *tabla)
{
    if (tabla == NULL)
    {
        return 1;
    }
    return tabla->cantidad == MAX_LISTA;
}

void insertarVariableASM(const char *nombre, const char *tipoDato)
{
    if (existeSimboloPorNombre(&tabla_simbolos, nombre) == 1)
    {
        informarDuplicadoSimbolo(nombre);
        return;
    }

    t_simbolo simbolo = crearSimbolo(nombre, tipoDato, "", "");
    insertarSimbolo(&tabla_simbolos, &simbolo);
}

void procesarSimbolo(const char *lexema, const char *tipo_token)
{
    // 1. Formatear el símbolo
    char nombre_simbolo[MAX_LONG_NOMBRE_SIMBOLO];
    char tipo_simbolo[MAX_LONG_TIPO_SIMBOLO];
    char valor_simbolo[MAX_LONG_VALOR_SIMBOLO];
    char longitud_simbolo[MAX_LONG_LONGITUD_SIMBOLO];
    const char *tipo_para_informe = "";

    formatearSimbolo(lexema, tipo_token, nombre_simbolo, tipo_simbolo,
                     valor_simbolo, longitud_simbolo, &tipo_para_informe);

    // 2. Verificar si ya existe
    if (strcmp(tipo_token, TIPO_TOKEN_ID) == 0)
    {
        // Para IDs, verificar solo por nombre
        if (existeSimboloPorNombre(&tabla_simbolos, nombre_simbolo) == 1)
        {
            informarDuplicadoSimbolo(nombre_simbolo);
            return;
        }
    }
    else
    {
        // Para constantes, verificar solo por valor
        if (existeSimboloPorTipoValor(&tabla_simbolos, tipo_token, valor_simbolo) == 1)
        {
            informarDuplicadoSimbolo(nombre_simbolo);
            return;
        }
    }

    // 3. Verificar si la tabla está llena
    if (tablaSimbolosLlena(&tabla_simbolos))
    {
        printf("Error: Tabla de símbolos llena, no se puede insertar el símbolo %s '%s'\n", tipo_token, lexema);
        return;
    }

    // 4. Crear e insertar símbolo
    t_simbolo simbolo = crearSimbolo(nombre_simbolo, tipo_simbolo, valor_simbolo, longitud_simbolo);
    int resultado = insertarSimbolo(&tabla_simbolos, &simbolo);

    if (resultado == 1)
    {
        informarExitoInsertarSimbolo(nombre_simbolo, lexema, tipo_para_informe);
    }
    else
    {
        printf("Error: No se pudo insertar el símbolo %s '%s'\n", tipo_token, lexema);
    }
}

void crearTablaSimbolos(void)
{
    inicializarTablaSimbolos(&tabla_simbolos);
}

int exportarTablaSimbolos(const char *nombre_archivo)
{
    if (nombre_archivo == NULL)
    {
        printf("Error: Nombre de archivo NULL\n");
        return 0;
    }

    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
        return 0;
    }

    fprintf(archivo, "%s|%s|%s|%s\n", "NOMBRE", "TIPO", "VALOR", "LONGITUD");

    // Iterar sobre todos los símbolos
    for (int i = 0; i < tabla_simbolos.cantidad; i++)
    {
        t_simbolo *simbolo = &(tabla_simbolos.elementos[i]);
        fprintf(archivo, "%s|%s|%s|%s\n",
                simbolo->nombre,
                simbolo->tipoDato,
                simbolo->valor,
                simbolo->longitud);
    }

    fclose(archivo);

    return 1;
}

// Auxiliares para init_variables
t_simbolo *buscarSimboloPorNombre(const char *nombre)
{
    for (int i = 0; i < tabla_simbolos.cantidad; i++)
    {
        if (strcmp(tabla_simbolos.elementos[i].nombre, nombre) == 0)
        {
            return &tabla_simbolos.elementos[i];
        }
    }
    return NULL;
}

t_simbolo *buscarSimboloPorValor(const char *valor)
{
    for (int i = 0; i < tabla_simbolos.cantidad; i++)
    {
        if (strcmp(tabla_simbolos.elementos[i].valor, valor) == 0)
        {
            return &tabla_simbolos.elementos[i];
        }
    }
    return NULL;
}

t_tabla_simbolos *duplicarTablaSimbolos()
{
    t_tabla_simbolos *copia = (t_tabla_simbolos *)malloc(sizeof(t_tabla_simbolos));
    if (copia == NULL)
    {
        printf("Error: No se pudo asignar memoria para duplicar la tabla de símbolos\n");
        return NULL;
    }

    copia->cantidad = tabla_simbolos.cantidad;
    for (int i = 0; i < tabla_simbolos.cantidad; i++)
    {
        copia->elementos[i] = tabla_simbolos.elementos[i];
    }

    return copia;
}

void eliminarTablaSimbolosDuplicada(t_tabla_simbolos *tabla)
{
    if (tabla == NULL)
    {
        return;
    }

    free(tabla);
}