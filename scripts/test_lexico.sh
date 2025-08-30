#!/bin/bash

## Uso: ./test_lexico.sh [-s] <archivo_lexico> <archivo_de_test>
## -s: Flag para incluir y.tab.h (modo sintáctico)

# ----------------------------------- OTROS ---------------------------------- #
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

# --------------------------- VALIDACION PARAMETROS -------------------------- #
if [ $# -lt 2 ]; then
    echo "❌ Error: Debe proporcionar ambos archivos"
    echo "Uso: $0 [-s] <archivo_lexico> <archivo_de_test>"
    echo "  -s: Incluir y.tab.h para integración con sintáctico"
    echo "Ejemplo: $0 Lexico_only.l tests/single_tokens.txt"
    echo "Ejemplo: $0 -s Lexico.l tests/single_tokens.txt"
    exit 1
fi

# Verificar que el archivo léxico existe
if [ ! -f "$1" ]; then
    echo "❌ Error: El archivo léxico '$1' no existe"
    exit 1
fi

# Verificar que el archivo de test existe
if [ ! -f "$2" ]; then
    echo "❌ Error: El archivo de test '$2' no existe"
    exit 1
fi


# ----------------------------------- PATHS ---------------------------------- #
# PARAMETROS
PATH_PARAM_FILE_LEXICO="$1"
PATH_PARAM_FILE_TEST="$2"

# SALIDA
NAME_FOLDER_OUTPUT="test_outputs"

NAME_FILE_OUTPUT="output.txt"

PATH_OUTPUT="$NAME_FOLDER_OUTPUT/$NAME_FILE_OUTPUT"

# TEMP
NAME_FOLDER_TEMP="temp"

NAME_FILE_TEMP_LEXICO_COPY="lexico.tmp"
NAME_FILE_TEMP_RES_FLEX="lex.yy.c"
NAME_FILE_TEMP_RES_GCC="analizador_lexico"

PATH_TEMP_LEXICO_COPY="$NAME_FOLDER_TEMP/$NAME_FILE_TEMP_LEXICO_COPY"
PATH_TEMP_RES_FLEX="$NAME_FOLDER_TEMP/$NAME_FILE_TEMP_RES_FLEX"
PATH_TEMP_RES_GCC="$NAME_FOLDER_TEMP/$NAME_FILE_TEMP_RES_GCC"


# Crear directorio de salidas si no existe
mkdir -p "$NAME_FOLDER_OUTPUT"


# ------------------------------- MODO ELEGIDO ------------------------------- #
if [ "$INCLUDE_YACC" = true ]; then
    # Modo sintáctico: mantener y.tab.h incluido, ajustar path de utils.h
    sed 's|#include "utils/utils.h"|#include "../utils/utils.h"|g' "$PATH_PARAM_FILE_LEXICO" > "$PATH_TEMP_LEXICO_COPY"
    printf "%-40s %s\n" "ANALISIS LEXICO Y SINTACTICO:" "manteniendo y.tab.h incluido"
else
    # Modo léxico únicamente: comentar y.tab.h y ajustar path de utils.h
    sed -e 's|#include "y.tab.h"|// #include "y.tab.h"|g' -e 's|#include "utils/utils.h"|#include "../utils/utils.h"|g' "$PATH_PARAM_FILE_LEXICO" > "$PATH_TEMP_LEXICO_COPY"
    printf "%-40s %s\n" "SOLO ANALISIS LEXICO:" "comentando y.tab.h"
fi



# ------------------------------ INFORMAR PATHS ------------------------------ #
echo "═══════════════════════════════════════════════════════════════════════════════════════"
printf "%-40s %s\n" "PATH ARCHIVO LEXICO:" "$PATH_PARAM_FILE_LEXICO"
printf "%-40s %s\n" "PATH ARCHIVO PRUEBAS:" "$PATH_PARAM_FILE_TEST"
printf "%-40s %s\n" "PATH ARCHIVO SALIDA:" "$PATH_OUTPUT"



# ----------------------------------- FLEX ----------------------------------- #
echo "═══════════════════════════════════════════════════════════════════════════════════════"
echo "Ejecutando flex..."
flex "$PATH_TEMP_LEXICO_COPY"

if [ $? -ne 0 ]; then
    echo "❌ Error: Falló la compilación con flex"
    exit 1
fi

# Mover el archivo generado por flex al directorio temp
if [ -f "$NAME_FILE_TEMP_RES_FLEX" ]; then
    mv "$NAME_FILE_TEMP_RES_FLEX" "$PATH_TEMP_RES_FLEX"
else
    echo "❌ Error: flex no generó el archivo $NAME_FILE_TEMP_RES_FLEX"
    exit 1
fi



# -------------------------------- COMPILACION ------------------------------- #
echo "═══════════════════════════════════════════════════════════════════════════════════════"
echo "Compilando lexico..."
gcc "$PATH_TEMP_RES_FLEX" -o "$PATH_TEMP_RES_GCC"

if [ $? -ne 0 ]; then
    echo "❌ Error: Falló la compilación con gcc"
    rm -f "$PATH_TEMP_RES_FLEX"
    exit 1
fi



# ----------------------------------- TEST ----------------------------------- #
echo "═══════════════════════════════════════════════════════════════════════════════════════"
echo "Ejecutando test..."
./"$PATH_TEMP_RES_GCC" "$PATH_PARAM_FILE_TEST" > "${PATH_OUTPUT}" 2>&1



# ---------------------------- INFORMAR RESULTADOS --------------------------- #
echo "═══════════════════════════════════════════════════════════════════════════════════════"
if [ $? -eq 0 ]; then
    printf "%-40s %s\n" "RESULTADO:" "✅ OK"
else
    printf "%-40s %s\n" "RESULTADO:" "❌ ERROR"
fi
printf "%-40s %s\n" "ARCHIVO SALIDA:" "$PATH_OUTPUT"



# --------------------------------- LIMPIEZA --------------------------------- #
echo "═══════════════════════════════════════════════════════════════════════════════════════"
printf "%-40s %s\n" "Limpiando archivos temporales:" "$NAME_FILE_TEMP_LEXICO_COPY, $NAME_FILE_TEMP_RES_FLEX, $NAME_FILE_TEMP_RES_GCC"
rm -f "$PATH_TEMP_LEXICO_COPY"
rm -f "$NAME_FILE_TEMP_RES_FLEX"
rm -f "$NAME_FILE_TEMP_RES_GCC"