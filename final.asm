include macros2.asm
include number.asm

.MODEL LARGE  ; Modelo de Memoria
.386          ; Tipo de Procesador
.STACK 200h   ; Bytes en el Stack

MAXTEXTSIZE EQU 256


.DATA
; variables tabla simbolos
varString db MAXTEXTSIZE dup (?),'$'
varInt dd ?
varFloat dd ?
_4 dd 4.0
_10 dd 10.0

; variables auxiliares para resultados intermedios de operaciones aritméticas
@aux0 dd ?


.CODE
START:
	mov AX,@DATA  ; inicializa el segmento de datos
	mov DS,AX
	mov es,ax ;

; operacion arimetica
	FLD _10
	FLD _4
	FADD
	FSTP @aux0
; asignacion
	FLD @aux0
	FSTP varInt
; funcion write
	DisplayFloat varInt, 0
	newLine


mov ax,4c00h   ; Indica que debe finalizar la ejecución
int 21h
END START
