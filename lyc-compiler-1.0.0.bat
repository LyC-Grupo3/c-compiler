:: Script para windows
flex Lexico.l
bison -dyv Sintactico.y

gcc lex.yy.c y.tab.c Sintactico.c utils/archivos.c utils/informes.c utils/simbolo.c utils/tabla_simbolos.c utils/utils.c utils/validacion.c -o compilador.exe

compilador.exe test.txt

@echo off
del compilador.exe
del lex.yy.c
del y.tab.c
del y.tab.h
del y.output

pause