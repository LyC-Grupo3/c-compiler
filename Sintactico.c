#include "./Sintactico.h"

char operadorComparacionPendientePolaca[50];

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
        inicializarPila();

        crearTablaSimbolos();
        abrirArchivoSalidaLexico("test_outputs/output_lexico.txt");
        abrirArchivoSalidaSintactico("test_outputs/output_sintactico.txt");

        yyparse();

        exportarPolaca("test_outputs/polaca.txt");
        exportarTablaSimbolos("symbol-table.txt");

        eliminarPolaca();
        eliminarPila();
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

void setOperadorComparacionPendientePolaca(char *operador)
{
    strncpy(operadorComparacionPendientePolaca, operador, sizeof(operadorComparacionPendientePolaca) - 1);
    operadorComparacionPendientePolaca[sizeof(operadorComparacionPendientePolaca) - 1] = '\0';
}

char *getOperadorComparacionPendientePolaca()
{
    return (char *)operadorComparacionPendientePolaca;
}