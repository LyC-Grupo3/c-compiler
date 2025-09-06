@echo off
REM Script para Windows

REM Usar el primer parámetro como archivo de prueba, o prueba.txt por defecto
if "%~1"=="" (
    set ARCHIVO_PRUEBA=prueba.txt
) else (
    set ARCHIVO_PRUEBA=%~1
)

echo Compilando y ejecutando: %ARCHIVO_PRUEBA%
echo.

flex Lexico.l
if %ERRORLEVEL% neq 0 (
    echo Error en flex.
    pause
    exit /b 1
)

bison -dyv Sintactico.y
if %ERRORLEVEL% neq 0 (
    echo Error en bison.
    pause
    exit /b 1
)

gcc lex.yy.c y.tab.c Sintactico.c utils/archivos.c utils/informes.c utils/simbolo.c utils/tabla_simbolos.c utils/utils.c utils/validacion.c -o compilador.exe
if %ERRORLEVEL% neq 0 (
    echo Error en la compilacion.
    pause
    exit /b 1
)

compilador.exe "%ARCHIVO_PRUEBA%"

REM Limpiar archivos temporales
if exist lex.yy.c del lex.yy.c
if exist y.tab.c del y.tab.c
if exist y.output del y.output
if exist y.tab.h del y.tab.h
if exist compilador.exe del compilador.exe

pause
