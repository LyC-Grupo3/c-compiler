#include "./assembler.h"

/* ---------------------------------- PILAS --------------------------------- */
t_pila *pila_operandos;
t_pila *pila_auxiliares_aritmetica;

void generarAssembler()
{
    /* ----------------------------- ACRHIVO POLACA ----------------------------- */
    FILE *archivoPolacaModificada = NULL;

    archivoPolacaModificada = fopen("test_outputs/debug_polaca_modificada.txt", "w");
    if (archivoPolacaModificada == NULL)
    {
        printf("[ASSEMBLER] Error al crear el archivo debug_polaca_modificada.txt\n");
        exit(1);
    }

    /* ---------------------------- ARCHIVO ASSEMBLER --------------------------- */
    FILE *archivoAssembler = fopen("final.asm", "w");

    if (archivoAssembler == NULL)
    {
        printf("[ASSEMBLER] Error al crear el archivo final.asm\n");
        exit(1);
    }

    /* ----------------------------- TABLA SIMBOLOS ----------------------------- */
    t_tabla_simbolos *tabla_simbolos_copia = duplicarTablaSimbolos();

    /* ------------------------------- POLACA ----------------------------------- */
    t_polaca *polaca_copia = duplicarPolaca();
    formatearPolacaParaAssembler(polaca_copia);

    /* ---------------------------------- PILAS --------------------------------- */
    pila_operandos = crearPila();
    pila_auxiliares_aritmetica = crearPila();

    /* ------------------------------- CODIGO ASM ------------------------------- */
    escribirCabecera(archivoAssembler);
    escribirSegmentoData(archivoAssembler, tabla_simbolos_copia);
    escribirSegmentCode(archivoAssembler, polaca_copia);
    archivoAssembler = escribirVariablesAuxiliaresASM(archivoAssembler);
    escribirSegmentoEnd(archivoAssembler);

    /* ---------------------------------- DEBUG --------------------------------- */
    imprimirEstadoPolacaHorizontal(polaca_copia->inicio, archivoPolacaModificada);

    printf("\n\n[ASSEMBLER] Codigo generado.\n");

    fclose(archivoAssembler);
    fclose(archivoPolacaModificada);

    /* ----------------------------- LIMPIEZA MEMORIA --------------------------- */
    eliminarPila(pila_operandos);
    eliminarPila(pila_auxiliares_aritmetica);
    eliminarPolacaDuplicada(polaca_copia);
    eliminarTablaSimbolosDuplicada(tabla_simbolos_copia);
}

