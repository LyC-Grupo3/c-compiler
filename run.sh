## Script para Unix

# Usar el primer parámetro como archivo de prueba, o prueba.txt por defecto
ARCHIVO_PRUEBA=${1:-prueba.txt}

flex Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c Sintactico.c utils/informes.c utils/simbolo.c utils/tabla_simbolos.c utils/utils.c utils/validacion.c -o compilador
./compilador "$ARCHIVO_PRUEBA"
rm lex.yy.c
rm y.tab.c
rm y.output
rm y.tab.h
rm compilador