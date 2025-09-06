#include "./Sintactico.h"

int main(int argc, char *argv[])
{
    if ((yyin = fopen(argv[1], "rt")) == NULL)
    {
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
        return 1;
    }
    else
    {
        crearTablaSimbolos();
        abrirArchivoSalidaLexico("test_outputs/output_lexico.txt");
        abrirArchivoSalidaSintactico("test_outputs/output_sintactico.txt");

        yyparse();

        exportarTablaSimbolos("symbol-table.txt");
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