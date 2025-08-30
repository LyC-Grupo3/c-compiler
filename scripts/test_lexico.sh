#!/bin/bash

## Uso: ./test_lexico.sh [-s] <archivo_lexico> <archivo_de_test>
## -s: Flag para incluir y.tab.h (modo sintáctico)

INCLUDE_YACC=false

# Procesar flags
while getopts "s" opt; do
    case $opt in
        s)
            INCLUDE_YACC=true
            ;;
        \?)
            echo "Flag inválida: -$OPTARG" >&2
            exit 1
            ;;
    esac
done

# Ajustar índices después de procesar flags
shift $((OPTIND-1))

if [ $# -lt 2 ]; then
    echo "Error: Debe proporcionar ambos archivos"
    echo "Uso: $0 [-s] <archivo_lexico> <archivo_de_test>"
    echo "  -s: Incluir y.tab.h para integración con sintáctico"
    echo "Ejemplo: $0 Lexico_only.l tests/single_tokens.txt"
    echo "Ejemplo: $0 -s Lexico.l tests/single_tokens.txt"
    exit 1
fi

# Verificar que el archivo léxico existe
if [ ! -f "$1" ]; then
    echo "Error: El archivo léxico '$1' no existe"
    exit 1
fi

# Verificar que el archivo de test existe
if [ ! -f "$2" ]; then
    echo "Error: El archivo de test '$2' no existe"
    exit 1
fi

LEXICO_FILE="$1"
TEST_FILE="$2"

# Crear directorio de salidas si no existe
mkdir -p test_outputs

# Generar nombre del archivo de salida basado en los archivos de entrada
LEXICO_BASENAME=$(basename "$LEXICO_FILE" .l)
TEST_BASENAME=$(basename "$TEST_FILE" .txt)
OUTPUT_FILE="test_outputs/${LEXICO_BASENAME}_${TEST_BASENAME}_output.txt"

# Preparar archivo léxico temporal
TEMP_LEXICO_FILE="${LEXICO_FILE}.tmp"

if [ "$INCLUDE_YACC" = true ]; then
    # Modo sintáctico: mantener y.tab.h incluido
    cp "$LEXICO_FILE" "$TEMP_LEXICO_FILE"
    echo "Modo sintáctico: manteniendo y.tab.h incluido"
else
    # Modo léxico únicamente: comentar y.tab.h para evitar errores
    sed 's|#include "y.tab.h"|// #include "y.tab.h"|g' "$LEXICO_FILE" > "$TEMP_LEXICO_FILE"
    echo "Modo léxico únicamente: comentando y.tab.h"
fi

echo "Compilando analizador léxico desde: $LEXICO_FILE"
flex "$TEMP_LEXICO_FILE"

if [ $? -ne 0 ]; then
    echo "Error: Falló la compilación con flex"
    exit 1
fi

echo "Compilando con gcc..."
gcc lex.yy.c -o test_lexico

if [ $? -ne 0 ]; then
    echo "Error: Falló la compilación con gcc"
    rm -f lex.yy.c
    exit 1
fi

echo "Ejecutando prueba con archivo: $TEST_FILE"
echo "Guardando salida en: $OUTPUT_FILE"
./test_lexico "$TEST_FILE" > "$OUTPUT_FILE" 2>&1

if [ $? -eq 0 ]; then
    echo "✓ Prueba completada exitosamente"
    echo "Resultados guardados en: $OUTPUT_FILE"
else
    echo "⚠ La prueba completó con errores o warnings"
    echo "Revisa la salida en: $OUTPUT_FILE"
fi

echo "Limpiando archivos temporales..."
rm -f lex.yy.c
rm -f test_lexico
rm -f "$TEMP_LEXICO_FILE"