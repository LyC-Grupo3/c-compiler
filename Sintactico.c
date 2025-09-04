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

        exportarTablaSimbolos("test_outputs/tabla_simbolos.txt");
        cerrarArchivoSalidaLexico();
        cerrarArchivoSalidaLexicoSintactico();
    }
    fclose(yyin);
    printf("FINAL DE EJCUCION");
    return 0;
}

int yyerror(const char *msg)
{
    char msg_error_linea[100];
    char msg_error_msg[100];
    char msg_error_token[100];

    sprintf(msg_error_linea, "[SINTACTICO] ERROR en línea     : %d\n", yylineno);
    sprintf(msg_error_msg, "             Mensaje            : %s\n", msg);
    sprintf(msg_error_token, "             Token problemático : '%s'\n", yytext);

    printf("%s", msg_error_linea);
    printf("%s", msg_error_msg);
    printf("%s", msg_error_token);

    if (archivo_salida_sintactico != NULL)
    {
        fprintf(archivo_salida_sintactico, "%s", msg_error_linea);
        fprintf(archivo_salida_sintactico, "%s", msg_error_msg);
        fprintf(archivo_salida_sintactico, "%s", msg_error_token);
        fflush(archivo_salida_sintactico);
    }

    exit(1);
}

void informarMatchLexicoSintactico(const char *mensaje1, const char *mensaje2)
{
    char mensaje_formateado[500];
    sprintf(mensaje_formateado, "[SINTACTICO]   %30s -> %-30s\n", mensaje1, mensaje2);

    printf("%s", mensaje_formateado);
    if (archivo_salida_sintactico != NULL)
    {
        fprintf(archivo_salida_sintactico, "%s", mensaje_formateado);
        fflush(archivo_salida_sintactico);
    }
}

void abrirArchivoSalidaSintactico(const char *nombre_archivo)
{
    archivo_salida_sintactico = fopen(nombre_archivo, "w");
    if (archivo_salida_sintactico == NULL)
    {
        -printf("Error: No se pudo abrir el archivo %s para escritura\n", nombre_archivo);
    }
    else
    {
        fprintf(archivo_salida_sintactico, "=== ANÁLISIS SINTÁCTICO ===\n");
        fflush(archivo_salida_sintactico);
    }
}

void cerrarArchivoSalidaLexicoSintactico(void)
{
    if (archivo_salida_sintactico != NULL)
    {
        fclose(archivo_salida_sintactico);
        archivo_salida_sintactico = NULL;
    }
}