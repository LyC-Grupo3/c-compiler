@echo off
REM ============================================================================
REM Uso: test_lexico.bat [-s] <archivo_lexico> <archivo_de_test>
REM -s : Flag para incluir y.tab.h (modo sintáctico)
REM ============================================================================

setlocal ENABLEDELAYEDEXPANSION

REM ---------------------------- PROCESAR FLAGS ----------------------------
set INCLUDE_YACC=false
if "%~1"=="-s" (
    set INCLUDE_YACC=true
    shift
)

REM -------------------------- VALIDAR PARAMETROS --------------------------
if "%~2"=="" (
    echo ❌ Error: Debe proporcionar ambos archivos
    echo Uso: %~nx0 [-s] ^<archivo_lexico^> ^<archivo_de_test^>
    exit /b 1
)

set PATH_PARAM_FILE_LEXICO=%~1
set PATH_PARAM_FILE_TEST=%~2

if not exist "%PATH_PARAM_FILE_LEXICO%" (
    echo ❌ Error: El archivo léxico "%PATH_PARAM_FILE_LEXICO%" no existe
    exit /b 1
)

if not exist "%PATH_PARAM_FILE_TEST%" (
    echo ❌ Error: El archivo de test "%PATH_PARAM_FILE_TEST%" no existe
    exit /b 1
)

REM ------------------------------- PATHS ----------------------------------
set NAME_FOLDER_OUTPUT=test_outputs
set NAME_FILE_OUTPUT=output.txt
set PATH_OUTPUT=%NAME_FOLDER_OUTPUT%\%NAME_FILE_OUTPUT%

set NAME_FILE_TEMP_LEXICO_COPY=lexico_temp.l
set NAME_FILE_TEMP_RES_FLEX=lex.yy.c
set NAME_FILE_TEMP_RES_GCC=analizador_lexico.exe

if not exist "%NAME_FOLDER_OUTPUT%" mkdir "%NAME_FOLDER_OUTPUT%"

REM ------------------------------- MODO -----------------------------------
if "%INCLUDE_YACC%"=="true" (
    copy "%PATH_PARAM_FILE_LEXICO%" "%NAME_FILE_TEMP_LEXICO_COPY%" >nul
    echo ANALISIS LEXICO Y SINTACTICO: manteniendo y.tab.h incluido
) else (
    REM En batch no tenemos sed, usamos findstr para quitar la linea
    findstr /v /c:"#include \"y.tab.h\"" "%PATH_PARAM_FILE_LEXICO%" > "%NAME_FILE_TEMP_LEXICO_COPY%"
    echo SOLO ANALISIS LEXICO: comentando y.tab.h
)

echo ===============================================================================
echo PATH ARCHIVO LEXICO:    %PATH_PARAM_FILE_LEXICO%
echo PATH ARCHIVO PRUEBAS:   %PATH_PARAM_FILE_TEST%
echo PATH ARCHIVO SALIDA:    %PATH_OUTPUT%

REM ------------------------------- FLEX -----------------------------------
echo ===============================================================================
echo Ejecutando flex...
flex "%NAME_FILE_TEMP_LEXICO_COPY%"
if errorlevel 1 (
    echo ❌ Error: Fallo la compilacion con flex
    exit /b 1
)

if not exist "%NAME_FILE_TEMP_RES_FLEX%" (
    echo ❌ Error: flex no genero "%NAME_FILE_TEMP_RES_FLEX%"
    exit /b 1
)

REM ----------------------------- COMPILACION -------------------------------
echo ===============================================================================
echo Compilando lexico...
gcc "%NAME_FILE_TEMP_RES_FLEX%" -o "%NAME_FILE_TEMP_RES_GCC%"
if errorlevel 1 (
    echo ❌ Error: Fallo la compilacion con gcc
    del "%NAME_FILE_TEMP_RES_FLEX%"
    exit /b 1
)

REM -------------------------------- TEST ----------------------------------
echo ===============================================================================
echo Ejecutando test...
"%NAME_FILE_TEMP_RES_GCC%" "%PATH_PARAM_FILE_TEST%" > "%PATH_OUTPUT%" 2>&1

if errorlevel 1 (
    echo RESULTADO: ❌ ERROR
) else (
    echo RESULTADO: ✅ OK
)
echo ARCHIVO SALIDA: %PATH_OUTPUT%

REM ------------------------------- LIMPIEZA --------------------------------
echo ===============================================================================
echo Limpiando archivos temporales...
del "%NAME_FILE_TEMP_LEXICO_COPY%"
del "%NAME_FILE_TEMP_RES_FLEX%"
del "%NAME_FILE_TEMP_RES_GCC%"

endlocal