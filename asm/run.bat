@echo off
echo Compilando el programa en assembler...

cd TASM
echo.

rem Compilar el archivo ASM
tasm.exe ..\final.asm
if errorlevel 1 goto errores

rem Linkear el archivo OBJ
tlink.exe ..\final.obj
if errorlevel 1 goto errores

echo.
echo Compilacion exitosa
echo Ejecutando el programa:
echo -------------------
..\final.exe
echo -------------------
echo.

rem Limpieza de archivos temporales
del ..\final.obj
goto fin

:errores
echo.
echo Error durante la compilacion/linkeo
goto fin

:fin
cd ..
pause