#!/bin/bash

## Script para ejecutar todos los archivos de test de errores

echo "=== Ejecutando tests de errores del compilador ==="
echo ""

# Compilar una sola vez al inicio
echo "Compilando el proyecto..."
flex Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c Sintactico.c utils/archivos.c utils/informes.c utils/simbolo.c utils/tabla_simbolos.c utils/utils.c utils/validacion.c -o compilador

if [ $? -ne 0 ]; then
    echo "Error en la compilación. Abortando tests."
    exit 1
fi

echo "Compilación exitosa!"
echo ""

# Función para ejecutar un archivo de test
ejecutar_test() {
    local archivo=$1
    local carpeta=$2
    
    echo "========================================="
    echo "Ejecutando: $carpeta/$archivo"
    echo "========================================="
    
    ./compilador "$carpeta/$archivo"
    echo ""
}

# Ejecutar tests de la carpeta tests_error
echo "=== TESTS DE ERROR (tests_error) ==="
echo ""
for archivo in tests_error/*.txt; do
    if [ -f "$archivo" ]; then
        nombre_archivo=$(basename "$archivo")
        ejecutar_test "$nombre_archivo" "tests_error"
    fi
done

# Limpiar archivos temporales
echo "Limpiando archivos temporales..."
rm -f lex.yy.c y.tab.c y.output y.tab.h compilador

echo ""
echo "=== Todos los tests completados ==="