/* -------------------------------------------------------------------------- */
/*                            CODIGO ASM - CABECERA                           */
/* -------------------------------------------------------------------------- */
void escribirCabecera(FILE *archivo)
{
    fprintf(archivo, "include macros2.asm\n");
    fprintf(archivo, "include number.asm\n\n");
    fprintf(archivo, ".MODEL LARGE  ; Modelo de Memoria\n");
    fprintf(archivo, ".386          ; Tipo de Procesador\n");
    fprintf(archivo, ".STACK 200h   ; Bytes en el Stack\n");
    fprintf(archivo, "\nMAXTEXTSIZE EQU 256\n");
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
    fprintf(archivo, "; variables tabla simbolos\n");

    // Definir las variables usadas en la tabla de símbolos
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

FILE *escribirVariablesAuxiliaresASM(FILE *archivo)
{
    if (pilaVacia(pila_auxiliares_aritmetica))
    {
        return archivo; // No hay variables auxiliares para escribir
    }

    /* -------------------------------- AUXILIAR -------------------------------- */
    // Limpiar duplicados de la pila de auxiliares
    t_pila *pilaAuxSinDuplicados = crearPila();

    t_nodo_pila *nodoActual = pila_auxiliares_aritmetica->tope;
    while (nodoActual != NULL)
    {
        // Verificar si el contenido ya está en la pila sin duplicados
        int encontrado = 0;
        t_nodo_pila *nodoBusqueda = pilaAuxSinDuplicados->tope;
        while (nodoBusqueda != NULL)
        {
            if (strcmp(nodoBusqueda->contenido, nodoActual->contenido) == 0)
            {
                encontrado = 1;
                break;
            }
            nodoBusqueda = nodoBusqueda->siguiente;
        }

        // Si no se encontró, apilarlo en la pila sin duplicados
        if (!encontrado)
        {
            apilar(pilaAuxSinDuplicados, nodoActual->contenido);
        }

        nodoActual = nodoActual->siguiente;
    }

    // Reemplazar la pila original con la pila sin duplicados
    eliminarPila(pila_auxiliares_aritmetica);
    pila_auxiliares_aritmetica = pilaAuxSinDuplicados;

    /* ----------------------------------- OK ----------------------------------- */
    // Cerrar el archivo original
    fclose(archivo);

    // Abrir el archivo para lectura
    FILE *archivoLectura = fopen("final.asm", "r");
    if (archivoLectura == NULL)
    {
        printf("[ASSEMBLER] Error al abrir final.asm para lectura\n");
        exit(1);
    }

    // Crear archivo temporal
    FILE *archivoTemp = fopen("final.asm.tmp", "w");
    if (archivoTemp == NULL)
    {
        printf("[ASSEMBLER] Error al crear archivo temporal\n");
        fclose(archivoLectura);
        exit(1);
    }

    char linea[512];
    int dentroDeSectionData = 0;
    int yaEscribiAuxiliares = 0;

    // Copiar todo el archivo, insertando las auxiliares después de las variables de .DATA
    while (fgets(linea, sizeof(linea), archivoLectura) != NULL)
    {
        fputs(linea, archivoTemp);

        // Detectar cuando entramos a la sección .DATA
        if (strstr(linea, ".DATA") != NULL)
        {
            dentroDeSectionData = 1;
        }
        // Detectar cuando salimos de .DATA (empieza .CODE)
        else if (strstr(linea, ".CODE") != NULL)
        {
            dentroDeSectionData = 0;
        }
        // Si estamos en .DATA y encontramos una línea vacía o el inicio de .CODE, escribir auxiliares
        else if (dentroDeSectionData && !yaEscribiAuxiliares)
        {
            // Verificar si es una línea vacía (solo espacios/newline) o ya llegamos al final de las variables
            int esLineaVacia = 1;
            for (int i = 0; linea[i] != '\0'; i++)
            {
                if (linea[i] != ' ' && linea[i] != '\t' && linea[i] != '\n' && linea[i] != '\r')
                {
                    esLineaVacia = 0;
                    break;
                }
            }

            if (esLineaVacia)
            {
                fprintf(archivoTemp, "; variables auxiliares para resultados intermedios de operaciones aritméticas\n");
                t_nodo_pila *nodoActual = pila_auxiliares_aritmetica->tope;
                while (nodoActual != NULL)
                {
                    fprintf(archivoTemp, "%s dd ?\n", nodoActual->contenido);
                    nodoActual = nodoActual->siguiente;
                }
                fprintf(archivoTemp, "\n");
                yaEscribiAuxiliares = 1;
            }
        }
    }

    fclose(archivoLectura);
    fclose(archivoTemp);

    // Reemplazar el archivo original con el temporal
    remove("final.asm");
    rename("final.asm.tmp", "final.asm");

    // Reabrir el archivo en modo append para que el resto del código pueda continuar
    FILE *archivoNuevo = fopen("final.asm", "a");
    if (archivoNuevo == NULL)
    {
        printf("[ASSEMBLER] Error al reabrir final.asm\n");
        exit(1);
    }

    return archivoNuevo;
}

/* -------------------------------------------------------------------------- */
/*                              CODIGO ASM - CODE                             */
/* -------------------------------------------------------------------------- */
void escribirSegmentCode(FILE *archivo, t_polaca *polaca)
{
    fprintf(archivo, "\n\n.CODE\n");

    fprintf(archivo, "; ------------------------ utiles ------------------------ \n");
    /* ------------------------------------ 1 ----------------------------------- */
    fprintf(archivo, "; devuelve en BX la cantidad de caracteres que tiene un string\n; DS:SI apunta al string.\n");
    fprintf(archivo, "STRLEN PROC\n\tmov bx,0\nSTRL01:\n\tcmp BYTE PTR [SI+BX],'$'\n\tje STREND\n\tinc BX\n\tjmp STRL01\nSTREND:\n\tret\nSTRLEN ENDP\n");
    /* ------------------------------------ 2 ----------------------------------- */
    fprintf(archivo, "; copia DS:SI a ES:DI; busca la cantidad de caracteres\n");
    fprintf(archivo, "COPIAR PROC\n\tcall STRLEN\n\tcmp bx,MAXTEXTSIZE\n\tjle COPIARSIZEOK\n\tmov bx,MAXTEXTSIZE\nCOPIARSIZEOK:\n\tmov cx,bx\n\tcld\n\trep movsb\n\tmov al,'$'\n\tmov BYTE PTR [DI],al\n\tret\nCOPIAR ENDP");

    fprintf(archivo, "\n\n; ------------------------ mi programa ------------------------ \n");
    fprintf(archivo, "START:\n");
    fprintf(archivo, "; inicializa el segmento de datos\n");
    fprintf(archivo, "\tMOV AX,@DATA\n");
    fprintf(archivo, "\tMOV DS,AX\n");
    fprintf(archivo, "\tMOV es,ax ;\n\n");

    t_nodo_polaca *celdaActual = polaca->inicio;

    // para comparacion - salto condicional
    t_nodo_polaca *celdaOperadorComparacion = NULL;
    t_nodo_polaca *celdaNroSaltoComparacion = NULL;

    while (celdaActual != NULL)
    {
        // Si tengo una etiqueta de salto ASM debo generarla y ver luego que habia en la polaca original
        if (strncmp(celdaActual->contenido, "@ET_", strlen("@ET_")) == 0) // esEtiquetaASM
        {
            escribirASMEtiquetaSalto(archivo, celdaActual->contenido);
            eliminarEtiquetaDeCelda(celdaActual->contenido);
        }

        // Una vez manejada la etiqueta ASM, ver el contenido original de la polaca
        char *operadorASM = getInstruccionASMOperador(celdaActual);
        t_simbolo *simboloOperando = buscarSimboloPorNombre(celdaActual->contenido);

        if (strncmp(celdaActual->contenido, "ET", strlen("ET")) == 0 || strncmp(celdaActual->contenido, "_END", strlen("_END")) == 0) // esEtiquetaWhilePolaca o esFinCodigo
        {
            // dummy
        }
        else if (strcmp(celdaActual->contenido, "ABS") == 0) // esFuncionABS
        {
            escribirASMFuncionABS(archivo);
        }
        else if (strcmp(celdaActual->contenido, "READ") == 0) // esFuncionRead
        {
            escribirASMFuncionRead(archivo);
        }
        else if (strcmp(celdaActual->contenido, "WRITE") == 0) // esFuncionWrite
        {
            escribirASMFuncionWrite(archivo);
        }
        else if (simboloOperando != NULL) // esOperando
        {
            apilar(pila_operandos, celdaActual->contenido);
        }
        else if (operadorASM != NULL) // esOperadorAritmetico
        {
            escribirASMOperacionAritmetica(archivo, operadorASM);
        }
        else if (strcmp(celdaActual->contenido, ":=") == 0) // esAsignacion
        {
            escribirASMAsignacion(archivo);
        }
        else if (strcmp(celdaActual->contenido, "BI") == 0) // esSaltoIncondicional
        {
            celdaActual = celdaActual->siguiente;
            escribirASMSaltoIncondicional(archivo, celdaActual->contenido);
        }
        else if (strcmp(celdaActual->contenido, "CMP") == 0) // esSaltoCondicional
        {
            celdaActual = celdaActual->siguiente;
            celdaOperadorComparacion = celdaActual;

            celdaActual = celdaActual->siguiente;
            celdaNroSaltoComparacion = celdaActual;

            escribirASMSaltoCondicional(archivo, celdaOperadorComparacion->contenido, celdaNroSaltoComparacion->contenido);
        }
        else
        {
            printf("[ASSEMBLER] Error: Contenido de celda no reconocido: '%s'\n", celdaActual->contenido);
            exit(1);
        }

        // Avanzar al siguiente nodo
        celdaActual = celdaActual->siguiente;
    }
}

/* --------------------------- CODIGO ASM - UTILES -------------------------- */
int esOperando(t_nodo_polaca *celda)
{
    if (buscarSimboloPorNombre(celda->contenido) != NULL)
    {
        return 1;
    }
    return 0;
}

char *getInstruccionASMOperador(t_nodo_polaca *celda)
{
    if (strcmp(celda->contenido, "+") == 0)
    {
        return "FADD";
    }
    else if (strcmp(celda->contenido, "-") == 0)
    {
        return "FSUB";
    }
    else if (strcmp(celda->contenido, "*") == 0)
    {
        return "FMUL";
    }
    else if (strcmp(celda->contenido, "/") == 0)
    {
        return "FDIV";
    }

    return NULL;
}

/* ------------------ CODIGO ASM - OPERACION ARITMETICA --------------------- */
void escribirASMOperacionAritmetica(FILE *archivo, const char *operadorASM)
{
    /* -------------------------------- OPERANDOS ------------------------------- */
    char operando1[MAX_LONG_VALOR_SIMBOLO];
    strcpy(operando1, desapilar(pila_operandos));

    char operando2[MAX_LONG_VALOR_SIMBOLO];
    strcpy(operando2, desapilar(pila_operandos));

    char operandosCargaASM[256];

    if (strcmp(operadorASM, "FDIV") == 0 || strcmp(operadorASM, "FSUB") == 0)
    {
        snprintf(operandosCargaASM, sizeof(operandosCargaASM), "FLD %s\n\tFLD %s", operando2, operando1);
    }
    else
    {
        snprintf(operandosCargaASM, sizeof(operandosCargaASM), "FLD %s\n\tFLD %s", operando1, operando2);
    }

    /* ------------------------------ AUX RESULTADO ----------------------------- */
    static int cantAuxResultados = 0;
    char varAuxASM[64];
    snprintf(varAuxASM, sizeof(varAuxASM), "@aux%d", cantAuxResultados++);
    apilar(pila_operandos, varAuxASM);
    apilar(pila_auxiliares_aritmetica, varAuxASM);

    /* -------------------------------- ASM FINAL ------------------------------- */
    fprintf(archivo, "; operacion arimetica\n");
    fprintf(archivo, "\t%s\n\t%s\n\tFSTP %s\n", operandosCargaASM, operadorASM, varAuxASM);
}

/* ------------------------- CODIGO ASM - ASIGNACION ------------------------ */
void escribirASMAsignacion(FILE *archivo)
{
    /* -------------------------------- VARIABLE -------------------------------- */
    char variable[MAX_LONG_NOMBRE_SIMBOLO];
    strcpy(variable, desapilar(pila_operandos));

    /* ----------------------------- VALOR A ASIGNAR ---------------------------- */
    char valor[MAX_LONG_VALOR_SIMBOLO];
    strcpy(valor, desapilar(pila_operandos));
    t_simbolo *simboloValorOperando = buscarSimboloPorNombre(valor);

    /* -------------------------------- ASM FINAL ------------------------------- */
    char asignacionASM[256];

    // Si encontramos el símbolo y es un string, usamos la rutina de copia.
    // Si el símbolo no existe (ej. temporales @aux...), lo tratamos como un valor numérico.
    if (simboloValorOperando != NULL && strcmp(simboloValorOperando->tipoDato, TIPO_TOKEN_CONST_STR) == 0)
    {
        snprintf(asignacionASM, sizeof(asignacionASM), "\tMOV SI, OFFSET %s\n\tMOV DI, OFFSET %s\n\tCALL COPIAR", valor, variable);
    }
    else
    {
        snprintf(asignacionASM, sizeof(asignacionASM), "\tFLD %s\n\tFSTP %s", valor, variable);
    }

    fprintf(archivo, "; asignacion\n");
    fprintf(archivo, "%s\n", asignacionASM);
}

/* ----------------------- CODIGO ASM - ETIQUETA SALTO ---------------------- */
void escribirASMEtiquetaSalto(FILE *archivo, const char *contenidoCelda)
{
    char *posSeparadorEtiquetaContenidoOrig = strstr(contenidoCelda, ":");
    char etiquetaSaltoASM[100];

    strncpy(etiquetaSaltoASM, contenidoCelda + 1, posSeparadorEtiquetaContenidoOrig - contenidoCelda);
    etiquetaSaltoASM[posSeparadorEtiquetaContenidoOrig - contenidoCelda] = '\0';

    /* -------------------------------- ASM FINAL ------------------------------- */
    fprintf(archivo, "%s\n", etiquetaSaltoASM);
}

void eliminarEtiquetaDeCelda(char *contenidoCelda)
{
    char *posSeparadorEtiquetaContenidoOrig = strstr(contenidoCelda, ":");
    if (posSeparadorEtiquetaContenidoOrig == NULL)
    {
        return;
    }

    char *nuevoInicio = posSeparadorEtiquetaContenidoOrig + 1;
    memmove(contenidoCelda, nuevoInicio, strlen(nuevoInicio) + 1);
}

/* -------------------- CODIGO ASM - SALTO INCONDICIONAL -------------------- */
void escribirASMSaltoIncondicional(FILE *archivo, const char *contenidoCelda)
{
    fprintf(archivo, "; salto incondicional\n");
    fprintf(archivo, "\tJMP %s\n", contenidoCelda);
}

/* --------------------- CODIGO ASM - SALTO CONDICIONAL --------------------- */
void escribirASMSaltoCondicional(FILE *archivo, const char *operadorComparacion, const char *nroSalto)
{
    /* -------------------------------- OPERANDOS ------------------------------- */
    char operando2[MAX_LONG_VALOR_SIMBOLO];
    strcpy(operando2, desapilar(pila_operandos));

    char operando1[MAX_LONG_VALOR_SIMBOLO];
    strcpy(operando1, desapilar(pila_operandos));

    /* --------------------------- OPERADOR COMPRACION -------------------------- */
    char instruccionSaltoASM[4];

    if (strcmp(operadorComparacion, VALOR_POLACA_OP_IGUAL) == 0)
    {
        strcpy(instruccionSaltoASM, VALOR_ASM_OP_IGUAL);
    }
    else if (strcmp(operadorComparacion, VALOR_POLACA_OP_DISTINTO) == 0)
    {
        strcpy(instruccionSaltoASM, VALOR_ASM_OP_DISTINTO);
    }
    else if (strcmp(operadorComparacion, VALOR_POLACA_OP_MAYOR) == 0)
    {
        strcpy(instruccionSaltoASM, VALOR_ASM_OP_MAYOR);
    }
    else if (strcmp(operadorComparacion, VALOR_POLACA_OP_MAYOR_IGUAL) == 0)
    {
        strcpy(instruccionSaltoASM, VALOR_ASM_OP_MAYOR_IGUAL);
    }
    else if (strcmp(operadorComparacion, VALOR_POLACA_OP_MENOR) == 0)
    {
        strcpy(instruccionSaltoASM, VALOR_ASM_OP_MENOR);
    }
    else if (strcmp(operadorComparacion, VALOR_POLACA_OP_MENOR_IGUAL) == 0)
    {
        strcpy(instruccionSaltoASM, VALOR_ASM_OP_MENOR_IGUAL);
    }

    /* -------------------------------- ASM FINAL ------------------------------- */
    fprintf(archivo, "; condicional\n");
    fprintf(archivo, "\tFLD %s\n\tFCOMP %s\n\tFSTSW ax\n\tSAHF\n\t%s %s\n", operando1, operando2, instruccionSaltoASM, nroSalto);
}

/* ------------------------ CODIGO ASM - FUNCION ABS ------------------------ */
void escribirASMFuncionABS(FILE *archivo)
{
    /* -------------------------------- OPERANDO ------------------------------- */
    char operando[MAX_LONG_VALOR_SIMBOLO];
    strcpy(operando, desapilar(pila_operandos));

    /* -------------------------------- AUX RESULTADO -------------------------- */
    char varAuxASM[] = "@aux_abs";
    apilar(pila_operandos, varAuxASM);
    apilar(pila_auxiliares_aritmetica, varAuxASM);

    /* -------------------------------- ASM FINAL ------------------------------- */
    fprintf(archivo, "; funcion abs\n");
    fprintf(archivo, "\tFLD %s\n\tFABS\n\tFSTP %s\n", operando, varAuxASM);
}

/* ------------------------ CODIGO ASM - FUNCION READ ----------------------- */
void escribirASMFuncionRead(FILE *archivo)
{
    /* -------------------------------- OPERANDO ------------------------------- */
    char operando[MAX_LONG_VALOR_SIMBOLO];
    strcpy(operando, desapilar(pila_operandos));
    t_simbolo *simboloOperando = buscarSimboloPorNombre(operando);

    /* -------------------------------- ASM FINAL ------------------------------- */
    char readASM[300];

    if (strcmp(simboloOperando->tipoDato, TIPO_TOKEN_CONST_INT) == 0 || strcmp(simboloOperando->tipoDato, TIPO_TOKEN_CONST_FLOAT) == 0)
    {
        snprintf(readASM, sizeof(readASM), "\tGetFloat %s\n\tnewLine", operando);
    }
    else if (strcmp(simboloOperando->tipoDato, TIPO_TOKEN_CONST_STR) == 0)
    {

        snprintf(readASM, sizeof(readASM), "\tgetString %s\n\tnewLine", operando);
    }

    fprintf(archivo, "; funcion read\n");
    fprintf(archivo, "%s\n", readASM);
}

/* ----------------------- CODIGO ASM - FUNCION WRITE ----------------------- */
void escribirASMFuncionWrite(FILE *archivo)
{
    /* -------------------------------- OPERANDO ------------------------------- */
    char operando[MAX_LONG_VALOR_SIMBOLO];
    strcpy(operando, desapilar(pila_operandos));
    t_simbolo *simboloOperando = buscarSimboloPorNombre(operando);

    /* -------------------------------- ASM FINAL ------------------------------- */
    char writeASM[300];

    if (strcmp(simboloOperando->tipoDato, TIPO_TOKEN_CONST_INT) == 0)
    {
        snprintf(writeASM, sizeof(writeASM), "\tDisplayFloat %s, 0\n\tnewLine", operando);
    }
    else if (strcmp(simboloOperando->tipoDato, TIPO_TOKEN_CONST_STR) == 0)
    {
        snprintf(writeASM, sizeof(writeASM), "\tdisplayString %s\n\tnewLine", operando);
    }
    else if (strcmp(simboloOperando->tipoDato, TIPO_TOKEN_CONST_FLOAT) == 0)
    {
        snprintf(writeASM, sizeof(writeASM), "\tDisplayFloat %s, 2\n\tnewLine", operando);
    }
    else
    {
        printf("[ASSEMBLER] Error 3999: Tipo de dato no soportado en funcion WRITE para el operando %s\n", operando);
        exit(1);
    }

    fprintf(archivo, "; funcion write\n");
    fprintf(archivo, "%s\n", writeASM);
}

/* -------------------------------------------------------------------------- */
/*                              CODIGO ASM - END                              */
/* -------------------------------------------------------------------------- */
void escribirSegmentoEnd(FILE *archivo)
{
    fprintf(archivo, "\n\n; indica que debe finalizar la ejecución\n");
    fprintf(archivo, "MOV AX,4C00H\n");
    fprintf(archivo, "INT 21H\n");
    fprintf(archivo, "END START\n");
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
            // Solo reemplazar si la celda anterior es NULL o no es un salto (los numeros en saltos son direcciones de celda)
            if (celdaAnterior == NULL || !esCeldaSalto(celdaAnterior))
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
