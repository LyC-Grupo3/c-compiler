:: Script para windows
@echo off
c:\GnuWin32\bin\flex.exe Lexico.l
echo "FLEX completo la especificacion lexica (se creo un archivo lex.yy.c)"
pause

bison -dyv Sintactico.y

::c:\MinGW\bin\gcc.exe lex.yy.c y.tab.c -o compilador.exe
c:\MinGW\bin\gcc.exe lex.yy.c -o compilador.exe
echo "GCC completo la compilacion (se creo un archivo compilador.exe)"
pause


::compilador.exe tests/single_tokens.txt
compilador.exe tests/validaciones.txt
echo "Se ejecuto el analizador lexico sobre el archivo de prueba"


::@echo off
del compilador.exe
del lex.yy.c
del y.tab.c
del y.tab.h
del y.output

echo "Se borraron los archivos generados en todo este proceso, finalizando.."

pause
