@echo off
cls
echo Compilando y ejecutando el programa...

cd TASM
tasm.exe ..\ejemplo.asm
if errorlevel 1 goto error

tlink.exe ..\ejemplo.obj
if errorlevel 1 goto error

..\ejemplo.exe
if errorlevel 1 goto error

del ..\ejemplo.obj
del ..\ejemplo.exe
goto fin

:error
echo Ha ocurrido un error!
pause
goto end

:fin
echo Ejecucion finalizada correctamente
pause

:end