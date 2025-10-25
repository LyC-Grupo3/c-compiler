PATH=C:\asm\TASM;

tasm C:\asm\numbers.asm
tasm C:\asm\final.asm
tlink final.obj numbers.obj
final.exe
del final.obj 
del numbers.obj 
del final.exe
del final.map