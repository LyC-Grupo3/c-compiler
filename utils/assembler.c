#include "./assembler.h"

FILE *archivoPolacaModificada = NULL;

void generarAssembler()
{

    archivoPolacaModificada = fopen("test_outputs/debug_polaca_modificada.txt", "w");
    if (archivoPolacaModificada == NULL)
    {
        printf("[ASSEMBLER] Error al crear el archivo debug_polaca_modificada.txt\n");
        exit(1);
    }

    t_tabla_simbolos *tabla_simbolos_copia = duplicarTablaSimbolos();
    t_polaca *polaca_copia = duplicarPolaca();

    formatearPolacaParaAssembler(polaca_copia);

    FILE *archivoAssembler = fopen("final.asm", "w");

    if (archivoAssembler == NULL)
    {
        printf("[ASSEMBLER] Error al crear el archivo final.asm\n");
        exit(1);
    }

    escribirCabecera(archivoAssembler);
    escribirSegmentoData(archivoAssembler, tabla_simbolos_copia);
    escribirSegmentCode(archivoAssembler);
    escribirSegmentoEnd(archivoAssembler);

    printf("\n\n[ASSEMBLER] Codigo generado.\n");
}

/* -------------------------------------------------------------------------- */
/*                            CODIGO ASM - CABECERA                           */
/* -------------------------------------------------------------------------- */
void escribirCabecera(FILE *archivo)
{
    fprintf(archivo, ".MODEL LARGE  ; Modelo de Memoria\n");
    fprintf(archivo, ".386          ; Tipo de Procesador\n");
    fprintf(archivo, ".STACK 200h   ; Bytes en el Stack\n");
}

