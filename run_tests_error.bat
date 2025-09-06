@echo off
REM Script para ejecutar todos los archivos de test de errores en Windows

echo === Ejecutando tests de errores del compilador ===
echo.

REM Compilar una sola vez al inicio
echo Compilando el proyecto...
flex Lexico.l
if %ERRORLEVEL% neq 0 (
    echo Error en flex. Abortando tests.
    pause
    exit /b 1
)

bison -dyv Sintactico.y
if %ERRORLEVEL% neq 0 (
    echo Error en bison. Abortando tests.
    pause
    exit /b 1
)

gcc lex.yy.c y.tab.c Sintactico.c utils/archivos.c utils/informes.c utils/simbolo.c utils/tabla_simbolos.c utils/utils.c utils/validacion.c -o compilador.exe
if %ERRORLEVEL% neq 0 (
    echo Error en la compilacion. Abortando tests.
    pause
    exit /b 1
)

echo Compilacion exitosa!
echo.

REM Ejecutar tests de la carpeta tests_error
echo === TESTS DE ERROR (tests_error) ===
echo.

for %%f in (tests_error\*.txt) do (
    echo =========================================
    echo Ejecutando: %%f
    echo =========================================
    compilador.exe "%%f"
    echo.
)

REM Limpiar archivos temporales
echo Limpiando archivos temporales...
if exist lex.yy.c del lex.yy.c
if exist y.tab.c del y.tab.c
if exist y.output del y.output
if exist y.tab.h del y.tab.h
if exist compilador.exe del compilador.exe

echo.
echo === Todos los tests completados ===
pause
