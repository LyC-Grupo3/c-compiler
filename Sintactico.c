#include "./Sintactico.h"

t_pila *pilaBase;

char nroCeldaActualPolacaAuxInicioTrueSoloParaOR[5000]; 

int main(int argc, char *argv[])
{
    if ((yyin = fopen(argv[1], "rt")) == NULL)
    {
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
        return 1;
    }
    else
    {
        inicializarPolaca();
        pilaBase = crearPila();
        inicializarTriangleHelper();
        inicializarInitVariablesHelper();
        inicializarTiposDatosHelper();
        inicializarCondicionalHelper();
        inicializarDebugPolaca("test_outputs/debug_polaca.txt");

        crearTablaSimbolos();
        abrirArchivoSalidaLexico("test_outputs/output_lexico.txt");
        abrirArchivoSalidaSintactico("test_outputs/output_sintactico.txt");

        yyparse();

        exportarPolaca("test_outputs/polaca.txt");
        exportarTablaSimbolos("symbol-table.txt");

        cerrarDebugPolaca();
        eliminarPolaca();
        eliminarPila(pilaBase);
        limpiarTriangleHelper();
        limpiarInitVariablesHelper();
        limpiarTiposDatosHelper();
        limpiarCondicionalHelper();
        cerrarArchivoSalidaLexico();
        cerrarArchivoSalidaLexicoSintactico();
    }
    fclose(yyin);
    return 0;
}

int yyerror(const char *msg)
{
    informarErrorSintactico(msg, yytext, yylineno);
    exit(1);
}

/* -------------------------------------------------------------------------- */
/*                   FUNCIONES AUXILIARES PARA BACKPATCHING                   */
/* -------------------------------------------------------------------------- */

void apilarNroCeldaActualPolaca(t_pila *pila)
{
    char *nroCeldaActual = getIndiceActualPolaca();
    apilar(pila, nroCeldaActual);
}

void apilarNroCeldaActualYAvanzarPolaca(t_pila *pila)
{
    apilarNroCeldaActualPolaca(pila);
    avanzarPolaca();
}

int desapilarNroCeldaYEscribirEnEllaNroCeldaActual(t_pila *pila)
{
    char indicePolacaChar[TAM_CONTENIDO_PILA];
    strcpy(indicePolacaChar, desapilar(pila));
    int nroCeldaDesapilada = atoi(indicePolacaChar);

    char *nroCeldaActual = getIndiceActualPolaca();
    insertarEnPolacaIndice(nroCeldaDesapilada, nroCeldaActual);

    return nroCeldaDesapilada;
}

int desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(t_pila *pila)
{
    char indicePolacaChar[TAM_CONTENIDO_PILA];
    strcpy(indicePolacaChar, desapilar(pila));
    int nroCeldaDesapilada = atoi(indicePolacaChar);

    char *nroCeldaActual = getIndiceActualPolaca();
    char nroCeldaActualMasUno[12];
    snprintf(nroCeldaActualMasUno, 12, "%d", atoi(nroCeldaActual) + 1);
    insertarEnPolacaIndice(nroCeldaDesapilada, nroCeldaActualMasUno);

    return nroCeldaDesapilada;
}

int desapilarNroCeldaYEscribirloEnCeldaActualPolaca(t_pila *pila)
{
    char indicePolacaChar[TAM_CONTENIDO_PILA];
    strcpy(indicePolacaChar, desapilar(pila));
    int nroCeldaDesapilada = atoi(indicePolacaChar);

    insertarEnPolaca(indicePolacaChar);

    return nroCeldaDesapilada;
}




void insertarEnPolacaNroCeldaActualMasTres()
{
    char *nroCeldaActual = getIndiceActualPolaca();
    char nroCeldaActualMasDos[12];
    snprintf(nroCeldaActualMasDos, 12, "%d", atoi(nroCeldaActual) + 3);
    insertarEnPolaca(nroCeldaActualMasDos);
}


void setearNroCeldaActualPolacaAuxInicioTrueSoloParaOR()
{
    strcpy(nroCeldaActualPolacaAuxInicioTrueSoloParaOR, getIndiceActualPolaca());
}


void desapilarNroCeldaYEscribirEnEllaNroCeldaAuxInicioTrueOR(t_pila *pila)
{
    char indicePolacaChar[TAM_CONTENIDO_PILA];
    strcpy(indicePolacaChar, desapilar(pila));
    int nroCeldaDesapilada = atoi(indicePolacaChar);

    insertarEnPolacaIndice(nroCeldaDesapilada, nroCeldaActualPolacaAuxInicioTrueSoloParaOR);

}

void desapilarNroCeldaYEscribirEnEllaValor(t_pila *pila, const char *valor)
{
    char indicePolacaChar[TAM_CONTENIDO_PILA];
    strcpy(indicePolacaChar, desapilar(pila));
    int nroCeldaDesapilada = atoi(indicePolacaChar);

    insertarEnPolacaIndice(nroCeldaDesapilada, valor);
}