/* -------------------------------------------------------------------------- */
/*                              CODIGO ASM - DATA                             */
/* -------------------------------------------------------------------------- */
void escribirSegmentoData(FILE *archivo, t_tabla_simbolos *tabla)
{
    int esSimboloString = 0;
    int esSimboloInt = 0;
    int tieneValor = 0;
    int tieneLongitud = 0;
    char valorStringASM[256];

    fprintf(archivo, "\n\n.DATA\n");

    for (int i = 0; i < tabla->cantidad; i++)
    {
        t_simbolo *simbolo = &(tabla->elementos[i]);

        esSimboloString = (strcmp(simbolo->tipoDato, TIPO_TOKEN_CONST_STR) == 0);
        esSimboloInt = (strcmp(simbolo->tipoDato, TIPO_TOKEN_CONST_INT) == 0);
        tieneValor = strlen(simbolo->valor);
        tieneLongitud = atoi(simbolo->longitud);

        if (esSimboloString) // es un simbolo de tipo string
        {
            if (!tieneLongitud) // es una variable de tipo string
            {
                sprintf(valorStringASM, "MAXTEXTSIZE dup (?),'$'");
            }
            else // es una constante de tipo string
            {
                sprintf(valorStringASM, "\"%s\",'$', %s dup (?)", simbolo->valor, simbolo->longitud);
            }

            fprintf(archivo, "%s db %s\n", simbolo->nombre, valorStringASM);
        }
        else // es un simbolo de tipo int o float
        {
            if (esSimboloInt) // int
            {
                char valorConDecimal[MAX_LONG_VALOR_SIMBOLO];
                sprintf(valorConDecimal, "%s%s", tieneValor ? simbolo->valor : "?", tieneValor ? ".0" : "");
                fprintf(archivo, "%s dd %s\n", simbolo->nombre, valorConDecimal);
            }
            else // float
            {
                fprintf(archivo, "%s dd %s\n", simbolo->nombre, tieneValor ? simbolo->valor : "?");
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                              CODIGO ASM - CODE                             */
/* -------------------------------------------------------------------------- */
void escribirSegmentCode(FILE *archivo)
{
    fprintf(archivo, "\n\n.CODE\n");
    fprintf(archivo, "mov AX,@DATA  ; inicializa el segmento de datos\n");
    fprintf(archivo, "mov DS,AX\n");
    fprintf(archivo, "mov es,ax ;\n");
}

/* -------------------------------------------------------------------------- */
/*                              CODIGO ASM - END                              */
/* -------------------------------------------------------------------------- */
void escribirSegmentoEnd(FILE *archivo)
{
    fprintf(archivo, "\n\nmove ax,4c00h   ; Indica que debe finalizar la ejecución\n");
    fprintf(archivo, "int 21h\n");
    fprintf(archivo, "End\n");
}


/* -------------------------------------------------------------------------- */
/*                              FORMATEAR POLACA                              */
/* -------------------------------------------------------------------------- */
void formatearPolacaParaAssembler(t_polaca *polaca)
{
    int nroCeldaActual = 0;
    t_nodo_polaca *celdaActual = polaca->inicio;
    t_nodo_polaca *celdaAnterior = NULL;
    t_nodo_polaca *celdaConNroSalto = NULL;
    t_nodo_polaca *celdaALaQueSaltar = NULL;
    int nroSalto = 0;
    t_simbolo *simboloEncontrado = NULL;
    char buf[100];

    // Reemplazar el contenido de una celda por su nombre en la tabla de simbolos
    while (celdaActual != NULL)
    {
        if (esCeldaNumero(celdaActual)) // TIPO_TOKEN_CONST_FLOAT o TIPO_TOKEN_CONST_INT
        {
            // Solo reemplazar si la celda actual no esta indicando el nro de celda a saltar y entonces es realmente un numero
            if (celdaActual == NULL || !esCeldaSalto(celdaAnterior))
            {
                actualizarContenidoCeldaPorNombreSimbolo(celdaActual, celdaActual->contenido);
            }
        }
        else if (esCeldaString(celdaActual)) // TIPO_TOKEN_CONST_STR
        {
            actualizarContenidoCeldaPorNombreSimbolo(celdaActual, limpiarComillas(celdaActual->contenido));
        }

        // Avanzar al siguiente nodo
        nroCeldaActual++;
        celdaAnterior = celdaActual;
        celdaActual = celdaActual->siguiente;
    }

    celdaActual = polaca->inicio;
    nroCeldaActual = 0;

    while (celdaActual != NULL)
    {
        if (esCeldaSalto(celdaActual))
        {
            // USO DE LA ETIQUETA DE SALTO
            // estoy parado en una celda de salto y su siguiente celda tiene el nro de celda a saltar ej: [ BI ]  [  nroSalto  ]
            // modifico el contenido de la celda siguiente para que tenga el formato ET_nroSalto y pase a ser ej: [ BI ] [ ET_nroSalto ]
            // entocnes me va a quedar el nombre de la etiqueta real de salto asembler a la cual deberia saltar en asm
            celdaConNroSalto = celdaActual->siguiente;
            nroSalto = atoi(celdaConNroSalto->contenido);
            sprintf(celdaConNroSalto->contenido, "ET_%d", nroSalto);

            // CREACION DE LA ETIQUETA DE SALTO
            // ahora tengo que ir a la celda a la que se salta y modificar su contenido para que tenga el formato ET_nroSalto: [contenidoActual]
            // para que la etiqueta asembler usada para saltar en el paso anterior exista
            // pero tambien tengo que guardar el contenido original de esa celda para no pisarlo
            // entonces luego al generar el assembler genero la etiqueta para que el salto sea valido y el contenido original dependiendo que sea lo traduzco o no
            if (nroSalto < polaca->contador)
            {
                celdaALaQueSaltar = obtenerDePolaca(polaca, nroSalto);

                if (strstr(celdaALaQueSaltar->contenido, "@ET_") == NULL)
                {
                    sprintf(buf, "@ET_%d:%s", nroSalto, celdaALaQueSaltar->contenido);
                    strcpy(celdaALaQueSaltar->contenido, buf);
                }
            }
            // puede suceder que el salto sea literalmente al fin del programa, que en ese caso en la polaca no exista esa celda realmente
            else
            {
                sprintf(buf, "@ET_%d:_END", nroSalto);
                insertarEnPolacaPers(polaca, buf);
            }
        }

        // Avanzar al siguiente nodo
        nroCeldaActual++;
        celdaAnterior = celdaActual;
        celdaActual = celdaActual->siguiente;
    }

    imprimirEstadoPolacaHorizontal(polaca->inicio, archivoPolacaModificada);
}

/* ------------------------ FORMATEAR POLACA - UTILES ----------------------- */
void actualizarContenidoCeldaPorNombreSimbolo(t_nodo_polaca *celda, const char *valorSimboloBuscar)
{
    t_simbolo *simbolo = buscarSimboloPorValor(valorSimboloBuscar);
    if (simbolo != NULL)
    {
        strcpy(celda->contenido, simbolo->nombre);
    }
    else
    {
        printf("[ASSEMBLER] Error 1099: No se encontro el simbolo con nombre %s en la tabla de simbolos.\n", valorSimboloBuscar);
        exit(1);
    }
}

char *limpiarComillas(char *str)
{
    static char cadenaSinComillas[MAX_LONG_VALOR_SIMBOLO];
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        strncpy(cadenaSinComillas, str + 1, len - 2);
        cadenaSinComillas[len - 2] = '\0';
    }
    else
    {
        strcpy(cadenaSinComillas, str);
    }
    return cadenaSinComillas;
}

int esCeldaNumero(t_nodo_polaca *celda)
{
    int lenCelda = strlen(celda->contenido);
    if ((celda->contenido[0] >= '0' && celda->contenido[0] <= '9') || (celda->contenido[0] == '-' && lenCelda > 1))
    {
        return 1;
    }

    return 0;
}

int esCeldaString(t_nodo_polaca *celda)
{
    int lenCelda = strlen(celda->contenido);
    if (lenCelda >= 2 && celda->contenido[0] == '"' && celda->contenido[lenCelda - 1] == '"')
    {
        return 1;
    }

    return 0;
}

int esCeldaSalto(t_nodo_polaca *celda)
{
    // Caso 1 es una cadena
    if (esCeldaString(celda))
    {
        return 0;
    }

    // Caso 2 es una celda que ya modifique previamente y le genere una etiqueta de salto
    char *celdaContenidoOriginal = strstr(celda->contenido, ":");
    if (celdaContenidoOriginal)
    {
        celdaContenidoOriginal++;
    }
    else
    {
        celdaContenidoOriginal = celda->contenido;
    }

    if (!strcmp(celdaContenidoOriginal, "BLE") ||
        !strcmp(celdaContenidoOriginal, "BEQ") ||
        !strcmp(celdaContenidoOriginal, "BNE") ||
        !strcmp(celdaContenidoOriginal, "BGT") ||
        !strcmp(celdaContenidoOriginal, "BLT") ||
        !strcmp(celdaContenidoOriginal, "BGE") ||
        !strcmp(celdaContenidoOriginal, "BI"))
    {
        return 1;
    }

    return 0;
}
